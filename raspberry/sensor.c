#include <stdio.h>
#include <wiringPi.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <softPwm.h>

#define DMOTOR 17 // GPIO 17
#define LED1 18   // GPIO 18
// #define LED2 27   // GPIO 18
#define WMOTOR1 5 // GPIO 5
#define WMOTOR2 6 // GPIO 6

void DMotor();
void WMotor();
void INThandler(int);
void init();
void LED(int);

// Main Function
int main()
{
    puts("w졸리다");

    int bright;

    if (wiringPiSetupGpio() == -1)
    {
        return 1;
    }

    // signal(SIGINT, INThandler);

    pinMode(DMOTOR, OUTPUT);
    pinMode(LED1, PWM_OUTPUT);

    while (1)
    {
        LED(bright);
        DMotor();
        // WMotor1();
        WMotor();
    }

    return 0;
}

void DMotor()
{

    softPwmCreate(DMOTOR, 0, 300);
    softPwmWrite(DMOTOR, 10); // - 방향
    delay(800);               //600ms 동안 softPwmWrite()상태가 지속됨
    softPwmWrite(DMOTOR, 27); //+ 방향
    delay(600);

    puts("난 모터다");
}

void WMotor()
{
    softPwmCreate(WMOTOR2, 0, 100);
    softPwmCreate(WMOTOR1, 0, 100);

    softPwmWrite(WMOTOR1, 10); 
    softPwmWrite(WMOTOR2, 17); 
    delay(800);
    softPwmWrite(WMOTOR1, 21); 
    softPwmWrite(WMOTOR2, 5); 
    delay(800);

    puts("난 윈도우들이다");

}

void INThandler(int sig)
{
    printf("\ngoodbye\n");
    init();
    exit(0);
}

void init()
{
    digitalWrite(DMOTOR, LOW);
}

void LED(int bright)
{

    for (bright = 0; bright < 1024; ++bright)
    {
        pwmWrite(LED1, bright);
        delay(1);
    }

    for (bright = 1023; bright >= 0; --bright)
    {
        pwmWrite(LED1, bright);
        delay(1);
    }

    puts("나는 lED다");
}