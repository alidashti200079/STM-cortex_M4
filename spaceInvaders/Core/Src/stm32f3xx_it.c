/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f3xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
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

extern TIM_HandleTypeDef htim4;
TIM_HandleTypeDef *pwm_timer = &htim4;	// Point to PWM Timer configured in CubeMX
uint32_t pwm_channel = TIM_CHANNEL_4;   // Select configured PWM channel number

const Tone *volatile melody_ptr;
volatile uint16_t melody_tone_count;
volatile uint16_t current_tone_number;
volatile uint32_t current_tone_end;
volatile uint16_t volume = 50;          // (0 - 1000)
volatile uint32_t last_button_press;
volatile uint32_t pause = 0;

const Tone menuT[] = { //Bloody Tears from Castlevania II
	{0, 374},
	{784, 374},
	{880, 374},
	{932, 374},
	{880, 374},
	{698, 374},
	{880, 374},
	{784, 374},
	{0, 374},
	{784, 374},
	{880, 374},
	{932, 374},
	{1047, 374},
	{932, 374},
	{880, 374},
	{784, 374},
	{0, 374},
	{784, 374},
	{880, 374},
	{932, 374},
	{880, 374},
	{698, 374},
	{880, 374},
	{784, 374},
	{1175, 374},
	{0, 187},
	{1047, 187},
	{0, 374},
	{932, 374},
	{880, 374},
	{932, 187},
	{1047, 187},
	{1397, 187},
	{0, 187},
	{0, 374},
	{784, 93},
	{587, 93},
	{1175, 93},
	{587, 93},
	{1047, 93},
	{587, 93},
	{932, 93},
	{587, 93},
	{880, 93},
	{587, 93},
	{932, 93},
	{587, 93},
	{880, 93},
	{587, 93},
	{784, 93},
	{587, 93},
	{880, 93},
	{587, 93},
	{932, 93},
	{587, 93},
	{1047, 93},
	{587, 93},
	{932, 93},
	{587, 93},
	{880, 93},
	{587, 93},
	{698, 93},
	{587, 93},
	{880, 93},
	{587, 93},
	{784, 93},
	{587, 93},
	{784, 93},
	{587, 93},
	{1175, 93},
	{587, 93},
	{1047, 93},
	{587, 93},
	{932, 93},
	{587, 93},
	{880, 93},
	{587, 93},
	{932, 93},
	{587, 93},
	{880, 93},
	{587, 93},
	{932, 93},
	{587, 93},
	{880, 93},
	{587, 93},
	{932, 93},
	{587, 93},
	{1047, 93},
	{587, 93},
	{932, 93},
	{587, 93},
	{880, 93},
	{587, 93},
	{698, 93},
	{587, 93},
	{880, 93},
	{587, 93},
	{784, 93},
	{587, 93},
	{784, 93},
	{587, 93},
	{1175, 93},
	{587, 93},
	{1047, 93},
	{587, 93},
	{932, 93},
	{587, 93},
	{880, 93},
	{587, 93},
	{932, 93},
	{587, 93},
	{880, 93},
	{587, 93},
	{784, 93},
	{587, 93},
	{880, 93},
	{587, 93},
	{932, 93},
	{587, 93},
	{1047, 93},
	{587, 93},
	{932, 93},
	{587, 93},
	{880, 93},
	{587, 93},
	{698, 93},
	{587, 93},
	{880, 93},
	{587, 93},
	{784, 93},
	{587, 93},
	{932, 93},
	{587, 93},
	{1175, 93},
	{587, 93},
	{1047, 93},
	{587, 93},
	{932, 93},
	{587, 93},
	{880, 93},
	{587, 93},
	{932, 93},
	{587, 93},
	{880, 93},
	{587, 93},
	{784, 93},
	{587, 93},
	{880, 93},
	{587, 93},
	{932, 93},
	{587, 93},
	{1047, 93},
	{587, 93},
	{932, 93},
	{587, 93},
	{880, 93},
	{587, 93},
	{698, 93},
	{587, 93},
	{880, 93},
	{587, 93},
	{784, 93},
	{587, 93},
	{1047, 93},
	{1047, 93},
	{1397, 93},
	{1175, 187},
	{0, 93},
	{0, 187},
	{0, 374},
	{1047, 93},
	{932, 93},
	{1047, 280},
	{1397, 280},
	{1175, 561},
	{1047, 187},
	{932, 187},
	{1047, 187},
	{1397, 93},
	{1175, 187},
	{0, 93},
	{0, 187},
	{0, 374},
	{1047, 187},
	{1175, 187},
	{1245, 280},
	{1397, 280},
	{1175, 280},
	{0, 93},
	{1245, 187},
	{0, 187},
	{1047, 187},
	{1397, 93},
	{1175, 187},
	{0, 93},
	{0, 187},
	{0, 374},
	{1047, 187},
	{932, 187},
	{1047, 280},
	{1397, 280},
	{1175, 561},
	{1047, 187},
	{932, 187},
	{1047, 187},
	{1397, 93},
	{1175, 187},
	{0, 93},
	{0, 187},
	{0, 374},
	{1047, 187},
	{1175, 187},
	{1245, 280},
	{1397, 280},
	{587, 187},
	{740, 187},
	{698, 187},
	{880, 187},
	{880, 280},
	{784, 561},
	{880, 280},
	{784, 561},
	{880, 280},
	{784, 561},
	{932, 187},
	{880, 187},
	{784, 187},
	{698, 187},
	{880, 280},
	{784, 280},
	{587, 187},
	{880, 280},
	{784, 280},
	{587, 187},
	{880, 280},
	{784, 280},
	{587, 187},
	{932, 374},
	{1047, 374},
	{880, 374},
	{932, 374},
	{784, 93},
	{587, 93},
	{1175, 93},
	{587, 93},
	{1047, 93},
	{587, 93},
	{932, 93},
	{587, 93},
	{880, 93},
	{587, 93},
	{932, 93},
	{587, 93},
	{880, 93},
	{587, 93},
	{784, 93},
	{587, 93},
	{880, 93},
	{587, 93},
	{932, 93},
	{587, 93},
	{1047, 93},
	{587, 93},
	{932, 93},
	{587, 93},
	{880, 93},
	{587, 93},
	{698, 93},
	{587, 93},
	{880, 93},
	{587, 93},
	{784, 93},
	{587, 93},
	{784, 93},
	{587, 93},
	{1175, 93},
	{587, 93},
	{1047, 93},
	{587, 93},
	{932, 93},
	{587, 93},
	{880, 93},
	{587, 93},
	{932, 93},
	{587, 93},
	{880, 93},
	{587, 93},
	{932, 93},
	{587, 93},
	{880, 93},
	{587, 93},
	{932, 93},
	{587, 93},
	{1047, 93},
	{587, 93},
	{932, 93},
	{587, 93},
	{880, 93},
	{587, 93},
	{698, 93},
	{587, 93},
	{880, 93},
	{587, 93},
	{784, 93},
	{587, 93},
	{784, 93},
	{587, 93},
	{1175, 93},
	{587, 93},
	{1047, 93},
	{587, 93},
	{932, 93},
	{587, 93},
	{880, 93},
	{587, 93},
	{932, 93},
	{587, 93},
	{880, 93},
	{587, 93},
	{784, 93},
	{587, 93},
	{880, 93},
	{587, 93},
	{932, 93},
	{587, 93},
	{1047, 93},
	{587, 93},
	{932, 93},
	{587, 93},
	{880, 93},
	{587, 93},
	{698, 93},
	{587, 93},
	{880, 93},
	{587, 93},
	{784, 93},
	{587, 93},
	{932, 93},
	{587, 93},
	{1175, 93},
	{587, 93},
	{1047, 93},
	{587, 93},
	{932, 93},
	{587, 93},
	{880, 93},
	{587, 93},
	{932, 93},
	{587, 93},
	{880, 93},
	{587, 93},
	{784, 93},
	{587, 93},
	{880, 93},
	{587, 93},
	{932, 93},
	{587, 93},
	{1047, 93},
	{587, 93},
	{932, 93},
	{587, 93},
	{880, 93},
	{587, 93},
	{698, 93},
	{587, 93},
	{880, 187},
	{784, 46},
	{880, 46},
	{932, 46},
	{1047, 46},
	{1175, 93},
	{784, 93},
	{932, 93},
	{784, 93},
	{1047, 93},
	{784, 93},
	{1175, 93},
	{784, 93},
	{1047, 93},
	{784, 93},
	{880, 93},
	{784, 93},
	{1397, 93},
	{784, 93},
	{1175, 93},
	{622, 93},
	{1175, 374},
	{0, 374},
	{523, 187},
	{0, 187},
	{440, 140},
	{466, 140},
	{523, 93},
	{1175, 93},
	{392, 93},
	{466, 93},
	{392, 93},
	{523, 93},
	{392, 93},
	{1175, 93},
	{392, 93},
	{1047, 93},
	{349, 93},
	{440, 93},
	{349, 93},
	{698, 93},
	{349, 93},
	{1175, 93},
	{311, 93},
	{1175, 93},
	{0, 187},
	{330, 93},
	{349, 93},
	{415, 187},
	{0, 187},
	{466, 187},
	{0, 187},
	{622, 93},
	{415, 93},
	{494, 93},
	{415, 93},
	{554, 93},
	{415, 93},
	{622, 93},
	{415, 93},
	{554, 93},
	{370, 93},
	{466, 93},
	{370, 93},
	{740, 93},
	{370, 93},
	{622, 93},
	{659, 93},
	{587, 374},
	{0, 374},
	{554, 187},
	{0, 187},
	{466, 140},
	{494, 140},
	{554, 93},
	{622, 93},
	{415, 93},
	{494, 93},
	{415, 93},
	{554, 93},
	{415, 93},
	{622, 93},
	{415, 93},
	{554, 93},
	{370, 93},
	{466, 93},
	{370, 93},
	{740, 93},
	{370, 93},
	{622, 93},
	{659, 93},
	{622, 374},
	{0, 187},
	{622, 93},
	{659, 93},
	{740, 93},
	{554, 93},
	{659, 93},
	{277, 93},
	{622, 93},
	{659, 93},
	{784, 93},
	{932, 93},
	{831, 93},
	{622, 93},
	{1245, 93},
	{622, 93},
	{1109, 93},
	{622, 93},
	{988, 93},
	{622, 93},
	{932, 93},
	{622, 93},
	{988, 93},
	{622, 93},
	{932, 93},
	{622, 93},
	{831, 93},
	{622, 93},
	{932, 93},
	{622, 93},
	{988, 93},
	{622, 93},
	{1109, 93},
	{622, 93},
	{988, 93},
	{622, 93},
	{932, 93},
	{622, 93},
	{740, 93},
	{622, 93},
	{932, 93},
	{622, 93},
	{831, 93},
	{622, 93},
	{831, 93},
	{622, 93},
	{1245, 93},
	{622, 93},
	{1109, 93},
	{622, 93},
	{988, 93},
	{622, 93},
	{932, 93},
	{622, 93},
	{988, 93},
	{622, 93},
	{932, 93},
	{622, 93},
	{831, 93},
	{622, 93},
	{932, 93},
	{622, 93},
	{988, 93},
	{622, 93},
	{1109, 93},
	{622, 93},
	{988, 93},
	{622, 93},
	{932, 93},
	{622, 93},
	{740, 93},
	{622, 93},
	{932, 93},
	{622, 93},
	{831, 93},
	{622, 93},
	{831, 93},
	{622, 93},
	{1245, 93},
	{622, 93},
	{1109, 93},
	{622, 93},
	{988, 93},
	{622, 93},
	{932, 93},
	{622, 93},
	{988, 93},
	{622, 93},
	{932, 93},
	{622, 93},
	{831, 93},
	{622, 93},
	{932, 93},
	{622, 93},
	{988, 93},
	{622, 93},
	{1109, 93},
	{622, 93},
	{988, 93},
	{622, 93},
	{932, 93},
	{622, 93},
	{740, 93},
	{622, 93},
	{932, 93},
	{622, 93},
	{831, 93},
	{622, 93},
	{831, 93},
	{622, 93},
	{1245, 93},
	{622, 93},
	{1109, 93},
	{622, 93},
	{988, 93},
	{622, 93},
	{932, 93},
	{622, 93},
	{988, 93},
	{622, 93},
	{932, 93},
	{622, 93},
	{831, 93},
	{622, 93},
	{932, 93},
	{622, 93},
	{988, 93},
	{622, 93},
	{1109, 93},
	{622, 93},
	{988, 93},
	{622, 93},
	{932, 93},
	{622, 93},
	{740, 93},
	{622, 93},
	{932, 93},
	{622, 93},
	{831, 93},
	{622, 93},
	{1109, 187},
	{1480, 93},
	{1245, 187},
	{0, 93},
	{0, 187},
	{0, 374},
	{1109, 187},
	{988, 187},
	{1109, 280},
	{1480, 280},
	{1245, 561},
	{1109, 187},
	{988, 187},
	{1109, 187},
	{1480, 93},
	{1245, 187},
	{0, 93},
	{0, 187},
	{0, 374},
	{1109, 187},
	{988, 187},
	{1319, 280},
	{1397, 280},
	{1245, 280},
	{0, 93},
	{1319, 187},
	{0, 93},
	{0, 93},
	{1109, 187},
	{1480, 93},
	{1245, 187},
	{0, 93},
	{0, 187},
	{0, 374},
	{1109, 187},
	{988, 187},
	{1109, 280},
	{1480, 280},
	{1245, 561},
	{1109, 187},
	{988, 187},
	{1109, 187},
	{1480, 93},
	{1245, 187},
	{0, 93},
	{0, 187},
	{0, 374},
	{554, 187},
	{622, 187},
	{659, 280},
	{698, 280},
	{622, 187},
	{784, 187},
	{831, 187},
	{932, 187},
	{932, 280},
	{831, 280},
	{932, 280},
	{831, 280},
	{932, 280},
	{831, 280},
	{1976, 187},
	{932, 187},
	{831, 187},
	{740, 187},
	{932, 280},
	{1661, 280},
	{622, 187},
	{932, 280},
	{1661, 280},
	{622, 187},
	{932, 280},
	{1661, 280},
	{622, 187},
	{988, 187},
	{1109, 187},
	{932, 187},
	{988, 187},
	{831, 187},
	{0, 187},
	{0, 93},
	{0, 0}
};

