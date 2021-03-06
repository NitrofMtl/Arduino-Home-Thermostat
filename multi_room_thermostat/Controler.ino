inline void microChrono(char* chronoId, void (*timedCallback)() ) {
  unsigned long firstCheckPoint = micros();
  timedCallback();
  unsigned long chrono = micros() - firstCheckPoint;
  Serial << "Function " << chronoId << chrono << "uSec" << endl;
}

void regulator() {  //output regulation loop per channel
  //microChrono("inputRead ", inputRead();
  inputRead();
  regulator_outputs();
}

void regulator_outputs() { //outputs controler at 10Hz
  for (byte i = 0; i < 10; i++) {
    outChannelID[i].ssrOut(inChannelID[i]);//ssr controler
  }
}

void inputRead() {
  while (adc.available()) {
    uint16_t* data = ADC_Sampler::data();  //get data from buffer
    for ( byte ch = 0; ch < numChannel; ch++) {
    //read analog input and convert to temp  
      inChannelID[ch].Ainput = RTDRead.readBit(data[ch] - inChannelID[ch].offset) ;   // buffer input variable and add offset adjustement
      //Serial << "input: " << inChannelID[ch].Ainput << char(186) << "C" << endl;
    }
  }
}

//-----------------------------------------------------------

void checkWeeklyAlarm() {
  digitalClockDisplay(now(), Serial);
  weeklyAlarm.handler();
}

void setSp(int numAlarm) { //set setpoint corresponding to the alarm trigged
  for (byte i = 0; i < numChannel; i++) {
    //if (outChannelID[i].channelSwitch) {  //<== callball will not trig anyway withou switch...
      if ( 5 <= alarmMem[numAlarm][i]) { //add this condition to ignore 0 to keep setpoint to current setting
        outChannelID[i].sp = alarmMem[numAlarm][i];
      }
    //}
  }
  backup(); //save data after change apply
}
