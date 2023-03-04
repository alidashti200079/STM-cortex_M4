/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f3xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f3xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */
typedef struct
{
    uint16_t frequency;
    uint16_t duration;
} Tone;
/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define ARRAY_LENGTH(arr) (sizeof(arr) / sizeof((arr)[0]))
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
unsigned char buffer[100]="";
int position = 0;
unsigned char error0[23] = "error, Wrong request \n";
unsigned char error1[24] = "error, Wrong password \n";
unsigned char options[115] = "Welcome\n choose the one option : \n SET MAX LEVEL[N] \n SET LEVEL[N] \n SET WAIT[N] \n SET LED{ON/OFF} \n TEST# \n";
unsigned char error2[60] = "error, The desired floor is larger than the allowed limit \n";
unsigned char error3[51] = "error, The entered number is not a factor of 100 \n";
unsigned char error4[48] = "error, The entered number is out of the range \n";
unsigned char error5[55] = "error, One or more floors requested are out of range \n";
unsigned char error6[33] = "error, Duplicate floor entered \n";
unsigned char start0[35] = "Enter 'START' when you are ready \n";
unsigned char correct0[38] = "correct, the test is ready to start \n";
unsigned char running[13] = "Running... \n";
unsigned char ledoff[17] = "LED turned off \n";
unsigned char ledon[16] = "LED turned on \n";
unsigned char correct1[46] = "correct, The elevator stop time was changed \n";
unsigned char correct2[44] = "correct, The floor you are on has changed \n";
unsigned char correct3[37] = "correct, The max floor was changed \n";
unsigned char out[28] = "You are out of admin mode \n";

int floorGo = 0;
int floorIn = 0;
int moveDelay = 1500;
int stopDelay = 2000;
int Button4 = 0;
int fix = 0;
int run = 0;
int sizeFloor = 9; //default
int i = 0;
int howMuchJob = 0;
int adminIn = 0;
int adminReq = 0;
int y = 0;
int LEDon = 1;
int testSize = 0;

# define SIZE 100
int requestQueue[SIZE];
int Rear = - 1;
int Front = - 1;

extern TIM_HandleTypeDef htim15;
TIM_HandleTypeDef *pwm_timer = &htim15;
uint32_t pwm_channe1 = TIM_CHANNEL_1;

const Tone *volatile melody_ptr;
volatile uint16_t melody_tone_count;
volatile uint16_t current_tone_number;
volatile uint32_t current_tone_end;
volatile uint16_t volume = 100;          // (0 - 1000)
volatile uint32_t last_button_press;
volatile uint32_t pause = 0;

const Tone biz[] = {
	{   400,  250},
	{   0,  250},
	{   400,  250},
	{   0,  250},
};

void warning_start() {
	HAL_TIM_PWM_Start(pwm_timer, pwm_channe1);
}

void warning_stop() {
	HAL_TIM_PWM_Stop(pwm_timer, pwm_channe1);
}

void PWM_Change_Tone(uint16_t pwm_freq, uint16_t volume) // pwm_freq (1 - 20000), volume (0 - 1000)
{
    if (pwm_freq == 0 || pwm_freq > 20000)
    {
        __HAL_TIM_SET_COMPARE(pwm_timer, pwm_channe1, 0);
    }
    else
    {
        const uint32_t internal_clock_freq = HAL_RCC_GetSysClockFreq();
        const uint16_t prescaler = 1 + internal_clock_freq / pwm_freq / 60000;
        const uint32_t timer_clock = internal_clock_freq / prescaler;
        const uint32_t period_cycles = timer_clock / pwm_freq;
        const uint32_t pulse_width = volume * period_cycles / 1000 / 2;

        pwm_timer->Instance->PSC = prescaler - 1;
        pwm_timer->Instance->ARR = period_cycles - 1;
        pwm_timer->Instance->EGR = TIM_EGR_UG;
        __HAL_TIM_SET_COMPARE(pwm_timer, pwm_channe1, pulse_width); // pwm_timer->Instance->CCR2 = pulse_width;
    }
}

void Change_Melody(const Tone *melody, uint16_t tone_count)
{
    melody_ptr = melody;
    melody_tone_count = tone_count;
    current_tone_number = 0;
}

