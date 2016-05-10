/*
 * CooperativeScheduler.c
 *
 * Created: 5/2/2016 12:02:40 AM
 * Author : Dean McNiven
 */
#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include "stdio.h"

int main(void)
{
    //Setup Ticks generation
    ASSR = 0x00;    //Timer2: Internal Clock
    TCCR2A = 0x02;  //CTC Mode
    TCCR2B = 0x07;  //Prescale: 1024
    OCR2A = 0xEB;   //Count: 235 (~15ms)
    TIMSK2 = 0x02;  //Enable Compare Interrupt


    /* Replace with your application code */
    while (1)
    {

    }
}

ISR(TIMER2_COMPA_vect)
{
    //Tick has occured
    //schedule something?
}
