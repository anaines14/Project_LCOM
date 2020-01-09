#include "rtc.h"
#include <lcom/lcf.h>

uint32_t minutes = 0, hours = 0;

void wait_valid_rtc(void) {
  uint32_t regA = 0;
  do {
    //disable();
    sys_outb(RTC_ADDR_REG, 10);
    sys_inb(RTC_DATA_REG, &regA);
    //enable()
  } while (regA & RTC_UIP); //
}

int read_time() {
  wait_valid_rtc();

  if (sys_outb(RTC_ADDR_REG, READ_MIN))
    return 1;
  if (sys_inb(RTC_DATA_REG, &minutes))
    return 1;
  if (sys_outb(RTC_ADDR_REG, READ_HOURS))
    return 1;
  if (sys_inb(RTC_DATA_REG, &hours))
    return 1;
  hours = BCDToDecimal(hours);
  minutes = BCDToDecimal(minutes);

  return 0;
}

static int hook_id;
int(rtc_subscribe_int)(uint8_t *bit_no) {
  hook_id = (int) *bit_no;

  if (sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE, &hook_id))
    return 1;

  return 0;
}

int(rtc_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_id))
    return 1;
  return 0;
}

int set_alarm() { 
  if (sys_outb(RTC_ADDR_REG, ALARM_MIN))
    return 1;

  if (sys_outb(RTC_DATA_REG, ALARM_IGNORE_VALUE))
    return 1;
  if (sys_outb(RTC_ADDR_REG, ALARM_HOURS))
    return 1;
  if (sys_outb(RTC_DATA_REG, ALARM_IGNORE_VALUE))
    return 1;
  if (sys_outb(RTC_ADDR_REG, ALARM_SEC))
    return 1;
  if (sys_outb(RTC_DATA_REG, SECONDS))
    return 1;

  if (sys_outb(RTC_ADDR_REG, RTC_REG_B))
    return 1;
  
  uint32_t aux = 0;

  if (sys_inb(RTC_DATA_REG, &aux)) 
    return 1;
  
  if (sys_outb(RTC_DATA_REG, aux | BIT(5))) 
    return 1;
  
  uint8_t regC = 0;
  if (sys_outb(RTC_ADDR_REG, RTC_REG_C))  
        return 1;
      
  if(util_sys_inb(RTC_DATA_REG, &regC)) 
    return 1;
  
  if (sys_outb(RTC_ADDR_REG, RTC_REG_C)) 
    return 1;
  if (sys_outb(RTC_DATA_REG, regC | BIT(7)))
    return 1;

  if (sys_outb(RTC_ADDR_REG, RTC_REG_C)) 
    return 1;
  if (util_sys_inb(RTC_DATA_REG, &regC)) 
    return 1;
   
  return 0;
}

void rtc_ih(){
  minutes++;
  if (minutes % 60 == 0) 
    hours++;
  set_alarm();
}


