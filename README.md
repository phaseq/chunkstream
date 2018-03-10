# chunkstream
A high-performance in-memory C++ streaming buffer

For larger memory streams the reallocation costs of a `std::vector` may be prohibitive. The allocation scheme of this class is similar to that of a `std::deque`, but with customizable chunk size. This allows the selection of larger chunk sizes than the default 16 bytes.

## Benchmark
The benchmarks were performed on an IvyBridge processor, on newer platforms the numbers may differ. The number behind the test name indicates the number of 64 bit values that were serialized and deserialized.

In unsafe mode chunkstream will not warn when you try to read out-of-bounds. In safe mode it will throw an exception in that situation. Unfortunately this has a quite high performance penalty.

For comparison we show the performance of a std::stringstream.

```
--------------------------------------------------------------------
Benchmark                             Time           CPU Iterations
--------------------------------------------------------------------
BM_chunkstream/1                     76 ns         76 ns    8850229    100.62MB/s
BM_chunkstream/8                    115 ns        115 ns    6116101   531.654MB/s
BM_chunkstream/64                   425 ns        425 ns    1656025   1.12297GB/s
BM_chunkstream/512                 2741 ns       2741 ns     254808   1.39176GB/s
BM_chunkstream/1024                5793 ns       5789 ns     120213   1.31795GB/s
BM_chunkstream/8192               46774 ns      46758 ns      14960   1.30535GB/s
BM_chunkstream/65536             387596 ns     387420 ns       1852   1.26034GB/s
BM_chunkstream/10485760        87288983 ns   87189500 ns          8   917.542MB/s
BM_chunkstream/83886080      1016545053 ns 1015852000 ns          1   630.013MB/s
BM_chunkstream_safe/1                77 ns         77 ns    8980922   99.0262MB/s
BM_chunkstream_safe/8               133 ns        133 ns    5361684   459.795MB/s
BM_chunkstream_safe/64              576 ns        576 ns    1198979   848.196MB/s
BM_chunkstream_safe/512            4021 ns       4019 ns     176359   972.021MB/s
BM_chunkstream_safe/1024           8145 ns       8142 ns      87689   959.484MB/s
BM_chunkstream_safe/8192          65929 ns      65922 ns      10606   948.088MB/s
BM_chunkstream_safe/65536        530234 ns     530214 ns       1309   943.016MB/s
BM_chunkstream_safe/10485760  108677150 ns  108221000 ns          6   739.228MB/s
BM_chunkstream_safe/83886080 1185751630 ns 1185108000 ns          1   540.035MB/s
BM_stringstream/1                   127 ns        127 ns    5500896   59.8884MB/s
BM_stringstream/8                   685 ns        683 ns    1026152   89.3768MB/s
BM_stringstream/64                 3483 ns       3480 ns     201163   140.292MB/s
BM_stringstream/512               21592 ns      21557 ns      32437   181.205MB/s
BM_stringstream/1024              42156 ns      42131 ns      16579   185.434MB/s
BM_stringstream/8192             327918 ns     327837 ns       2157   190.643MB/s
BM_stringstream/65536           2631322 ns    2625464 ns        267   190.442MB/s
BM_stringstream/10485760      521976166 ns  521080000 ns          1   153.527MB/s
BM_stringstream/83886080     4264214737 ns 4257052000 ns          1   150.339MB/s
```
