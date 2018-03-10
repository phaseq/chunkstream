#include <benchmark/benchmark.h>
#include <sstream>
#include "chunkstream.hpp"

static void BM_chunkstream(benchmark::State& state) {
	long long* src = new long long[state.range(0)];
	long long* dst = new long long[state.range(0)];
	for (int i = 0; i < state.range(0) * sizeof(long long); ++i)
		reinterpret_cast<char*>(src)[i] = random() * 255;
	memset(dst, 0, state.range(0));
	for (auto _ : state) {
		fb::chunkstream<8096, false> ss;
		for (int i = 0; i < state.range(0); ++i)
			ss << src[i];
		for (int i = 0; i < state.range(0); ++i)
			ss >> dst[i];
		benchmark::DoNotOptimize(dst);
	}
	state.SetBytesProcessed(
		int64_t(state.iterations()) * int64_t(state.range(0)) * sizeof(long long));
	delete[] src;
	delete[] dst;
}
BENCHMARK(BM_chunkstream)
	->Arg(1)
	->Arg(8)
	->Arg(64)
	->Arg(512)
	->Arg(1 << 10)
	->Arg(8 << 10)
	->Arg(1 << 16)
	->Arg(10 * 1024 * 1024)
	->Arg(80 * 1024 * 1024);

static void BM_chunkstream_safe(benchmark::State& state) {
	long long* src = new long long[state.range(0)];
	long long* dst = new long long[state.range(0)];
	for (int i = 0; i < state.range(0) * sizeof(long long); ++i)
		reinterpret_cast<char*>(src)[i] = random() * 255;
	memset(dst, 0, state.range(0));
	for (auto _ : state) {
		fb::chunkstream<8096> ss;
		for (int i = 0; i < state.range(0); ++i)
			ss << src[i];
		for (int i = 0; i < state.range(0); ++i)
			ss >> dst[i];
		benchmark::DoNotOptimize(dst);
	}
	state.SetBytesProcessed(
		int64_t(state.iterations()) * int64_t(state.range(0)) * sizeof(long long));
	delete[] src;
	delete[] dst;
}
BENCHMARK(BM_chunkstream_safe)
	->Arg(1)
	->Arg(8)
	->Arg(64)
	->Arg(512)
	->Arg(1 << 10)
	->Arg(8 << 10)
	->Arg(1 << 16)
	->Arg(10 * 1024 * 1024)
	->Arg(80 * 1024 * 1024);

class std_memorystream : public std::stringstream {
public:
	std_memorystream() : std::stringstream(std::ios::in | std::ios::out | std::ios::binary) {}
	template <typename T>
	std_memorystream& operator<<(const T& val) {
		write(reinterpret_cast<const char*>(&val), sizeof(val));
		return *this;
	}
	template <typename T>
	std_memorystream& operator>>(T& val) {
		read(reinterpret_cast<char*>(&val), sizeof(val));
		return *this;
	}
};

static void BM_stringstream(benchmark::State& state) {
	long long* src = new long long[state.range(0)];
	long long* dst = new long long[state.range(0)];
	for (int i = 0; i < state.range(0) * sizeof(long long); ++i)
		reinterpret_cast<char*>(src)[i] = random() * 255;
	memset(dst, 0, state.range(0));
	for (auto _ : state) {
		std_memorystream ss;
		for (int i = 0; i < state.range(0); ++i)
			ss << src[i];
		for (int i = 0; i < state.range(0); ++i)
			ss >> dst[i];
		benchmark::DoNotOptimize(dst);
	}
	state.SetBytesProcessed(
		int64_t(state.iterations()) * int64_t(state.range(0)) * sizeof(long long));
	delete[] src;
	delete[] dst;
}
BENCHMARK(BM_stringstream)
	->Arg(1)
	->Arg(8)
	->Arg(64)
	->Arg(512)
	->Arg(1 << 10)
	->Arg(8 << 10)
	->Arg(1 << 16)
	->Arg(10 * 1024 * 1024)
	->Arg(80 * 1024 * 1024);

BENCHMARK_MAIN();