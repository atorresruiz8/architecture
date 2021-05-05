#include "tips.h"
#include <stdbool.h>
#include <limits.h>

/* The following two functions are defined in util.c */

/* finds the highest 1 bit, and returns its position, else 0xFFFFFFFF */
unsigned int uint_log2(word w); 

/* return random int from 0..x-1 */
int randomint( int x );

/*
  This function allows the lfu information to be displayed

    assoc_index - the cache unit that contains the block to be modified
    block_index - the index of the block to be modified

  returns a string representation of the lfu information
 */
char* lfu_to_string(int assoc_index, int block_index)
{
  /* Buffer to print lfu information -- increase size as needed. */
  static char buffer[9];
  sprintf(buffer, "%u", cache[assoc_index].block[block_index].accessCount);

  return buffer;
}

/*
  This function allows the lru information to be displayed

    assoc_index - the cache unit that contains the block to be modified
    block_index - the index of the block to be modified

  returns a string representation of the lru information
 */
char* lru_to_string(int assoc_index, int block_index)
{
  /* Buffer to print lru information -- increase size as needed. */
  static char buffer[9];
  sprintf(buffer, "%u", cache[assoc_index].block[block_index].lru.value);

  return buffer;
}

/*
  This function initializes the lfu information

    assoc_index - the cache unit that contains the block to be modified
    block_number - the index of the block to be modified

*/
void init_lfu(int assoc_index, int block_index)
{
  cache[assoc_index].block[block_index].accessCount = 0;
}

/*
  This function initializes the lru information

    assoc_index - the cache unit that contains the block to be modified
    block_number - the index of the block to be modified

*/
void init_lru(int assoc_index, int block_index)
{
  cache[assoc_index].block[block_index].lru.value = 0;
}

/* Steps to Run
Open Xming
'make clean'
'make'
'export DISPLAY=:0'
'./tips'
 */

// Helper functions
TransferUnit retrieve() {
  if (block_size == 4) {
    return WORD_SIZE; // regular sized word, return default
  } else if (block_size == 8) {
    return DOUBLEWORD_SIZE; // double the regular sized word
  } else if (block_size == 16) {
    return QUADWORD_SIZE; // four times the regular sized word
  } else {
    return OCTWORD_SIZE; // only remaining option, eight times the regular size word
  }

  // default
  return WORD_SIZE;
}

int replacement(unsigned int index) {
  int replace; // used to tell us where we're going to replace a block
  switch(policy) { // cache replacement policy
    case 0: { // random
      replace = randomint(assoc); // Generate a random number based on set associativity
      return replace; // return the index
    }

    case 1: { // LRU
      //unsigned minimum = cache[0].block[index].accessCount; // initialize a minimum count check to 
      unsigned minimum = INT_MAX; // initalize a minimum value to INT MAX to use for comparing counts
      for (int i = 1; i < assoc; i++) { // can't have 0 associativity, so loop through starting at 1
        if (minimum > cache[index].block[i].accessCount) { // compare by scaling minimum down sequentially based on count
          minimum = cache[index].block[i].accessCount;
          replace = i; // the index i is where the least recently used block is, return so we know which it is
        }
      } return replace; // return the index
    }

    case 2: { //LFU
    // accounted for in the switch statement, but left blank as we're only focusing on LRU and Random replacement policies
      return replace;
    }
  }
}

void updateLRUReplace(unsigned int index, int replace) {
  cache[index].block[replace].accessCount = 1;
  cache[index].block[replace].lru.value = 1;
}

void updateLRU(unsigned int index, int i) {
  cache[index].block[i].accessCount = 1;
  cache[index].block[i].lru.value = 1;
}

void updateValidBit(unsigned int index, int replace, unsigned int tag) {
  cache[index].block[replace].valid = VALID; // there is now data in the block, so update its valid bit
  cache[index].block[replace].tag = tag;
}

