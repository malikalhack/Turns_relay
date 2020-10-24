/************************************
* Title		: Turn_relay.c			*
* Release	: 0.1.A					*
* Creator	: Malik					*
* MCU		: ATtiny24				*
* Created	: 20.08.2016 13:32:51	*
* Changed	: 24.10.2020			*
* Frequency	: 8ћ√ц (int)			*
************************************/

#include "standard.h"
/****************************** Private prototypes ********************************/
ISR (TIM0_OVF_vect);
ISR (ANA_COMP_vect);
void init (void);
/****************************** Private  variables ********************************/
static BYTE anti_dr = 0;
volatile BYTE count[2] = { 61, 0 };
volatile BYTE control=0;
/*	Ѕит 7 - reserve
	Ѕит 6 - Out (0 - low level, 1 - high level)
	Ѕит 5 - "Thanks"
	Ѕит 4 - "Left turn"
	Ѕит 3 - "Right turn"
	Ѕит 2 - "Alarm"
	Ѕит 1 - reserve
	Ѕит 0 - Malfunction	*/
volatile BYTE flags=0;
/*	Ѕит 7 - reserve
	Ѕит 6 - reserve
	Ѕит 5 - reserve
	Ѕит 4 - reserve
	Ѕит 3 - reserve
	Ѕит 2 - Right turn flag
	Ѕит 1 - Left turn flag
	Ѕит 0 - Button click flag	*/
/**********************************************************************************/
int main(void) {
    init();
	while (1);
}
/**********************************************************************************/
void init (void) {
	DDRA	= 0xc0;
	PORTA	= 0;
	DDRB	= 0x07;
	PORTB	= 0;
	TCCR0B	= 0x04; //8,192mS
	TIMSK0	= BIT(TOIE0);
	//DIDR0	= BIT(ADC1D)|BIT(ADC0D);
	ACSR	= BIT(ACD)|BIT(ACIE)|BIT(ACIS1)|BIT(ACIS0);
	sei();
}
/**********************************************************************************/
ISR (TIM0_OVF_vect) {
	/* 8,19288mS */
	register BYTE t_control = control;
	if(!(t_control & BIT(5))) {
		register BYTE temp = PINA&BIT(5); //Button click check
		if (!(temp)) {
			if (anti_dr < 6)anti_dr++;
			if(anti_dr == 5) {
				PORTB |= 0x3;
				count[1] = 0;
				t_control |= BIT(5);
				flags |= BIT(0);
			}
		}
		else anti_dr = 0;
	}
	if (!(--count[0])) {
		count[0] = 61;
		PORTB ^= BIT(2);
		t_control ^= BIT(6);
		if ((t_control & BIT(6)) && (t_control & 0x3c) && (count[1] < 200)) count[1]++;
	}
	if (t_control&BIT(6)) {
		register BYTE feedback = PINA & 0x9;
		switch (feedback) {
			/* both turns off */
			case 0x0:
				t_control &= 0xe6;
				break;
			/* right turn on */
			case 0x1:
				ACSR &= ~BIT(ACD);
				if (!(t_control & BIT(3))) {
					t_control |= BIT(3);
					flags |= BIT(2);
					count[1] = 0;
					PORTB |= BIT(1);
				}
				break;
			/* left turn on */
			case 0x8:
				ACSR &= ~BIT(ACD);
				if (!(t_control & BIT(4))) {
					t_control |= BIT(4);
					flags |= BIT(1);
					count[1] = 0;
					PORTB |= BIT(0);
				}
				break;
			/* Both turns on */
			case 0x9:
				PORTA |= BIT(6);
				if(!(t_control & BIT(5))) PORTB &= 0xfc;
				t_control &= 0xe7;
				break;
			/* default: asm ("nop"); */
		}
		if ((feedback == 0x8) || (feedback == 0x1)) {
			if (t_control & BIT(0)) {
				switch (count[0]) {
					case 0 ... 19:
						PORTA |= 0xc0;
						break;
					case 20 ... 40:
						PORTA &= 0x3f;
						break;
					case 41 ... 61:
						PORTA |= 0xc0;
						break;
				}
			}
			else {
				PORTA|=BIT(6);
				if (count[1] > 9) PORTA |= BIT(7);
			}
		}
	}
	else {
		PORTA &= 0x3f;
		if (count[1] >= 3) {
			PORTB &= 0xfc;
			if (t_control&BIT(5)) t_control &= ~BIT(5);
		}
	}
	ACSR |= BIT(ACD);
	control = t_control;
}
/**********************************************************************************/
ISR (ANA_COMP_vect) { control |= BIT(0); }
/**********************************************************************************/
