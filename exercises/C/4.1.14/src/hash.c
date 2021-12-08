#include <stdio.h>
#include "../include/hash.h"
#include <string.h>
#include <limits.h>


void debug(uint32_t hash, uint32_t blocks, uint32_t block, int start, int end, int charIndex) {
  printf("-----------------------------------\n");
  printf("hash = %08x %i : blocks = %u\n",hash,hash,blocks);
  printf("block = %08x\n",block);
  printf("startIndex: %i\n",start);
  printf("endIndex: %i\n",end);
  printf("charIndex: %i\n",charIndex);
  printf("-----------------------------------\n"); 
  return;
}

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
uint32_t hash (char * s, size_t slen) {

  if(s==NULL) { return 0; }
  
  //debug
  /* printf("s: %s\n", s); */
  //debug
  
  int r = slen % BLOCKSIZE;
  if(r != 0) {
    int newslen = slen+(BLOCKSIZE-r);
    char s2[newslen];
    strcpy(s2,s);
    for(int i = slen; i < newslen; i++) {
      s2[i] = '\0';
    }
    slen = newslen;
    s = s2;
  }

  uint32_t hash = UINT_MAX;
  uint32_t blocks = slen/BLOCKSIZE;
  uint32_t block,k = 0;
  int start,end,charIndex = 0;
  for(int i = 0; i < blocks; i++) {
    start = i*BLOCKSIZE;
    end = (i+1)*BLOCKSIZE;
    block = 0;
    for(int j = 0; j < BLOCKSIZE; j++) {
      charIndex = j+start;
      hash += (int)s[charIndex]; //add byte to hash, store result as new hash
      k = (int)s[charIndex] << ((BLOCKSIZE-j-1)*8); // write byte to block
      block += k;
      //debug(hash, blocks, block, start, end, charIndex);
    }
    hash ^= block; //xor block with hash
  }

  //debug
  /* debug(hash, blocks, block, start, end, charIndex); */
  /* printf("s: %s\n", s); */
  //debug
  
  return hash;
}
