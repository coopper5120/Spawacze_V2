/*
 * UART_Parser.c
 *
 *  Created on: 16 cze 2019
 *      Author: Bartek
 */

#include "UART_Parser.h"
#include "MCP4661.h"
#include <stdbool.h>
#include "main.h"
#include "adc.h"

volatile uint8_t receivedData;
uint8_t measureFlag = 0;
static parserstate_t parser_state = PARSED;
 uint8_t uartFrame[28];
extern uint32_t time = 0;


uint16_t test[11] = {1,2,3,4,5,6,7,8,9,10,11};///!!!

//uint8_t ok_msg[] = "OK!\n\r";

void UART_Parse(uint8_t input)
{
	uint8_t tmp[2];
	static uint8_t msg[3];
	//static uint8_t parser_lock = 0;
	static uint8_t cmd = 0xff;
	__disable_irq();


	if(parser_state == PARSED)
	{
		msg[0] = input;				// first byte / one byte command
		cmd = (msg[0] >> 4) & 0x0F;
		parser_state = CMD_OK;
	}

	if(parser_state == ONE_MORE)	// second byte (2 bytes command) / third byte (3 bytes command)
	{
		msg[2] = input;
		parser_state = READY;
	}

	if(parser_state == TWO_MORE)   // second byte (3 bytes command)
	{
		msg[1] = input;
		parser_state = ONE_MORE;
		goto exit;
	}

	switch(cmd)
	{
	case SET_DEFAULTS_CMD:
		break;
	case SET_GAIN_CMD:
		if(parser_state == CMD_OK)
		{
			parser_state = ONE_MORE;
			goto exit;
		}
		else if(parser_state == READY)
		{
			/*Setting gain option is intentionally disabled*/
//			uint8_t channelNumber = (msg[0] & 0x0F);
//			uint8_t potNumber = (channelNumber/2);
//			__enable_irq();
//			MCP4661_SetWiper(msg[2],potNumber,channelNumber%2);

			parser_state = PARSED;
		}
		break;
	case READ_GAIN_CMD:
		if(parser_state == CMD_OK)
		{
//				uint8_t channelNumber = (msg[0] & 0x0F)>>4;
			uint8_t channelNumber = (msg[0] & 0x0F);
			uint8_t potNumber = (channelNumber/2);
			__enable_irq();
			uint8_t wiperRead = MCP4661_ReadWiper(potNumber,channelNumber%2);
			if(!measureFlag)	HAL_UART_Transmit_IT(&huart1,&wiperRead,sizeof(wiperRead));
			parser_state = PARSED;
		}
		break;
	case REACALIBRATE_CMD:
		break;
	case SET_CHANNELS_CMD:
		if(parser_state == CMD_OK)
		{
			parser_state = ONE_MORE;
			goto exit;
		}
		else if(parser_state == READY)
		{
//			ADC_Mask = 0;
//			ADC_Mask = ((msg[0] & 0x0F)<<8) | msg[2];
//			__enable_irq();
//			if(!measureFlag)	HAL_UART_Transmit_IT(&huart1,ok_msg,sizeof(ok_msg));
//
			parser_state = PARSED;
		}
		break;
//	case READ_CHANNELS_CMD:
//		if(parser_state == READY)
//		{
//			tmp[0] = ADC_Mask>>8;
//			tmp[1] = (uint8_t)(ADC_Mask & 0x00FF);
//			__enable_irq();
//			if(!measureFlag)	HAL_UART_Transmit_IT(&huart1,tmp,2);
//			parser_state = PARSED;
//		}
//		break;
//	case ENABLE_DSP_CMD:
//		break;
//	case DISABLE_DSP_CMD:
//		break;
//	case SET_DSP_COEFF_CMD:
//		break;
	case START_MEAS_CMD:
		if(parser_state == CMD_OK)
		{
			measureFlag = 1;
//			UART_PrepareData();
//			HAL_UART_Transmit_IT(&huart1, uartValue, dataToSendAmount * 2);
			time = 0;
			parser_state = PARSED;
		}
		break;
	case STOP_MEAS_CMD:
		if(parser_state == CMD_OK)
		{
			__disable_irq();
			measureFlag = 0;
			parser_state = PARSED;
			__enable_irq();
		}
		break;
	}

//	if(parser_state == CMD_OK) parser_state = PARSED;		//!!!!

		exit:
		__enable_irq();
		asm("nop");
}

void UART_PrepareData()
{
	uartFrame[START_CHAR_POS] = START_CHAR;
	memcpy((void*)&uartFrame[TIME_POS], (void*)&time, sizeof(time));
	memcpy((void*)&uartFrame[MEASUREMENTS_POS], (void*)adcValue, sizeof(adcValue));
	uartFrame[STOP_CHAR_POS] = STOP_CHAR;

//		uartFrame[START_CHAR_POS] = START_CHAR;
//		time = 0;
//		memcpy((void*)&uartFrame[TIME_POS], (void*)&time, sizeof(time));
//
////		for(uint8_t i =0; i++; i<12) {	test[i] = i;}
//
//		memcpy((void*)&uartFrame[MEASUREMENTS_POS], (void*)test, sizeof(adcValue));
//
//		uartFrame[STOP_CHAR_POS] = STOP_CHAR;




}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	  UART_Parse(receivedData);
	  HAL_UART_Receive_IT(&huart1, &receivedData, 1);
}

/* USER CODE BEGIN 1 */

