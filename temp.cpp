#include "hardware/flash.h"
#include "pico/stdlib.h"
#include "hardware/irq.h"
#include "hardware/sync.h"

/ We're going to erase and reprogram a region 256k from the start of flash.
// Once done, we can access this at XIP_BASE + 256k.
#define FLASH_TARGET_OFFSET (256 * 1024)

const uint8_t *flash_target_contents = (const uint8_t *) (XIP_BASE + FLASH_TARGET_OFFSET);

int main(void)
{
  uint16_t     temp;
  uint8_t  resetcount_EEPROM[FLASH_PAGE_SIZE];

  // Keep a reset count
  temp = flash_target_contents[0] + (flash_target_contents[1] * 256);
  temp++;

  #ifdef PRINTF_DEBUG
  printf("Read EEPROM value = %d\r\n", temp);
  #endif

  #ifdef PRINTF_DEBUG
  printf("YES! Erasing FLASH EEPROM - before we can reprogram it...\r\n");
  #endif
  resetcount_EEPROM[0] = temp & 0xFF;
  resetcount_EEPROM[1] = ((temp & 0xFF00) >> 8);
  uint32_t ints = save_and_disable_interrupts();
  flash_range_erase(FLASH_TARGET_OFFSET, FLASH_SECTOR_SIZE);
  flash_range_program(FLASH_TARGET_OFFSET, resetcount_EEPROM, FLASH_PAGE_SIZE);
  restore_interrupts (ints);
  
  #ifdef PRINTF_DEBUG
  printf("Written back EEPROM value = %d\r\n", temp);
  #endif
}