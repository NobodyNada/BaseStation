### Github Repository
https://github.com/osu-usli/BaseStation

# BaseStation

This is the code that runs on the payload base station, an Arduino Uno connected to a Semtech SX1280 ToF module and powered from a PC over USB.

The base station simply recieves packets from the rocket in an infinite loop. If it recieves a ranging request the ToF module automatically sends a ranging response; if it recieves a data packet containing a grid square it prints the grid square to the serial console.

The project relies on a modified version of [Stuart Robinson's SX1280 library](https://github.com/StuartsProjects/SX12XX-LoRa), included in the `SX1280.{h,cpp}` files in this repository. The modifications consist of the addition of a `receiveRangingOrData` method that configures the SX1280 module such that it can recieve either a ranging request or a LoRa data packet. Initially, we tried switching the module back-and-forth between ranging and data modes, but this proved to be extremely unreliable: oftentimes the radio would simply stop recieving packets altogether, and we aren't sure why -- the documentation from Semtech does not explain this behavior, and the symptoms were erratic and difficult to reproduce. However, we discovered that the SX1280 is fully capable of recieving data packets while in ranging mode (even though this feature is not documented), so we added this method which worked reliably for the competition flight. See also this GitHub issue: https://github.com/StuartsProjects/SX12XX-LoRa/issues/44

This software has no external dependencies besides the standard Arduino SDK. If you open the `BaseStation.ino` file in the Arduino IDE, it should compile and run out of the box.
