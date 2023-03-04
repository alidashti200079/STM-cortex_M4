#include "stm32f3xx_hal.h"

void initForLights() {

	__HAL_RCC_GPIOD_CLK_ENABLE();
	GPIO_InitTypeDef temp1;
	temp1.Pin = GPIO_PIN_7 | GPIO_PIN_8 ;
	temp1.Mode = GPIO_MODE_OUTPUT_PP;
	temp1.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOD, &temp1);
}


void initForBlueButton() {

	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef temp1;
	temp1.Pin = GPIO_PIN_0;
	temp1.Mode = GPIO_MODE_INPUT;
	temp1.Speed = GPIO_SPEED_FREQ_HIGH;
	temp1.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOA, &temp1);
}


void initForSevenSeg() {

	__HAL_RCC_GPIOC_CLK_ENABLE();
	GPIO_InitTypeDef temp;
	temp.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
	temp.Mode = GPIO_MODE_OUTPUT_PP;
	temp.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOC, &temp);
}


void initForLED() {

	__HAL_RCC_GPIOE_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
}


void turnOnLights(int number) {

	switch (number) {
		case 0:
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, 0);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, 1);
			break;
		case 1:
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, 1);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, 0);
			break;
		case 2:
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, 0);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, 1);
			break;
		case 3:
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, 1);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, 0);
			break;
		case 4:
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, 0);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, 1);
			break;
		case 5:
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, 1);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, 0);
			break;
		case 6:
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, 0);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, 1);
			break;
		case 7:
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, 1);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, 0);
			break;
		case 8:
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, 0);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, 1);
			break;
		case 9:
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, 1);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, 0);
			break;
	};
}

void count(int number) {

	switch (number) {
		case 0:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5, 1);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, 0);

			break;
		case 1:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1 | GPIO_PIN_2, 1);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5, 0);
			break;
		case 2:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_6, 1);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2 | GPIO_PIN_5, 0);
			break;
		case 3:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_6, 1);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4 | GPIO_PIN_5, 0);
			break;
		case 4:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_5 | GPIO_PIN_6, 1);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0 | GPIO_PIN_3 | GPIO_PIN_4, 0);

			break;
		case 5:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_5 | GPIO_PIN_6, 1);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1| GPIO_PIN_4, 0);

			break;
		case 6:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6, 1);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1 , 0);
			break;
		case 7:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2, 1);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6, 0);
			break;
		case 8:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6, 1);
			break;
		case 9:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_5 | GPIO_PIN_6, 1);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4 , 0);
			break;
	};
	turnOnLights(number);
}

//void LED(int LNum) {
//
//	switch (LNum) {
//			case 0:
//				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15, 0);
//				break;
//			case 1:
//				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, 1);
//				break;
//			case 2:
//				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, 1);
//				break;
//			case 3:
//				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, 1);
//				break;
//			case 4:
//				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, 1);
//				break;
//			case 5:
//				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, 1);
//				break;
//			case 6:
//				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, 1);
//				break;
//			case 7:
//				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, 1);
//				break;
//			case 8:
//				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, 1);
//				break;
//			case 9:
//				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, 0);
//				break;
//			case 10:
//				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, 0);
//				break;
//			case 11:
//				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, 0);
//				break;
//			case 12:
//				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, 0);
//				break;
//			case 13:
//				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, 0);
//				break;
//			case 14:
//				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, 0);
//				break;
//			case 15:
//				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, 0);
//				break;
//			case 16:
//				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, 0);
//				break;
//		};
//}

void LED(int LNum) {

	switch (LNum) {
			case 1:
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, 1);
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, 0);
				break;
			case 2:
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, 1);
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, 0);
				break;
			case 3:
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, 1);
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, 0);
				break;
			case 4:
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, 1);
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, 0);
				break;
			case 5:
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, 1);
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, 0);
				break;
			case 6:
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, 1);
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, 0);
				break;
			case 7:
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, 1);
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, 0);
				break;
			case 8:
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, 1);
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, 0);
				break;
		};
}


int main () {

	HAL_Init();
	initForLights();
	initForBlueButton();
	initForSevenSeg();
	initForLED();

	int current_number = 0;
	int x = 1;
	int LNum = 0;

	while(1) {

			if(LNum == 9) {
				LNum = 1;
			}

			if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)) {

				// start counting
				if (current_number == 9) x = 0;
				if (current_number == 0) x = 1;
				if (x == 1)
					count(++current_number);
				else {
					current_number = 0;
					LNum++;
				}

				HAL_Delay(200);

			} else {

				// stop counting
				count(current_number);
				LED(LNum);
			}
	}




}
