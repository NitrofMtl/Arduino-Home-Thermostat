void microChrono(char* chronoId, void (*timedCallback)() ) {
  unsigned long firstCheckPoint = micros();
  timedCallback();
  unsigned long chrono = micros() - firstCheckPoint;
  Serial << "Function " << chronoId << chrono << "uSec" << endl;
}


void regulator_inputs() {  //output regulation loop per channel
  //microChrono("inputRead ", inputRead();
  inputRead();
}

void regulator_outputs() { //outputs controler at 10Hz
  for (byte i = 0; i < 10; i++) {
    outChannelID[i].ssrOut(&inChannelID[i]);//ssr controler
  }
}

void inputRead() {
  for ( byte ch = 0; ch < numChannel; ch++) {
    if (inChannelID[ch].RTDSwitch) {     //read analog input and convert to temp
      inChannelID[ch].Ainput = RTDRead.readBit(adc.read(inChannelID[ch].AinputPin) - inChannelID[ch].offset) ;   // buffer input variable and add offset adjustement
      //Serial << "input: " << inChannelID[ch].Ainput << char(186) << "C" << endl;
    }
  }
}

//-----------------------------------------------------------

void checkWeeklyAlarm(){
  SPAlarm.handler();
}

void setSp(int numAlarm) { //set setpoint coresponding to the alarm trigged
  for (byte i = 0; i < (numChannel); i++) {
    if (outChannelID[i].channelSwitch) {
      if ( 5 < alarmMem[numAlarm][i]) { //add this condition to ignore 0 to keep setpoint to current setting
        outChannelID[i].sp = alarmMem[numAlarm][i];
      }      
    }
  }
  backup(); //save data after change apply
}
