/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "LiquidCrystal.h"
#include <stdio.h>
#include <stdlib.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

I2C_HandleTypeDef hi2c1;

RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim6;

UART_HandleTypeDef huart2;

PCD_HandleTypeDef hpcd_USB_FS;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_SPI1_Init(void);
static void MX_USB_PCD_Init(void);
static void MX_ADC1_Init(void);
static void MX_RTC_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM4_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM6_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int onMenu = 0;
int x = 0;
extern int kpad;
int start_game = 0;
int mode = 1;
int endGame = 0;
int gameOn = 0;
extern int n;
int shot = 0;
int shot_row[200];
int shot_col[200];
int enemy_col[200];
int enemy_row[200];
int nCop = 0;
int shotE_row[200];
int shotE_col[200];
int shotE_count = 0;
int shotE = 0;
int kill = 0;
int health1 = 7;
int health2 = 5;
int health3 = 3;
int ship_row;
int ship_col;
int showOnce = 0;
int showOnce2 = 0;
int bosshealth = 13;
int ship_bossfight_health = 2;
int shot_enemyBoss = 0;
int autoFire = 0;

unsigned char msg1[19] = "Enter your name: \n";
unsigned char msg2[88] = "The game has started \n Game mode : Easy \n You have to kill 15 enemies \n health : 7 \n";
unsigned char msg3[90] = "The game has started \n Game mode : Normal \n You have to kill 25 enemies \n health : 5 \n";
unsigned char msg4[88] = "The game has started \n Game mode : Hard \n You have to kill 35 enemies \n health : 3 \n";
unsigned char msg5[24] = "Be careful, health : 1\n";
unsigned char msg6[33] = "Good job, Only 3 enemies to win\n";
unsigned char msg7[12] = "You Won :)\n";
unsigned char msg8[13] = "You Lose :(\n";
unsigned char msg9[67] = "Welcome to boss mode\n your health : 2 AND enemy boss health : 13\n";

RTC_DateTypeDef mydate;
RTC_TimeTypeDef mytime;
uint8_t Time[6];
uint8_t Date[9];

unsigned char data[1];

extern uint8_t name[16];

typedef unsigned char byte;

byte sword[] = {
  0x04,
  0x04,
  0x04,
  0x04,
  0x15,
  0x0E,
  0x04,
  0x04
};

byte enemy[] = {
  0x0C,
  0x0E,
  0x0F,
  0x18,
  0x18,
  0x0F,
  0x0E,
  0x0C
};

byte ship[] = {
  0x10,
  0x1E,
  0x1C,
  0x07,
  0x07,
  0x1C,
  0x1E,
  0x10
};

byte bullet1[] = {
  0x00,
  0x0E,
  0x00,
  0x00,
  0x00,
  0x00,
  0x0E,
  0x00
};

byte bullet2[] = {
  0x08,
  0x1F,
  0x08,
  0x00,
  0x00,
  0x08,
  0x1F,
  0x08
};

byte bullet3[] = {
  0x00,
  0x00,
  0x03,
  0x1E,
  0x1E,
  0x03,
  0x00,
  0x00
};

byte smile[] = {
  0x00,
  0x00,
  0x0A,
  0x00,
  0x11,
  0x0E,
  0x00,
  0x00
};

byte sad[] = {
  0x00,
  0x00,
  0x0A,
  0x00,
  0x0E,
  0x11,
  0x00,
  0x00
};

byte full[] = {
  0x1F,
  0x1F,
  0x1F,
  0x1F,
  0x1F,
  0x1F,
  0x1F,
  0x1F
};

byte boss1[] = {
  0x01,
  0x03,
  0x03,
  0x1F,
  0x1F,
  0x03,
  0x03,
  0x01
};

byte boss2[] = {
  0x00,
  0x07,
  0x04,
  0x1C,
  0x1F,
  0x1C,
  0x04,
  0x07
};

byte boss3[] = {
  0x07,
  0x04,
  0x1C,
  0x1F,
  0x1C,
  0x04,
  0x07,
  0x00
};

void start_page() {
	LiquidCrystal(GPIOD, GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10, GPIO_PIN_11, GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14);

	createChar(0, sword);
	createChar(1, enemy);
	createChar(2, ship);

	setCursor(0,0);
	print("#");
	print("#");
	setCursor(0,1);
	print("#");

	setCursor(18,0);
	print("#");
	print("#");
	setCursor(19,1);
	print("#");

	setCursor(20,1);
	print("#");
	print("#");
	setCursor(20,0);
	print("#");

	setCursor(38,1);
	print("#");
	print("#");
	setCursor(39,0);
	print("#");

	setCursor(7,0);
	print("SPACE");
	setCursor(5,1);
	print("INVADERS ");
	write(0);

	int i = 5;
	while ( i > 0 )
	{
		if ( i == 5 )
		{
			setCursor(31,0);
			print(" ");
			setCursor(34,0);
			write(1);

			setCursor(26,0);
			print(" ");
			setCursor(26,1);
			write(2);
		}
		if ( i == 4 )
		{
			setCursor(34,0);
			print(" ");
			setCursor(33,0);
			write(1);

			setCursor(26,1);
			print(" ");
			setCursor(26,0);
			write(2);
		}
		if ( i == 3 )
		{
			setCursor(33,0);
			print(" ");
			setCursor(32,0);
			write(1);

			setCursor(26,0);
			print(" ");
			setCursor(26,1);
			write(2);
		}
		if ( i == 2 )
		{
			setCursor(32,0);
			print(" ");
			setCursor(31,0);
			write(1);

			setCursor(26,1);
			print(" ");
			setCursor(26,0);
			write(2);
			i = 6;
		}
		if ( onMenu == 1 )
		{
			i = 0;
		}

		HAL_Delay(1000);
		i--;
	}

}

