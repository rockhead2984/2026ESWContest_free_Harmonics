# escalator-control-system

# 🚀 혼잡도 기반 능동형 에스컬레이터 흐름 제어 및 무선 관제 시스템

> 제 24회 임베디드 SW 경진대회 출품작 (다학제 융합 프로젝트)

---

## 📌 프로젝트 개요

에스컬레이터의 '한 줄 서기' 문화로 인한 기계적 편마모와 구동축 하중 불균형 문제를 해결하기 위해, 실시간 센서 데이터와 사용자 행동 양식을 융합한 **타협형 지능 시스템(Adaptive System)** 을 구축합니다.

---

## 🛠️ 기술 스택

| 분야 | 기술 |
|---|---|
| 펌웨어 | STM32 F446RE, FreeRTOS, C |
| 무선 통신 | ESP8266, MQTT, WiFi |
| 센서 | VL53L0X (ToF 거리), INA219 (전류) |
| 통신 | CAN Bus, UART, I2C |
| 서버 | Python, Flask, MySQL, Mosquitto |
| 대시보드 | HTML, JavaScript, WebSocket (SocketIO) |

---

## 🏗️ 시스템 구조

[Node A - 센서부] [Node B - 구동부]
VL53L0X 거리 측정 → CAN 통신 → 모터 속도 제어
INA219 전류 측정 ↕ PID 제어
부저/LED 피드백 ← 피드백
↓
ESP8266
↓ WiFi/MQTT
Mosquitto 브로커
↓
MySQL DB
↓
웹 대시보드 (실시간)


---

## 📁 레포지토리 구조

escalator-control-system/

├── node_a/ # STM32 Node A 펌웨어 (CubeIDE)

├── esp8266/ # ESP8266 Arduino 코드

├── server/ # Python 서버 (Flask + MQTT + MySQL)

└── README.md


---

## ⚙️ 실행 방법

**1. Mosquitto 실행**

"C:\Program Files\mosquitto\mosquitto" -v -c "C:\Program Files\mosquitto\mosquitto.conf"


**2. STM32 전원 연결** (자동 실행)

**3. 대시보드 실행**

py server/dashboard.py


**4. 브라우저 접속**

http://127.0.0.1:5000


---

## 👥 팀 구성

| 전공 | 역할 |
|---|---|
| 컴퓨터공학 | STM32 펌웨어, FreeRTOS, MQTT, 웹 서버 |
| 전기공학 | 회로 설계, CAN 통신, PWM 제어 |
| 종합 | 3D 프린팅, PID 제어, 기구부 설계 |

---

## 📊 현재 구현 현황

- [x] VL53L0X 거리 측정
- [x] 부저 혼잡 알림
- [x] ESP8266 MQTT 무선 전송
- [x] MySQL 데이터 저장
- [x] 실시간 웹 대시보드
- [x] CAN 통신
- [x] Node B 모터 제어
- [x] INA219 전류 모니터링
