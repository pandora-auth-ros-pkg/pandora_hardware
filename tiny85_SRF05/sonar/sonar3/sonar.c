/*
 * sonar3.c
 *
 * Created: 23/12/2012 7:42:14 μμ
 *  Author: Thomas
 */ 

#include "sonar.h"


ISR(TIM1_OVF_vect){
	if ((SonarFlags&SonarBusy)&&(SonarFlags&SonarEchoHi)&&!(SonarFlags&SonarEchoLo))
	{
		SonarRange.RangeHi++;
		if (SonarRange.RangeHi>20)
		{
			SonarFlags&=~(SonarBusy|SonarEchoHi|SonarEchoLo|SonarNewData);
			SonarFlags|=SonarOvrFl;
		}
	}
	else
	{
		if(SonarFlags&SonarBusy)
		{
			SonarFlags&=~(SonarBusy|SonarEchoHi|SonarEchoLo|SonarNewData);
			SonarFlags|=SonarErr;
		}		
	}
}

ISR(USI_START_VECTOR){
	USI_Start_Condition_ISR();
}

ISR(USI_OVERFLOW_VECTOR){
	USI_Counter_Overflow_ISR();
}

ISR(PCINT0_vect){
	if (PINB & EchoPin)			//if PB1=1 then echo_start
	{
		if((SonarFlags&SonarBusy)&&!(SonarFlags&SonarEchoHi)&&!(SonarFlags&SonarEchoLo))
		{
			SonarFlags|=SonarEchoHi;	
			TCNT1=0;
			TIFR |= (1<<TOV1);
			SonarRange.RangeHi=0;
		}
		else
		{
			SonarFlags&=~(SonarBusy|SonarEchoHi|SonarEchoLo|SonarNewData);
		}
	}		
	else							//else echo_end
	{
		if((SonarFlags&SonarBusy)&&(SonarFlags&SonarEchoHi)&&!(SonarFlags&SonarEchoLo))
		{
			SonarFlags|=SonarEchoLo;
			SonarRange.RangeLo = TCNT1;
			SonarRange.Range=(SonarRange.Range*34)>>5;
			Sonar_H = SonarRange.RangeHi;
			Sonar_L = SonarRange.RangeLo;
			SonarFlags|=SonarNewData;
			SonarFlags&=~(SonarBusy|SonarEchoHi|SonarEchoLo|SonarErr|SonarOvrFl);
		}
		else
		{
			SonarFlags&=~(SonarBusy|SonarEchoHi|SonarEchoLo|SonarNewData); // ?|SonarErr? too
		}
	}
}

ISR(ADC_vect){
	IR_data=ADCH;
}

void send_packet (unsigned char S_Status, unsigned char Sonar_H, unsigned char Sonar_L, unsigned char IR_data)
{
	USI_TWI_Transmit_Byte(S_Status);
	USI_TWI_Transmit_Byte(IR_data);
	USI_TWI_Transmit_Byte(Sonar_H);
	USI_TWI_Transmit_Byte(Sonar_L);
	SonarFlags &=~SonarNewData;
}

void start_sonar( void )
{
	SonarRange.Range=0;
	SonarFlags|=SonarBusy;					//sonar_busy=1
	SonarFlags&=~(SonarEchoHi|SonarEchoLo|SonarOvrFl|SonarErr);

	PORTB |= TriggerPin;					//send trigger 
	_delay_us(25);
	PORTB &= ~TriggerPin;					

	TCNT1=0;
	TIFR |= (1<<TOV1);
}


int main( void )
{
	SonarFlags=0;
	Sonar_H=0;
	Sonar_L=0;
	IR_data=0;
	
	// Own TWI slave address
	unsigned char TWI_slaveAddress = 0x41;
	
	USI_TWI_Slave_Initialise( TWI_slaveAddress );
	TCCR1=0x0A;		//TCNT1 prescaler 256 __ start TCNT1
	TIMSK|=(1<<TOIE1);
	DDRB |=TriggerPin;			//PB3 as output for TRIG
	DDRB &= ~EchoPin;			//PB1 as input for ECHO
	//PORTB &= ~(1<<PB5);		//pull-up enable ????????
	MCUCR = MCUCR&0xFC|1;
	GIMSK|=(1<<PCIE);			//init pin change interrupts..
	PCMSK|=EchoPin;				//..at pin1
	ADMUX = 0x22;				//VREF VCC, Left Adjust, INPUT CHANNEL ADC2:PB4
	ADCSRA = 0x9E;				//Enable ADC module, clear IRQ_Flag, Enable IRQ, Prescaler 64
	DIDR0 = 0x10;				//Disable digital input for pin PB4
	
	
	__asm__ ("SEI");
		
	while(1)
	{

		if( USI_TWI_Data_In_Receive_Buffer())// && ---------------- )
		{
			M_Status = USI_TWI_Receive_Byte();
			
			if (M_Status==M_trig)
			{
				while(ADCSRA&(1<<ADSC)){}		//Do NOT start trigger if A/D conversion in progress (NOICE CANCELING TECHNIQUE)
				if (!(SonarFlags&SonarBusy))	//start new measurement if none running now
					{start_sonar();}			//CLI??????
				ADCSRA |= 1<<ADSC;				//Start ADC conversion for IR
				//start_ir();
			}
			if (M_Status==M_read)
			{
				send_packet(SonarFlags /*& 0x0f*/, Sonar_H, Sonar_L, IR_data);			//CLI?????
			}
		}

	}
	
	return 0;
}