const Tone doom[] = {
	{82, 119},
	{82, 119},
	{165, 119},
	{82, 119},
	{82, 119},
	{147, 119},
	{82, 119},
	{82, 119},
	{131, 119},
	{82, 119},
	{82, 119},
	{117, 119},
	{82, 119},
	{82, 119},
	{123, 119},
	{131, 119},
	{82, 119},
	{82, 119},
	{165, 119},
	{82, 119},
	{82, 119},
	{147, 119},
	{82, 119},
	{82, 119},
	{131, 119},
	{82, 119},
	{82, 119},
	{117, 719},
	{82, 119},
	{82, 119},
	{165, 119},
	{82, 119},
	{82, 119},
	{147, 119},
	{82, 119},
	{82, 119},
	{131, 119},
	{82, 119},
	{82, 119},
	{117, 119},
	{82, 119},
	{82, 119},
	{123, 119},
	{131, 119},
	{82, 119},
	{82, 119},
	{165, 119},
	{82, 119},
	{82, 119},
	{147, 119},
	{82, 119},
	{82, 119},
	{131, 119},
	{82, 119},
	{82, 119},
	{117, 719},
	{82, 119},
	{82, 119},
	{165, 119},
	{82, 119},
	{82, 119},
	{147, 119},
	{82, 119},
	{82, 119},
	{131, 119},
	{82, 119},
	{82, 119},
	{117, 119},
	{82, 119},
	{82, 119},
	{123, 119},
	{131, 119},
	{82, 119},
	{82, 119},
	{165, 119},
	{82, 119},
	{82, 119},
	{147, 119},
	{82, 119},
	{82, 119},
	{131, 119},
	{82, 119},
	{82, 119},
	{117, 719},
	{82, 119},
	{82, 119},
	{165, 119},
	{82, 119},
	{82, 119},
	{147, 119},
	{82, 119},
	{82, 119},
	{131, 119},
	{82, 119},
	{82, 119},
	{117, 119},
	{82, 119},
	{82, 119},
	{123, 119},
	{131, 119},
	{82, 119},
	{82, 119},
	{165, 119},
	{82, 119},
	{82, 119},
	{147, 119},
	{82, 119},
	{82, 119},
	{185, 89},
	{147, 89},
	{123, 89},
	{220, 89},
	{185, 89},
	{123, 89},
	{147, 89},
	{185, 89},
	{220, 89},
	{185, 89},
	{147, 89},
	{123, 89},
	{82, 119},
	{82, 119},
	{165, 119},
	{82, 119},
	{82, 119},
	{147, 119},
	{82, 119},
	{82, 119},
	{131, 119},
	{82, 119},
	{82, 119},
	{117, 119},
	{82, 119},
	{82, 119},
	{123, 119},
	{131, 119},
	{82, 119},
	{82, 119},
	{165, 119},
	{82, 119},
	{82, 119},
	{147, 119},
	{82, 119},
	{82, 119},
	{131, 119},
	{82, 119},
	{82, 119},
	{117, 719},
	{82, 119},
	{82, 119},
	{165, 119},
	{82, 119},
	{82, 119},
	{147, 119},
	{82, 119},
	{82, 119},
	{131, 119},
	{82, 119},
	{82, 119},
	{117, 119},
	{82, 119},
	{82, 119},
	{123, 119},
	{131, 119},
	{82, 119},
	{82, 119},
	{165, 119},
	{82, 119},
	{82, 119},
	{147, 119},
	{82, 119},
	{82, 119},
	{247, 89},
	{196, 89},
	{165, 89},
	{196, 89},
	{247, 89},
	{330, 89},
	{196, 89},
	{247, 89},
	{330, 89},
	{247, 89},
	{392, 89},
	{494, 89},
	{110, 119},
	{110, 119},
	{220, 119},
	{110, 119},
	{110, 119},
	{196, 119},
	{110, 119},
	{110, 119},
	{175, 119},
	{110, 119},
	{110, 119},
	{156, 119},
	{110, 119},
	{110, 119},
	{165, 119},
	{175, 119},
	{110, 119},
	{110, 119},
	{220, 119},
	{110, 119},
	{110, 119},
	{196, 119},
	{110, 119},
	{110, 119},
	{175, 119},
	{110, 119},
	{110, 119},
	{156, 719},
	{110, 119},
	{110, 119},
	{220, 119},
	{110, 119},
	{110, 119},
	{196, 119},
	{110, 119},
	{110, 119},
	{175, 119},
	{110, 119},
	{110, 119},
	{156, 119},
	{110, 119},
	{110, 119},
	{165, 119},
	{175, 119},
	{110, 119},
	{110, 119},
	{220, 119},
	{110, 119},
	{110, 119},
	{196, 119},
	{110, 119},
	{110, 119},
	{220, 89},
	{175, 89},
	{147, 89},
	{220, 89},
	{175, 89},
	{147, 89},
	{262, 89},
	{220, 89},
	{175, 89},
	{220, 89},
	{175, 89},
	{147, 89},
	{82, 119},
	{82, 119},
	{165, 119},
	{82, 119},
	{82, 119},
	{147, 119},
	{82, 119},
	{82, 119},
	{131, 119},
	{82, 119},
	{82, 119},
	{117, 119},
	{82, 119},
	{82, 119},
	{123, 119},
	{131, 119},
	{82, 119},
	{82, 119},
	{165, 119},
	{82, 119},
	{82, 119},
	{147, 119},
	{82, 119},
	{82, 119},
	{131, 119},
	{82, 119},
	{82, 119},
	{117, 719},
	{82, 119},
	{82, 119},
	{165, 119},
	{82, 119},
	{82, 119},
	{147, 119},
	{82, 119},
	{82, 119},
	{131, 119},
	{82, 119},
	{82, 119},
	{117, 119},
	{82, 119},
	{82, 119},
	{123, 119},
	{131, 119},
	{82, 119},
	{82, 119},
	{165, 119},
	{82, 119},
	{82, 119},
	{147, 119},
	{82, 119},
	{82, 119},
	{131, 119},
	{82, 119},
	{82, 119},
	{117, 719},
	{139, 119},
	{139, 119},
	{277, 119},
	{139, 119},
	{139, 119},
	{247, 119},
	{139, 119},
	{139, 119},
	{220, 119},
	{139, 119},
	{139, 119},
	{196, 119},
	{139, 119},
	{139, 119},
	{208, 119},
	{220, 119},
	{123, 119},
	{123, 119},
	{247, 119},
	{123, 119},
	{123, 119},
	{220, 119},
	{123, 119},
	{123, 119},
	{196, 119},
	{123, 119},
	{123, 119},
	{175, 719},
	{82, 119},
	{82, 119},
	{165, 119},
	{82, 119},
	{82, 119},
	{147, 119},
	{82, 119},
	{82, 119},
	{131, 119},
	{82, 119},
	{82, 119},
	{117, 119},
	{82, 119},
	{82, 119},
	{123, 119},
	{131, 119},
	{82, 119},
	{82, 119},
	{165, 119},
	{82, 119},
	{82, 119},
	{147, 119},
	{82, 119},
	{82, 119},
	{247, 89},
	{196, 89},
	{165, 89},
	{196, 89},
	{247, 89},
	{330, 89},
	{196, 89},
	{247, 89},
	{330, 89},
	{247, 89},
	{392, 89},
	{494, 89},
	{82, 119},
	{82, 119},
	{165, 119},
	{82, 119},
	{82, 119},
	{147, 119},
	{82, 119},
	{82, 119},
	{131, 119},
	{82, 119},
	{82, 119},
	{117, 119},
	{82, 119},
	{82, 119},
	{123, 119},
	{131, 119},
	{82, 119},
	{82, 119},
	{165, 119},
	{82, 119},
	{82, 119},
	{147, 119},
	{82, 119},
	{82, 119},
	{131, 119},
	{82, 119},
	{82, 119},
	{117, 719},
	{82, 119},
	{82, 119},
	{165, 119},
	{82, 119},
	{82, 119},
	{147, 119},
	{82, 119},
	{82, 119},
	{131, 119},
	{82, 119},
	{82, 119},
	{117, 119},
	{82, 119},
	{82, 119},
	{123, 119},
	{131, 119},
	{82, 119},
	{82, 119},
	{165, 119},
	{82, 119},
	{82, 119},
	{147, 119},
	{82, 119},
	{82, 119},
	{185, 89},
	{156, 89},
	{123, 89},
	{185, 89},
	{156, 89},
	{123, 89},
	{196, 89},
	{147, 89},
	{123, 89},
	{311, 89},
	{156, 89},
	{123, 89},
	{82, 119},
	{82, 119},
	{165, 119},
	{82, 119},
	{82, 119},
	{147, 119},
	{82, 119},
	{82, 119},
	{131, 119},
	{82, 119},
	{82, 119},
	{117, 119},
	{82, 119},
	{82, 119},
	{123, 119},
	{131, 119},
	{82, 119},
	{82, 119},
	{165, 119},
	{82, 119},
	{82, 119},
	{147, 119},
	{82, 119},
	{82, 119},
	{131, 119},
	{82, 119},
	{82, 119},
	{117, 719},
	{82, 119},
	{82, 119},
	{165, 119},
	{82, 119},
	{82, 119},
	{147, 119},
	{82, 119},
	{82, 119},
	{131, 119},
	{82, 119},
	{82, 119},
	{117, 119},
	{82, 119},
	{82, 119},
	{123, 119},
	{131, 119},
	{82, 119},
	{82, 119},
	{165, 119},
	{82, 119},
	{82, 119},
	{147, 119},
	{82, 119},
	{82, 119},
	{330, 89},
	{247, 89},
	{196, 89},
	{392, 89},
	{330, 89},
	{196, 89},
	{247, 89},
	{294, 89},
	{330, 89},
	{392, 89},
	{330, 89},
	{196, 89},
	{82, 119},
	{82, 119},
	{165, 119},
	{82, 119},
	{82, 119},
	{147, 119},
	{82, 119},
	{82, 119},
	{131, 119},
	{82, 119},
	{82, 119},
	{117, 119},
	{82, 119},
	{82, 119},
	{123, 119},
	{131, 119},
	{82, 119},
	{82, 119},
	{165, 119},
	{82, 119},
	{82, 119},
	{147, 119},
	{82, 119},
	{82, 119},
	{131, 119},
	{82, 119},
	{82, 119},
	{117, 719},
	{110, 119},
	{110, 119},
	{220, 119},
	{110, 119},
	{110, 119},
	{196, 119},
	{110, 119},
	{110, 119},
	{175, 119},
	{110, 119},
	{110, 119},
	{156, 119},
	{110, 119},
	{110, 119},
	{165, 119},
	{175, 119},
	{110, 119},
	{110, 119},
	{220, 119},
	{110, 119},
	{110, 119},
	{196, 119},
	{110, 119},
	{110, 119},
	{220, 89},
	{175, 89},
	{147, 89},
	{220, 89},
	{175, 89},
	{147, 89},
	{262, 89},
	{220, 89},
	{175, 89},
	{220, 89},
	{175, 89},
	{147, 89},
	{82, 119},
	{82, 119},
	{165, 119},
	{82, 119},
	{82, 119},
	{147, 119},
	{82, 119},
	{82, 119},
	{131, 119},
	{82, 119},
	{82, 119},
	{117, 119},
	{82, 119},
	{82, 119},
	{123, 119},
	{131, 119},
	{82, 119},
	{82, 119},
	{165, 119},
	{82, 119},
	{82, 119},
	{147, 119},
	{82, 119},
	{82, 119},
	{131, 119},
	{82, 119},
	{82, 119},
	{117, 719},
	{82, 119},
	{82, 119},
	{165, 119},
	{82, 119},
	{82, 119},
	{147, 119},
	{82, 119},
	{82, 119},
	{131, 119},
	{82, 119},
	{82, 119},
	{117, 119},
	{82, 119},
	{82, 119},
	{123, 119},
	{131, 119},
	{82, 119},
	{82, 119},
	{165, 119},
	{82, 119},
	{82, 119},
	{147, 119},
	{82, 119},
	{82, 119},
	{131, 119},
	{82, 119},
	{82, 119},
	{117, 719},
	{82, 119},
	{82, 119},
	{165, 119},
	{82, 119},
	{82, 119},
	{147, 119},
	{82, 119},
	{82, 119},
	{131, 119},
	{82, 119},
	{82, 119},
	{117, 119},
	{82, 119},
	{82, 119},
	{123, 119},
	{131, 119},
	{82, 119},
	{82, 119},
	{165, 119},
	{82, 119},
	{82, 119},
	{147, 119},
	{82, 119},
	{82, 119},
	{131, 119},
	{82, 119},
	{82, 119},
	{117, 719},
	{82, 119},
	{82, 119},
	{165, 119},
	{82, 119},
	{82, 119},
	{147, 119},
	{82, 119},
	{82, 119},
	{131, 119},
	{82, 119},
	{82, 119},
	{117, 119},
	{82, 119},
	{82, 119},
	{123, 119},
	{131, 119},
	{82, 119},
	{82, 119},
	{165, 119},
	{82, 119},
	{82, 119},
	{147, 119},
	{82, 119},
	{82, 119},
	{247, 89},
	{196, 89},
	{165, 89},
	{123, 89},
	{165, 89},
	{196, 89},
	{262, 89},
	{247, 89},
	{196, 89},
	{247, 89},
	{196, 89},
	{165, 89},
	{0, 0}
};