void menu() {

	musicOn();

	createChar(1, enemy);

	x = 0;
	int i = 39;
	while ( onMenu == 1 ) {

		HAL_ADC_Start_IT(&hadc1);

		if ( x == 0 ) {
			setCursor(0,0);
			print("                                                                                  ");

			setCursor(3,1);
			print("Start Game -> B");
			setCursor(25,0);
			print("About -> C");

			x = 1;
		}

		setCursor(i,1);
		write(1);

		HAL_Delay(250);
		setCursor(8,0);
		print("MENU");
		HAL_Delay(550);
		setCursor(8,0);
		print("    ");

		setCursor(i,1);
		print(" ");
		i--;
		if ( i == 19 ) i = 39;

		if( kpad == 'B' || kpad == 'C' || kpad == 'D') onMenu = 0;
	}
	if( kpad == 'C' ) {
		about();
	} else if ( kpad == 'B' ) {
		start_game = 1;
		startGame();
	} else if ( kpad == 'D' ) {
		boss_fight();
	}

}

void about() {

	x = 0;
	while ( kpad == 'C' ) {
		if ( x == 0 )
		{
			setCursor(0,0);
			print("                                                                                  ");
			setCursor(5,1);
			print("Ali Dashti");
			setCursor(23,0);
			print("Sajjad Cheravi");
			x = 1;
		}

		HAL_Delay(250);
		setCursor(7,0);
		print("about");
		HAL_Delay(550);
		setCursor(7,0);
		print("     ");

		HAL_RTC_GetTime(&hrtc, &mytime, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &mydate, RTC_FORMAT_BIN);
		setCursor(20,1);
		sprintf(Time, " %2d:%2d:%2d ", mytime.Hours, mytime.Minutes, mytime.Seconds);
		print(Time);
		sprintf(Date, " 20%d/%d/%d", mydate.Year, mydate.Month, mydate.Date);
		print(Date);

	}
	if ( kpad == 'A' ) {
		NVIC_SystemReset();
	} else {
		kpad = 'C';
		about();
	}

}


void startGame() {

	x = 0;
	while ( kpad == 'B' ) {
		if ( x == 0 && start_game == 1 )
		{
			HAL_UART_Transmit(&huart2, msg1, sizeof(unsigned char) * 18, 100);

			setCursor(0,0);
			print("                                                                                  ");
			setCursor(2,1);
			print("Enter your name");
			setCursor(22,0);
			print("and then press *");
			x = 1;
		}
		if ( x == 0 && start_game == 2 )
		{
			setCursor(0,0);
			print("                                                                                  ");
			setCursor(2,0);
			print("your name is :");
			setCursor(3,1);
			print(name);
			setCursor(24,0);
			print("accept -> *");
			x = 1;
		}
		if ( x == 0 && start_game == 3 )
		{
			setCursor(0,0);
			print("                                                                                  ");
			setCursor(2,0);
			print("Select game mode");
			setCursor(5,1);
			print("Easy -> 1");
			setCursor(24,0);
			print("Normal -> 2");
			setCursor(25,1);
			print("Hard -> 3");
			x = 1;
		}
		if ( x == 0 && start_game == 4 )
		{
			setCursor(0,0);
			print("                                                                                  ");
			if ( mode == 1 )
			{
				setCursor(4,0);
				print("mode : Easy");
			}
			else if ( mode == 2 )
			{
				setCursor(4,0);
				print("mode : Normal");
			}
			else if ( mode == 3 )
			{
				setCursor(4,0);
				print("mode : Hard");
			}
			setCursor(4,1);
			print("are you sure?");
			setCursor(25,0);
			print("Start -> *");
			setCursor(26,1);
			print("Back -> #");
			x = 1;
		}

		if ( kpad == '#' )
		{
			kpad = 'B';
			start_game--;
			x = 0;
		}

		if ( kpad == '*' )
		{
			kpad = 'B';
			start_game++;
			x = 0;
		} else if ( kpad == 1 ) {
			kpad = 'B';
			start_game++;
			x = 0;
			mode = 1;
		} else if ( kpad == 2 ) {
			kpad = 'B';
			start_game++;
			x = 0;
			mode = 2;
		} else if ( kpad == 3 ) {
			kpad = 'B';
			start_game++;
			x = 0;
			mode = 3;
		}

		if ( start_game == 5 )
		{
			kpad = -1;
			gameOn = 1;
			if ( mode == 1 ) game_mode1();
			else if ( mode == 2 ) game_mode2();
			else if ( mode == 3 ) game_mode3();
		}

	}
	if ( kpad == 'A' ) {
		menu();
	} else if ( kpad == '#' ) {
		kpad = 'B';
		start_game--;
		startGame();
	} else if ( kpad == '*' ) {
		kpad = 'B';
		start_game++;
		startGame();
	} else if ( kpad == 1 ) {
		kpad = 'B';
		start_game++;
		mode = 1;
		startGame();
	} else if ( kpad == 2 ) {
		kpad = 'B';
		start_game++;
		mode = 2;
		startGame();
	} else if ( kpad == 3 ) {
		kpad = 'B';
		start_game++;
		mode = 3;
		startGame();
	} else {
		kpad = 'B';
		startGame();
	}

}

