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
			
//void delay(int time) {
//	int i;
//	for (i = 0; i < time*4000; i++); //4000 dobrane eksperymentalnie, aby op�nienie by�o w ms
//}

volatile uint32_t timer_ms = 0;

void SysTick_Handler() { //obs�uga przerwania od timera SysTick
	if (timer_ms) {
		timer_ms--; //co 1ms dekrementuj timer_ms
	}
}

void delay_ms(int time) {
	timer_ms = time;
	while (timer_ms);
}

int main(void)
{

	GPIO_InitTypeDef gpio; //obiekt gpio b�d�cy konfiguracj� port�w GPIO

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE); //pod��czenie zagara do portu A oraz C

	//ustawienie PA5 jako wyj�cie
	GPIO_StructInit(&gpio); //domy�lna konfiguracja
	gpio.GPIO_Pin = GPIO_Pin_5; // konfiguracja pinu 5
	gpio.GPIO_Mode = GPIO_Mode_Out_PP; // jako wyj�cie push-pull
	GPIO_Init(GPIOA, &gpio); //inicjalizacja modu�u GPIOA

	//ustawienie PC13 jako wej�cie
	gpio.GPIO_Pin = GPIO_Pin_13;
	gpio.GPIO_Mode = GPIO_Mode_IPU; //wej�cie z rezystorem pull-up
	GPIO_Init(GPIOC, &gpio);

	SysTick_Config(SystemCoreClock / 1000); //przerwanie od timera SysTick co 1ms

	while(1) {
		if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) == 0) { //je�li przycisk wci�ni�ty
			GPIO_SetBits(GPIOA, GPIO_Pin_5); //zapalenie diody LD2
			delay_ms(100);
			GPIO_ResetBits(GPIOA, GPIO_Pin_5); //zgaszenie diody LD2
			delay_ms(100);
		} else {
			GPIO_ResetBits(GPIOA, GPIO_Pin_5);
		}

	}
}
