void regulator_inputs() {  //output regulation loop per channel
  inputRead();
  programAlarm();// alarm check  
}

void regulator_outputs(){ //outputs controler at 10Hz
    for (byte i = 0; i < 10; i++) outChannelID[i].ssrOut(&inChannelID[i]);//ssr controler
}

void inputRead() {// input reading and caculate temp.
  for ( byte ch = 0; ch < numChannel; ch++) {
    if (inChannelID[ch].RTDSwitch) {     //read analog input and convert to temp
      inChannelID[ch].Ainput = RTDRead[ch].read(inChannelID[ch].AinputPin) + inChannelID[ch].offset;   // buffer input variable and add offset adjustement
      // if (ch == 5) Serial.println();
      // Serial.print(channelName[ch] + ": " + inChannelID[ch].AinputPin + ", " );
      // if (ch == numChannel - 1) Serial.println();
    }
  }
}

//-----------------------------------------------------------

void init_alarmMemory() {   ///probably to be remove
  for (byte i = 0; i < numAlarm; i++) {
    for (byte j = 0; j < numSetpoint; j++) {
      alarmMem[i][j] = 21;
    }
  }
}

void programAlarm() { //live monitoring function
  SPAlarm.weeklyAlarmMon(); // monitoring if an alarm have to be trigger
  for ( byte i = 0; i <= 9; i++ ) {
    alarmTrigger(i); //trigger alarm if true
  }
}

void alarmTrigger(byte numAlarm) { // excute if trigger as beren turn on by AlarmMon
  if ( SPAlarm.trigger(numAlarm) == 1 ) {
    setSp(numAlarm);
  }
}

void setSp(byte numAlarm) {
  for (byte i = 0; i < (numChannel); i++) {
    if (outChannelID[i].channelSwitch) {
      outChannelID[i].sp = alarmMem[numAlarm][i];
    }
  }
}

void checkLatch(){

}