const Tone bump1[] = {
	{100, 300},
	{130, 300},
	{170, 200},
	{100, 150},
	{0, 0}
};

const Tone bump2[] = {
	{200, 220},
	{230, 200},
	{200, 100},
	{0, 0}
};

const Tone bump3[] = {
	{650, 90},
	{550, 130},
	{320, 220},
	{0, 0}
};

const Tone shot1[] = {
	{500, 200},
	{900, 130},
	{350, 90},
	{0, 0}
};

const Tone silent[] = {
	{0, 0}
};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

unsigned char buffer[100]="";
int position = 0;

extern int onMenu;
int kpad = -1;
extern int start_game;
extern int enemy_col[200];
extern int enemy_row[200];
extern int gameOn;
int n = 1;
int t = 0;

uint8_t name[16] ;


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

void musicOn() {
	PWM_Start();
	Change_Melody(menuT, ARRAY_LENGTH(menuT));
}

void bump_1() {
	Change_Melody(bump1, ARRAY_LENGTH(bump1));
}

void bump_2() {
	Change_Melody(bump2, ARRAY_LENGTH(bump2));
}

void bump_3() {
	Change_Melody(bump3, ARRAY_LENGTH(bump3));
}

void silent_() {
	Change_Melody(silent, ARRAY_LENGTH(silent));
}

