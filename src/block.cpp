#include <cassert>

#include <crypto++/sha.h>

#include "block.h"

using CryptoPP::SHA256;

void increment_big_block_array(Big_block_array *arr) {
    uint8_t carry = 1;
    int i = 0;
    int size = arr->data.size();
    while (carry && i < size) {
        uint8_t &num = (uint8_t&)arr->data[i];
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
        arr->data.push_back(uint8_t(1));
    }
}

void set_big_block_array(Big_block_array *arr, uint64_t num) {
    arr->data.clear();
    while (num) {
        arr->data.push_back(0xFF & num);
        num >>= 8;
    }
}

void get_big_block_array(uint64_t *num, const Big_block_array *arr) {
    uint32_t size = arr->data.size();
    assert(size <= 4);
    *num = 0;
    for (uint32_t i = 0; i < size; i++) {
        *num |= uint64_t(arr->data[i]) << (8 * i);
    }
}

void deserialize_big_block_array(Big_block_array *x, const std::vector<byte> *vecPtr) {
    const std::vector<byte> &vec = *vecPtr;
    x->data = {};
    if (vec.size() == 0) {
        return;
    }
    uint8_t length = uint8_t(vec[0]);
    assert(length > 0); // TODO: support representations of integers longer than 2048 bits
    for (uint8_t i = 0; i < length; i++) {
        x->data.push_back(vec[i + 1]);
    }
}

void serialize_big_block_array(std::vector<byte> *vecPtr, const Big_block_array *x) {
    std::vector<byte> &vec = *vecPtr;
    uint8_t size = x->data.size();
    assert(size < 256); // TODO: support representations of integers longer than 2048 bits
    if (size == 0) {
        vec = {0};
        size = 1;
    }
    else {
        vec = x->data;
    }
    vec.insert(vec.begin(), size);
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

void hash_block(byte *hash, Block block) {
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
