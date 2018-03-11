#ifndef BLOCK_H
#define BLOCK_H

#include <cstdint>
#include <vector>

#include <crypto++/sha.h>

static unsigned char magicNumber[] = { 0x91, 0xF9, 0xCE, 0xB9 };

struct Big_block_array {
    std::vector<byte> data;
};

struct Block {
    byte magicNumber[4];
    byte hashPrevBlock[32];
    Big_block_array timestamp;
    Big_block_array nonce;
};

void increment_big_block_array(Big_block_array&);
void set_big_block_array(Big_block_array&, uint64_t const&);
void get_big_block_array(uint64_t&, Big_block_array const&);

// Serialize and deserialize Big_block_array values representing
// unsigned integers.  Serialization format is as follows:

// For 2048-bit integers, the format is:

// [length][data]

// Where [length] is a single nonzero byte giving the length in bytes
// of [data] and [data] contains the integer in little-endian format.

// For integers greater than 2048 bits and less than 16384 bits, the
// format is:

// [0][length_2][length][data]

// Where [length_2] is a single nonzero byte giving the length of
// [length], whose value as a little-endian unsigned integer gives the
// length of [data];

// This can be iterated indefinitely.

// It has been argued that iterations beyond the second will never be
// necessary for various physical reasons, including that size of the
// data section of [0][255][length][data] is well over 2^273, the
// number of atoms in the observable universe.  However, I would like
// to point out several factors.  First, each atom can be put into
// more than 2 states, so even though there are only 2^273 atoms, they
// can represent more than 2^273 numbers.  Second, atoms are composed
// of smaller particles which can be separated and worked with
// individually.  Third, there are also neutrinos, dark matter, and
// undiscovered laws of physics that may make it possible to store
// arbitrarily large numbers in the far future within the confines of
// our universe.  There's a difference between building for tomorrow,
// and building for the end of time.
void deserialize_big_block_array(Big_block_array&, byte* const);
void serialize_big_block_array(byte*&, const Big_block_array&);

uint64_t block_milliseconds(Block);
void hash_block(byte*, const Block&);

#endif
