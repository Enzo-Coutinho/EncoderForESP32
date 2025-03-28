#include <Arduino.h>
#include "encoder_new_version/Encoder.h"

Encoder encoder(Config_Encoder::EncoderPorts::PORTA_1_ENCODER);

void setup() {
  Serial.begin(115200);
}

void loop() {
  Serial.printf("Encoder position: %f\n", encoder.getPosition());
}