void ship_do( int col ) { //col az 1 ta 4

	//createChar(2, ship);

	if ( col == 1 )
	{
		setCursor(20,1);
		print(" ");
		setCursor(0,1);
		print(" ");
		setCursor(0,0);
		write(2);
		ship_row = 0;
		ship_col = 0;
	} else if ( col == 2 ) {
		setCursor(0,0);
		print(" ");
		setCursor(20,0);
		print(" ");
		setCursor(0,1);
		write(2);
		ship_row = 0;
		ship_col = 1;
	} else if ( col == 3 ) {
		setCursor(0,1);
		print(" ");
		setCursor(20,1);
		print(" ");
		setCursor(20,0);
		write(2);
		ship_row = 20;
		ship_col = 0;
	} else if ( col == 4 ) {
		setCursor(20,0);
		print(" ");
		setCursor(0,0);
		print(" ");
		setCursor(20,1);
		write(2);
		ship_row = 20;
		ship_col = 1;
	}

}

void bullet_1( int row, int col ) {

	//createChar(3, bullet1);

	if ( row == 1 )
	{
		setCursor(row,col);
		write(3);
	} else if ( row == 20 ) {
		setCursor(row-1,col);
		print(" ");
	} else if ( row == 40 ) {
		setCursor(row-1,col);
		print(" ");
	} else {
		setCursor(row-1,col);
		print(" ");
		setCursor(row,col);
		write(3);
	}

}

void bullet_2( int row, int col ) {

	//createChar(4, bullet2);

	if ( row == 0 )
	{
		setCursor(row+1,col);
		print(" ");
	} else if ( row == 20 ) {
		setCursor(row+1,col);
		print(" ");
	} else {
		setCursor(row+1,col);
		print(" ");
		setCursor(row,col);
		write(4);
	}

}

void enemy_do( int row, int col ) {

	//createChar(1, enemy);

	if ( row == 19 || row == 39 )
	{
		setCursor(row,col);
		write(1);
	} else {
		setCursor(row+1,col);
		print(" ");
		setCursor(row,col);
		write(1);
	}

}

void bump() {

	//Ship's bullet to the enemy
	for ( int i = 0; i < n; i++ )
	{
		for ( int j = 0; j < shot; j++ )
		{
			if ( shot_row[j] < 40 )
			{
				if ( shot_row[j] == enemy_row[i] && shot_col[j] == enemy_col[i] )
				{
					setCursor(enemy_row[i],enemy_col[i]);
					print(" ");
					setCursor(enemy_row[i]-1,enemy_col[i]);
					print(" ");
					shot_row[j] = 41;
					enemy_row[i] = -1;
					kill++;
					bump_1();
				}
			}
		}
	}

	//Ship bullet to enemy bullet
	for ( int i = 0; i < shot; i++ )
	{
		for ( int j = 0; j < shotE_count; j++ )
		{
			if ( shot_row[i] < 40 && shotE_row[j] > 0 )
			{
				if ( shot_row[i] == shotE_row[j] && shot_col[i] == shotE_col[j] )
				{
					setCursor(shot_row[i]+1,shot_col[i]);
					print(" ");
					setCursor(shot_row[i],shot_col[i]);
					print(" ");
					setCursor(shot_row[i]-1,shot_col[i]);
					print(" ");
					shot_row[i] = 41;
					shotE_row[j] = -1;
					bump_2();
				}
			}
		}
	}

	//Enemy bullet to the ship
	for ( int i = 0; i < shotE_count; i++ )
	{
		if ( shotE_row[i] > -1 )
		{
			if ( shotE_row[i] == ship_row && shotE_col[i] == ship_col )
			{
				health1--;
				health2--;
				health3--;
				bump_3();
			}
		}
	}

	//Enemy to the ship
	for ( int i = 0; i < n; i++ )
	{
		if ( enemy_row[i] > -1 )
		{
			if ( enemy_row[i] == ship_row && enemy_col[i] == ship_col )
			{
				health1--;
				health2--;
				health3--;
				enemy_row[i] = -1;
				bump_3();
			}
		}
	}

	if ( mode == 1 )
	{
		if ( kill == 15 )
		{
			//end game and win
			//boss_fight();
			endGame = 1;
			victory(kill);
		} else if ( health1 == 0 )
		{
			//end game and lose
			endGame = 1;
			losing(kill);
		} else if ( health1 == 1 && showOnce == 0 ) {
			HAL_UART_Transmit(&huart2, msg5, sizeof(unsigned char) * 23, 100);
			showOnce = 1;
		}
		if ( kill == 12 && showOnce2 == 0 )
		{
			HAL_UART_Transmit(&huart2, msg6, sizeof(unsigned char) * 32, 100);
			showOnce2 = 1;
		}

	} else if ( mode == 2 ) {

		if ( kill == 25 )
		{
			//end game and win
			//boss_fight();
			endGame = 1;
			victory(kill);
		} else if ( health2 == 0 )
		{
			//end game and lose
			endGame = 1;
			losing(kill);
		} else if ( health2 == 1 && showOnce == 0 ) {
			HAL_UART_Transmit(&huart2, msg5, sizeof(unsigned char) * 23, 100);
			showOnce = 1;
		}
		if ( kill == 22 && showOnce2 == 0 )
		{
			HAL_UART_Transmit(&huart2, msg6, sizeof(unsigned char) * 32, 100);
			showOnce2 = 1;
		}

	} else if ( mode == 3 ) {

		if ( kill == 35 )
		{
			//end game and win
			//boss_fight();
			endGame = 1;
			victory(kill);
		} else if ( health3 == 0 )
		{
			//end game and lose
			endGame = 1;
			losing(kill);
		} else if ( health3 == 1 && showOnce == 0 ) {
			HAL_UART_Transmit(&huart2, msg5, sizeof(unsigned char) * 23, 100);
			showOnce = 1;
		}
		if ( kill == 32 && showOnce2 == 0 )
		{
			HAL_UART_Transmit(&huart2, msg6, sizeof(unsigned char) * 32, 100);
			showOnce2 = 1;
		}

	}


}

