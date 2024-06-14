#include "BlynkGO_CEClient.h"

// create a new instance of BlynkGO_CEClient
BlynkGO_CEClient::BlynkGO_CEClient(int physicalAddress, int inputPin, int outputPin) :
  CEC_Device(physicalAddress, inputPin, outputPin < 0 ? inputPin : outputPin)
{
  _ready = false;
}

// init BlynkGO_CEClient
void BlynkGO_CEClient::begin(CEC_DEVICE_TYPE type)
{
  CEC_Device::Initialize(type);
}

// return the ready state
bool BlynkGO_CEClient::isReady()
{
  return _ready;
}

// write a packet on the bus
bool BlynkGO_CEClient::write(int targetAddress, unsigned char * buffer, int count)
{
  return CEC_Device::TransmitFrame(targetAddress, buffer, count);
}

// return the logical address
int BlynkGO_CEClient::getLogicalAddress()
{
  return _logicalAddress;
}

// enable-disable promiscuous mode
void BlynkGO_CEClient::setPromiscuous(bool promiscuous)
{
  Promiscuous = promiscuous;
}

// enable-disable monitor mode
void BlynkGO_CEClient::setMonitorMode(bool monitorMode)
{
  MonitorMode = monitorMode;
}

// set callback function when a transmit is complete
void BlynkGO_CEClient::onTransmit(OnTransmitCompleteCallbackFunction callback)
{
  _onTransmitCompleteCallback = callback;
}

// set callback function when a new packet is received
void BlynkGO_CEClient::onReceive(OnReceiveCallbackFunction callback)
{
  _onReceiveCallback = callback;
}

// run, to be executed in the loop for the FSM
void BlynkGO_CEClient::run()
{

  CEC_Device::Run();
}


// ----- PRIVATE METHODS -----

// OnTransmitComplete redefinition, if a callback function is available, call it
void BlynkGO_CEClient::OnTransmitComplete(bool success)
{
  if(_onTransmitCompleteCallback)
    _onTransmitCompleteCallback(success);

  CEC_Device::OnTransmitComplete(success);
}

// OnReceive redefinition, if a callback function is available, call it
// if not, call the parent function
void BlynkGO_CEClient::OnReceive(int source, int dest, unsigned char * buffer, int count)
{
  if(_onReceiveCallback)
    _onReceiveCallback(source, dest, buffer, count);
  else
    CEC_Device::OnReceive(source, dest, buffer, count);

  if(!MonitorMode && dest == _logicalAddress && count == 1 && buffer[0] == 0x83) {
    unsigned char buffer[4];
    buffer[0] = 0x84;
    buffer[1] = _physicalAddress >> 8;
    buffer[2] = _physicalAddress;
    buffer[3] = _deviceType;
    TransmitFrame(0xF, buffer, 4);
  }
}

// OnReady redefinition, to save the current status
void BlynkGO_CEClient::OnReady()
{
  _ready = true;
}
