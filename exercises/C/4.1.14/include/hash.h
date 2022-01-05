#ifndef HASH_H
#define HASH_H

#include <stdint.h>
#include <stdlib.h>

#define BLOCKSIZE sizeof(uint32_t)

/*
 * Implement a hash function that uses the Like-Literally-A-Masterpiece-Algorithm (LLAMA) algorithm.
 * 
 * LLAMA:
 *      Given a string S and the length of that string
 *      If the length of the string is not a multiple of the BLOCKSIZE, pad the
 *      string with NULL bytes.
 *      Initialize the hash uint32_t max value.
 *      For each block of size uint32 in S:
 *          For each byte in block:
 *              Add byte to hash, store the result as the new hash.
 *          XOR the current hash with the current block, store the result
 *      Return the result as the hash.
 *
 * Consider the string "Hello" and a BLOCKSIZE of 4
 * Because the length of the string is 5 and 5 is not a multiple of 4,
 * we will pad the string so it becomes "Hello\x00\x00\x00"
 *
 * When XORing the block with the current hash, treat each block as if it were
 * a 32-bit integer. Suppose the current hash is 0xdeadbeef, and the current
 * block is "ABCD". Treating the block like an integer we get 0x41424344.
 * current hash XOR current block = 0xdeadbeef ^ 0x41424344 = 0x9feffdab
 * 
 * Notes:
 *   - This is not a cryptographic hashing algorithm
 *   - Collisions are okay for these purposes
 *   - Feel free to define helper functions
 */

/* @brief Hash function for the LLAMA algorithm.
 * @param s The input string
 * @param slen The length of the input string
 * @return uint32 hash value, 0 if input is a null pointer
 */
uint32_t hash (char * s, size_t slen);

#endif
