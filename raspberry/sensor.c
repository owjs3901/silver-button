#include <stdio.h>
#include <wiringPi.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <softPwm.h>
#include <stdint.h>
#include "data.h"

#define DMOTOR 0   // GPIO 17
#define LED1 4     // GPIO 23
#define LED2 5     // GPIO 24
#define WMOTOR1 21 // GPIO 5
#define WMOTOR2 22 // GPIO 6


void DMotor(int t)
{
    softPwmCreate(DMOTOR, 0, 300);
    softPwmWrite(DMOTOR, t ? 1 : 27); // - 방향  
}

void WMotor(int t)
{
    softPwmCreate(WMOTOR2, 0, 100);
    softPwmCreate(WMOTOR1, 0, 100);

    softPwmWrite(WMOTOR1, t ? 21 :10);
    softPwmWrite(WMOTOR2, t ? 5 : 17);   
}

void LiLED(int t) //거실조명
{
  digitalWrite(LED1, t ? HIGH : LOW);
}

void RLED(int t) //방조명
{
  digitalWrite(LED2, t ? HIGH : LOW);
}