void shot_1() {
	Change_Melody(shot1, ARRAY_LENGTH(shot1));
}

void bossfightmelody() {
	Change_Melody(doom, ARRAY_LENGTH(doom));
}

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim6;
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

  /* USER CODE END EXTI0_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
  /* USER CODE BEGIN EXTI0_IRQn 1 */

  for( int i = 0; i < 4; i++ ) {

	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, 0);

	  if(i == 0)
	  {
		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, 1);
		  if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_0))
		  {
			  kpad = 1;
		  }
	  }
	  else if(i == 1)
	  {
		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 1);
		  if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_0))
		  {
			  kpad = 4;
		  }
	  }
	  else if(i == 2)
	  {
		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, 1);
		  if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_0))
		  {
			  kpad = 7;
		  }
	  }
	  else if(i == 3)
	  {
		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, 1);
		  if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_0))
		  {
			  kpad = '*';
		  }
	  }

  }

  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, 1);

  /* USER CODE END EXTI0_IRQn 1 */
}

/**
  * @brief This function handles EXTI line1 interrupt.
  */
void EXTI1_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI1_IRQn 0 */

  /* USER CODE END EXTI1_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
  /* USER CODE BEGIN EXTI1_IRQn 1 */

  for( int i = 0; i < 4; i++ ) {

	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, 0);

	  if(i == 0)
	  {
		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, 1);
		  if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_1))
		  {
			  kpad = 2;
		  }
	  }
	  else if(i == 1)
	  {
		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 1);
		  if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_1))
		  {
			  kpad = 5;
		  }
	  }
	  else if(i == 2)
	  {
		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, 1);
		  if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_1))
		  {
			  kpad = 8;
		  }
	  }
	  else if(i == 3)
	  {
		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, 1);
		  if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_1))
		  {
			  kpad = 0;
		  }
	  }

  }

  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, 1);

  /* USER CODE END EXTI1_IRQn 1 */
}