void victory( int kill ) {

	HAL_UART_Transmit(&huart2, msg7, sizeof(unsigned char) * 11, 100);

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, 1);
	createChar(5, smile);

	setCursor(0,0);
	print("                                                                                  ");

	char ckill[2];
	sprintf(ckill, "%d", kill);
	while ( kpad != 'A' )
	{
		setCursor(5,0);
		write(5);

		setCursor(7,0);
		print("VICTORY");

		setCursor(15,0);
		write(5);

		setCursor(3,1);
		print(name);

		setCursor(26,0);
		print("you won!");


		setCursor(25,1);
		print("Kill : ");
		print(ckill);
	}

	if ( kpad == 'A' ) NVIC_SystemReset();

}

void losing( int kill ) {

	HAL_UART_Transmit(&huart2, msg8, sizeof(unsigned char) * 12, 100);

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, 1);
	createChar(6, sad);

	setCursor(0,0);
	print("                                                                                  ");

	char ckill[2];
	sprintf(ckill, "%d", kill);
	while ( kpad != 'A' )
	{
		setCursor(4,0);
		write(6);

		setCursor(6,0);
		print("Game Over");

		setCursor(16,0);
		write(6);

		setCursor(3,1);
		print(name);

		setCursor(26,0);
		print("you lose!");


		setCursor(25,1);
		print("Kill : ");
		print(ckill);
	}

	if ( kpad == 'A' ) NVIC_SystemReset();

}

//Easy
void game_mode1() {

	silent_();

	HAL_UART_Transmit(&huart2, msg2, sizeof(unsigned char) * 84, 100);

	HAL_TIM_Base_Start_IT(&htim2);

	createChar(1, enemy);
	createChar(2, ship);
	createChar(3, bullet1);
	createChar(4, bullet2);

	setCursor(0,0);
	print("                                                                                  ");

	enemy_col[0] = 0;
	enemy_col[1] = 0;
	enemy_row[0] = 19;
	enemy_row[1] = 39;

	int col = 2;
	ship_do(col);

	int r = 0;

	while ( endGame == 0 ) {

		//Auto control
		if ( kpad == 1 )
		{
			autoShip(col);
			if ( col == 4 )
			{
				col = 1;
			} else {
				col++;
			}
		}

		//fire
		if ( kpad == 5 )
		{
			if ( col == 1 )
			{
				shot_col[shot] = 0;
				shot_row[shot] = 1;
			} else if ( col == 2 ) {
				shot_col[shot] = 1;
				shot_row[shot] = 1;
			} else if ( col == 3 ) {
				shot_col[shot] = 0;
				shot_row[shot] = 21;
			} else if ( col == 4 ) {
				shot_col[shot] = 1;
				shot_row[shot] = 21;
			}

			shot++;
			kpad = -1;
			shot_1();
		}
		if ( shot > 0 )
		{
			for ( int i = 0; i < shot; i++ )
			{
				if ( shot_row[i] < 41 )
				{
					bullet_1(shot_row[i], shot_col[i]);
				}
				if ( shot_row[i] == 20 ) shot_row[i] = 40;
				shot_row[i]++;
			}
		}

		//turn right
		if ( kpad == 9 )
		{
			if ( col == 4 )
			{
				col = 1;
			} else {
				col++;
			}
			kpad = -1;
		}

		//turn left
		if ( kpad == 7 )
		{
			if ( col == 1 )
			{
				col = 4;
			} else {
				col--;
			}
			kpad = -1;
		}

		ship_do(col);

		if ( nCop != n )
		{
			for ( int i = 0; i < n; i = i + 2 )
			{
				if ( enemy_row[i] >= 0 ) enemy_do(enemy_row[i], enemy_col[i]);
				if ( enemy_row[i+1] >= 0 ) enemy_do(enemy_row[i+1], enemy_col[i+1]);
				if ( enemy_row[i] == 20 ) enemy_row[i] = -1;
				if ( enemy_row[i+1] == 20 ) enemy_row[i+1] = -1;
			}

			nCop = n;
			shotE = 0;
		}

		if ( n%2 == 0 && shotE == 0 )
		{
			r = rand() % n ;
			if ( enemy_row[r] > 1 )
			{
				shotE_row[shotE_count] = enemy_row[r]-1;
				shotE_col[shotE_count] = enemy_col[r];
				shotE = 1;
				shotE_count++;
			}
		}

		if ( shotE_count > 0 )
		{
			for ( int i = 0; i < shotE_count; i++ )
			{
				if ( shotE_row[i] >= 0 )
				{
					bullet_2(shotE_row[i], shotE_col[i]);
					HAL_Delay(50);
					for ( int j = 0; j < n; j++ )
					{
						if ( shotE_row[i] > 0 && enemy_row[j] > 0 )
						{
							if ( shotE_row[i] == enemy_row[j]-1 && shotE_col[i] == enemy_col[j] )
							{
								 enemy_do(enemy_row[j], enemy_col[j]);
							}
						}
					}
					shotE_row[i]--;
				}
				if ( shotE_row[i] == 19 ) shotE_row[i] = -1;

			}
		}

		bump();

	}

}

