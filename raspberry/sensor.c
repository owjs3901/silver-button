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

 /*
 문을 열고 닫는 함수.
 */
void DMotor(int t)
{
    softPwmCreate(DMOTOR, 0, 300);
    softPwmWrite(DMOTOR, t ? 1 : 27); // - 방향  
}

/*
창문을 열고 닫는 함수.
*/
void WMotor(int t) { 
    softPwmCreate(WMOTOR2, 0, 100);
    softPwmCreate(WMOTOR1, 0, 100);

    softPwmWrite(WMOTOR1, t ? 21 :10);
    softPwmWrite(WMOTOR2, t ? 5 : 17);   
}

/*
거실 조명 on, off 조절 함수.
*/
void LiLED(int t) 
{
  digitalWrite(LED1, t ? HIGH : LOW);
}

/*
방 조명을 on, off 조절 함수.
*/
void RLED(int t) 
{
  digitalWrite(LED2, t ? HIGH : LOW);
}
