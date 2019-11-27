#include "VBE.h"
#include <lcom/lcf.h>

void *video_mem; /* frame-buffer VM address */
static uint16_t h_res;
static uint16_t v_res;
static uint8_t bits_per_pixel;
static uint16_t graphics_mode;
static uint8_t RedMaskSize;
static uint8_t RedFieldPosition;
static uint8_t GreenMaskSize;
static uint8_t GreenFieldPosition;
static uint8_t BlueMaskSize;
static uint8_t BlueFieldPosition;
static uint8_t MemoryModel;


void draw_xpm(uint16_t x, uint16_t y, enum xpm_image_type type, xpm_image_t img){
	
	uint8_t *aux = video_mem;
	unsigned Bps = (bits_per_pixel + 7) >> 3;
	uint16_t lim_x = h_res;
	uint16_t lim_y = v_res;
	uint8_t counter = 0;
	
	if(x + img.width <  h_res)
		lim_x = x + img.width;
	if(y + img.height < v_res)
		lim_y = y + img.height;
	
	for(uint16_t i = y; i < lim_y; i++){
	  aux += (x + i * h_res) * (Bps);
	  for(uint16_t j = x; j < lim_x; j++){
		  counter = 0;
		  while(counter != (Bps)){
	
	   	  //aux = img.bytes[((i-x) + (j-y) * img.width)*(Bps) + counter];
	      counter++;
		  }
	  }
	}
	
	
}


int define_pattern(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step){ //Only for lab5, however, i'd rather break file conventions than have to use non static global variables, so it's got to be like this
  uint16_t width_each_rectangle = h_res / no_rectangles;
  uint16_t height_each_rectangle = v_res / no_rectangles;

  if (MemoryModel != 6 && MemoryModel != 7) {
    printf("We won't do that here ??");
    return 1;
  }

  else {
  
  uint32_t first_red = ((first >> RedFieldPosition) % (1 << RedMaskSize)) << RedFieldPosition;
  uint32_t first_blue = ((first >> BlueFieldPosition) % (1 << BlueMaskSize)) << BlueFieldPosition;
  uint32_t first_green = ((first >> GreenFieldPosition) % (1 << GreenMaskSize)) << GreenFieldPosition;

  uint32_t red;
  uint32_t blue;
  uint32_t green;
  uint32_t colour;
  int col = 0;
  int row = 0;

  for (int rec_y = 0; rec_y < height_each_rectangle * no_rectangles; rec_y += height_each_rectangle) { //Para cada retangulo em y
    for (int rec_x = 0; rec_x < width_each_rectangle * no_rectangles; rec_x += width_each_rectangle) { //Para cada retangulo em x
                                                                                                     //Define-se a cor do retangulo
      /*red = (((first_red + rec_x * step) >> RedFieldPosition) % (1 << RedMaskSize)) << RedFieldPosition;
      blue = (((first_blue + rec_y * step) >> BlueFieldPosition) % (1 << BlueMaskSize)) << BlueFieldPosition;
      green = (((first_green + ((rec_x + rec_y) * step)) >> GreenFieldPosition) % (1 << GreenMaskSize)) << GreenFieldPosition;*/
      
	  red = (((first_red + row * step) >> RedFieldPosition) % (1 << RedMaskSize)) << RedFieldPosition;
      blue = (((first_blue + col * step) >> BlueFieldPosition) % (1 << BlueMaskSize)) << BlueFieldPosition;
      green = (((first_green + ((row + col) * step)) >> GreenFieldPosition) % (1 << GreenMaskSize)) << GreenFieldPosition;
		
		
		
	  colour = red + blue + green;
	
	  for (int y = rec_y; y < rec_y + height_each_rectangle; y++) { //Para cada linha do ret�ngulo
        vg_draw_hline(rec_x, y, width_each_rectangle, colour);
	  }

    row++;
	}
    row = 0;
    col++;
  }
  col = 0;
  return 0;
  }
}

