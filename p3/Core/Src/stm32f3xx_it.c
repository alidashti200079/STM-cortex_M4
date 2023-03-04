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
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST      0

extern TIM_HandleTypeDef htim2;
TIM_HandleTypeDef *pwm_timer = &htim2;	// Point to PWM Timer configured in CubeMX
uint32_t pwm_channel = TIM_CHANNEL_1;   // Select configured PWM channel number

const Tone *volatile melody_ptr;
volatile uint16_t melody_tone_count;
volatile uint16_t current_tone_number;
volatile uint32_t current_tone_end;
volatile uint16_t volume = 30;          // (0 - 1000)
volatile uint32_t last_button_press;
volatile uint32_t pause = 0;

const Tone super_mario_bros[] = {
	{2637,306}, // E7 x2
	{   0,153}, // x3 <-- Silence
	{2637,153}, // E7
	{   0,153}, // x3
	{2093,153}, // C7
	{2637,153}, // E7
	{   0,153}, // x3
	{3136,153}, // G7
	{   0,459}, // x3
	{1586,153}, // G6
	{   0,459}, // x3

	{2093,153}, // C7
	{   0,306}, // x2
	{1586,153}, // G6
	{   0,306}, // x2
	{1319,153}, // E6
	{   0,306}, // x2
	{1760,153}, // A6
	{   0,153}, // x1
	{1976,153}, // B6
	{   0,153}, // x1
	{1865,153}, // AS6
	{1760,153}, // A6
	{   0,153}, // x1

	{1586,204}, // G6
	{2637,204}, // E7
	{3136,204}, // G7
	{3520,153}, // A7
	{   0,153}, // x1
	{2794,153}, // F7
	{3136,153}, // G7
	{   0,153}, // x1
	{2637,153}, // E7
	{   0,153}, // x1
	{2093,153}, // C7
	{2349,153}, // D7
	{1976,153}, // B6
	{   0,306}, // x2

	{2093,153}, // C7
	{   0,306}, // x2
	{1586,153}, // G6
	{   0,306}, // x2
	{1319,153}, // E6
	{   0,306}, // x2
	{1760,153}, // A6
	{   0,153}, // x1
	{1976,153}, // B6
	{   0,153}, // x1
	{1865,153}, // AS6
	{1760,153}, // A6
	{   0,153}, // x1

	{1586,204}, // G6
	{2637,204}, // E7
	{3136,204}, // G7
	{3520,153}, // A7
	{   0,153}, // x1
	{2794,153}, // F7
	{3136,153}, // G7
	{   0,153}, // x1
	{2637,153}, // E7
	{   0,153}, // x1
	{2093,153}, // C7
	{2349,153}, // D7
	{1976,153}, // B6

	{   0,  0}	// <-- Disable PWM
};

const Tone Nokia[] = {

  // Nokia Ringtone
  // Score available at https://musescore.com/user/29944637/scores/5266155

  {NOTE_E5, 181},
  {NOTE_D5, 203},
  {NOTE_FS4, 323},
  {NOTE_GS4, 288},
  {NOTE_CS5, 216},
  {NOTE_B4, 242},
  {NOTE_D4, 365},
  {NOTE_E4, 331},
  {NOTE_B4, 242},
  {NOTE_A4, 271},
  {NOTE_CS4, 401},
  {NOTE_E4, 331},
  {NOTE_A4, 271},

  {   0,  200},

  {NOTE_E5, 181},
  {NOTE_D5, 203},
  {NOTE_FS4, 323},
  {NOTE_GS4, 288},
  {NOTE_CS5, 216},
  {NOTE_B4, 242},
  {NOTE_D4, 365},
  {NOTE_E4, 331},
  {NOTE_B4, 242},
  {NOTE_A4, 271},
  {NOTE_CS4, 401},
  {NOTE_E4, 331},
  {NOTE_A4, 271},

  {   0,  200},

  {NOTE_E5, 181},
  {NOTE_D5, 203},
  {NOTE_FS4, 323},
  {NOTE_GS4, 288},
  {NOTE_CS5, 216},
  {NOTE_B4, 242},
  {NOTE_D4, 365},
  {NOTE_E4, 331},
  {NOTE_B4, 242},
  {NOTE_A4, 271},
  {NOTE_CS4, 401},
  {NOTE_E4, 331},
  {NOTE_A4, 271},

  {   0,  0}
};

