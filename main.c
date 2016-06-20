/*
 * CooperativeScheduler.c
 *
 * Created: 5/2/2016 12:02:40 AM
 * Author : Dean McNiven
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "stdio.h"

#ifdef GREEDY_TASK
    #define F_CPU 16000000UL
    #include <util/delay.h>
#endif

#include "coop_sched.h"
#include "display.h"

void setup_timer(void);
void yield(uint8_t pid, uint32_t num_ticks);
void task_one(uint8_t);
void task_two(uint8_t);
void task_three(uint8_t);

#define NUM_TASKS 3
volatile task_t tasks[NUM_TASKS] = {
    {
        task_one,
        RUNNABLE,
        0, 0
    }, {
        task_two,
        RUNNABLE,
        0, 0
    }, {
        task_three,
        RUNNABLE,
        0, 0
    }
};

volatile uint32_t ticks = 0;
uint8_t count_h = 0;
uint8_t count_l = 0;

int main()
{
    cli();
    setup_timer();
    setup_display();
    sei();

    int current_task = 0;

    while (1)
    {
        switch (tasks[current_task].state) {
        case RUNNABLE:
            (*tasks[current_task].entry)(current_task);
            break;
        case WAIT:
            if ((tasks[current_task].tick_roll == 0) && (ticks >= tasks[current_task].wait_ticks)) {
                tasks[current_task].state = RUNNABLE;
                (*tasks[current_task].entry)(current_task);
            } else if ((tasks[current_task].tick_roll > 0) && (ticks < tasks[current_task].wait_ticks)) {
                tasks[current_task].tick_roll = 0;
            }
            break;
        case HALT:
            break;
        }

        current_task++;
        if (current_task >= NUM_TASKS) current_task = 0;
    }
}

void setup_timer()
{
    //Setup Ticks generation
    ASSR = 0x00;    //Timer2: Internal Clock
    TCCR2A = 0x02;  //CTC Mode
    TCCR2B = 0x07;  //Prescale: 1024
    OCR2A = 0x9C;   //Count: 156 (~10ms)
    TIMSK2 = 0x02;  //Enable Compare Interrupt
}

ISR(TIMER2_COMPA_vect) {
    uint8_t sReg = SREG;
    ticks++;
    SREG = sReg;
}

void yield(uint8_t pid, uint32_t num_ticks) {
    cli();
    tasks[pid].state = WAIT;
    tasks[pid].wait_ticks = ticks + num_ticks;
    tasks[pid].tick_roll = (tasks[pid].wait_ticks < ticks) ? 1 : 0;
    sei();
}

void task_one(uint8_t pid) {
    LED = (LED ^ LED1_MASK);
    yield(pid, 25);
}

void task_two(uint8_t pid) {
    LED = (LED ^ LED0_MASK);
#ifdef GREEDY_TASK
    pid++; //does nothing - just stops compiler errors dure to -Wall
    _delay_ms(1000);
#else
    yield(pid, 100);
#endif
}

void task_three(uint8_t pid) {
    BCD = (count_h << BCD_OFFSET) | (1 << BCD_L_MSD);
    BCD = (count_l << BCD_OFFSET) | (1 << BCD_L_LSD);
    BCD = BCD_BLANK;

    count_l++;
    if (count_l >= 10) {
        count_l = 0;
        count_h++;
    }
    if (count_h >= 10) count_h = 0;

    yield(pid, 50);
}