void Update_Melody()
{
    if( pause == 0 )
    {
    	if ((HAL_GetTick() > current_tone_end) && (current_tone_number < melody_tone_count))
    	    {
    	        const Tone active_tone = *(melody_ptr + current_tone_number);
    	        PWM_Change_Tone(active_tone.frequency, volume);
    	        current_tone_end = HAL_GetTick() + active_tone.duration;
    	        current_tone_number++;
    	    }
    }
}

void warning() {
	Change_Melody(biz, ARRAY_LENGTH(biz));
}

void LED() {
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, 1);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, 1);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, 1);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, 1);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, 1);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, 1);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, 1);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, 1);
	HAL_Delay(250);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, 0);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, 0);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, 0);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, 0);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, 0);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, 0);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, 0);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, 0);
	HAL_Delay(250);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, 1);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, 1);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, 1);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, 1);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, 1);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, 1);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, 1);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, 1);
	HAL_Delay(250);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, 0);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, 0);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, 0);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, 0);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, 0);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, 0);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, 0);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, 0);
}

void elevator() {
	pop();
	while ( requestQueue[Front] != floorIn && requestQueue[Front] > -1 && Rear > -1 && Front <= Rear )
	{
		if ( floorIn < requestQueue[Front] )
		{
			HAL_Delay(moveDelay);
			floorIn++;
		} else if ( floorIn > requestQueue[Front] ) {
			HAL_Delay(moveDelay);
			floorIn--;
		}
		if ( requestQueue[Front] == floorIn )
		{
			HAL_Delay(stopDelay);
		}
	}
	run = 0;
}

void push(int floorGo) {
	int rep = 0;
	int i;
	for ( i = 0; i <= Rear; i++ )
	{
		if ( requestQueue[i] == floorGo )
		{
			rep = 1;
		}
	}
	if ( rep == 0 )
	{
		LED();
		if ( Rear < 100 )
		{
			Rear++;
			requestQueue[Rear] = floorGo;
			howMuchJob++;
		}
	}
}

void pop() {
	if ( Front > Rear )
	{
		Rear = -1;
		Front = -1;
	}
	if ( Front < 100 )
	{
		requestQueue[Front] = -1;
		Front++;
	}
}

void removeAll() {
	while ( Rear > -1  )
	{
		requestQueue[Rear] = -1;
		Rear--;
	}
	Rear = -1;
	Front = -1;
	run = 0;
	howMuchJob = 0;
}

void doJob() {
	if( run == 0 && howMuchJob > 0)
	{
		run = 1;
		elevator();
		howMuchJob--;
	}
}

void set_max_level( int n ) {
	sizeFloor = n;
	floorGo = 0;
	floorIn = 0;
}

void set_level( int n ) {
	floorIn = n;
}

void set_wait( int n ) {
	stopDelay = n;
}

void set_LED( int on ) {
	LEDon = on;
}

void test( int floor[] ) {

	run = 1;

	int i = 0;
	while ( floor[i] && floor[i] > -1 ) {
		if ( floor[i] <= sizeFloor )
		{
			push(floor[i]);
			testSize++;
		}
		i++;
	}

}

void start() {
	howMuchJob = testSize;
	run = 0;
}

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim15;
extern UART_HandleTypeDef huart2;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */
  Update_Melody();
  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F3xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f3xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles EXTI line0 interrupt.
  */
void EXTI0_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI0_IRQn 0 */

	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) && fix == 0 )
	{
		if (floorGo > 0 )
		{
			floorGo--;
			fix = 1;
		}
	}
  /* USER CODE END EXTI0_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
  /* USER CODE BEGIN EXTI0_IRQn 1 */

  /* USER CODE END EXTI0_IRQn 1 */
}

/**
  * @brief This function handles EXTI line1 interrupt.
  */
void EXTI1_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI1_IRQn 0 */

	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) && fix == 0 )
	{
		if (floorGo < sizeFloor )
		{
			floorGo++;
			fix = 1;
		}
	}

  /* USER CODE END EXTI1_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
  /* USER CODE BEGIN EXTI1_IRQn 1 */

  /* USER CODE END EXTI1_IRQn 1 */
}

/**
  * @brief This function handles EXTI line3 interrupt.
  */
void EXTI3_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI3_IRQn 0 */

	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3) && fix == 0)
	{
		push(floorGo);
		fix = 1;
//		if ( run == 0 )
//		{
//			run = 1;
//			elevator();
//		}
	}

  /* USER CODE END EXTI3_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
  /* USER CODE BEGIN EXTI3_IRQn 1 */

  /* USER CODE END EXTI3_IRQn 1 */
}