const Tone  PinkPanther[] = {

  // Pink Panther theme
  // Score available at https://musescore.com/benedictsong/the-pink-panther
  // Theme by Masato Nakamura, arranged by Teddy Mason

  {REST,20},
  {REST,40},
  {REST,80},
  {NOTE_DS4,287},

  {NOTE_E4,250},
  {REST,180},
  {NOTE_FS4,287},
  {NOTE_G4,250},
  {REST,180},
  {NOTE_DS4,287},
  {NOTE_E4,250},
  {NOTE_FS4,287},
  {NOTE_G4,250},
  {NOTE_C5,271},
  {NOTE_B4,181},
  {NOTE_E4,271},
  {NOTE_G4,229},
  {NOTE_B4,181},

  {NOTE_AS4,292},
  {NOTE_A4,204},
  {NOTE_G4,229},
  {NOTE_E4,271},
  {NOTE_D4,305},

  {NOTE_E4,271},
  {REST,140},
  {REST,180},
  {NOTE_DS4,287},


  {NOTE_E4,271},
  {REST,180},
  {NOTE_FS4,242},
  {NOTE_G4,229},
  {REST,180},
  {NOTE_DS4,287},

  {NOTE_E4,271},
  {NOTE_FS4,242},
  {NOTE_G4,229},
  {NOTE_C5,171},
  {NOTE_B4,181},
  {NOTE_G4,229},
  {NOTE_B4,181},
  {NOTE_E5,236},

  {NOTE_DS5,144},
  {NOTE_D5,153},
  {REST,140},
  {REST,180},
  {NOTE_DS4,287},

  {NOTE_E4,271},
  {REST,180},
  {NOTE_FS4,242},
  {NOTE_G4,229},
  {REST,180},
  {NOTE_DS4,287},

  {NOTE_E4,271},
  {NOTE_FS4,242},
  {NOTE_G4,229},
  {NOTE_C5,171},
  {NOTE_B4,181},
  {NOTE_E4,271},
  {NOTE_G4,229},
  {NOTE_B4,181},

  {NOTE_AS4,192},
  {NOTE_A4,204},
  {NOTE_G4,229},
  {NOTE_E4,271},
  {NOTE_D4,305},

  {NOTE_E4,271},
  {REST,140},

  {REST,140},
  {NOTE_E5,136},
  {NOTE_D5,153},
  {NOTE_B4,181},
  {NOTE_A4,204},
  {NOTE_G4,229},
  {NOTE_E4,271},

  {NOTE_AS4,192},
  {NOTE_A4,204},
  {NOTE_AS4,192},
  {NOTE_A4,204},
  {NOTE_AS4,192},
  {NOTE_A4,204},
  {NOTE_AS4,192},
  {NOTE_A4,204},

  {NOTE_G4,229},
  {NOTE_E4,271},
  {NOTE_D4,305},
  {NOTE_E4,271},
  {NOTE_E4,320},
  {NOTE_E4,400},

  {   0,  0}
};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void PWM_Start()
{
    HAL_TIM_PWM_Start(pwm_timer, pwm_channel);
}

void PWM_Stop()
{
	HAL_TIM_PWM_Stop(pwm_timer, pwm_channel);
}

void PWM_Change_Tone(uint16_t pwm_freq, uint16_t volume) // pwm_freq (1 - 20000), volume (0 - 1000)
{
    if (pwm_freq == 0 || pwm_freq > 20000)
    {
        __HAL_TIM_SET_COMPARE(pwm_timer, pwm_channel, 0);
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
        __HAL_TIM_SET_COMPARE(pwm_timer, pwm_channel, pulse_width); // pwm_timer->Instance->CCR2 = pulse_width;
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

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
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
  * @brief This function handles USART2 global interrupt / USART2 wake-up interrupt through EXTI line 26.
  */
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */

  /* USER CODE END USART2_IRQn 0 */
  HAL_UART_IRQHandler(&huart2);
  /* USER CODE BEGIN USART2_IRQn 1 */

	extern unsigned char data[1];

	switch(data[0]) {
		case'm':
			PWM_Start();
			Change_Melody(super_mario_bros, ARRAY_LENGTH(super_mario_bros));
		break;
		case'n':
			PWM_Start();
			Change_Melody(Nokia, ARRAY_LENGTH(Nokia));
		break;
		case'p':
			PWM_Start();
			Change_Melody(PinkPanther, ARRAY_LENGTH(PinkPanther));
		break;
		case'H':
			if( volume < 1000 ) {
				volume = volume + 5;
			}
		break;
		case'L':
			if( volume > 0 ) {
				volume = volume - 5;
			}
		break;
		case'S':
			PWM_Stop();
			pause = 1;
		break;
		case'P':
			PWM_Start();
			pause = 0;
		break;
	}

	HAL_UART_Receive_IT(&huart2,data,sizeof(data));
  /* USER CODE END USART2_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
