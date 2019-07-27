/********************************
* Название: Turn_relay.c		*
* Версия: alpha					*
* Автор	: Malik					*
* Дата 	: 20.08.2016 13:32:51	*
* Для AVR: ATtiny24				*
* Тактовая частота: 8МГц (int)	*
********************************/
#include "Standart_H.h"
ISR (TIM0_OVF_vect)
{ //8,19288mS
 	register unsigned char control_temp=control;
	if(!(control_temp&BIT(5)))
	{
		register unsigned char temp=PINA&BIT(5); //Проверка нажатия кнопки
		static unsigned char anti_dr;
		if (!(temp))
		{
			if (anti_dr<6)anti_dr++;
				if(anti_dr==5)
				{
					PORTB|=0x3;
					count[1]=0;
					control_temp|=BIT(5);
					flags|=BIT(0);
				}
		}
		else anti_dr=0;
	}
/**************************************/
	if (!(--count[0]))
	{
		count[0]=61;
		PORTB^=BIT(2);
		control_temp^=BIT(6);
		if (control_temp&BIT(6)) if (control_temp&0x3c) if (count[1]<200) count[1]++;
	}
	if (control_temp&BIT(6))
		{
			register unsigned char feedback=PINA&0x9;
			switch (feedback)
				{
					case 0x0: //оба поворота выключены
						control_temp&=0xe6;
						break;
					case 0x1: //правый поворот включён
						ACSR&=~BIT(ACD);
						if (!(control_temp&BIT(3)))
						{
							control_temp|=BIT(3);
							flags|=BIT(2);
							count[1]=0;
							PORTB|=BIT(1);
						}
						break;
					case 0x8: //левый поворот включён
						ACSR&=~BIT(ACD);
						if (!(control_temp&BIT(4)))
						{
							control_temp|=BIT(4);
							flags|=BIT(1);
							count[1]=0;
							PORTB|=BIT(0);
						}
						break;
					case 0x9: //оба поворота включены
						PORTA|=BIT(6);
						if(!(control_temp&BIT(5))) PORTB&=0xfc;
						control_temp&=0xe7;
						break;
					//default: asm ("nop");
				}
				if ((feedback==0x8)||(feedback==0x1))
				{
					if (control_temp&BIT(0))
					{
						switch (count[0])
						{
						case 0 ... 19:
							PORTA|=0xc0; 
							break;
 						case 20 ... 40:
							PORTA&=0x3f; 
 							break;
 						case 41 ... 61:
							PORTA|=0xc0; 
 							break;
						}
					}
					else
					{
						PORTA|=BIT(6);
						if (count[1]>9) PORTA|=BIT(7);
					}
				}
		}
		else
		{
			PORTA&=0x3f;
			if (count[1]>=3)
			{
				PORTB&=0xfc;
				if (control_temp&BIT(5)) control_temp&=~BIT(5);
			}
		}
	ACSR|=BIT(ACD);
	control=control_temp;
}
ISR (ANA_COMP_vect)
{
	control|=BIT(0);
}

int main(void)
{
    init();
	while (1) 
    {
		
    }
}

void init (void)
{
	DDRA=0xc0;
	PORTA=0;
	DDRB=0x07;
	PORTB=0;
	TCCR0B=0x04; //8,192mS
	TIMSK0=BIT(TOIE0);
	//DIDR0=BIT(ADC1D)|BIT(ADC0D);
	ACSR=BIT(ACD)|BIT(ACIE)|BIT(ACIS1)|BIT(ACIS0);
	sei();
}