int (setGraphicsMode)(uint16_t mode) {
  graphics_mode = mode;

  //Initialize first MB of memory
  mmap_t map;
  map.phys = 0;
  map.size = 1024 * 1024;
  lm_alloc(1024 * 1024, &map);

  vbe_mode_info_t t;
  vbe_get_mode_info(mode, &t);
  
  bits_per_pixel = t.BitsPerPixel;
  h_res = t.XResolution;
  v_res = t.YResolution;
  RedMaskSize = t.RedMaskSize;
  RedFieldPosition = t.RedFieldPosition;
  GreenMaskSize = t.GreenMaskSize;
  GreenFieldPosition = t.GreenFieldPosition;
  BlueMaskSize = t.BlueMaskSize;
  BlueFieldPosition = t.BlueFieldPosition;
  MemoryModel = t.MemoryModel;
  
  unsigned int vram_base; /* VRAM�s physical addresss */
  vram_base = (unsigned int) t.PhysBasePtr;

  int sum = 0;
  if (bits_per_pixel % 8 != 0) {
    sum = 1;
  }


  int res;
  struct minix_mem_range mr; /* physical memory range */
  unsigned int vram_size = h_res * v_res * ((bits_per_pixel / 8) + sum); /* VRAM�s size, but you can use
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

  r.ax = 0x4F02;         // VBE call, function 02 -- set VBE mode
  r.bx = 1 << 14 | mode; // set bit 14: linear framebuffer
  r.intno = 0x10;
  if (sys_int86(&r) != OK) {
    printf("set_vbe_mode: sys_int86() failed \n");
    return 1;
  }
  return 0;
  
}

void(vg_fill32)(uint32_t color, uint32_t *ptr) {
  *ptr = color; //Preenche o pixel (para o caso bits_per_pixel == 32)
}
void(set_pixel)(uint32_t color, uint8_t *ptr, uint8_t Bytes_per_pixel) { //After the function, you've got to use ptr++
  *ptr = ((color >> (0)) % BIT(8));
  ptr++;

  if (Bytes_per_pixel >= 2) {
    *ptr = ((color >> (8)) % BIT(8));
    ptr++;
  }
  if (Bytes_per_pixel >= 3) {
    *ptr = ((color >> (16)) % BIT(8));
    ptr++;
  }
  if (Bytes_per_pixel == 4) {
    *ptr = ((color >> (24)) % BIT(8));
    ptr++;
  }
}
int(draw_hline)(uint16_t x, uint16_t y, uint16_t width, uint32_t color) {
  
	if (y > v_res) 
	{
		return 2;
	}
	
  unsigned endOfLine = h_res;
  if (x + width < endOfLine) {
    endOfLine = x + width;
  }
  unsigned Bps = (bits_per_pixel + 7) >> 3; //Bytes por pixel
  /*
  void *a = malloc(bps);
  void *current_point = a;
  current_point = video_mem;
  current_point += x + y + h_res;
  for (unsigned i = x; i < endOfLine; i++, current_point++) {
    current_point = color;
  }
  */
  if (bits_per_pixel <= 8) {
    uint8_t *current_point = video_mem;
    current_point += (x + y * h_res) * (Bps);
    for (unsigned i = x; i < endOfLine; i++, current_point++) {
      *current_point = color;
	}
  }
  else if (bits_per_pixel <= 16) {
    uint16_t *current_point = video_mem;
    current_point += x + y * h_res;
    for (unsigned i = x; i < endOfLine; i++, current_point++) {
      *current_point = color;
    }
  }
  else if (bits_per_pixel <= 24) {
    uint8_t *current_point = video_mem;
    current_point += (x + y * h_res) * Bps;
  //int count = 16;
    for (unsigned i = x; i < endOfLine; i++) {
      set_pixel(color, current_point, Bps);
      current_point += 3;
	  }
  }

  else if (bits_per_pixel <= 32) {
  //Se for de 32 bits_per_pixel
  uint32_t *current_point = video_mem;
  current_point+= x + y * h_res;
    for (unsigned i = x; i < endOfLine; i++, current_point++) {
      vg_fill32(color, current_point);
    }
  }
  return 0;
}


//Ir buscar info sobre modo grafico
//Allocar memoria 
//Mudar
//lcom_reboot_to_update
