#!/usr/bin/env python

import re
import sys

def main(argv):
    if len(argv) != 2:
        print 'Error parameters!'
        return

    in_file = argv[1]

    with open(in_file, 'r') as f:
        lines = f.readlines()

    # regular
    pattern1 = re.compile(r'.*l2.*')
    pattern2 = re.compile(r'.*cpu\d.*')
    
    # result
    l2_content = ''
    cpu_content = ''
    other_content = ''

    for line in lines:
        if pattern1.match(line):
            l2_content = l2_content + line
        elif pattern2.match(line):
            cpu_content = cpu_content + line
        else :
            other_content = other_content + line

    # write file
    with open('l2.debug', 'w') as f:
        f.writelines(l2_content)
   
    with open('cpu.debug', 'w') as f:
        f.writelines(cpu_content)

    with open('other.debug', 'w') as f:
        f.writelines(other_content) 


if __name__ == '__main__':
    main(sys.argv)
