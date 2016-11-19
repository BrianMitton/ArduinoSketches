#include <Wire.h>

void setup()
{
  Wire.begin();
 
  Serial.begin(9600);
  while (!Serial);             // wait for serial monitor
  Serial.println("\nI2C ");

   byte error, address;
  int nDevices;
 
  Serial.println("Scanning...");
 
  nDevices = 0;
  for(address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
 
    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");
 
      nDevices++;
    }
    else if (error==4)
    {
      Serial.print("Unknow error at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");

   delay(1000);           // wait 5 seconds
    
}



#define NPA201_BAROMETER 0x27
void loop()
{
  short c;
  short npa_status;
  int   bridge;
  int   temp;
  int   temperature;
  int   pressure;
  
  Wire.requestFrom(NPA201_BAROMETER, 1);    // request Status

  while(Wire.available())    // slave may send less than requested
  {
    c = Wire.read();    // receive a byte as character
    Serial.print(c);         // print the character
    Serial.println();
  }

  Wire.beginTransmission(0x27); // transmit to device  (0x27)
                              // device address is specified in datasheet
  Wire.write(0xAC);             // sends read request  
  Wire.endTransmission(); 

 
  do
  {
    c = 0;
    Wire.requestFrom(NPA201_BAROMETER, 1);    // request Status
    while(Wire.available())    // slave may send less than requested
    {
      c = Wire.read();    // receive a byte as character
//      Serial.print(c);         // print the character
//      Serial.println();
    }
  }while ((c & 0x20) != 0x00);
  
//delay(1000);           // wait a sec
  
  //Wire.beginTransmission(NPA201_BAROMETER);
  Wire.requestFrom(NPA201_BAROMETER, 5);    // request 6 bytes from slave device #2

//  while(Wire.available())    // slave may send less than requested
//  {
//    c = Wire.read();    // receive a byte as character
//    Serial.print(c);         // print the character
//    Serial.print(", ");         // print
//  }

  npa_status = Wire.read();    // receive a byte
  c = Wire.read();    // receive a byte
  bridge = c;
  bridge = bridge<<8;
  c = Wire.read();    // receive a byte
  bridge += c;

  c = Wire.read();    // receive a byte
  temp = c;
  temp = temp<<8;
  c = Wire.read();    // receive a byte
  temp += c;

  temperature = (((temp>>8) * (85+40))>>8) -40;

  // Absolute pressure. Not adjusted for heigth above sea level
  pressure =  (((((long)bridge * ((1260-260)))>>8)+(260<<8))*10)>>8;

  Serial.print ("Status: ");
  Serial.print (npa_status);
  Serial.print (", Bridge ");
  Serial.print (bridge);
  Serial.print (", Temperature ");
  Serial.print (temperature);
  Serial.print (", mBars ");
  Serial.print (pressure/10);
  Serial.print (".");
  Serial.print (pressure%10);
  Serial.println();

  //error = Wire.endTransmission();
  delay(2000);           // wait

}
