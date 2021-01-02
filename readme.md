# (2021) [ STM32Cube.AI ] (온라인) 서울 하드웨어 해커톤

## 팀명 : 실버버튼
  작품명 : 

## 시연영상

## 구성원
- **[오정민](https://github.com/owjs3901)(팀장)**
>

- **[소호정](https://github.com/bona0722)(팀원)**
>

- **[윤예준](https://github.com/yj4165)(팀원)**
>


## 개발 개요
- 현재 저출산 및 평균수명이 높아지면서 노인인구 비중이 증가하는 추세이다.
하지만 다수를 차지하는 노인들은 ICT기술 이용의 사각지대에 놓여있다.
특히 노인들에게 가장 필요하면서 UI조차 잘 마련되어있지 않은 스마트 홈을 “리모컨”이라는 익숙한 매개체를 통하여 컨트롤 할 수 있게 구현해 IoT 스마트홈에 대한 진입 및 학습 장벽을 낮추고자 한다.

## UI

## 개발환경
- 하드웨어 구성
  - Raspberry Pi 4
  
| col1 | col2 |
| :---: | :---: |
| 구성 | 비고 |
| DHT11(온습도센서 모듈[SEN030000]) | 1 |
| 5AG3HD00 | 1 |
| SG90 | 1 |

  - STM32
  
  | col1 | col2 |
  | :---: | :---: |
  | HC-06 | 1 |
  | 택트스위치 | 1 |
  | 4201ASR-3 | 1 |
  
- Raspberry Pi 4(Raspbian 기반) Pin Map
![Alt text](/Images/RPI4.png)

- STM32 Pin Map
![Alt text](/Images/stm32_circuit.jpg)

## 아키텍처
- Service Architecture 
![Alt text](/Images/Home.png)


