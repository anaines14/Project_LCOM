#include "utils.h"

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  *lsb = (uint8_t)(val % 256);
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  *msb = (uint8_t)(val / 256);
  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
  uint32_t val = 0;
  if(sys_inb(port, &val)){
	  return 1;
  }
  
  *value = (uint8_t) val; //Dereferencia��o do pointer
  return 0;
}

int maxInt(int a, int b) {
	if (a > b) {
		return a;
	}
	return b;
}

int minInt(int a, int b) {
	if (a < b) {
		return a;
	}
	return b;
}

