#include "utils.h"

int (util_sys_inb)(int port, uint8_t *value) {
  uint32_t val = 0;
  if(sys_inb(port, &val)){
	  return 1;
  }
  
  *value = (uint8_t) val; //Dereferencia��o do pointer
  return 0;
}

int maxInt(int a, int b) {
	if (a > b)
		return a;
	return b;
}

uint32_t BCDToDecimal(uint32_t BCD) {
  return (((BCD >> 4) * 10) + (BCD & 0xF));
}
