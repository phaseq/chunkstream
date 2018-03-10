#pragma once
#include <cstring>
#include <memory>

namespace fb {
template <size_t CHUNK_SIZE>
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
			memcpy(
				m_curChunk->data.get() + m_writePos,
				reinterpret_cast<const char*>(&val),
				sizeof(val));
			m_writePos += sizeof(val);
		} else {
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
		if (m_readPos + sizeof(val) <= ((m_curChunk == m_curChunkR) ? m_writePos : CHUNK_SIZE)) {
			memcpy(reinterpret_cast<char*>(&val), m_curChunkR->data.get() + m_readPos, sizeof(val));
			m_readPos += sizeof(val);
		} else {
			char* dst = reinterpret_cast<char*>(&val);
			char* end = dst + sizeof(val);
			size_t readBufferEnd = (m_curChunk == m_curChunkR) ? m_writePos : CHUNK_SIZE;
			size_t read = std::min(sizeof(val), readBufferEnd - m_readPos);
			memcpy(dst, m_curChunkR->data.get() + m_readPos, read);
			dst += read;
			do {
				m_curChunkR = m_curChunkR->next.get();
				readBufferEnd = (m_curChunk == m_curChunkR) ? m_writePos : CHUNK_SIZE;
				read = std::min(size_t(end - dst), readBufferEnd);
				memcpy(dst, m_curChunkR->data.get(), read);
				dst += read;
				m_readPos = read;
			} while (dst != end && m_curChunkR->next);
		}
		return *this;
	}

private:
	chunk m_firstChunk;
	chunk* m_curChunk;
	chunk* m_curChunkR;
	size_t m_readPos, m_writePos;
};
}  // namespace fb