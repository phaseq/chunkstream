# chunkstream
A high-performance in-memory C++ streaming buffer

For larger memory streams the reallocation costs of a `std::vector` may be prohibitive. The allocation scheme of this class is similar to that of a `std::deque`, but with customizable chunk size. This allows the selection of larger chunk sizes than the default 16 bytes.

## Usage
This class currently only offers the streaming operators `<<` and `>>`:
```C++
fb::chunkstream<1024> ss;
ss << "temperature: " << 23.2f << 'C';
char temperature[14];
float value;
char unit;
ss >> temperature >> value >> unit;
```

As you can see all values will be logged in their byte representation. This is exactly what you want for serialization. Number-to-string formatting as in the standard iostreams is not supported.

## Benchmark
The benchmarks were performed on an IvyBridge processor, on newer platforms the numbers may differ. The number behind the test name indicates the number of 64 bit values that were serialized and deserialized.

In unsafe mode chunkstream will not warn when you try to read out-of-bounds. In safe mode it will throw an exception in that situation. Unfortunately this has a quite high performance penalty.

For comparison we show the performance of a std::stringstream.

```
--------------------------------------------------------------------
Benchmark                             Time           CPU Iterations
--------------------------------------------------------------------
BM_chunkstream/1                     75 ns         75 ns    9198423   102.357MB/s
BM_chunkstream/8                    128 ns        112 ns    6157798    544.43MB/s
BM_chunkstream/64                   414 ns        413 ns    1692182   1.15487GB/s
BM_chunkstream/512                 2653 ns       2648 ns     265784   1.44051GB/s
BM_chunkstream/1024                5742 ns       5553 ns     124285   1.37392GB/s
BM_chunkstream/8192               44469 ns      44375 ns      15752   1.37545GB/s
BM_chunkstream/65536             361573 ns     360826 ns       1937   1.35323GB/s
BM_chunkstream/10485760        87927302 ns   87657250 ns          8   912.646MB/s
BM_chunkstream/83886080      1031748357 ns 1028072000 ns          1   622.524MB/s
BM_chunkstream_safe/1                79 ns         79 ns    8803370   96.8014MB/s
BM_chunkstream_safe/8               135 ns        135 ns    5208178   452.193MB/s
BM_chunkstream_safe/64              590 ns        589 ns    1180458     828.9MB/s
BM_chunkstream_safe/512            4060 ns       4052 ns     172543   963.997MB/s
BM_chunkstream_safe/1024           8097 ns       8085 ns      85622   966.351MB/s
BM_chunkstream_safe/8192          66192 ns      66111 ns      10561   945.375MB/s
BM_chunkstream_safe/65536        535251 ns     534198 ns       1298   935.983MB/s
BM_chunkstream_safe/10485760  110879479 ns  110665167 ns          6   722.901MB/s
BM_chunkstream_safe/83886080 1243729411 ns 1240167000 ns          1    516.06MB/s
BM_stringstream/1                   128 ns        128 ns    5467511   59.7379MB/s
BM_stringstream/8                   680 ns        679 ns    1010203   89.9544MB/s
BM_stringstream/64                 3465 ns       3456 ns     202465   141.272MB/s
BM_stringstream/512               21500 ns      21453 ns      32621   182.087MB/s
BM_stringstream/1024              42074 ns      41973 ns      16702   186.131MB/s
BM_stringstream/8192             328420 ns     326210 ns       2146   191.594MB/s
BM_stringstream/65536           2632482 ns    2625213 ns        267   190.461MB/s
BM_stringstream/10485760      515215283 ns  514152000 ns          1   155.596MB/s
BM_stringstream/83886080     4376047366 ns 4293354000 ns          1   149.068MB/s
```
