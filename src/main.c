/**
 ******************************************************************************
 * @file    main.c
 * @author  Ac6
 * @version V1.0
 * @date    01-December-2013
 * @brief   Default main function.
 ******************************************************************************
 */

#include "stm32f4xx.h"
#include "stm32f4xx_nucleo.h"

void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct;

	/* GPIO Ports Clock Enable */
	__GPIOC_CLK_ENABLE()
	;
	__GPIOD_CLK_ENABLE()
	;
	__GPIOA_CLK_ENABLE()
	;
	__GPIOB_CLK_ENABLE()
	;
	/*Configure GPIOC pins; D4 - D7 = PC8 - PC11*/
	GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIOB Pins; RS, RW, En = PB0 - PB2*/
	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}
void numberToGpio(char number) // change a 4 bit number into gpio outputs
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, 0);	//reset pins

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, 0);

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, 0);

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, 0);
	if (number >= 8)

	{

		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, 1);

		number = number - 8;

	}

	if (number >= 4)

	{

		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, 1);

		number = number - 4;

	}

	if (number >= 2)

	{

		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, 1);

		number = number - 2;

	}

	if (number >= 1)

	{

		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, 1);

	}
}

void send4bitCommand(char number)	// send a 4 bit command
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 0);	//rs=0 ==> cmd

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 0);	//R/W=O ==> write

	HAL_Delay(5);

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 1);	//En=1

	HAL_Delay(5);

	numberToGpio(number);

	HAL_Delay(5);

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 0);	//En=0

	HAL_Delay(5);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, 0);	//reset pins

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, 0);

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, 0);

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, 0);
}

void sendCommand(char number) {
	char lower = number & 0x0F;

	char higher = (number >> 4) & 0x0F;

	send4bitCommand(higher);

	send4bitCommand(lower);
}

void sendData(char number) {
	char lower = number & 0x0F;

	char higher = (number >> 4) & 0x0F;

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 1);	//rs=1 ==> data

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 0);	//R/W=O ==> write

	HAL_Delay(5);

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 1);	//En=1

	HAL_Delay(5);

	numberToGpio(higher);

	HAL_Delay(5);

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 0);	//En=0

	HAL_Delay(5);

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, 0);	//reset pins

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, 0);

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, 0);

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, 0);

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 1);	//rs=1 ==> data

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 1);	//R/W=O ==> write

	HAL_Delay(5);

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 1);	//En=1

	HAL_Delay(5);

	numberToGpio(lower);

	HAL_Delay(5);

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 0);	//En=0

	HAL_Delay(5);

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, 0);	//reset pins

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, 0);

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, 0);

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, 0);
}

void initLCD() {
//init sequence

	HAL_Delay(100);

	send4bitCommand(0x3);

	HAL_Delay(5);

	send4bitCommand(0x3);// no need to add delay, since i already added 1sec delay in the sendCommadn function

	send4bitCommand(0x3);

	send4bitCommand(0x2);	//function set 4 bit

	sendCommand(0x28);	//function set 4 bit 2 lines, 5\*8

	sendCommand(0x08);//forced to put 0x08 for the initialization ==> display off

	sendCommand(0x01);	//clear

	HAL_Delay(5);

	sendCommand(0x06);	//entry mode set incremente et shift cursor

//display control function to put the display on

	sendCommand(0x0F);//the "real" cursor or display shift: Display, cursor and blink on
	HAL_Delay(5);
}

void sendString(char *str) {
	while (*str)
		sendData(*str++);
}
int main(void) {
	HAL_Init();
	MX_GPIO_Init();
	//SystemClock_Config(); If you want to see this as well, let me know
	initLCD();
	sendString("test");
	while (1) {

	}
}
