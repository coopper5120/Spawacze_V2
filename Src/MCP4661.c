/*
 * MCP4661.c
 *
 *  Created on: 16 cze 2019
 *      Author: Bartek
 */
#include "MCP4661.h"

const uint8_t Rw_value[6][2] = {
		{100,100},
		{100,100},
		{100,100},
		{100,100},
		{100,100},
		{100,100}
}; //@TODO - to be filled in properly!!!

// ADRESSES
const uint8_t MCP4661_addr[6] = {
		0x50,
		0x54,
		0x5C,
		0x58,
		0x56,
		0x52
};

inline static uint16_t ceil_f(float num)
{
	uint16_t int_num = (uint16_t)num;
	if(int_num != num)
		return num + 1;
	return num;
}

void MCP4661_IncrementWiper(uint8_t pot_number, uint8_t wiper_index)
{
	uint8_t cmd;

	cmd = (wiper_index << 4) | (INCREMENT_CMD << 2);
	HAL_I2C_Master_Transmit_DMA(&hi2c2, MCP4661_addr[pot_number], &cmd, 1);
}

void MCP4661_DecrementWiper(uint8_t pot_number, uint8_t wiper_index)
{
	uint8_t cmd;

	cmd = (wiper_index << 4) | (DECREMENT_CMD << 2);
	HAL_I2C_Master_Transmit_DMA(&hi2c2, MCP4661_addr[pot_number], &cmd, 1);
}

void MCP4661_SetWiper(uint16_t value, uint8_t pot_number, uint8_t wiper_index)
{
		uint8_t cmd[2];
		if(value > STEPS_NUM) //if value given is higher than maximal number of steps
		{
			value = STEPS_NUM;
		}
		cmd[0] = ((value & 0x100) >> 8); //MSB is sent at LSB of mode command, fig. 7.2 at p.59 in MCP4661 datasheet
		if(wiper_index == 0)
			cmd[0] |= (VOLATILE_WIPER_0_ADDR<<4) | (WRITE_CMD<<2);
		else
			cmd[0] |= (VOLATILE_WIPER_1_ADDR<<4) | (WRITE_CMD<<2);

		cmd[1] = value;
		HAL_I2C_Master_Transmit(&hi2c2, MCP4661_addr[pot_number], cmd, 2,1);
}

uint16_t MCP4661_ReadWiper(uint8_t pot_number, uint8_t wiper_index)
{
	uint8_t cmd;
	uint8_t buf[2];
	if(wiper_index == 0)
		cmd = (VOLATILE_WIPER_0_ADDR<<4) | (READ_CMD<<2);
	else
		cmd = (VOLATILE_WIPER_1_ADDR<<4) | (READ_CMD<<2);

	HAL_I2C_Mem_Read(&hi2c2, MCP4661_addr[pot_number], cmd, I2C_MEMADD_SIZE_8BIT, buf, 2,1);
	return buf[1] | ((buf[0] & 0x01) << 8);
}

void MCP4661_SetResistance(uint16_t value, uint8_t pot_number, uint8_t wiper_index)
{
		uint16_t wiper_value;
		if(value <= Rw_value[pot_number][wiper_index]) //if you want to set less than null offset
		{
			wiper_value = 0;
		}
		else if (value >= RESISTANCE) //if you want to set more than maximal resistance
		{
			wiper_value = STEPS_NUM;
		}
		else
		{
			wiper_value = (uint16_t)ceil_f(STEPS_NUM * ((float)(value - Rw_value[pot_number][wiper_index]) / RESISTANCE));
		}
		MCP4661_SetWiper(wiper_value, pot_number, wiper_index);
}

uint16_t MCP4661_ReadResistance(uint8_t pot_number, uint8_t wiper_index)
{
	uint16_t value_read;

	value_read = MCP4661_ReadWiper(pot_number, wiper_index);
	return (uint16_t)((((uint32_t)value_read * RESISTANCE) / STEPS_NUM) + Rw_value[pot_number][wiper_index]);

}

