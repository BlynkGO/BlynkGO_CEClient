#include <BlynkGO_CEClient.h>
#include <TridentTD_LineNotify.h>

// List device types:
//  0x1000 -- Recording 1
//  0x2000 -- Recording 2
//  0x3000 -- Tuner 1
//  0x4000 -- Playback 1

#define CEC_PHYSICAL_ADDRESS    0x1000
#define CEC_INPUT_PIN           16    // ขาสัญญาณ CEC ต่อเข้า ESP32 ขา 16
#define CEC_OUTPUT_PIN          17

#define SSID                    "-------"
#define PASSWORD                "-------"
#define LINE_TOKEN              "---------------"
#define LINE_NOTIFY_INTERVAL    15000  // ms

// create a CEC client
BlynkGO_CEClient CEC(CEC_PHYSICAL_ADDRESS, CEC_INPUT_PIN, CEC_OUTPUT_PIN);
bool HDMI_data_received = false;

void setup()
{
  Serial.begin(115200);
  WiFi.begin(SSID, PASSWORD);
  LINE.setToken(LINE_TOKEN);

  // initialize the client with the default device type (PLAYBACK)
  CEC.begin();

  // enable promiscuous mode (print all the incoming messages)
  CEC.setPromiscuous(true);

  // enable monitor mode (do not transmit)
  CEC.setMonitorMode(true);

  CEC.onReceive([](int src, int dest, uint8_t * data, int len) {
    if(len > 0) {
      Serial.println("[HDMI] Data Received!");
      HDMI_data_received = true;
    }
  });
}

void loop()
{
  CEC.update();

  if(HDMI_data_received) {
    HDMI_data_received = false;
    if(WiFi.isConnected()) {
      static uint32_t timer_linenotify;
      if(timer_linenotify <= millis()) {
        timer_linenotify = millis() + LINE_NOTIFY_INTERVAL;
        Serial.println("[LINE] notify...");
        LINE.notify("[HDMI] data received");
      }
    }
  }
}