/**
  * @brief This function handles EXTI line2 and Touch Sense controller.
  */
void EXTI2_TSC_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI2_TSC_IRQn 0 */

  /* USER CODE END EXTI2_TSC_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
  /* USER CODE BEGIN EXTI2_TSC_IRQn 1 */

  for( int i = 0; i < 4; i++ ) {

	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, 0);

	  if(i == 0)
	  {
		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, 1);
		  if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_2))
		  {
			  kpad = 3;
		  }
	  }
	  else if(i == 1)
	  {
		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 1);
		  if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_2))
		  {
			  kpad = 6;
		  }
	  }
	  else if(i == 2)
	  {
		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, 1);
		  if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_2))
		  {
			  kpad = 9;
		  }
	  }
	  else if(i == 3)
	  {
		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, 1);
		  if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_2))
		  {
			  kpad = '#';
		  }
	  }

  }

  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, 1);

  /* USER CODE END EXTI2_TSC_IRQn 1 */
}

/**
  * @brief This function handles EXTI line3 interrupt.
  */
void EXTI3_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI3_IRQn 0 */

  /* USER CODE END EXTI3_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
  /* USER CODE BEGIN EXTI3_IRQn 1 */

  for( int i = 0; i < 4; i++ ) {

	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, 0);

	  if(i == 0)
	  {
		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, 1);
		  if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_3))
		  {
			  kpad = 'A';
			  onMenu = 1;
		  }
	  }
	  else if(i == 1)
	  {
		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 1);
		  if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_3))
		  {
			  kpad = 'B';
		  }
	  }
	  else if(i == 2)
	  {
		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, 1);
		  if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_3))
		  {
			  kpad = 'C';
		  }
	  }
	  else if(i == 3)
	  {
		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, 1);
		  if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_3))
		  {
			  kpad = 'D';
		  }
	  }

  }

  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, 1);

  /* USER CODE END EXTI3_IRQn 1 */
}

