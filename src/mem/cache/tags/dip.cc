/*
 * Copyright (c) 2012-2013 ARM Limited
 * All rights reserved.
 *
 * The license below extends only to copyright in the software and shall
 * not be construed as granting a license to any other intellectual
 * property including but not limited to intellectual property relating
 * to a hardware implementation of the functionality of the software
 * licensed hereunder.  You may use the software subject to the license
 * terms below provided that you ensure that this notice is replicated
 * unmodified and in its entirety in all distributions of the software,
 * modified or unmodified, in source code or in binary form.
 *
 * Copyright (c) 2003-2005,2014 The Regents of The University of Michigan
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Authors: Yao Liu
 */

/**
 * @file
 * Definitions of a DIP tag store.
 */

#include "base/random.hh"
#include "debug/CacheRepl.hh"
#include "debug/CacheDip.hh"
#include "mem/cache/tags/dip.hh"
#include "mem/cache/base.hh"

DIP::DIP(const Params *p)
    : BaseSetAssoc(p),
      useLRU(true), psel(0), bipctr(0)
{
}

BaseSetAssoc::BlkType*
DIP::accessBlock(Addr addr, bool is_secure, Cycles &lat, int master_id)
{
    BlkType *blk = BaseSetAssoc::accessBlock(addr, is_secure, lat, master_id);

    if (blk != NULL) {
        // move this block to head of the MRU list
        sets[blk->set].moveToHead(blk);
        DPRINTF(CacheRepl, "set %x: moving blk %x (%s) to MRU\n",
                blk->set, regenerateBlkAddr(blk->tag, blk->set),
                is_secure ? "s" : "ns");
    }

    return blk;
}

BaseSetAssoc::BlkType*
DIP::findVictim(Addr addr) const
{
    int set = extractSet(addr);

    // debug info
    DPRINTF(CacheDip, "numset: %x; assoc: %x; DIP: bipctr: %x; psel: %x; useLRU: %x \n",
                numSets, assoc, bipctr, psel, useLRU);

    // grab a replacement candidate
    BlkType *blk = sets[set].blks[assoc - 1];

    if (blk->isValid()) {
        DPRINTF(CacheRepl, "set %x: selecting blk %x for replacement\n",
                set, regenerateBlkAddr(blk->tag, set));
    }

    return blk;
}

void
DIP::insertBlock(PacketPtr pkt, BlkType *blk)
{
    BaseSetAssoc::insertBlock(pkt, blk);

    int set = extractSet(pkt->getAddr());
    // sets[set].moveToTail(blk);

    int offset = extractBlkOffset(pkt->getAddr());

    // update PSEL selector
    if (useLRU) {
        psel ++;
    } else {
	psel --;
    }
    
    // update BIP counter
    if (!useLRU) {
    bipctr ++;
    if (bipctr >= 32) {
	bipctr = 0;
    }
    }

    // choose policy
    int dss = offset % 32;
    if (0 == dss) { // LRU
	useLRU = true;
    } else if (31 == dss) { // BIP
	useLRU = false;
    } else { // choose by PSEL
	if (psel == 0) {
	    useLRU = true;
	} else {
	    useLRU = false;
	}
    }

    if (useLRU) {  // LRU policy
    	sets[set].moveToHead(blk);
    } else {  // BIP policy
	if (bipctr == 0) {
	    sets[set].moveToHead(blk);
	} else {
	    sets[set].moveToTail(blk);
	}
    }
}

void
DIP::invalidate(BlkType *blk)
{
    BaseSetAssoc::invalidate(blk);

    // should be evicted before valid blocks
    int set = blk->set;
    sets[set].moveToTail(blk);
}

DIP*
DIPParams::create()
{
    return new DIP(this);
}
