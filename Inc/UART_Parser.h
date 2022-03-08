#include "usart.h"
#include "adc.h"


/*
 * COMMAND BYTE:
 * C3|C2|C1|C0|x|x|x|x|
 * C3:C0 :
 * 0000 - set default settings (gain, filter etc.) / reset board?
 * 0001 - set gain to particular channel
 * 0010 - read gain from particular channel
 * 0011 - start recalibration procedure
 * 0100 - set active channels
 * 0101 - read active channels
 * 0110 - enable DSP filter
 * 0111 - disable DSP filter
 * 1000 - set DSP coefficients
 * 1001 - start measures
 * 1010 - stop measures
 */

/*
 * SET GAIN COMMAND: (0001)
 * |0|0|0|1|CH3|CH2|CH1|CH0| |G7|G6|G5|G4|G3|G2|G1|G0|
 *
 * CH3:CH0 - channel number - FROM ZERO
 * G7:G0 - channel gain
 */

/*
 * READ GAIN COMMAND: (0010)
 * |0|0|1|0|CH3|CH2|CH1|CH0|
 * received:               |G7|G6|G5|G4|G3|G2|G1|G0|
 *
 * CH3:CH0 - channel number - FROM ZERO
 * G7:G0 - read channel gain
 */

/*
 * RECALIBRATE COMMAND: (0011)
 * |0|0|1|1|x|x|x|x|
 *
 * Starts internal ADC calibration procedure
 */

/*
 * SET ACTIVE CHANNELS COMMAND: (0100)
 * |0|1|0|0|CH12|CH11|CH10|CH9| |CH8|CH7|CH6|CH5|CH4|CH3|CH2|CH1|
 *
 * CH12:CH1 - channel bit mask
 *
 * Enables channels that have "1" on their position, otherwise disables them
 */

/*
 * READ ACTIVE CHANNELS COMMAND: (0101)
 * |0|1|0|1|x|x|x|x|
 * received:       |0|0|0|0|CH12|CH11|CH10|CH9| |CH8|CH7|CH6|CH5|CH4|CH3|CH2|CH1|
 * CH12:CH1 - channel bit mask
 *
 * Sends channels status: 1 - enabled 0 - disabled
 */

#define SET_DEFAULTS_CMD 0
#define SET_GAIN_CMD 1
#define READ_GAIN_CMD 2
#define REACALIBRATE_CMD 3
#define SET_CHANNELS_CMD 4
#define READ_CHANNELS_CMD 5
#define ENABLE_DSP_CMD 6
#define DISABLE_DSP_CMD 7
#define SET_DSP_COEFF_CMD 8
#define START_MEAS_CMD 9
#define STOP_MEAS_CMD 10

#define START_CHAR  '<'
#define STOP_CHAR	'>'

#define UART_FRAME_LENGTH	28
#define START_CHAR_POS		0
#define TIME_POS			1
#define MEASUREMENTS_POS	5
#define STOP_CHAR_POS		27

typedef enum parserstate {CMD_OK,READY,TWO_MORE,ONE_MORE,PARSED} parserstate_t;

extern uint8_t uartFrame[28];

extern volatile uint8_t receivedData;
extern uint8_t measureFlag;
extern uint8_t  dataToSendAmount;
extern uint32_t time;


//void UART_DMA_RxCompleted();
void UART_PrepareData();
void UART_Parse(uint8_t input);
