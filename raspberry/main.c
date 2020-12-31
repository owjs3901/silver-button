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

#define SLAVE_Address 0x3A

#define MAXTIMINGS 83

#define DHTPIN 7

int dht11_dat[5] = {
    0,
};

float humidity = 0, temperature = 0;

void read_dht11_dat()
{
  uint8_t laststate = HIGH;
  uint8_t counter = 0;
  uint8_t j = 0, i;
  uint8_t flag = HIGH;
  uint8_t state = 0;

  float f;

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
    humidity = dht11_dat[0];
    humidity += (float)dht11_dat[1] * 0.1;
    temperature = dht11_dat[2];
    temperature += (float)dht11_dat[3] * 0.1;
  }

  else
    printf("humidity = %.1f %% Temperature = %.1f *C\n", humidity, temperature);
}

int server_socket = -1;



int main(void)
{
  int rtc, i;
  int rawTemp[2];

  float SensorTemp, ObjTemp;

  //bluetooth
  struct sockaddr_rc addr = { 0 };
  int s, status;
  char *dest = "98:D3:71:FD:BA:62";

  // allocate a socket
  s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

  // set the connection parameters (who to connect to)
  addr.rc_family = AF_BLUETOOTH;
  addr.rc_channel = (uint8_t) 1;
  str2ba( dest, &addr.rc_bdaddr );

  // connect to server
  status = connect(s, (struct sockaddr *)&addr, sizeof(addr));

  if (status < 0) {
    perror("connect");
    return 0;
  }

  puts("bconnection");

  connection();

  wiringPiSetup(); // Wiring Pi setup

  puts("bdelay");

  delay(1000); // Waiting for sensor initialization

  puts("delay");

  if ((rtc = wiringPiI2CSetup(SLAVE_Address)) == -1) // Slave Adr = 0x3A
  {
    fprintf(stderr, "Unable to initialise I2C : %s\n", strerror(errno));
    return 1;
  }

  while (1)
  {
    for (i = 0; i < 2; i++)
    {
      rawTemp[i] = wiringPiI2CReadReg16(rtc, 0x06 + i);
      delay(1); //Don't delete this line
    }
    SensorTemp = (float)rawTemp[0] * 0.02 - 273.15;
    ObjTemp = (float)rawTemp[1] * 0.02 - 273.15;

    printf("SenT:%3.2f, ObjT:%3.2f\n", SensorTemp, ObjTemp);
    read_dht11_dat();
    delay(100);

    char data[100] = {0,};

    sprintf(data, "%f %f %f", humidity, temperature, ObjTemp);

    int sendFlag = send(server_socket, data, 100, 0);
    if(sendFlag < 0){
      perror("send error");
      connection();
    }
    else {
      memset(data, 0, 50);
      if(server_socket != -1){
        int recvFlag = recv(server_socket, data, 50, 0);
        char msg;
        if(recvFlag < 0){
          perror("recv error");
          connection();
          
        }
        printf("RECV DATA %s\n", data);
        
        write(s, !strcmp(data,"GO") ? "1" : "0", 2);
      }
    }

  }
  close(s);
  return 0;
}
