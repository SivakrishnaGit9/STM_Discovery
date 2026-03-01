/*
 * Controlling.h
 *
 *  Created on: 22-Feb-2026
 *      Author: Durga
 */

#ifndef INC_CONTROLLING_H_
#define INC_CONTROLLING_H_

#include "queue.h"
#include "timers.h"

extern TaskHandle_t main_menu_handle, receive_command_handle, print_data_handle, led_menu_handle, rtc_menu_handle;
extern QueueHandle_t send_queue, receive_queue;
extern TimerHandle_t led_timer_handle[4];

extern volatile uint8_t User_Input;


extern void vMain_Menu(void *param);
extern void vReceive_Command(void *param);
extern void vPrint_Data(void *param);
extern void vLED_Menu(void *param);
extern void vRTC_Menu(void *param);
extern void LED_Timer_Call_Back(TimerHandle_t xTimer);

extern void LED_e1(void);
extern void LED_e2(void);
extern void LED_e3(void);
extern void LED_e4(void);
extern void LED_Effect_none(void);

typedef struct
{
	uint8_t Payload[10];
	uint32_t Length;
}Receive_Command_t;

typedef enum
{
	sMain_Menu = 0,
	sLED_Menu,
	sRTC_Menu,
	sRTC_DateConfig,
	sRTC_TimeConfig,
	sRTC_Report,
}Current_State_t;

#endif /* INC_CONTROLLING_H_ */