//Normal
void game_mode2() {

	silent_();

	HAL_UART_Transmit(&huart2, msg3, sizeof(unsigned char) * 86, 100);

	HAL_TIM_Base_Start_IT(&htim3);

	createChar(1, enemy);
	createChar(2, ship);
	createChar(3, bullet1);
	createChar(4, bullet2);

	setCursor(0,0);
	print("                                                                                  ");

	enemy_col[0] = 0;
	enemy_col[1] = 0;
	enemy_row[0] = 19;
	enemy_row[1] = 39;

	int col = 2;
	ship_do(col);

	int r = 0;

	while ( endGame == 0 ) {

		//Auto control
		if ( kpad == 1 )
		{
			autoShip(col);
			if ( col == 4 )
			{
				col = 1;
			} else {
				col++;
			}
		}

		//fire
		if ( kpad == 5 )
		{
			if ( col == 1 )
			{
				shot_col[shot] = 0;
				shot_row[shot] = 1;
			} else if ( col == 2 ) {
				shot_col[shot] = 1;
				shot_row[shot] = 1;
			} else if ( col == 3 ) {
				shot_col[shot] = 0;
				shot_row[shot] = 21;
			} else if ( col == 4 ) {
				shot_col[shot] = 1;
				shot_row[shot] = 21;
			}

			shot++;
			kpad = -1;
			shot_1();
		}
		if ( shot > 0 )
		{
			for ( int i = 0; i < shot; i++ )
			{
				if ( shot_row[i] < 41 )
				{
					bullet_1(shot_row[i], shot_col[i]);
				}
				if ( shot_row[i] == 20 ) shot_row[i] = 40;
				shot_row[i]++;
			}
		}

		//turn right
		if ( kpad == 9 )
		{
			if ( col == 4 )
			{
				col = 1;
			} else {
				col++;
			}
			kpad = -1;
		}

		//turn left
		if ( kpad == 7 )
		{
			if ( col == 1 )
			{
				col = 4;
			} else {
				col--;
			}
			kpad = -1;
		}

		ship_do(col);

		if ( nCop != n )
		{
			for ( int i = 0; i < n; i = i + 2 )
			{
				if ( enemy_row[i] >= 0 ) enemy_do(enemy_row[i], enemy_col[i]);
				if ( enemy_row[i+1] >= 0 ) enemy_do(enemy_row[i+1], enemy_col[i+1]);
				if ( enemy_row[i] == 20 ) enemy_row[i] = -1;
				if ( enemy_row[i+1] == 20 ) enemy_row[i+1] = -1;
			}

			nCop = n;
			shotE = 0;
		}

		if ( n%2 == 0 && shotE == 0 )
		{
			r = rand() % n ;
			if ( enemy_row[r] > 1 )
			{
				shotE_row[shotE_count] = enemy_row[r]-1;
				shotE_col[shotE_count] = enemy_col[r];
				shotE = 1;
				shotE_count++;
			}
		}

		if ( shotE_count > 0 )
		{
			for ( int i = 0; i < shotE_count; i++ )
			{
				if ( shotE_row[i] >= 0 )
				{
					bullet_2(shotE_row[i], shotE_col[i]);
					HAL_Delay(50);
					for ( int j = 0; j < n; j++ )
					{
						if ( shotE_row[i] > 0 && enemy_row[j] > 0 )
						{
							if ( shotE_row[i] == enemy_row[j]-1 && shotE_col[i] == enemy_col[j] )
							{
								 enemy_do(enemy_row[j], enemy_col[j]);
							}
						}
					}
					shotE_row[i]--;
				}
				if ( shotE_row[i] == 19 ) shotE_row[i] = -1;

			}
		}

		bump();

	}

}

