/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f10x.h"

volatile uint32_t timer_ms = 0;
volatile uint32_t flag = 0;
volatile uint32_t isKeyDown = 0;
volatile uint32_t led = 0;

void SysTick_Handler() { //obsluga przerwania od timera SysTick
	if (timer_ms) {
		timer_ms--; //co 1ms dekrementuj timer_ms
	}
}

void delay_ms(int time) {
	timer_ms = time;
	while (timer_ms);
}

void EXTI15_10_IRQHandler()
{
    if (EXTI_GetITStatus(EXTI_Line13)) {
        if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) == 0) {
            flag = !flag;
            isKeyDown = 1;
            GPIO_SetBits(GPIOA,GPIO_Pin_5);
        } else {
        	isKeyDown = 0;
            GPIO_ResetBits(GPIOA,GPIO_Pin_5);
        }

        EXTI_ClearITPendingBit(EXTI_Line13);
    }
}

void zad1() {
	GPIO_SetBits(GPIOC, 1<<led);
	delay_ms(100);
	GPIO_ResetBits(GPIOC, 1<<led);
	if (flag == 0) {
		if (++led >= 10) {
			led = 0;
		}
	} else {
		if (--led <= 0) {
			led = 9;
		}
	}
}

void zad2() {
	GPIO_SetBits(GPIOC, 1<<led);
	if (isKeyDown) {
		isKeyDown = 0;
		GPIO_ResetBits(GPIOC, 1<<led);
		if (++led >= 10) {
			led = 0;
		}
	}
}

int main(void)
{
	GPIO_InitTypeDef gpio;
	EXTI_InitTypeDef exti;
	NVIC_InitTypeDef nvic;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA, ENABLE); //podlaczenie zagara
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //modul funkcji alternatywnych

	//wyjscia PC0-PC9
	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP; //wyjscie push-pull
	GPIO_Init(GPIOC, &gpio);

	//wyjscie PA5 (LD2)
	gpio.GPIO_Pin = GPIO_Pin_5;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP; //wyjscie push-pull
	GPIO_Init(GPIOA, &gpio);

	//wejscie PC13 (B1)
	gpio.GPIO_Pin = GPIO_Pin_13;
	gpio.GPIO_Mode = GPIO_Mode_IPU; //wejscie z rezystorem pull-up
	GPIO_Init(GPIOC, &gpio);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource13);

	EXTI_StructInit(&exti);
	exti.EXTI_Line = EXTI_Line13;
	exti.EXTI_Mode = EXTI_Mode_Interrupt;
	exti.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	exti.EXTI_LineCmd = ENABLE;
	EXTI_Init(&exti);

	nvic.NVIC_IRQChannel = EXTI15_10_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0x00;
	nvic.NVIC_IRQChannelSubPriority = 0x00;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);

	SysTick_Config(SystemCoreClock / 1000); //przerwanie od timera SysTick co 1ms

	while(1) {
		zad2();
	}
}
