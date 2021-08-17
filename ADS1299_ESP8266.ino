#include <SPI.h>
#include <SoftwareSerial.h>

const byte WAKEUP = 0b00000010;     // Wake-up from standby mode
const byte STANDBY = 0b00000100;   // Enter Standby mode
const byte RESET = 0b00000110;   // Reset the device
const byte START = 0b00001000;   // Start and restart (synchronize) conversions
const byte STOP = 0b00001010;   // Stop conversion
const byte RDATAC = 0b00010000;   // Enable Read Data Continuous mode (default mode at power-up) 
const byte SDATAC = 0b00010001;   // Stop Read Data Continuous mode
const byte RDATA = 0b00010010;   // Read data by command; supports multiple read back

//Register Read Commands
const byte RREG = 0b00000000;
const byte WRET = 0b00000000;

const int CS = D8; //chip select pin
const int DRDY = D1; //data ready pin

const float tCLK = 0.000666;

boolean deviceIDReturned = false;

SoftwareSerial btSerial(D4, D2);


void setup() {

  btSerial.begin(115200);
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV2); //Divides 16MHz clock by 16 to set CLK speed to 1MHz
  SPI.setDataMode(SPI_MODE1);  //clock polarity = 0; clock phase = 1 (pg. 8)
  SPI.setBitOrder(MSBFIRST);  //data format is MSB (pg. 25)
  
  // initalize the  data ready and chip select pins:
  pinMode(DRDY, INPUT);
  pinMode(CS, OUTPUT);

  delay(10);  //delay to ensure connection
  
  digitalWrite(CS, LOW); //Low to communicated
  SPI.transfer(RESET); 
  digitalWrite(CS, HIGH); //Low to communicated
  
  REG();

  delay(1000);
}

void loop(){
  static bool stream = 0;
  if(btSerial.available()){
    char com = btSerial.read();
    if(com == 'b') {Start(); stream = 1;}
    if(com == 's') {stream = 0;}
    if(com == 'v') {btSerial.print("$$$");}
  }

  if(stream){updateData();}
}