/**
  * @brief This function handles EXTI line4 interrupt.
  */
void EXTI4_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI4_IRQn 0 */
	if ( HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4) && Button4 == 0 )
	{
		warning_start();
		Button4 = 1;
	} else if ( HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4) && Button4 == 1 ) {
		warning_stop();
		Button4 = 0;
	}
  /* USER CODE END EXTI4_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
  /* USER CODE BEGIN EXTI4_IRQn 1 */

  /* USER CODE END EXTI4_IRQn 1 */
}

/**
  * @brief This function handles TIM1 break and TIM15 interrupts.
  */
void TIM1_BRK_TIM15_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_BRK_TIM15_IRQn 0 */

  /* USER CODE END TIM1_BRK_TIM15_IRQn 0 */
  HAL_TIM_IRQHandler(&htim15);
  /* USER CODE BEGIN TIM1_BRK_TIM15_IRQn 1 */

  /* USER CODE END TIM1_BRK_TIM15_IRQn 1 */
}

/**
  * @brief This function handles TIM3 global interrupt.
  */
void TIM3_IRQHandler(void)
{
  /* USER CODE BEGIN TIM3_IRQn 0 */
	if ( Button4 == 1 )
	{
		warning();
		if ( LEDon == 1 )
		{
			LED();
		}

	}

	//fix baraye chand bar feshorde shodan dokme ha
	fix = 0;

	doJob();
  /* USER CODE END TIM3_IRQn 0 */
  HAL_TIM_IRQHandler(&htim3);
  /* USER CODE BEGIN TIM3_IRQn 1 */

  /* USER CODE END TIM3_IRQn 1 */
}

/**
  * @brief This function handles TIM4 global interrupt.
  */
void TIM4_IRQHandler(void)
{
  /* USER CODE BEGIN TIM4_IRQn 0 */
	show(floorIn, floorGo);

	if ( i == 50000 ) {
		i=0;
		fix=0;
	}
	i++;

  /* USER CODE END TIM4_IRQn 0 */
  HAL_TIM_IRQHandler(&htim4);
  /* USER CODE BEGIN TIM4_IRQn 1 */

  /* USER CODE END TIM4_IRQn 1 */
}