//Hard
void game_mode3() {

	silent_();

	HAL_UART_Transmit(&huart2, msg4, sizeof(unsigned char) * 84, 100);

	HAL_TIM_Base_Start_IT(&htim6);

	createChar(1, enemy);
	createChar(2, ship);
	createChar(3, bullet1);
	createChar(4, bullet2);

	setCursor(0,0);
	print("                                                                                  ");

	enemy_col[0] = 0;
	enemy_col[1] = 0;
	enemy_row[0] = -1;
	enemy_row[1] = -1;

	int col = 2;
	ship_do(col);

	int r = 0;

	while ( endGame == 0 ) {

		//Auto control
		if ( kpad == 1 )
		{
			autoShip(col);
			if ( col == 4 )
			{
				col = 1;
			} else {
				col++;
			}
		}

		//fire
		if ( kpad == 5 )
		{
			if ( col == 1 )
			{
				shot_col[shot] = 0;
				shot_row[shot] = 1;
			} else if ( col == 2 ) {
				shot_col[shot] = 1;
				shot_row[shot] = 1;
			} else if ( col == 3 ) {
				shot_col[shot] = 0;
				shot_row[shot] = 21;
			} else if ( col == 4 ) {
				shot_col[shot] = 1;
				shot_row[shot] = 21;
			}

			shot++;
			kpad = -1;
			shot_1();
		}
		if ( shot > 0 )
		{
			for ( int i = 0; i < shot; i++ )
			{
				if ( shot_row[i] < 41 )
				{
					bullet_1(shot_row[i], shot_col[i]);
				}
				if ( shot_row[i] == 20 ) shot_row[i] = 40;
				shot_row[i]++;
			}
		}

		//turn right
		if ( kpad == 9 )
		{
			if ( col == 4 )
			{
				col = 1;
			} else {
				col++;
			}
			kpad = -1;
		}

		//turn left
		if ( kpad == 7 )
		{
			if ( col == 1 )
			{
				col = 4;
			} else {
				col--;
			}
			kpad = -1;
		}

		ship_do(col);

		if ( nCop != n )
		{
			for ( int i = 0; i < n; i = i + 2 )
			{
				if ( enemy_row[i] >= 0 ) enemy_do(enemy_row[i], enemy_col[i]);
				if ( enemy_row[i+1] >= 0 ) enemy_do(enemy_row[i+1], enemy_col[i+1]);
				if ( enemy_row[i] == 20 ) enemy_row[i] = -1;
				if ( enemy_row[i+1] == 20 ) enemy_row[i+1] = -1;
			}

			nCop = n;
			shotE = 0;
		}

		if ( n%4 == 0 && shotE == 0 )
		{
			r = rand() % n ;
			if ( enemy_row[r] > 1 )
			{
				shotE_row[shotE_count] = enemy_row[r]-1;
				shotE_col[shotE_count] = enemy_col[r];
				shotE = 1;
				shotE_count++;
			}
		}

		if ( shotE_count > 0 )
		{
			for ( int i = 0; i < shotE_count; i++ )
			{
				if ( shotE_row[i] >= 0 )
				{
					bullet_2(shotE_row[i], shotE_col[i]);
					for ( int j = 0; j < n; j++ )
					{
						if ( shotE_row[i] > 0 && enemy_row[j] > 0 )
						{
							if ( shotE_row[i] == enemy_row[j]-1 && shotE_col[i] == enemy_col[j] )
							{
								 enemy_do(enemy_row[j], enemy_col[j]);
							}
						}
					}
					shotE_row[i]--;
				}
				if ( shotE_row[i] == 19 ) shotE_row[i] = -1;

			}
		}

		bump();

	}

}

void boss( int row , int col ) {

	setCursor(row-1,1);
	print("     ");
	setCursor(row-1,0);
	print("     ");
	setCursor(row+20-1,1);
	print("     ");
	setCursor(row+20-1,0);
	print("     ");

	if ( col == 2 )
	{
		//middle
		setCursor(row,1);
		write(8);
		setCursor(row+20,0);
		write(8);
		setCursor(row+1,1);
		write(8);
		setCursor(row+21,0);
		write(8);

		setCursor(row-1,1);
		write(5);
		setCursor((row+20)-1,0);
		write(5);

		setCursor(row+2,1);
		write(6);
		setCursor((row+21)+1,0);
		write(7);

	} else if ( col == 3 ) {
		//right
		setCursor(row+20,0);
		write(8);
		setCursor(row+20,1);
		write(8);
		setCursor(row+21,0);
		write(8);
		setCursor(row+21,1);
		write(8);

		setCursor((row+20)-1,1);
		write(5);
		setCursor((row+20)-1,0);
		write(5);

		setCursor((row+21)+1,1);
		write(6);
		setCursor((row+21)+1,0);
		write(7);

	} else if ( col == 1 ) {
		//left
		setCursor(row,1);
		write(8);
		setCursor(row,0);
		write(8);
		setCursor(row+1,1);
		write(8);
		setCursor(row+1,0);
		write(8);

		setCursor(row-1,1);
		write(5);
		setCursor(row-1,0);
		write(5);

		setCursor(row+2,1);
		write(6);
		setCursor(row+2,0);
		write(7);
	}

}

