#ifndef __BLYNKGO_CECLIENT_H__
#define __BLYNKGO_CECLIENT_H__

#include "CEC/CEC_Device.h"

// List device types:
//  0x1000 -- Recording 1
//  0x2000 -- Recording 2
//  0x3000 -- Tuner 1
//  0x4000 -- Playback 1

class BlynkGO_CEClient : public CEC_Device
{
    typedef void (*OnReceiveCallbackFunction)(int, int, unsigned char *, int);
    typedef void (*OnTransmitCompleteCallbackFunction)(bool);

  public:
    BlynkGO_CEClient(int physicalAddress, int inputPin, int outputPin = -1);
    void begin(CEC_DEVICE_TYPE type = CEC_LogicalDevice::CDT_PLAYBACK_DEVICE);
    bool isReady();
    bool write(int targetAddress, unsigned char * buffer, int count);
    int  getLogicalAddress();
    void setPromiscuous(bool promiscuous);
    void setMonitorMode(bool monitorMode);
    void onTransmit(OnTransmitCompleteCallbackFunction);
    void onReceive(OnReceiveCallbackFunction);
    void run();

    inline void update()      { this->run();  }

  private:
    void OnTransmitComplete(bool);
    void OnReceive(int source, int dest, unsigned char * buffer, int count);
    void OnReady();
    OnTransmitCompleteCallbackFunction _onTransmitCompleteCallback;
    OnReceiveCallbackFunction _onReceiveCallback;
    bool _ready;
};

#endif // __BLYNKGO_CECLIENT_H__