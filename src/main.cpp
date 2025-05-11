#include <Arduino.h>
#include "CommandManager.hpp"
#include "IODebouncer.h"
#include "PamiHardware.hpp"
#include "TestManager.hpp"
#include "PamIA.hpp"
#include "AsservManager.hpp"
#include "Chrono.hpp"
#include "DetectionManager.hpp"
#include "RoadMap.hpp"
#include <vector>
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

#define STOP_MOVING_MS 100000 // TODO valeurs de test
#define DETECTION_THRESHOLD_CM 10

// Pour chaque PAMI, on associe une feuille de route à une adresse de SRF08
std::vector<RoadMap> roadMaps = {
  RoadMap(254, {
    {ROADMAP_COMMAND::CELEBRATE, 1000, 60, 40, 0},
    {ROADMAP_COMMAND::WAIT, 2000, 0, 0, 0},
    {ROADMAP_COMMAND::GO_TO, 0, 1000, 0, DETECTION_THRESHOLD_CM},
    {ROADMAP_COMMAND::GO_TO, 0, 1000, 100, DETECTION_THRESHOLD_CM},
  })
};

IODebouncer btnTirette(IO_ID_TIRETTE, IO_DEBOUNCE_PERIOD, false);
IODebouncer btnCouleur(IO_ID_COULEUR, IO_DEBOUNCE_PERIOD, false);
IODebouncer btnGpio1(IO_ID_GRPIO_1, IO_DEBOUNCE_PERIOD, false);
IODebouncer btnGpio2(IO_ID_GRPIO_2, IO_DEBOUNCE_PERIOD, false);
SRF08 srf08(0, 1000); // On ne sait pas quel SRF08 est connecté au boot : adresse 0 en attendant
Servo servo1;

PamiHardWare pamiHardware(&btnTirette, &btnCouleur, &btnGpio1, &btnGpio2, &srf08, &servo1);
TestManager testManager(&Serial, &pamiHardware);
CommandManager commandManager(&Serial, &testManager);
AsservManager asservManager(&Serial0, &Serial);
DetectionManager detectionManager(&pamiHardware);
PamIA pamiA(&Serial, &pamiHardware, &asservManager, &detectionManager, roadMaps, STOP_MOVING_MS);

void setup() {
  Serial.begin(UART_PC_SPEED);
  Serial.setTimeout(10);
  Serial0.begin(UART_NUCLEO);
  Serial0.setTimeout(10);
  servo1.attach(IO_ID_GPIO_SERVO_01);
  Wire.begin(IO_I2C_SDA, IO_I2C_SCL, 100000);

  delay(2000);
  Serial.println("Detection du SRF08 branche...");
  for (int i=0; i<roadMaps.size(); i++) {
    Wire.beginTransmission(roadMaps[i].getSrfAddress());
    if (Wire.endTransmission() == 0) {
      srf08.setAddress(roadMaps[i].getSrfAddress());
      break;
    }
  }
  if (srf08.getAddress() == 0) {
    Serial.println("SRF08 non trouve!");
  } else {
    Serial.print("SRF08 trouve : ");
    Serial.println(srf08.getAddress());
  }
}

void loop() {
  commandManager.heartBeat();
  testManager.heartBeat();
  asservManager.heartBeat();
  detectionManager.heartBeat();
  pamiA.heartBeat();
}
