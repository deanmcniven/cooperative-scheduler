/*
 * CooperativeScheduler.c
 *
 * Created: 5/2/2016 12:02:40 AM
 * Author : Dean McNiven
 */
#include <avr/io.h>
#include "stdio.h"

#include "coop_sched.h"

void setupHardware(void);
void taskOne(uint8_t);

#define NUM_TASKS 1
volatile task_t Tasks[NUM_TASKS] = {
    {
        taskOne
    }
};

int main()
{
    setupHardware();
    int currentTask = 0;

    while (1)
    {
        (*Tasks[currentTask].entry)(currentTask);

        currentTask++;
        if (currentTask >= NUM_TASKS) currentTask = 0;
    }
}

void setupHardware()
{
    //Led on PB5 output
    DDRB = 1 << DDB5;
}


}

void taskOne(uint8_t pid) {
    if (PORTB && 0x10) PORTB = (0 << PORTB5);
    else PORTB = (1 << PORTB5);
    yield(pid, 66);
}