/**
  * @brief This function handles ADC1 and ADC2 interrupts.
  */
void ADC1_2_IRQHandler(void)
{
  /* USER CODE BEGIN ADC1_2_IRQn 0 */

	int vol = HAL_ADC_GetValue(&hadc1);

	if ( vol < 300 )
	{
		volume = 0;
	} else if ( vol >= 300 && vol < 700 ) {
		volume = 5;
	} else if ( vol >= 700 && vol < 1300 ) {
		volume = 10;
	} else if ( vol >= 1300 && vol < 1600 ) {
		volume = 15;
	} else if ( vol >= 1600 && vol < 1900 ) {
		volume = 20;
	} else if ( vol >= 1900 && vol < 2200 ) {
		volume = 25;
	} else if ( vol >= 2200 && vol < 2500 ) {
		volume = 30;
	} else if ( vol >= 2500 && vol < 2800 ) {
		volume = 35;
	} else if ( vol >= 2800 && vol < 3100 ) {
		volume = 40;
	} else if ( vol >= 3100 && vol < 3400 ) {
		volume = 45;
	} else if ( vol >= 3400 && vol < 3700 ) {
		volume = 50;
	} else if ( vol >= 3700 ) {
		volume = 100;
	}

  /* USER CODE END ADC1_2_IRQn 0 */
  HAL_ADC_IRQHandler(&hadc1);
  /* USER CODE BEGIN ADC1_2_IRQn 1 */

  /* USER CODE END ADC1_2_IRQn 1 */
}