void bump_bossfight( int bossRow1, int bossCol1 ,int bossRow2, int bossCol2 ) {

	//Ship's bullet to the enemy
	for ( int j = 0; j < shot; j++ )
	{
		if ( shot_row[j] < 40 )
		{
			if ( (shot_row[j] == bossRow1 && shot_col[j] == bossCol1) || (shot_row[j] == bossRow2 && shot_col[j] == bossCol2) )
			{
				setCursor(shot_row[j]-1,shot_col[j]);
				print(" ");
				shot_row[j] = 41;
				bosshealth--;
				bump_1();
			}
		}
	}

	//Ship bullet to enemy bullet
	for ( int i = 0; i < shot; i++ )
	{
		for ( int j = 0; j < shot_enemyBoss; j++ )
		{
			if ( shot_row[i] < 40 && shotE_row[j] > 0 )
			{
				if ( shot_row[i] == shotE_row[j] && shot_col[i] == shotE_col[j] )
				{
					setCursor(shot_row[i]+1,shot_col[i]);
					print("  ");
					setCursor(shot_row[i],shot_col[i]);
					print("  ");
					setCursor(shot_row[i]-1,shot_col[i]);
					print("  ");
					shot_row[i] = 41;
					shotE_row[j] = -1;
					bump_2();
				}
			}
			if (shot_enemyBoss >= 6 ) break;
		}
	}

	//Enemy bullet to the ship
	for ( int i = 0; i < shot_enemyBoss; i++ )
	{
		if ( shotE_row[i] > -1 )
		{
			if ( shotE_row[i] == ship_row && shotE_col[i] == ship_col )
			{
				ship_bossfight_health--;
				bump_3();
			}
		}
		if (shot_enemyBoss >= 6 ) break;
	}

	//Enemy to the ship
	for ( int i = 0; i < n; i++ )
	{
		if ( bossRow1 > -1 )
		{
			if ( (bossRow1 == ship_row && bossCol1 == ship_col) || (bossRow2 == ship_row && bossCol2 == ship_col) )
			{
				ship_bossfight_health--;
				bump_3();
			}
		}
	}


	if ( ship_bossfight_health == 0 )
	{
		losing(kill);
	} else if ( bosshealth == 0 ) {
		kill++;
		victory(kill);
	}

}

void boss_fight() {

	silent_();

	HAL_UART_Transmit(&huart2, msg9, sizeof(unsigned char) * 65, 100);

	createChar(8, full);
	createChar(5, boss1);
	createChar(6, boss2);
	createChar(7, boss3);

	createChar(2, ship);
	createChar(3, bullet1);
	createChar(4, bullet3);

	setCursor(0,0);
	print("                                                                                  ");

	bossfightmelody();

	int row = 17;
	int colE = 2;
	boss(row, colE);

	int col = 2;
	ship_do(col);

	int i = 100;

	while ( bosshealth > 0 && ship_bossfight_health > 0 ) {

		//fire
		if ( kpad == 5 )
		{
			if ( col == 1 )
			{
				shot_col[shot] = 0;
				shot_row[shot] = 1;
			} else if ( col == 2 ) {
				shot_col[shot] = 1;
				shot_row[shot] = 1;
			} else if ( col == 3 ) {
				shot_col[shot] = 0;
				shot_row[shot] = 21;
			} else if ( col == 4 ) {
				shot_col[shot] = 1;
				shot_row[shot] = 21;
			}

			shot++;
			kpad = -1;
			shot_1();
		}
		if ( shot > 0 )
		{
			for ( int i = 0; i < shot; i++ )
			{
				if ( shot_row[i] < 41 )
				{
					bullet_1(shot_row[i], shot_col[i]);
				}
				if ( shot_row[i] == 20 ) shot_row[i] = 40;
				shot_row[i]++;
			}
		}

		//turn right
		if ( kpad == 9 )
		{
			if ( col == 4 )
			{
				col = 1;
			} else {
				col++;
			}
			kpad = -1;
		}

		//turn left
		if ( kpad == 7 )
		{
			if ( col == 1 )
			{
				col = 4;
			} else {
				col--;
			}
			kpad = -1;
		}

		ship_do(col);

		if ( shot_enemyBoss > 0 )
		{
			for ( int i = 0; i < shot_enemyBoss; i = i + 2 )
			{
				if ( shotE_row[i] >= 0 )
				{
					bullet_2(shotE_row[i], shotE_col[i]);
					HAL_Delay(50);
					shotE_row[i]--;
				}
				if ( shotE_row[i+1] >= 0 )
				{
					bullet_2(shotE_row[i+1], shotE_col[i+1]);
					HAL_Delay(50);
					shotE_row[i+1]--;
				}
				if ( shotE_row[i] == 19 ) shotE_row[i] = -1;
				if ( shotE_row[i+1] == 19 ) shotE_row[i+1] = -1;
				if ( i == 4 ) break;
			}
		}

		if ( i == 80 )
		{
			row--;
			shot_enemyBoss = 0;
			boss(row, colE);
			shotE_row[0] = row-3;
			shotE_col[0] = 1;
			shotE_row[1] = row+17;
			shotE_col[1] = 0;
			shot_enemyBoss = shot_enemyBoss + 2;
		} else if ( i == 60 ) {
			colE++;
			boss(row, colE);
			shotE_row[2] = row+17;
			shotE_col[2] = 0;
			shotE_row[3] = row+17;
			shotE_col[3] = 1;
			shot_enemyBoss = shot_enemyBoss + 2;
		} else if ( i == 40 ) {
			colE--;
			boss(row, colE);
		} else if ( i == 20 ) {
			colE--;
			boss(row, colE);
			shotE_row[4] = row-3;
			shotE_col[4] = 0;
			shotE_row[5] = row-3;
			shotE_col[5] = 1;
			shot_enemyBoss = shot_enemyBoss + 2;
		} else if ( i == 0 ) {
			colE++;
			boss(row, colE);
			i = 100;
		}
		i--;

		if ( colE == 2 )
		{
			bump_bossfight(row-1,1,row+19,0);
		} else if ( colE == 3 ) {
			bump_bossfight(row+19,1,row+19,0);
		} else if ( colE == 1 ) {
			bump_bossfight(row-1,1,row-1,0);
		}



	}
}

