#include <Arduino.h>
#include "CommandManager.hpp"
#include "IODebouncer.h"
#include "PamiHardware.hpp"
#include "TestManager.hpp"
#include <ESP32Servo.h>

#define PIN_LIDAR_PWM 18
#define PWM_CHANNEL 0
#define PWM_FREQ 1000
#define PWM_RESOLUTION 10

#define UART_PC_SPEED 115200
#define UART_NUCLEO 115200

#define IO_DEBOUNCE_PERIOD 5

#define IO_ID_TIRETTE 0
#define IO_ID_COULEUR 1
#define IO_ID_GRPIO_1 2
#define IO_ID_GRPIO_2 3
#define IO_ID_GPIO_SERVO_01 4
#define IO_ID_GPIO_SERVO_02 5
#define IO_I2C_SDA 8
#define IO_I2C_SCL 7

IODebouncer btnTirette(IO_ID_TIRETTE, IO_DEBOUNCE_PERIOD, false);
IODebouncer btnCouleur(IO_ID_COULEUR, IO_DEBOUNCE_PERIOD, false);
IODebouncer btnGpio1(IO_ID_GRPIO_1, IO_DEBOUNCE_PERIOD, false);
IODebouncer btnGpio2(IO_ID_GRPIO_2, IO_DEBOUNCE_PERIOD, false);
SRF08 srf08(254, 1000);
Servo servo1;

PamiHardWare pamiHardware(&btnTirette, &btnCouleur, &btnGpio1, &btnGpio2, &srf08, &servo1);
TestManager testManager(&Serial, &pamiHardware);
CommandManager commandManager(&Serial, &testManager);

void setup() {
  Serial.begin(UART_PC_SPEED);
  Serial.setTimeout(10);
  Serial0.begin(UART_NUCLEO);
  servo1.attach(IO_ID_GPIO_SERVO_01);
  Wire.begin(IO_I2C_SDA, IO_I2C_SCL, 100000);
}

void loop() {
  commandManager.heartBeat();
  testManager.heartBeat();
}
