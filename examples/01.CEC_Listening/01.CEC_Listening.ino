/*
    01.CEC_Listening.ino

    Basic example to demonstrate the use of the CEClient library
    The client is configured in promiscuous and monitor mode
    to receive all the messages on the CEC bus

    No specific callback function is defined, therefore the client
    calls the default one, which prints the packets on the Serial port

    Use http://www.cec-o-matic.com/ to decode captured messages
*/

#include <BlynkGO_CEClient.h>

// List device types:
//  0x1000 -- Recording 1
//  0x2000 -- Recording 2
//  0x3000 -- Tuner 1
//  0x4000 -- Playback 1

#define CEC_PHYSICAL_ADDRESS    0x1000
#define CEC_INPUT_PIN           16    // ขาสัญญาณ CEC ต่อเข้า ESP32 ขา 16
#define CEC_OUTPUT_PIN          17

// create a CEC client
BlynkGO_CEClient CEC(CEC_PHYSICAL_ADDRESS, CEC_INPUT_PIN, CEC_OUTPUT_PIN);

void setup()
{

  Serial.begin(115200);

  // initialize the client with the default device type (PLAYBACK)
  CEC.begin();

  // enable promiscuous mode (print all the incoming messages)
  CEC.setPromiscuous(true);

  // enable monitor mode (do not transmit)
  CEC.setMonitorMode(true);

  CEC.onReceive([](int src, int dest, uint8_t * data, int len) {
    if(len > 0) {
      Serial.println("[HDMI] Data Received!");
    }
  });
}

void loop()
{
  CEC.update();
}