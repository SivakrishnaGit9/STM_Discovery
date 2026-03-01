/*
 * Controlling.c
 *
 *  Created on: 22-Feb-2026
 *      Author: Durga
 */
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "Controlling.h"
#include <string.h>

Current_State_t Current_State = sMain_Menu;
Receive_Command_t Receive_Command;

extern UART_HandleTypeDef huart2;

void Process_Command(Receive_Command_t *Receice_Command);
void Command_Extraction(Receive_Command_t *Receice_Command);

void LED_Effect(int Effect);
void LED_Effect_none (void);

char *Invalid_Msg = "|********INVALID DATA ENTERED*******|";

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	uint8_t Dummy;

	if(xQueueIsQueueFullFromISR(receive_queue) == pdFALSE)
	{
		xQueueSendFromISR(receive_queue, (void *)&User_Input, NULL);
	}
	else
	{
		if(User_Input == '\n')
		{
			xQueueReceiveFromISR(receive_queue, (void *)&Dummy, NULL);
			xQueueSendFromISR(receive_queue, (void *)&User_Input, NULL);
		}
	}

	if(User_Input == '\n')
	{
		xTaskNotifyFromISR(receive_command_handle, 0, eNoAction, NULL);
	}

	HAL_UART_Receive_IT(&huart2, (uint8_t *)&User_Input, 1);
}

void vMain_Menu(void *param)
{
	uint32_t Receive_Command_t_add;
	Receive_Command_t *Receive_Command;
	char *Main_Menu_Msg = "|--------------------------|\n\r"
								"|			Main Menu		|\n\r"
								"|--------------------------|\n\r"
								"|LED Effect 	-------> 0	|\n\r"
								"|Date and Time -------> 1	|\n\r"
								"|Exit			-------> 2	|\n\r"
								"							\n\r"
								"Enter your choice 	:";
	while(1)
	{
		xQueueSend(send_queue,Main_Menu_Msg,portMAX_DELAY);

		xTaskNotifyWait(0,0,&Receive_Command_t_add,portMAX_DELAY);

		Receive_Command = (Receive_Command_t*)Receive_Command_t_add;

		if(Receive_Command->Length == 1)
		{
			switch((Receive_Command->Payload[0]-48))
			{
				case 0:
					Current_State = sLED_Menu;
					xTaskNotify(led_menu_handle,0,eNoAction);
					break;

				case 1:
					Current_State = sRTC_Menu;
					xTaskNotify(rtc_menu_handle,0,eNoAction);
					break;

				case 2:
					break;

				default:
					xQueueSend(send_queue,&Invalid_Msg,portMAX_DELAY);
					continue;
			}
		}
		else
		{
			xQueueSend(send_queue,&Invalid_Msg,portMAX_DELAY);
			continue;
		}

		xTaskNotifyWait(0,0,NULL,portMAX_DELAY);
	}
}

void vReceive_Command(void *param)
{
	BaseType_t Status;

	while(1)
	{
		Status = xTaskNotifyWait(pdFALSE,pdFALSE,NULL,portMAX_DELAY);

		if(Status == pdTRUE)
		{
			Process_Command(&Receive_Command);
		}
	}
}

void Process_Command(Receive_Command_t *Receive_Command)
{
	Command_Extraction((Receive_Command_t *)Receive_Command);

	switch(Current_State)
	{
		case sMain_Menu:
			xTaskNotify(main_menu_handle, (uint32_t)Receive_Command, eSetValueWithOverwrite);
		break;

		case sLED_Menu:
			xTaskNotify(led_menu_handle, (uint32_t)Receive_Command, eSetValueWithOverwrite);
		break;

		case sRTC_Menu:
		case sRTC_DateConfig:
		case sRTC_TimeConfig:
		case sRTC_Report:
			xTaskNotify(rtc_menu_handle, (uint32_t)Receive_Command, eSetValueWithOverwrite);
		break;
	}
}

void Command_Extraction(Receive_Command_t *Receive_Command)
{
	uint8_t Recv_Byte, i=0;
	BaseType_t Status;

	do
	{
		Status = xQueueReceive(receive_queue, &Recv_Byte, 0);
		if(Status == pdTRUE)
		{
			Receive_Command->Payload[i++] = Recv_Byte;
		}
	}while(Recv_Byte != '\n');

	Receive_Command->Payload[i-1] 	= '\0';
	Receive_Command->Length 		= i-1;
}

void vPrint_Data(void *param)
{
	char *msg;
	BaseType_t Status;

	while(1)
	{
		Status = xQueueReceive(send_queue,&msg, portMAX_DELAY);

		if(Status == pdTRUE)
			HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen((const char *)msg), HAL_MAX_DELAY);
	}
}

