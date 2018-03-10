#pragma once
#include <cstring>
#include <memory>

namespace fb {
template <size_t CHUNK_SIZE, bool ENABLE_EXCEPTIONS = true>
class chunkstream {
	struct chunk {
		chunk() : data(new char[CHUNK_SIZE]) {}
		std::unique_ptr<char> data;
		std::unique_ptr<chunk> next;
	};

public:
	chunkstream()
		: m_curChunk(&m_firstChunk), m_curChunkR(&m_firstChunk), m_readPos(0), m_writePos(0) {}
	~chunkstream() {
		// If we would let the unique pointers expire we would trigger many recursive calls.
		// Instead, destruct manually.
		if (m_firstChunk.next) {
			std::unique_ptr<chunk> c;
			std::swap(c, m_firstChunk.next);
			while (c->next) {
				std::unique_ptr<chunk> c2;
				std::swap(c->next, c2);
				c = std::move(c2);
			}
		}
	}

	template <typename T>
	chunkstream& operator<<(const T& val) {
		if (m_writePos + sizeof(val) <= CHUNK_SIZE) {
			// This should be the usual case: the value fits in the current chunk. By specifying a
			// compile-time constant copy size the compiler can optimize this better.
			memcpy(
				m_curChunk->data.get() + m_writePos,
				reinterpret_cast<const char*>(&val),
				sizeof(val));
			m_writePos += sizeof(val);
		} else {
			// We will have to allocate a new chunk to fit all bytes.
			const char* src = reinterpret_cast<const char*>(&val);
			const char* end = src + sizeof(val);
			size_t sz = CHUNK_SIZE - m_writePos;
			memcpy(m_curChunk->data.get() + m_writePos, src, sz);
			src += sz;
			do {
				m_curChunk->next.reset(new chunk());
				m_curChunk = m_curChunk->next.get();
				size_t sz = std::min(size_t(end - src), CHUNK_SIZE);
				memcpy(m_curChunk->data.get(), src, sz);
				m_writePos = sz;
				src += sz;
			} while (src != end);
		}
		return *this;
	}

	template <typename T>
	chunkstream& operator>>(T& val) {
		if (m_readPos + sizeof(val) <= CHUNK_SIZE) {
			// This should be the usual case: the value can be read entirely from the  current
			// chunk. By specifying a compile-time constant copy size the compiler can optimize this
			// better.
			memcpy(reinterpret_cast<char*>(&val), m_curChunkR->data.get() + m_readPos, sizeof(val));
			m_readPos += sizeof(val);
		} else {
			// We will have to read the value from multiple chunks.
			char* dst = reinterpret_cast<char*>(&val);
			const char* end = dst + sizeof(val);
			const size_t sz = CHUNK_SIZE - m_readPos;
			memcpy(dst, m_curChunkR->data.get() + m_readPos, sz);
			dst += sz;
			do {
				if (!m_curChunkR->next) {
					if (ENABLE_EXCEPTIONS)
						throw std::runtime_error("out of bounds read!");
					else
						return *this;
				}
				m_curChunkR = m_curChunkR->next.get();
				const size_t sz = std::min(size_t(end - dst), CHUNK_SIZE);
				memcpy(dst, m_curChunkR->data.get(), sz);
				dst += sz;
				m_readPos = sz;
			} while (dst != end && m_curChunkR->next);
		}
		if (ENABLE_EXCEPTIONS && m_curChunk == m_curChunkR && m_readPos > m_writePos)
			throw std::runtime_error("out of bounds read!");
		return *this;
	}

private:
	chunk m_firstChunk;
	chunk* m_curChunk;
	chunk* m_curChunkR;
	size_t m_readPos, m_writePos;
};
}  // namespace fb