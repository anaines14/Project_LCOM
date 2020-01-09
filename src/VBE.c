#include "VBE.h"
#include <lcom/lcf.h>

void *video_mem; /* frame-buffer VM address */

int (setGraphicsMode)(uint16_t mode) {
  
  //Initialize first MB of memory
  mmap_t map;
  map.phys = 0;
  map.size = 1024 * 1024;
  lm_alloc(1024 * 1024, &map);

  vbe_mode_info_t t;
  vbe_get_mode_info(mode, &t);
  
  unsigned int vram_base; /* VRAM�s physical addresss */
  vram_base = (unsigned int) t.PhysBasePtr;

  int sum = 0;
  if (BITS_PER_PIXEL % 8 != 0) {
    sum = 1;
  }

  int res;
  struct minix_mem_range mr; /* physical memory range */
  unsigned int vram_size = H_RES * V_RES * ((BITS_PER_PIXEL / 8) + sum); /* VRAM�s size, but you can use
  the frame-buffer size, instead */
  
  /* Allow memory mapping */
  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + vram_size;
  if (OK != (res = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))) //Pedir autoriza��o para aceder aos registos
    panic("sys_privctl (ADD_MEM) failed: %d\n", res);

  /* Map memory */
  video_mem = vm_map_phys(SELF, (void *) mr.mr_base, vram_size); //Onde se pode mudar as cores dos pixeis
  if (video_mem == MAP_FAILED)
	  panic("couldnt map video memory");
  
  //Set graphics mode
  reg86_t r;
  memset(&r, 0, sizeof(reg86_t)); //Faz set de r todo a 0

  r.ax = 0x4F02; // VBE call, function 02 -- set VBE mode
  r.bx = 1 << 14 | mode; // set bit 14: linear framebuffer
  r.intno = 0x10;
  if (sys_int86(&r) != OK) {
    printf("set_vbe_mode: sys_int86() failed \n");
    return 1;
  }
  return 0;
}
