#include <TheThingsNetwork.h>
#include <pb_encode.h>
#include "measurement.pb.h"

TheThingsNetwork ttn(Serial1, Serial, TTN_FP_EU868);

byte appEUI[8] = { 0x70, 0xB3, 0xD5, 0x7E, 0xF0, 0x00, 0x00, 0xBF };
byte appKey[16] = { 0x9B, 0xB5, 0xAD, 0x34, 0xEF, 0x26, 0xBD, 0x72, 0x4B, 0x26, 0xAA, 0xAC, 0x70, 0xE8, 0x82, 0x04 };
byte buffer[51];

void setup()
{
  pinMode(2, INPUT);
  
  Serial1.begin(57600);
  Serial.begin(9600);

  while (!Serial);

  ttn.join(appEUI, appKey);
}

void loop()
{  
  bool motion = digitalRead(2) == HIGH;
  uint32_t water = 682 - analogRead(A0);
  
  Serial.print(F("Motion = "));
  Serial.print(motion);
  Serial.print(F("; water = "));
  Serial.println(water);

  api_Measurement measurement = api_Measurement_init_default;
  measurement.motion = motion;
  measurement.water = water;

  pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
  pb_encode(&stream, api_Measurement_fields, &measurement);

  ttn.sendBytes(buffer, stream.bytes_written);

  delay(10000);
}