/**
  * @brief This function handles USART2 global interrupt / USART2 wake-up interrupt through EXTI line 26.
  */
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */

  /* USER CODE END USART2_IRQn 0 */
  HAL_UART_IRQHandler(&huart2);
  /* USER CODE BEGIN USART2_IRQn 1 */



  	extern unsigned char data[1];


  	switch(data[0]){

	case '\n':

		if ( howMuchJob == 0 && buffer[0] == 'A' && buffer[1] == 'D' && buffer[2] == 'M' && buffer[3] == 'I' && buffer[4] == 'N'
			&& buffer[5] == '#' )
		{
			if ( buffer[6] == '1' && buffer[7] == '2' && buffer[8] == '3' && buffer[9] == '4' )
			{
				HAL_UART_Transmit(&huart2, options, sizeof(unsigned char) * 108, 1000);
				adminIn = 1;
				adminReq = 1;
			} else {
				HAL_UART_Transmit(&huart2, error1, sizeof(unsigned char) * 23, 100);
			}
		} else if ( adminIn == 0 ) {
			HAL_UART_Transmit(&huart2, error0, sizeof(unsigned char) * 22, 100);
		}



		if ( adminIn == 1 && buffer[0] == 'S' && buffer[1] == 'E' && buffer[2] == 'T' && buffer[3] == ' ' && buffer[4] == 'M'
					&& buffer[5] == 'A' && buffer[6] == 'X' && buffer[7] == ' ' && buffer[8] == 'L' && buffer[9] == 'E' && buffer[10] == 'V'
							&& buffer[11] == 'E' && buffer[12] == 'L' && buffer[13] == '[' && buffer[15] == ']')
		{
			int n;
			if ( buffer[14] == '0' )
			{
				n = 0;
			} else if ( buffer[14] == '1' ) {
				n = 1;
			} else if ( buffer[14] == '2' ) {
				n = 2;
			} else if ( buffer[14] == '3' ) {
				n = 3;
			} else if ( buffer[14] == '4' ) {
				n = 4;
			} else if ( buffer[14] == '5' ) {
				n = 5;
			} else if ( buffer[14] == '6' ) {
				n = 6;
			} else if ( buffer[14] == '7' ) {
				n = 7;
			} else if ( buffer[14] == '8' ) {
				n = 8;
			} else if ( buffer[14] == '9' ) {
				n = 9;
			} else {
				n = 9;
			}

			set_max_level(n);
			HAL_UART_Transmit(&huart2, correct3, sizeof(unsigned char) * 36, 100);
			removeAll();
			adminReq = 1;

		} else if ( adminIn == 1  && buffer[0] == 'S' && buffer[1] == 'E' && buffer[2] == 'T' && buffer[3] == ' ' && buffer[4] == 'L'
				&& buffer[5] == 'E' && buffer[6] == 'V' && buffer[7] == 'E' && buffer[8] == 'L' && buffer[9] == '[' && buffer[11] == ']') {

			int n;
			if ( buffer[10] == '0' )
			{
				n = 0;
			} else if ( buffer[10] == '1' ) {
				n = 1;
			} else if ( buffer[10] == '2' ) {
				n = 2;
			} else if ( buffer[10] == '3' ) {
				n = 3;
			} else if ( buffer[10] == '4' ) {
				n = 4;
			} else if ( buffer[10] == '5' ) {
				n = 5;
			} else if ( buffer[10] == '6' ) {
				n = 6;
			} else if ( buffer[10] == '7' ) {
				n = 7;
			} else if ( buffer[10] == '8' ) {
				n = 8;
			} else if ( buffer[10] == '9' ) {
				n = 9;
			} else {
				n = 9;
			}
			if ( n <= sizeFloor ) {
				set_level(n);
				HAL_UART_Transmit(&huart2, correct2, sizeof(unsigned char) * 43, 100);
				removeAll();
			} else {
				HAL_UART_Transmit(&huart2, error2, sizeof(unsigned char) * 59, 100);
			}
			adminReq = 1;

		} else if ( adminIn == 1 && buffer[0] == 'S' && buffer[1] == 'E' && buffer[2] == 'T' && buffer[3] == ' ' && buffer[4] == 'W'
				&& buffer[5] == 'A' && buffer[6] == 'I' && buffer[7] == 'T' && buffer[8] == '[') {

			int n;
			if ( buffer[9] == '0' )
			{
				n = 0;
			} else if ( buffer[9] == '1' ) {
				n = 1;
				if ( buffer[10] == '0' )
				{
					n = n*10;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '1' ) {
					n = n*10 + 1;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '2' ) {
					n = n*10 + 2;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '3' ) {
					n = n*10 + 3;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '4' ) {
					n = n*10 + 4;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '5' ) {
					n = n*10 + 5;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '6' ) {
					n = n*10 + 6;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '7' ) {
					n = n*10 + 7;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '8' ) {
					n = n*10 + 8;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '9' ) {
					n = n*10 + 9;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				}
			} else if ( buffer[9] == '2' ) {
				n = 2;
				if ( buffer[10] == '0' )
				{
					n = n*10;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '1' ) {
					n = n*10 + 1;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '2' ) {
					n = n*10 + 2;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '3' ) {
					n = n*10 + 3;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '4' ) {
					n = n*10 + 4;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '5' ) {
					n = n*10 + 5;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '6' ) {
					n = n*10 + 6;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '7' ) {
					n = n*10 + 7;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '8' ) {
					n = n*10 + 8;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '9' ) {
					n = n*10 + 9;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				}
			} else if ( buffer[9] == '3' ) {
				n = 3;
				if ( buffer[10] == '0' )
				{
					n = n*10;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '1' ) {
					n = n*10 + 1;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '2' ) {
					n = n*10 + 2;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '3' ) {
					n = n*10 + 3;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '4' ) {
					n = n*10 + 4;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '5' ) {
					n = n*10 + 5;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '6' ) {
					n = n*10 + 6;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '7' ) {
					n = n*10 + 7;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '8' ) {
					n = n*10 + 8;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '9' ) {
					n = n*10 + 9;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				}
			} else if ( buffer[9] == '4' ) {
				n = 4;
				if ( buffer[10] == '0' )
				{
					n = n*10;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '1' ) {
					n = n*10 + 1;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '2' ) {
					n = n*10 + 2;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '3' ) {
					n = n*10 + 3;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '4' ) {
					n = n*10 + 4;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '5' ) {
					n = n*10 + 5;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '6' ) {
					n = n*10 + 6;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '7' ) {
					n = n*10 + 7;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '8' ) {
					n = n*10 + 8;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '9' ) {
					n = n*10 + 9;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				}
			} else if ( buffer[9] == '5' ) {
				n = 5;
				if ( buffer[10] == '0' )
				{
					n = n*10;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '1' ) {
					n = n*10 + 1;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '2' ) {
					n = n*10 + 2;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '3' ) {
					n = n*10 + 3;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '4' ) {
					n = n*10 + 4;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '5' ) {
					n = n*10 + 5;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '6' ) {
					n = n*10 + 6;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '7' ) {
					n = n*10 + 7;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '8' ) {
					n = n*10 + 8;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '9' ) {
					n = n*10 + 9;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				}
			} else if ( buffer[9] == '6' ) {
				n = 6;
				if ( buffer[10] == '0' )
				{
					n = n*10;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '1' ) {
					n = n*10 + 1;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '2' ) {
					n = n*10 + 2;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '3' ) {
					n = n*10 + 3;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '4' ) {
					n = n*10 + 4;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '5' ) {
					n = n*10 + 5;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '6' ) {
					n = n*10 + 6;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '7' ) {
					n = n*10 + 7;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '8' ) {
					n = n*10 + 8;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '9' ) {
					n = n*10 + 9;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				}
			} else if ( buffer[9] == '7' ) {
				n = 7;
				if ( buffer[10] == '0' )
				{
					n = n*10;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '1' ) {
					n = n*10 + 1;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '2' ) {
					n = n*10 + 2;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '3' ) {
					n = n*10 + 3;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '4' ) {
					n = n*10 + 4;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '5' ) {
					n = n*10 + 5;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '6' ) {
					n = n*10 + 6;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '7' ) {
					n = n*10 + 7;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '8' ) {
					n = n*10 + 8;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '9' ) {
					n = n*10 + 9;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				}
			} else if ( buffer[9] == '8' ) {
				n = 8;
				if ( buffer[10] == '0' )
				{
					n = n*10;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '1' ) {
					n = n*10 + 1;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '2' ) {
					n = n*10 + 2;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '3' ) {
					n = n*10 + 3;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '4' ) {
					n = n*10 + 4;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '5' ) {
					n = n*10 + 5;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '6' ) {
					n = n*10 + 6;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '7' ) {
					n = n*10 + 7;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '8' ) {
					n = n*10 + 8;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '9' ) {
					n = n*10 + 9;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				}
			} else if ( buffer[9] == '9' ) {
				n = 9;
				if ( buffer[10] == '0' )
				{
					n = n*10;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '1' ) {
					n = n*10 + 1;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '2' ) {
					n = n*10 + 2;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '3' ) {
					n = n*10 + 3;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '4' ) {
					n = n*10 + 4;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '5' ) {
					n = n*10 + 5;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '6' ) {
					n = n*10 + 6;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '7' ) {
					n = n*10 + 7;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '8' ) {
					n = n*10 + 8;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				} else if ( buffer[10] == '9' ) {
					n = n*10 + 9;
					if( buffer[11] == '0' ) {
						n = n * 10;
						if( buffer[12] == '0' ) {
							n = n * 10;
						}
					}
				}
			}


			if ( n % 100 == 0 ) {
				if( n >= 500 && n <= 5000 && ( buffer[12] == ']' || buffer[13] == ']') )
				{
					set_wait(n);
					HAL_UART_Transmit(&huart2, correct1, sizeof(unsigned char) * 45, 100);
				} else {
					HAL_UART_Transmit(&huart2, error4, sizeof(unsigned char) * 47, 100);
				}
			} else {
				HAL_UART_Transmit(&huart2, error3, sizeof(unsigned char) * 50, 100);
			}


			adminReq = 1;

		} else if ( adminIn == 1 && buffer[0] == 'S' && buffer[1] == 'E' && buffer[2] == 'T' && buffer[3] == ' ' && buffer[4] == 'L'
				&& buffer[5] == 'E' && buffer[6] == 'D' ) {

			adminReq = 1;
			int on;
			if ( buffer[8] == 'O' && buffer[9] == 'F' && buffer[10] == 'F')
			{
				on = 0;
				set_LED(on);
				HAL_UART_Transmit(&huart2, ledoff, sizeof(unsigned char) * 16, 100);
			} else if ( buffer[8] == 'O' && buffer[9] == 'N')
			{
				on = 1;
				set_LED(on);
				HAL_UART_Transmit(&huart2, ledon, sizeof(unsigned char) * 15, 100);
			}

		} else if ( adminIn == 1 && buffer[0] == 'T' && buffer[1] == 'E' && buffer[2] == 'S' && buffer[3] == 'T' && buffer[4] == '#') {

			int floor[5];
			floor[0] = -1;
			floor[1] = -2;
			floor[2] = -3;
			floor[3] = -4;
			floor[4] = -5;
			int i;
			int j = 0;
			for ( i = 5; i < position+1; i++ )
			{
				if ( buffer[i] == '0' )
				{
					floor[j] = 0;
					j++;
				} else if ( buffer[i] == '1' ) {
					floor[j] = 1;
					j++;
				} else if ( buffer[i] == '2' ) {
					floor[j] = 2;
					j++;
				} else if ( buffer[i] == '3' ) {
					floor[j] = 3;
					j++;
				} else if ( buffer[i] == '4' ) {
					floor[j] = 4;
					j++;
				} else if ( buffer[i] == '5' ) {
					floor[j] = 5;
					j++;
				} else if ( buffer[i] == '6' ) {
					floor[j] = 6;
					j++;
				} else if ( buffer[i] == '7' ) {
					floor[j] = 7;
					j++;
				} else if ( buffer[i] == '8' ) {
					floor[j] = 8;
					j++;
				} else if ( buffer[i] == '9' ) {
					floor[j] = 9;
					j++;
				}
			}

			int equal = 0;
			int k = 0;
			int l = 0;
			for ( k = 0; k < 5; k++ )
			{
				for ( l = k+1; l < 5; l++ )
				{
					if ( floor[k] == floor[l] )
					{
						equal = 1;
					}
				}
			}

			if ( j == 1 && floor[0] > sizeFloor )
			{
				HAL_UART_Transmit(&huart2, error5, sizeof(unsigned char) * 54, 100);
			} else if ( j == 2 && (floor[0] > sizeFloor || floor[1] > sizeFloor) ) {
				HAL_UART_Transmit(&huart2, error5, sizeof(unsigned char) * 54, 100);
			} else if ( j == 3 && (floor[0] > sizeFloor || floor[1] > sizeFloor || floor[2] > sizeFloor) ) {
				HAL_UART_Transmit(&huart2, error5, sizeof(unsigned char) * 54, 100);
			} else if ( j == 4 && (floor[0] > sizeFloor || floor[1] > sizeFloor || floor[2] > sizeFloor || floor[3] > sizeFloor) ) {
				HAL_UART_Transmit(&huart2, error5, sizeof(unsigned char) * 54, 100);
			} else if ( j == 5 && (floor[0] > sizeFloor || floor[1] > sizeFloor || floor[2] > sizeFloor || floor[3] > sizeFloor || floor[4] > sizeFloor) ) {
				HAL_UART_Transmit(&huart2, error5, sizeof(unsigned char) * 54, 100);
			} else if ( equal == 1 ) {
				HAL_UART_Transmit(&huart2, error6, sizeof(unsigned char) * 32, 100);
			} else {
				test(floor);
				HAL_UART_Transmit(&huart2, correct0, sizeof(unsigned char) * 37, 100);
				HAL_UART_Transmit(&huart2, start0, sizeof(unsigned char) * 34, 100);
			}

			adminReq = 1;

		} else if ( adminIn == 1 && buffer[0] == 'S' && buffer[1] == 'T' && buffer[2] == 'A' && buffer[3] == 'R' && buffer[4] == 'T') {

			start();
			HAL_UART_Transmit(&huart2, running, sizeof(unsigned char) * 12, 100);
			adminIn = 0;
			HAL_UART_Transmit(&huart2, out, sizeof(unsigned char) * 27, 100);

		} else if ( adminIn == 1 && adminReq == 0) {
			HAL_UART_Transmit(&huart2, error0, sizeof(unsigned char) * 22, 100);
		}
		adminReq = 0;


		//HAL_UART_Transmit(&huart2, buffer, position, 100);
		position = 0;
		break;
	default:
		if ( position < 100 )
		{
			buffer[position] = data[0];
			buffer[position+1] = '\0';
			position++;
		}
		break;
	}

  HAL_UART_Receive_IT(&huart2,data,sizeof(data));
  /* USER CODE END USART2_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
