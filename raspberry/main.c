#include <stdint.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "wiringPi.h"
#include "wiringPiI2C.h"
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include "data.h"
#include "sensor.c"

#define SLAVE_Address 0x3A
#define TEMHUM 29 // GPIO 21
#define MAXTIMINGS 83
#define DHTPIN 7 // GPIO 4

int dht11_dat[5] = {
    0,
};

int s;
float hum = 0, tem = 0;
struct s_data sd;
int cur[4] = {
    0,
}; //current state


void read_dht11_dat();

/*
ctrl + c를 눌렀을 때 시그널 콜백 함수
*/
void INThandler(int sig) 
{
  printf("\nInterrupt Bluetooth\n");
  close(s);
  exit(0);
}

/*
소켓 통신을 이용한 블루투스 통신.
리모콘으로 누른 버튼에 대한 데이터를 받고 값에 따라 대응하는 센서로 변환.
변환된 데이터를 블루투스 통신을 이용해 리모콘으로 보냄.
*/
int main(void)
{
  int rtc, i;
  int buf[2] = {
      0,
  };
  //bluetooth
  struct sockaddr_rc addr = {0};
  int status;
  char *dest = "98:D3:71:FD:BA:62";
  signal(SIGINT, INThandler);
  // allocate a socket
  s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

  // set the connection parameters (who to connect to)
  addr.rc_family = AF_BLUETOOTH;
  addr.rc_channel = (uint8_t)1;
  str2ba(dest, &addr.rc_bdaddr);

  // connect to server
  status = connect(s, (struct sockaddr *)&addr, sizeof(addr));

  if (status < 0)
  {
    perror("connect");
    return 0;
  }

  puts("bconnection");

  wiringPiSetup(); // Wiring Pi setup

  puts("bdelay");

  delay(1000); // Waiting for sensor initialization

  puts("delay");

  if (wiringPiSetup() == -1)
  {
    return 1;
  }
  if ((rtc = wiringPiI2CSetup(SLAVE_Address)) == -1) // Slave Adr = 0x3A
  {
    fprintf(stderr, "Unable to initialise I2C : %s\n", strerror(errno));
    return 1;
  }

 
  signal(SIGINT, INThandler); // 시그널 핸들러 등록

  pinMode(DMOTOR, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(WMOTOR1, OUTPUT);
  pinMode(WMOTOR2, OUTPUT);
 
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);

  while (1)
  {
    printf("0state : %d, cur :  %d\n", cur[0], sd.state[0]);
    printf("1state : %d, cur :  %d\n", cur[1], sd.state[1]);
    printf("2state : %d, cur :  %d\n", cur[2], sd.state[2]);
    printf("3state : %d, cur :  %d\n", cur[3], sd.state[3]);
    if (sd.state[0] != cur[0]) // 1번 버튼 누를 시 Room_led state 변환 및 적용
      RLED(cur[0] = sd.state[0]);
    if (sd.state[1] != cur[1]) // 2번 버튼 누를 시 hall_led 변환 state 변환 및 적용
      LiLED(cur[1] = sd.state[1]);
    if (sd.state[2] != cur[2]) // 3번 버튼 누를 시 window 변환 state 변환 및 적용
      WMotor(cur[2] = sd.state[2]);
    if (sd.state[3] != cur[3]) // 4번 버튼 누를 시 door 변환 state 변환 및 적용
      DMotor(cur[3] = sd.state[3]);

    read_dht11_dat(); //온습도 센싱
    delay(100);

    char data[100] = {
        0,
    };

    sprintf(data, "%f %f ", sd.hum, sd.temp);

    memset(data, 0, 50);
    char msg;
    write(s, &sd, sizeof(sd)); // 블루투스 데이터 전송
    read(s, &sd, sizeof(sd)); // 블루투스 데이터 수신
  }
  close(s);
  return 0;
}

/*
온습도를 측정 함수.
*/
void read_dht11_dat()
{
  //unsigned 8 bit type
  uint8_t laststate = HIGH;
  uint8_t counter = 0;
  uint8_t j = 0, i;
  uint8_t flag = HIGH;
  uint8_t state = 0;

  dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;

  pinMode(DHTPIN, OUTPUT);
  digitalWrite(DHTPIN, LOW);
  delay(18);

  digitalWrite(DHTPIN, HIGH);
  delayMicroseconds(30);
  pinMode(DHTPIN, INPUT);

  for (i = 0; i < MAXTIMINGS; i++)
  {
    counter = 0;
    while (digitalRead(DHTPIN) == laststate)
    {
      counter++;
      delayMicroseconds(1);

      if (counter == 200)
        break;
    }

    laststate = digitalRead(DHTPIN);

    if (counter == 200)
      break; // if while breaked by timer, break for

    if ((i >= 4) && (i % 2 == 0))
    {
      dht11_dat[j / 8] <<= 1;
      if (counter > 20)
        dht11_dat[j / 8] |= 1;
      j++;
    }
  }

  if ((j >= 40) && (dht11_dat[4] == ((dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xff)))
  {
    printf("humidity = %d.%d %% Temperature = %d.%d *C \n", dht11_dat[0], dht11_dat[1], dht11_dat[2], dht11_dat[3]);
    hum = dht11_dat[0];
    hum += (float)dht11_dat[1] * 0.1;
    tem = dht11_dat[2];
    tem += (float)dht11_dat[3] * 0.1;
  }
  else
    printf("humidity = %.1f %% Temperature = %.1f *C\n", hum, tem);

  sd.hum = hum;
  sd.temp = tem;

}
