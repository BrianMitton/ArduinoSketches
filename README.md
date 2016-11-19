
This is a quick example to see how to read an Amphenol NPA 201 Pressure sensor.
All it does is read the sensor and spit the results out the USB virtual serial port.
I developed it on an M0, but I think it should be easy to adapt to most any Arduino.

The code in setup that scans for IIC addresses in use is because a customer of mine was shipped some parts that did not answer to address 0x27.
I did not write the scan code and have forgotten where I copied it from.  Apologies to the original author.

Enjoy