/*
  This is the primary function you are filling out,
  You are free to add helper functions if you need them

  @param addr 32-bit byte address
  @param data a pointer to a SINGLE word (32-bits of data)
  @param we   if we == READ, then data used to return
              information back to CPU

              if we == WRITE, then data used to
              update Cache/DRAM
*/
void accessMemory(address addr, word* data, WriteEnable we)
{
  /* Declare variables here */
  unsigned int offset;
  unsigned int index;
  unsigned int tag;
  address temp = addr;
  bool hit = false;
  //unsigned int* bytesAddr = convertBytes(addr);

  /* handle the case of no cache at all - leave this in */
  if(assoc == 0) {
    accessDRAM(addr, (byte*)data, WORD_SIZE, we);
    return;
  }

  /*
  You need to read/write between memory (via the accessDRAM() function) and
  the cache (via the cache[] global structure defined in tips.h)

  Remember to read tips.h for all the global variables that tell you the
  cache parameters

  The same code should handle random, LFU, and LRU policies. Test the policy
  variable (see tips.h) to decide which policy to execute. The LRU policy
  should be written such that no two blocks (when their valid bit is VALID)
  will ever be a candidate for replacement. In the case of a tie in the
  least number of accesses for LFU, you use the LRU information to determine
  which block to replace.

  Your cache should be able to support write-through mode (any writes to
  the cache get immediately copied to main memory also) and write-back mode
  (and writes to the cache only gets copied to main memory when the block
  is kicked out of the cache.

  Also, cache should do allocate-on-write. This means, a write operation
  will bring in an entire block if the block is not already in the cache.

  To properly work with the GUI, the code needs to tell the GUI code
  when to redraw and when to flash things. Descriptions of the animation
  functions can be found in tips.h
  */

  /* Start adding code here */
  if (block_size == 4) { // the max offset for a block of 4 bytes is 3
    offset = 0x3 & temp; // offset is the last 3 bits of temp
    temp = temp >> 2; 
  } else if (block_size == 8) { // the max offset for a block of 8 bytes is 7
    offset = 0x7 & temp; // offset is the last 7 bits of temp
    temp = temp >> 3;
  } else if (block_size == 16) { // the max offset for a block of 16 bytes is F, hex for 15
    offset = 0xF & temp; // offset is the last 15 bits of temp
    temp = temp >> 4;
  } else if (block_size == 32) { // the max offset for a block of 32 bytes is 1F, hex for 31
    offset = 0x1F & temp; // offset is the last 31 bits of temp
    temp = temp >> 5;
  }

  if (set_count == 1) {
    index = 0;
  } else if (set_count == 2) {
    index = 0x1 & temp;
    temp = temp >> 1;
  } else if (set_count == 4) {
    index = 0x3 & temp;
    temp = temp >> 2;
  } else if (set_count == 8) {
    index = 0x7 & temp;
    temp = temp >> 3;
  } else if (set_count == 16) {
    index = 0xF & temp;
    temp = temp >> 4;
  }

  tag = temp;

  switch (we) {
    case READ:
      if (memory_sync_policy == WRITE_THROUGH) { // Write Through policy, update word in cache block and corresponding word in memory
        // Check to see if addr exists in the cache
        for (int i = 0; i < assoc; i++) {
          if (cache[index].block[i].valid == VALID) { // only check valid blocks as invalid ones will not have data to begin with
            if (tag == cache[index].block[i].tag) { // if the tags match, then we have a hit
              hit = true;
              highlight_offset(index, i, offset, HIT); // highlight in green the corresponding word
              memcpy(data, cache[index].block[i].data + offset, 4); // Read data from block. 4 is because 4 bytes = word
              updateLRU(index, i);
              break; // exit the loop
            }
          }
        } if (!hit) { // handle misses
          TransferUnit mode = retrieve(); // We're going to transfer a block from DRAM into the cache
          accessDRAM(addr, data, mode, READ); // Read data from DRAM and store it in the data variable
          int replace = replacement(index); // according to the replacement policy, find where we're going to replace a block
          highlight_offset(index, replace, offset, MISS); // highlight in red the corresponding word
          memcpy(cache[index].block[replace].data, data, block_size); // copy into the cache the data we got from DRAM

          // Update the block to show data is now it, along with setting its LRU information
          updateValidBit(index, replace, tag);
          updateLRUReplace(index, replace);
        }
        return; // end
      } else if (memory_sync_policy == WRITE_BACK) { // Write Back policy, only update word in cache block
        // Check if addr exists in the cache
        for (int i = 0; i < assoc; i++) {
          if (cache[index].block[i].valid == VALID) { // only check valid blocks as invalid ones will not have data to begin with
            if (tag == cache[index].block[i].tag) { // if the tags match, then we have a hit
              hit = true;
              highlight_offset(index, i, offset, HIT); // highlight in green the corresponding word
              memcpy(data, cache[index].block[i].data + offset, 4); // Read data from block. 4 is because 4 bytes = word
              updateLRU(index, i);
              break; // exit the loop
            }
          }
      } if (!hit) { // handle misses
        int replace = replacement(index); // return according to policy
        if (cache[index].block[replace].dirty == DIRTY) { // Add a dirty bit to each block showing memory needs to be updated when we replace blocks
          byte* block_data; // initialize a byte to store block data
          memcpy(block_data, cache[index].block[replace].data, block_size);
          TransferUnit mode = retrieve(); // Transfer a block into DRAM
          accessDRAM(addr, data, mode, WRITE); // Write data to memory
        } TransferUnit mode = retrieve(); // now we are going to transfer from DRAM to the cache
        accessDRAM(addr, data, mode, READ); // Read data from DRAM into the data variable
        highlight_offset(index, replace, offset, MISS); // highlight in red the corresponding word
        memcpy(cache[index].block[replace].data, data, block_size); // Copy what's in the data variable into the cache

        // Update the block to show data is now in it, along with setting its LRU information
        updateValidBit(index, replace, tag);
        updateLRUReplace(index, replace);

        // The data in our cache now reflects what is in the memory, so the block is no longer 'dirty.' It is 'virgin' now.
        cache[index].block[replace].dirty = VIRGIN;
      }
      return; // exit
  }

  case WRITE: 
    if (memory_sync_policy == WRITE_THROUGH) {
      // Check if addr exists in the cache and write to it
      for (int i = 0; i < assoc; i++) {
        if (cache[index].block[i].valid == VALID) { // Only check valid blocks as the others won't have data in them
          if (tag == cache[index].block[i].tag) { // if tags match, we have a hit
            highlight_offset(index, i, offset, HIT);
            memcpy(cache[index].block[i].data + offset, data, 4); // Write data into block
            updateLRU(index, i);
            memcpy(data, cache[index].block[i].data, block_size); // data now holds what's inside the block
          }
        }
      } // If we miss, write directly to the memory (DRAM)
      TransferUnit mode = retrieve(); // we're transfering a block to DRAM
      accessDRAM(addr, data, mode, WRITE); // write data to the memory
      return; // exit
    } else if (memory_sync_policy == WRITE_BACK) {
      // Check if addr exists in the cache and write to it
      for (int i = 0; i < assoc; i++) {
        if (cache[index].block[i].valid == VALID) { // Only check valid blocks
          if (tag == cache[index].block[i].tag) { // if tags match, we have a hit
            hit = true;
            highlight_offset(index, i, offset, HIT); // highlight in green the corresponding word
            memcpy(cache[index].block[i].data + offset, data, 4); // Write data into block
            updateLRU(index, i);
            cache[index].block[i].dirty = DIRTY; // what's in the cache does not match what's in memory, so we have a dirty block
          }
        }
      } if (!hit) { // handle misses
          int replace = replacement(index); // return according to policy
          highlight_offset(index, replace, offset, MISS); // highlight in red the corresponding word
          if (cache[index].block[replace].dirty == DIRTY) { // Add a dirty bit to each block showing memory needs to be updated when we replace blocks
          byte* block_data; // get block data
          memcpy(block_data, cache[index].block[replace].data, block_size);
          TransferUnit mode = retrieve(); // Transfer block to DRAM
          accessDRAM(addr, block_data, mode, WRITE); // Write data to DRAM
          } memcpy(cache[index].block[replace].data + offset, data, 4); // Copy data into cache

          // Update block to show data is now in it, along with setting its LRU information
          updateLRUReplace(index, replace);
          updateValidBit(index, replace, tag); // there is now data in the block, so update the valid bit
          cache[index].block[replace].dirty = DIRTY; // data in cache does not match data in memory
      }
      return; // exit
    }
  }

  /* This call to accessDRAM occurs when you modify any of the
     cache parameters. It is provided as a stop gap solution.
     At some point, ONCE YOU HAVE MORE OF YOUR CACHELOGIC IN PLACE,
     THIS LINE SHOULD BE REMOVED.
  */
  // accessDRAM(addr, (byte*)data, WORD_SIZE, we);
}
