#!/bin/bash

benchmark_name=('blackscholes' 'bodytrack' 'canneal' 'dedup' 'facesim' 'ferret' 'fluidanimate' 'freqmine' 'streamcluster' 'swaptions' 'vips' 'x264' 'rtview')

res_dir='res_parsec'

# rm -rf $res_dir
# mkdir $res_dir

for name in ${benchmark_name[@]}
do	
	# parse lru result
	src_file=../gem5/res_stats/stats_${name}.txt
	if [ -f $src_file ]; then
		python process_text.py $src_file ${res_dir}/res_${name}_lru.txt
		echo benchmark $name for LRU finished
	else
		echo benchmark $name for LRU haven\'t finished
	fi

	# parse dip result
	src_file=../gem5-Dip/res_stats/stats_${name}.txt
	if [ -f $src_file ]; then
		python process_text.py $src_file ${res_dir}/res_${name}_dip.txt
		echo benchmark $name for DIP finished
	else
		echo benchmark $name for DIP haven\'t finished
	fi	

	# parse lip result
	src_file=../gem5-Lip/res_stats/stats_${name}.txt
	if [ -f $src_file ]; then
		python process_text.py $src_file ${res_dir}/res_${name}_lip.txt
		echo benchmark $name for LIP finished
	else
		echo benchmark $name for LIP haven\'t finished
	fi
	
	# parse bip result
	src_file=../gem5-Bip/res_stats/stats_${name}.txt
	if [ -f $src_file ]; then
		python process_text.py $src_file ${res_dir}/res_${name}_bip.txt
		echo benchmark $name for BIP finished
	else
		echo benchmark $name for BIP haven\'t finished
	fi
	
	# parse dipi2 result
	src_file=../gem5-Test/res_stats/stats_${name}.txt
	if [ -f $src_file ]; then
		python process_text.py $src_file ${res_dir}/res_${name}_dip2.txt
		echo benchmark $name for DIP2 finished
	else
		echo benchmark $name for DIP2 haven\'t finished
	fi
done

