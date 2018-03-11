#include <cassert>
#include <limits>

#include <crypto++/sha.h>

#include "block.h"

using CryptoPP::SHA256;

void increment_big_block_array(Big_block_array &arr) {
    uint8_t carry = 1;
    int i = 0;
    int size = arr.data.size();
    while (carry && i < size) {
        uint8_t &num = (uint8_t&)arr.data[i];
        if (num == 0xFF) {
            num = 0;
        }
        else {
            num += carry;
            carry = 0;
        }
        i++;
    }
    if (carry) {
        arr.data.push_back(uint8_t(1));
    }
}

void set_big_block_array(Big_block_array &arr, uint64_t const &num) {
    uint64_t num_local = num;
    arr.data.clear();
    while (num_local) {
        arr.data.push_back(0xFF & num_local);
        num_local >>= 8;
    }
}

void get_big_block_array(uint64_t &num, Big_block_array const &arr) {
    int size = arr.data.size();
    assert(size <= sizeof(uint64_t));
    num = 0;
    for (uint32_t i = 0; i < size; i++) {
        num |= uint64_t(arr.data[i]) << (8 * i);
    }
}

void deserialize_big_block_array(Big_block_array &x, byte* const data) {
    uint8_t dataLength0 = data[0];
    assert(dataLength0 > 0);
    x.data.resize(dataLength0);
    memcpy(x.data.data(), data + 1, dataLength0);
}

void serialize_big_block_array(byte *&vecPtr, const Big_block_array &x) {
    uint64_t size = x.data.size();
    assert(size < std::numeric_limits<uint8_t>::max());
    if (size == 0) {
        vecPtr = new byte[2];
        vecPtr[0] = 1;
        vecPtr[1] = 0;
        return;
    }
    vecPtr = new byte[size + 1];
    vecPtr[0] = size;
    memcpy(vecPtr + 1, x.data.data(), size);
}

uint64_t block_milliseconds(Block block) {
    // TODO: Handle longer timestamps
    uint8_t timestampLength = block.timestamp.data.size();
    assert(timestampLength <= sizeof(uint64_t));
    uint64_t res = 0;
    for (int i = 0; i < timestampLength; i++) {
        res += uint64_t(block.timestamp.data[i]) << (i * 8);
    }
    return res;
}

void serialize_block(byte *data, const Block &block) {
    // make room for magic number and previous block hash
    unsigned int dataLength = 36;

    // make room for timestamp
    uint8_t timestampLength0 = block.timestamp.data[0];
    assert(timestampLength0 > 0);
    dataLength += 1 + timestampLength0;

    // make room for nonce
    uint8_t nonceLength0 = block.nonce.data[0];
    assert(nonceLength0 > 0);
    dataLength += 1 + nonceLength0;
}

void hash_block(byte *hash, const Block &block) {
    byte data[72];

    memcpy(data, block.magicNumber, 4);
    memcpy(data + 4, block.hashPrevBlock, 32);

    unsigned int dataLength = 36; // thirty six, counted 'em myself

    assert(false);
    // uint8_t timestampBytes = block.timestamp.data[0];
    // // TODO: Handle longer timestamps
    // assert(timestampBytes > 0 && timestampBytes < 4);
    // memcpy(data + dataLength, block.timestamp.data.data(), timestampBytes);
    // dataLength += timestampBytes;

    // uint8_t nonceBytes = block.nonce.length[0];
    // // TODO: Handle longer nonces
    // assert(nonceBytes > 0 && nonceBytes < 32);
    // memcpy(data + dataLength, block.nonce.data.data(), nonceBytes);
    // dataLength += nonceBytes;

    // SHA256().CalculateDigest(hash, data, dataLength);
}
