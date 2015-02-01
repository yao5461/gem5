
base_dir='./scripts/'
tail_name='_64c_simsmall.rcS'
# tail_name='_64c_simlarge.rcS'

# benchmark_name=('blackscholes' 'bodytrack' 'canneal' 'dedup' 'facesim' 'ferret' 'fluidanimate' 'freqmine' 'streamcluster' 'swaptions' 'vips' 'x264' 'rtview')

benchmark_name=('blackscholes' 'canneal' 'ferret' 'swaptions' 'vips' 'x264' 'rtview')

# benchmark_name=('bodytrack' 'dedup' 'facesim' 'fluidanimate' 'freqmine' 'streamcluster')

# l1I_cache_size='32kB'
# l1I_cache_asso='2'

# l1D_cache_size='64kB'
# l1D_cache_asso='2'

# l2_cache_size='2MB' 
# l2_cache_asso='8'

# cacheline_size='64' # 64B

mem_size='512MB'

cpu_type='atomic'
cpu_num=4

# tail_num=5000000000

# X86 | ALPHA
model='X86'
# model='ALPHA'

if [ $model = 'X86' ]; then
	kernel='x86_64-vmlinux-2.6.22.9.smp'
	disk_img='x86root-parsec.img'
else
	kernel='alpha_vmlinux_parsec'
	disk_img='linux-parsec-2-1-m5.img'
fi

debug=false
debug_info='CacheDip'
debug_outfile='debug.out'

for name in ${benchmark_name[@]}
do
	echo =========== benchmark: $name

	rm -rf m5mout/

	script_dir=$base_dir$name/$name$tail_name

	if $debug; then 
	./build/$model/gem5.opt --debug-flags=$debug_info --debug-file=$debug_outfile \
		./configs/example/fs.py --script=$script_dir \
		-n $cpu_num --cpu-type $cpu_type \
		--mem-size=$mem_size --caches --l2cache \
		--kernel=$kernel --disk-image=$disk_img 
	else
	./build/$model/gem5.opt ./configs/example/fs.py --script=$script_dir \
		-n $cpu_num --cpu-type $cpu_type \
		--mem-size=$mem_size --caches --l2cache \
		--kernel=$kernel --disk-image=$disk_img 
	fi

	cp m5out/stats.txt res_stats/stats_${name}.txt

	echo ========== finish benchmark: $name
done
        

