#include "types.h"
#include "riscv.h"
#include "defs.h"

static unsigned int rand_state = 1;

int
my_rand(void)
{
  rand_state = rand_state * 1103515245 + 12345 + ticks;
  return (int)((rand_state >> 16) & 0x7fff);
}