/**
  * @brief This function handles TIM2 global interrupt.
  */
void TIM2_IRQHandler(void)
{
  /* USER CODE BEGIN TIM2_IRQn 0 */

	if ( gameOn == 1 )
	{
		for ( int i = 0; i < n; i = i + 2 )
		{
			enemy_row[i]--;
			enemy_row[i+1]--;
		}

		n++;
		if ( n%2 == 0 && t == 0 ) {
			enemy_col[n] = 1;
			enemy_row[n] = 19;
		} else if ( t == 0 ) {
			enemy_col[n] = 1;
			enemy_row[n] = 39;
			t = 1;
		} else if ( n%2 == 0 && t == 1 ) {
			enemy_col[n] = 0;
			enemy_row[n] = 19;
		} else if ( t == 1 ) {
			enemy_col[n] = 0;
			enemy_row[n] = 39;
			t = 0;
		}
	}

  /* USER CODE END TIM2_IRQn 0 */
  HAL_TIM_IRQHandler(&htim2);
  /* USER CODE BEGIN TIM2_IRQn 1 */

  /* USER CODE END TIM2_IRQn 1 */
}

/**
  * @brief This function handles TIM3 global interrupt.
  */
void TIM3_IRQHandler(void)
{
  /* USER CODE BEGIN TIM3_IRQn 0 */

	if ( gameOn == 1 )
	{
		for ( int i = 0; i < n; i = i + 2 )
		{
			enemy_row[i]--;
			enemy_row[i+1]--;
		}

		n++;
		if ( n%2 == 0 && t == 0 ) {
			enemy_col[n] = 1;
			enemy_row[n] = 19;
		} else if ( t == 0 ) {
			enemy_col[n] = 1;
			enemy_row[n] = 39;
			t = 1;
		} else if ( n%2 == 0 && t == 1 ) {
			enemy_col[n] = 0;
			enemy_row[n] = 19;
		} else if ( t == 1 ) {
			enemy_col[n] = 0;
			enemy_row[n] = 39;
			t = 0;
		}
	}

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

		if ( kpad == 'B' && start_game == 1 )
		{
			for ( int i = 0; i < 16; i++ ) {
				name[i] = buffer[i];
			}
		}
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

/**
  * @brief This function handles Timer 6 interrupt and DAC underrun interrupts.
  */
void TIM6_DAC_IRQHandler(void)
{
  /* USER CODE BEGIN TIM6_DAC_IRQn 0 */

	if ( gameOn == 1 )
	{

		for ( int i = 0; i < n; i = i + 2 )
		{
			if ( enemy_row[i] >= 0 ) {
				enemy_row[i]--;
			}
			if ( enemy_row[i+1] >= 0 ) {
				enemy_row[i+1]--;
			}
		}

		n++;
		if ( n%2 == 0 && t == 0 ) {
			enemy_col[n] = 1;
			enemy_row[n] = 19;
		} else if ( t == 0 ) {
			enemy_col[n] = 1;
			enemy_row[n] = 39;
			t = 1;
		} else if ( n%2 == 0 && t == 1 ) {
			enemy_col[n] = 0;
			enemy_row[n] = 19;
		} else if ( t == 1 ) {
			enemy_col[n] = 0;
			enemy_row[n] = 39;
			t = 0;
		}

		if ( n == 40 ) n = 1;

	}

  /* USER CODE END TIM6_DAC_IRQn 0 */
  HAL_TIM_IRQHandler(&htim6);
  /* USER CODE BEGIN TIM6_DAC_IRQn 1 */

  /* USER CODE END TIM6_DAC_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
