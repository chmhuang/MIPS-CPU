#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory.h"

/* Pointer to simulator memory */
uint8_t *mem;

/* Called by program loader to initialize memory. */
uint8_t *init_mem() {
  assert (mem == NULL);
  mem = calloc(MEM_SIZE, sizeof(uint8_t)); // allocate zeroed memory
  return mem;
}

/* Returns 1 if memory access is ok, otherwise 0 */
int access_ok(uint32_t mipsaddr, mem_unit_t size) {
    //printf("mipsaddr = %u, size = %08x", mipsaddr, size);
    
    if (!((mipsaddr < MEM_SIZE) && (mipsaddr >= 1))) {
	return 0;
    } else if (mipsaddr == 0) {
	return 0;
    } else if (mipsaddr % size != 0) {
	return 0;
    } else if (!(size == 1 || size == 2 || size == 4)) {
	return 0;
    } else if (size + mipsaddr > MEM_SIZE) {
	return 0;
    }
    return 1;
    /* TODO YOUR CODE HERE */
}

/* Writes size bytes of value into mips memory at mipsaddr */
void store_mem(uint32_t mipsaddr, mem_unit_t size, uint32_t value) {
  if (!access_ok(mipsaddr, size)) {
    fprintf(stderr, "%s: bad write=%08x\n", __FUNCTION__, mipsaddr);
    exit(-1);
  }
  switch (size)
      {
      case SIZE_BYTE:
	  // memcpy(mipsaddr, &value, sizeof(uint8_t));
	  mem[mipsaddr] = ((uint8_t) value);
	  break;
	  
      case SIZE_HALF_WORD:
	  // memcpy(mipsaddr, &value, sizeof(uint8_t));
	  //value = value >> 8;
	  //memcpy((mipsaddr + 1), &value, sizeof(uint8_t));
	  mem[mipsaddr] = ((uint8_t) value);
	  mem[mipsaddr + 1] = ((uint8_t) (value >> 8));
	  break;

      default:
	  //memcpy(mipsaddr, &value, sizeof(uint8_t));
	  //value = value >> 8;
	  //memcpy((mipsaddr + 1), &value, sizeof(uint8_t));
	  //value = value >> 8;
	  //memcpy((mipsaddr + 2), &value, sizeof(uint8_t));
	  //value = value >> 8;
	  //memcpy((mipsaddr + 3), &value, sizeof(uint8_t));
	  mem[mipsaddr] = ((uint8_t) value);
	  mem[mipsaddr + 1] = ((uint8_t) (value >> 8));
	  mem[mipsaddr + 2] = ((uint8_t) (value >> 16));
	  mem[mipsaddr + 3] = ((uint8_t) (value >> 24));
      }
  /* TODO YOUR CODE HERE */

}

/* Returns zero-extended value from mips memory */
uint32_t load_mem(uint32_t mipsaddr, mem_unit_t size) {
  if (!access_ok(mipsaddr, size)) {
    fprintf(stderr, "%s: bad read=%08x\n", __FUNCTION__, mipsaddr);
    exit(-1);
  }
  switch (size)
      {
      case SIZE_BYTE:
	  return *(uint8_t*)(mem + mipsaddr);
	  break;
	  
      case SIZE_HALF_WORD:
	  return *(uint16_t*)(mem + mipsaddr);
	  break;

      default:
	  return *(uint32_t*)(mem + mipsaddr);
      }
  /* TODO YOUR CODE HERE */

  // incomplete stub to let mipscode/simple execute
  // (only handles size == SIZE_WORD correctly)
  // feel free to delete and implement your own way
  // ORIGINAL CODE return *(uint32_t*)(mem + mipsaddr);
}
