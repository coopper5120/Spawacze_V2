#include "i2c.h"

#define RESISTANCE 10000

#define STEPS_NUM 256

// MEASURED NULL OFFSET WIPER RESISTANCE TABLE [MCP_index][wiper_index]
extern const uint8_t Rw_value[6][2];

// ADRESSES TABLE
extern const uint8_t MCP4661_addr[6];

// MEMORY MAP
#define VOLATILE_WIPER_0_ADDR 0x00
#define VOLATILE_WIPER_1_ADDR 0x01
#define NONVOLATILE_WIPER_0_ADDR 0x02
#define NONVOLATILE_WIPER_1_ADDR 0x03
#define TCON_REG_ADDR 0x04
#define STATUS_REG_ADDR 0x05
#define EEPROM_1_ADDR 0x06
#define EEPROM_2_ADDR 0x07
#define EEPROM_3_ADDR 0x08
#define EEPROM_4_ADDR 0x09
#define EEPROM_5_ADDR 0x0A
#define EEPROM_6_ADDR 0x0B
#define EEPROM_7_ADDR 0x0C
#define EEPROM_8_ADDR 0x0D
#define EEPROM_9_ADDR 0x0E
#define EEPROM_10_ADDR 0x0F

// COMMANDS

#define WRITE_CMD 0
#define INCREMENT_CMD 1
#define DECREMENT_CMD 2
#define READ_CMD 3

void MCP4661_IncrementWiper(uint8_t pot_number, uint8_t wiper_index);
void MCP4661_DecrementWiper(uint8_t pot_number, uint8_t wiper_index);
void MCP4661_SetResistance(uint16_t value, uint8_t pot_number, uint8_t wiper_index);
uint16_t MCP4661_ReadResistance(uint8_t pot_number, uint8_t wiper_index);
void MCP4661_SetWiper(uint16_t value, uint8_t pot_number, uint8_t wiper_index);
uint16_t MCP4661_ReadWiper(uint8_t pot_number, uint8_t wiper_index);
