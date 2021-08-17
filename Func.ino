void getDevID(){
  digitalWrite(CS, 0);
  transfer(_SDATAC);
  transfer(_RREG);
  transfer(0x00);
  byte data = transfer(0x00);
  transfer(_RDATAC);
  digitalWrite(CS, 0);
  btSerial.print("Device ID:");
  btSerial.println(data, BIN);
}

void Start(){
  digitalWrite(CS, 0);
  transfer(_START);
  digitalWrite(CS, 1);
}

void REG(){
  WREG(CONFIG1, 0x96);
  WREG(CONFIG2, 0xC0);
  WREG(CONFIG3, 0xEC);
  WREG(LOFF, 0x02);
  WREG(CH1SET, 0x68);
  WREG(CH2SET, 0x68);
  WREG(CH3SET, 0x68);
  WREG(CH4SET, 0x68);
  WREG(CH5SET, 0x68);
  WREG(CH6SET, 0x68);
  WREG(CH7SET, 0x68);
  WREG(CH8SET, 0x68);
  WREG(BIAS_SENSP, 0xFD);
  WREG(BIAS_SENSN, 0xFD);
  WREG(LOFF_SENSP, 0x00);
  WREG(LOFF_SENSN, 0x00);
  WREG(LOFF_FLIP, 0x00);
  WREG(LOFF_STATP, 0x00);
  WREG(LOFF_STATN, 0x00);
  WREG(GPIO, 0x0F);
  WREG(MISC1, 0x00);
  WREG(MISC2, 0x00);
  WREG(CONFIG4, 0x00);
}

void WREG(byte _address, byte _value){
  byte opcode1 = _WREG + _address;  //001rrrrr; _RREG = 00100000 and _address = rrrrr
  digitalWrite(CS, 0);
  transfer(_SDATAC);
  transfer(opcode1);
  transfer(0x00);
  transfer(_value);
  transfer(_RDATAC);
  btSerial.println("Register 0x");
  btSerial.print(_address, HEX);
  btSerial.println(" modified");
}

void updateData(){
  static byte count;
  if(digitalRead(DRDY)==0){
    digitalWrite(CS, 0);
    btSerial.write(0xA0);
    btSerial.write(count);
    for(int i=0; i<9; i++){
      for(int j=0; j<3; j++){
        byte dataByte = transfer(0x00);
        if(i>0){btSerial.write(dataByte);}
      }
    }
    digitalWrite(CS, 1);
    
    for(int i=0; i<6; i++){btSerial.write(0x00);}
    btSerial.write(0xC0);
  }
}

byte transfer(byte _data){
  return SPI.transfer(_data);
}

float calc(long _val){
  float v = 5.0/pow(2, 24)*_val-2.5;
  return v;
}
