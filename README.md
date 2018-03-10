# chunkstream
A high-performance in-memory C++ streaming buffer

For larger memory streams the reallocation costs of a `std::vector` may be prohibitive. The allocation scheme of this class is similar to that of a `std::deque`, but with customizable chunk size. This allows the selection of larger chunk sizes than the default 16 bytes.

## Benchmark
The benchmarks were performed on an IvyBridge processor, on newer platforms the numbers may differ. The number behind the test name indicates the number of 64 bit values that were serialized and deserialized.

For comparison we show the performance of a std::stringstream.

```
----------------------------------------------------------------
Benchmark                         Time           CPU Iterations
----------------------------------------------------------------
BM_chunkstream/1                 86 ns         78 ns    9038206   98.2724MB/s
BM_chunkstream/8                137 ns        126 ns    5471529   482.613MB/s
BM_chunkstream/64               511 ns        463 ns    1524012   1053.66MB/s
BM_chunkstream/512             3201 ns       3058 ns     226564   1.24736GB/s
BM_chunkstream/1024            6801 ns       6313 ns     108648   1.20846GB/s
BM_chunkstream/8192           55811 ns      50977 ns      13640   1.19732GB/s
BM_chunkstream/65536         459186 ns     417970 ns       1684   1.16822GB/s
BM_chunkstream/10485760    96178712 ns   91890500 ns          8   870.601MB/s
BM_chunkstream/83886080  1212691933 ns 1137571000 ns          1   562.602MB/s
BM_stringstream/1               143 ns        130 ns    5400276   58.8713MB/s
BM_stringstream/8               771 ns        712 ns    1003009   85.7697MB/s
BM_stringstream/64             4149 ns       3570 ns     197892   136.783MB/s
BM_stringstream/512           22245 ns      21798 ns      31815   179.203MB/s
BM_stringstream/1024          43647 ns      42617 ns      16363   183.318MB/s
BM_stringstream/8192         405853 ns     337125 ns       2107   185.391MB/s
BM_stringstream/65536       2870768 ns    2668375 ns        261   187.380MB/s
BM_stringstream/10485760  549246276 ns  529058000 ns          1   151.212MB/s
BM_stringstream/83886080 4658431731 ns 4364733000 ns          1   146.630MB/s
```
