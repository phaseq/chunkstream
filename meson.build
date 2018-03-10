project('chunkstream', 'cpp')

gbench_inc = include_directories('/usr/local/Cellar/google-benchmark/1.3.0/include')

chunkstream = executable('chunkstream',
	sources: ['test.cpp'],
	include_directories: [gbench_inc],
	cpp_args: ['-march=native', '-std=c++11'],
	link_args: ['/usr/local/Cellar/google-benchmark/1.3.0/lib/libbenchmark.a'])