void autoShip( int col ) {

	if ( autoFire == 1 || autoFire == 2 || autoFire == 3 || autoFire == 4 ) {
		autoFire++;
	} else if ( autoFire == 5 ) {
		autoFire = 0;
	}

	//fire
	if ( col == 1 && autoFire == 0 )
	{
		shot_col[shot] = 0;
		shot_row[shot] = 1;
		shot++;
		shot_1();
	} else if ( col == 2 && autoFire == 0 ) {
		shot_col[shot] = 1;
		shot_row[shot] = 1;
		shot++;
		shot_1();
	} else if ( col == 3 && autoFire == 0 ) {
		shot_col[shot] = 0;
		shot_row[shot] = 21;
		shot++;
		shot_1();
	} else if ( col == 4 && autoFire == 0 ) {
		shot_col[shot] = 1;
		shot_row[shot] = 21;
		shot++;
		shot_1();
		autoFire = 1;
	}

}

void setTimeAndDate() {
	mytime.Hours=16;
	mytime.Minutes=35;
	mytime.Seconds=50;

	HAL_RTC_SetTime(&hrtc, &mytime, RTC_FORMAT_BIN);

	mydate.Year=23;
	mydate.Month=1;
	mydate.Date=9;

	HAL_RTC_SetDate(&hrtc, &mydate, RTC_FORMAT_BIN);
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_USB_PCD_Init();
  MX_ADC1_Init();
  MX_RTC_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USART2_UART_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */

  HAL_ADC_Start_IT(&hadc1);

  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, 1);

  setTimeAndDate();

  HAL_UART_Receive_IT(&huart2,data,sizeof(data));

  start_page();

  menu();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI
                              |RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB|RCC_PERIPHCLK_USART2
                              |RCC_PERIPHCLK_I2C1|RCC_PERIPHCLK_RTC
                              |RCC_PERIPHCLK_ADC12;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.Adc12ClockSelection = RCC_ADC12PLLCLK_DIV1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  PeriphClkInit.USBClockSelection = RCC_USBCLKSOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_MultiModeTypeDef multimode = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the ADC multi-mode
  */
  multimode.Mode = ADC_MODE_INDEPENDENT;
  if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.SamplingTime = ADC_SAMPLETIME_601CYCLES_5;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x2000090E;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 39;
  hrtc.Init.SynchPrediv = 999;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0x0;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 0x1;
  sDate.Year = 0x0;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_4BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 2399;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 99999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 14399;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 9999;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 0;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 65535;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */
  HAL_TIM_MspPostInit(&htim4);

}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 4799;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 9999;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USB Initialization Function
  * @param None
  * @retval None
  */
static void MX_USB_PCD_Init(void)
{

  /* USER CODE BEGIN USB_Init 0 */

  /* USER CODE END USB_Init 0 */

  /* USER CODE BEGIN USB_Init 1 */

  /* USER CODE END USB_Init 1 */
  hpcd_USB_FS.Instance = USB;
  hpcd_USB_FS.Init.dev_endpoints = 8;
  hpcd_USB_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
  hpcd_USB_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_FS.Init.battery_charging_enable = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_FS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_Init 2 */

  /* USER CODE END USB_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, CS_I2C_SPI_Pin|LD4_Pin|LD3_Pin|LD5_Pin
                          |LD7_Pin|LD9_Pin|LD10_Pin|LD8_Pin
                          |LD6_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pins : CS_I2C_SPI_Pin LD4_Pin LD3_Pin LD5_Pin
                           LD7_Pin LD9_Pin LD10_Pin LD8_Pin
                           LD6_Pin */
  GPIO_InitStruct.Pin = CS_I2C_SPI_Pin|LD4_Pin|LD3_Pin|LD5_Pin
                          |LD7_Pin|LD9_Pin|LD10_Pin|LD8_Pin
                          |LD6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : MEMS_INT3_Pin MEMS_INT4_Pin */
  GPIO_InitStruct.Pin = MEMS_INT3_Pin|MEMS_INT4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PC12 */
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PD0 PD1 PD2 PD3 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PD4 PD5 PD6 PD7 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI2_TSC_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_TSC_IRQn);

  HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/