void vLED_Menu(void *param)
{
	uint32_t Receive_Command_t_add;
	Receive_Command_t *Receive_Command;
	const char* LED_Menu_Msg = "\n\r|--------------------------|\n\r"
								"|			LED Menu		|\n\r"
								"|--------------------------|\n\r"
								"|LED Effect 	----> e1	|\n\r"
								"|LED Effect 	----> e2	|\n\r"
								"|LED Effect 	----> e3	|\n\r"
								"|LED Effect 	----> e4	|\n\r"
								"|LED Effect 	----> none	|\n\r"
								"							\n\r"
								"Enter your choice 	:";
	while(1)
	{
		xTaskNotifyWait(0,0,NULL,portMAX_DELAY);

		xQueueSend(send_queue,&LED_Menu_Msg,portMAX_DELAY);

		xTaskNotifyWait(0,0,&Receive_Command_t_add,portMAX_DELAY);

		Receive_Command = (Receive_Command_t*)Receive_Command_t_add;

		if(Receive_Command->Length <= 4)
		{
			if(!strcmp((char *)Receive_Command->Payload,"e1"))
				LED_Effect(1);

			else if(!strcmp((char *)Receive_Command->Payload,"e2"))
				LED_Effect(2);

			else if(!strcmp((char *)Receive_Command->Payload,"e3"))
				LED_Effect(3);

			else if(!strcmp((char *)Receive_Command->Payload,"e4"))
				LED_Effect(4);

			else if(!strcmp((char *)Receive_Command->Payload,"none"))
				LED_Effect_none();

			else
				xQueueSend(send_queue,&Invalid_Msg,portMAX_DELAY);
		}
		else
		{
			xQueueSend(send_queue,&Invalid_Msg,portMAX_DELAY);
		}

		Current_State = sMain_Menu;

		xTaskNotify(main_menu_handle,0,eNoAction);
	}
}

void LED_Timer_Call_Back(TimerHandle_t xTimer)
{
	uint8_t Timer_ID;

	Timer_ID = (uint32_t)pvTimerGetTimerID( xTimer );

	switch(Timer_ID)
	{
	case 1:
		LED_e1();
		break;

	case 2:
		LED_e2();
		break;

	case 3:
		LED_e3();
		break;

	case 4:
		LED_e4();
		break;
	}
}

void LED_Effect(int Effect)
{
	LED_Effect_none();

	xTimerStart(led_timer_handle[Effect-1], portMAX_DELAY);
}

void LED_e1(void)
{
	uint8_t Set=0;

	if(Set == 1)
	{
		HAL_GPIO_WritePin(GPIOD, Green_LED_Pin,	GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, Orange_LED_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, Red_LED_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, Blue_LED_Pin, GPIO_PIN_SET);

		Set = 0;
	}
	else
	{
		HAL_GPIO_WritePin(GPIOD, Green_LED_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, Orange_LED_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, Red_LED_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, Blue_LED_Pin, GPIO_PIN_RESET);

		Set = 1;
	}
}

void LED_e2(void)
{
	uint8_t Set=1;

	if(Set == 1)
	{
		HAL_GPIO_WritePin(GPIOD, Green_LED_Pin,	GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, Orange_LED_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, Red_LED_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, Blue_LED_Pin, GPIO_PIN_SET);

		Set = 0;
	}
	else
	{
		HAL_GPIO_WritePin(GPIOD, Green_LED_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, Orange_LED_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, Red_LED_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, Blue_LED_Pin, GPIO_PIN_RESET);

		Set = 1;
	}
}

void LED_e3(void)
{
	uint8_t Set_bit_Forward, i=0;

	Set_bit_Forward = (0x01<<i);

	HAL_GPIO_WritePin(GPIOD, Green_LED_Pin,		((Set_bit_Forward >> 0) & 0x01));
	HAL_GPIO_WritePin(GPIOD, Orange_LED_Pin, 	((Set_bit_Forward >> 1) & 0x01));
	HAL_GPIO_WritePin(GPIOD, Red_LED_Pin, 		((Set_bit_Forward >> 2) & 0x01));
	HAL_GPIO_WritePin(GPIOD, Blue_LED_Pin, 		((Set_bit_Forward >> 3) & 0x01));

	if(i >= 3)
		i = 0;
	else
		i++;
}

void LED_e4(void)
{
	uint8_t Set_bit_Reverse, i=0;

	Set_bit_Reverse = (0x08 >> i);

	HAL_GPIO_WritePin(GPIOD, Green_LED_Pin,		((Set_bit_Reverse >> 0) & 0x01));
	HAL_GPIO_WritePin(GPIOD, Orange_LED_Pin, 	((Set_bit_Reverse >> 1) & 0x01));
	HAL_GPIO_WritePin(GPIOD, Red_LED_Pin, 		((Set_bit_Reverse >> 2) & 0x01));
	HAL_GPIO_WritePin(GPIOD, Blue_LED_Pin, 		((Set_bit_Reverse >> 3) & 0x01));

	if(i >= 3)
		i = 0;
	else
		i++;
}

void LED_Effect_none (void)
{
	uint8_t i=0;

	for(i=0; i<4; i++)
		xTimerStop(led_timer_handle[i],portMAX_DELAY);

	HAL_GPIO_WritePin(GPIOD, Green_LED_Pin,	GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, Orange_LED_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, Red_LED_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, Blue_LED_Pin, GPIO_PIN_RESET);
}

void vRTC_Menu(void *param)
{
	while(1)
	{
		xTaskNotifyWait(0,0,NULL,portMAX_DELAY);
	}
}
