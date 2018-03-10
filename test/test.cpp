#include <gtest/gtest.h>
#include <sstream>
#include "chunkstream.hpp"

TEST(chunkstream, different_types) {
	fb::chunkstream<1024> ss;

	const float in1 = 1.0f;
	const double in2 = 2.0;
	const int32_t in3 = 3;
	const int64_t in4 = 4;
	ss << in1 << in2 << in3 << in4;

	float out1;
	double out2;
	int32_t out3;
	int64_t out4;
	ss >> out1 >> out2 >> out3 >> out4;

	ASSERT_EQ(in1, out1);
	ASSERT_EQ(in2, out2);
	ASSERT_EQ(in3, out3);
	ASSERT_EQ(in4, out4);
}

TEST(chunkstream, readme_sample) {
	fb::chunkstream<1024> ss;
	ss << "temperature: " << 23.2f << 'C';
	char temperature[14];
	float value;
	char unit;
	ss >> temperature >> value >> unit;

	ASSERT_EQ(0, strcmp(temperature, "temperature: "));
	ASSERT_EQ(23.2f, value);
	ASSERT_EQ('C', unit);
}

TEST(chunkstream, access_multiple_chunks) {
	fb::chunkstream<5> ss;
	ss << int32_t(0) << int(1);
	int32_t out1, out2;
	ss >> out1 >> out2;
	ASSERT_EQ(0, out1);
	ASSERT_EQ(1, out2);
}

TEST(chunkstream, read_write_alternate) {
	fb::chunkstream<5> ss;
	int32_t out;
	ss << int32_t(0) << int32_t(1);
	ss >> out;
	ASSERT_EQ(0, out);
	ss << int32_t(2);
	ss >> out;
	ASSERT_EQ(1, out);
	ss >> out;
	ASSERT_EQ(2, out);
}

TEST(read_out_of_bounds, over_border_quiet) {
	fb::chunkstream<5, false> ss;
	ss << int32_t(0);
	int32_t out;
	ss >> out;
	ASSERT_NO_THROW(ss >> out);
}

TEST(read_out_of_bounds, over_border_throws) {
	fb::chunkstream<5> ss;
	ss << int32_t(0);
	int32_t out;
	ss >> out;
	ASSERT_THROW(ss >> out, std::runtime_error);
}

TEST(read_out_of_bounds, over_border_exact_quiet) {
	fb::chunkstream<4, false> ss;
	ss << int32_t(0);
	int32_t out;
	ss >> out;
	ASSERT_NO_THROW(ss >> out);
}

TEST(read_out_of_bounds, over_border_exact_throws) {
	fb::chunkstream<4> ss;
	ss << int32_t(0);
	int32_t out;
	ss >> out;
	ASSERT_THROW(ss >> out, std::runtime_error);
}

TEST(read_out_of_bounds, in_chunk_quiet) {
	fb::chunkstream<1024, false> ss;
	ss << int32_t(0);
	int32_t out;
	ss >> out;
	ASSERT_NO_THROW(ss >> out);
}

TEST(read_out_of_bounds, in_chunk_throws) {
	fb::chunkstream<1024> ss;
	ss << int32_t(0);
	int32_t out;
	ss >> out;
	ASSERT_THROW(ss >> out, std::runtime_error);
}

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}