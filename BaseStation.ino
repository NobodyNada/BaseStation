// OSU AIAA 2021-2022 Payload base station
// Adapted from 55_Ranging_Slave by Stuart Robinson: 
// https://github.com/StuartsProjects/SX12XX-LoRa/blob/master/examples/SX128x_examples/Ranging/55_Ranging_Slave/55_Ranging_Slave.ino

#define programversion "V1.0"

#include <SPI.h>
#include "SX128XLT.h"
#include "Settings.h"

SX128XLT LT;


void loop()
{
  int16_t grid_square;

  // Wait for either a ranging request or a data packet from the payload.
  int packet_length = LT.receiveRangingOrData((byte*)&grid_square, sizeof(grid_square), RangingAddress, rangingRXTimeoutmS, TXpower, WAIT_RX); 

  if (packet_length == -1) {
    // Something went wrong.
    Serial.print("Error: ");
    LT.printIrqStatus();
    Serial.println();
    return;
  }
  else if (packet_length == 0) {
    // We recieved a ranging packet; the SX1280 will automatically send a response.
    Serial.println("Ranging success");
    return;
  } else if (packet_length != sizeof(grid_square)) {
    // We recieved a data packet, but it was the wrong length.
    Serial.println("Length mismatch");
    return;
  }
      
  // We recieved a data packet containg the grid square estimate.
  Serial.print("Got grid square: ");
  Serial.println(grid_square);
}


void led_Flash(unsigned int flashes, unsigned int delaymS)
{
  //flash LED to show board is alive
  unsigned int index;

  for (index = 1; index <= flashes; index++)
  {
    digitalWrite(LED1, HIGH);
    delay(delaymS);
    digitalWrite(LED1, LOW);
    delay(delaymS);
  }
}


void setup()
{
  Serial.begin(115200);            //setup Serial console ouput
  Serial.println();
  Serial.println(__FILE__);
  Serial.print(F("Compiled "));
  Serial.print(__TIME__);
  Serial.print(F(" "));
  Serial.println(__DATE__);
  Serial.println(F(programversion));
  Serial.println(F("Stuart Robinson"));
  Serial.println();

  Serial.println("55_Ranging_Slave Starting");

  pinMode(LED1, OUTPUT);
  led_Flash(2, 125);

  SPI.begin();

  if (LT.begin(NSS, NRESET, RFBUSY, DIO1, LORA_DEVICE))
  {
    Serial.println(F("Device found"));
    led_Flash(2, 125);
  }
  else
  {
    Serial.println(F("No device responding"));
    while (1)
    {
      led_Flash(50, 50);                                 //long fast speed flash indicates device error
    }
  }

  //The function call list below shows the complete setup for the LoRa device for ranging using the information
  //defined in the Settings.h file.
  //The 'Setup LoRa device for Ranging' list below can be replaced with a single function call, note that
  //the calibration value will be loaded automatically from the table in the library;
  //LT.setupRanging(Frequency, Offset, SpreadingFactor, Bandwidth, CodeRate, RangingAddress, RangingRole);

  LT.setupRanging(Frequency, Offset, SpreadingFactor, Bandwidth, CodeRate, RangingAddress, RANGING_SLAVE);

  //***************************************************************************************************
  //Setup LoRa device for Ranging Slave
  //***************************************************************************************************
  LT.setMode(MODE_STDBY_RC);
  LT.setPacketType(PACKET_TYPE_RANGING);
  LT.setModulationParams(SpreadingFactor, Bandwidth, CodeRate);
  LT.setPacketParams(12, LORA_PACKET_VARIABLE_LENGTH, 0, LORA_CRC_ON, LORA_IQ_NORMAL, 0, 0);
  LT.setRfFrequency(Frequency, Offset);
  LT.setTxParams(TXpower, RADIO_RAMP_02_US);
  LT.setRangingMasterAddress(RangingAddress);
  LT.setRangingSlaveAddress(RangingAddress);
  LT.setRangingCalibration(LT.lookupCalibrationValue(SpreadingFactor, Bandwidth));
  LT.setRangingRole(RANGING_SLAVE);
  LT.writeRegister(REG_RANGING_FILTER_WINDOW_SIZE, 8); //set up window size for ranging averaging
  LT.setHighSensitivity();
  //***************************************************************************************************


  LT.setRangingCalibration(11300);               //override automatic lookup of calibration value from library table

  Serial.print(F("Calibration,"));
  Serial.println(LT.getSetCalibrationValue());           //reads the calibratuion value currently set
}
