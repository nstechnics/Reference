/*
 * Microwave.c
 *
 * Created: 2021-07-19 오후 3:59:56
 * Author : NAMST
 */ 

/*
 * MEGA128_default.c
 *
 * Created: 2021-07-19 오후 12:17:26
 * Author : NAMST
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "atmel_start.h"
#include "main.h"
#include "uart.h"

#pragma used+
// Get a character
char getchar_0()
{
	char        status;

	while (((status = UCSR0A) & 0x80) == 0);
	return  UDR0;            // USART 0 Data Receive
}
// Putt a character
void putchar_0(int ich)
{
	while(!(UCSR0A & 0x20));
	UDR0 = (unsigned char) ich;
}
void Recieve_Buffer_Clr(void)
{
	unsigned short i;
	for (i = 0; i < UART0_RCV_BUFF_SIZE; i++)	UART0_R_Data[i] = 0x00;
	UART0_heder_flag = 0;
	UART0_R_Count = 0;
}
#pragma used-

ISR(USART0_RX_vect)
{
	// Data Recieve
	UART0_R_Data[UART0_R_Count] = getchar_0();
	//for(i = 0; i<=10; i++)
	//putchar_0(UART0_R_Data[i]);
	UART0_R_Count++;
	
	if((UART0_R_Data[UART0_R_Count-2] == 0xFF) && (UART0_R_Data[UART0_R_Count-1] == 0xFF))
	{
		UART0_heder_flag = 1;
		UART0_R_Count = 0;
	}
	if((UART0_heder_flag == 1) && (UART0_R_Count >= 1))
	{
		if(UART0_R_Data[0] == 0x05)
		{
			//Microwave_sens ON
			Microwave_sens_ena_flag = Microwave_sens_ena_flag | 0x01;		//Microwave_sens_ena_flag Set
		}
		Recieve_Buffer_Clr();
	}
}
ISR(TIMER0_OVF_vect)
{
	unsigned char high,low;
	float f_temp;
	unsigned short Distance0_cm;
	// Reinitialize Timer 0 value
	TCNT0=0xCE;
	Microwave0_cnt++;
	if(!ECHO_get_level())
	{
//		Microwave0_Distance = Microwave0_cnt;
//		Microwave0_cnt = 0;
		//Timer_0 Disable
		TIMSK=(0<<OCIE2) | (0<<TOIE2) | (0<<TICIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (0<<TOIE1) | (0<<OCIE0) | (0<TOIE0);
		if(Microwave0_cnt <= 1520)
		{
//			//cn of count * clk_width(us) / speed of sound(340m/s) / return_time
//			f_temp = (float)Microwave0_Distance * 25.0 / 58.0 / 2.0;
//			Distance0_cm = (unsigned short)(f_temp * 1000.0);
//			low = Distance0_cm & 0xff;
//			high = Distance0_cm >> 8;
			low = Microwave0_cnt & 0xff;
			high = Microwave0_cnt >> 8;

			putchar_0(0xff);
			putchar_0(0xff);
			//putchar_0((unsigned char)Microwave0_Distance);
			//putchar_0((unsigned char)Microwave0_Distance >> 8);
			putchar_0(high);
			putchar_0(low);
		}
		Microwave0_cnt = 0;
	}
}
int main(void)
{
	cli();
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	sei();
	
	while (1)
	{
		if(Microwave_sens_ena_flag & 0x01)
		{
			TRIG_set_level(true);
			_delay_us(20);
			TRIG_set_level(false);
			Microwave_sens_ena_flag = Microwave_sens_ena_flag & 0xfe;	//Microwave_sens_ena_flag Clear
			Microwave_Trig_flag = Microwave_Trig_flag | 0x01;			//Microwave_Trig_flag Set
		}
		if((Microwave_Trig_flag & 0x01) && ECHO_get_level())
		{
			//Timer_0 Enable
			TIMSK=(0<<OCIE2) | (0<<TOIE2) | (0<<TICIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (0<<TOIE1) | (0<<OCIE0) | (1<<TOIE0);
			Microwave_Trig_flag = Microwave_Trig_flag & 0xfe;			//Microwave_Trig_flag Clear
		}
	}
}
