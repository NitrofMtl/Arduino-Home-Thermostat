JsonObject& JSONbackup(JsonBuffer& jsonBuffer) {
  JsonObject& backup = jsonBuffer.createObject();
  JsonArray& config = backup.createNestedArray("configs");
  JsonObject& config_in = jsonBuffer.createObject();
  config_in["K"] = K;
  config_in["vs"] = vs;
  config_in["smm"] = smm;
  config.add(config_in);
  JsonArray& chan = config_in.createNestedArray("inputs");

  for (byte i = 0; i < numChannel; i++) {
    JsonObject& chan_in = jsonBuffer.createObject();
    chan_in["offset"] = inChannelID[i].offset;
    chan_in["canal"] = i;
    chan.add(chan_in);
  }
  JsonArray& inputs = backup.createNestedArray("inputs");
  for (byte i = 0; i < numChannel; i++) {
    JsonObject& chan_in = jsonBuffer.createObject();
    chan_in["name"] = inChannelID[i].channelName;
    chan_in["status"] = outChannelID[i].channelSwitch;
    chan_in["setPoint"] = outChannelID[i].sp;
    inputs.add(chan_in);
  }

  JsonArray& alarmID = backup.createNestedArray("alarms");
  for (byte i = 0; i < 10; i++) {
    JsonObject& alarm_in = SPAlarm.backupAlarm(i, jsonBuffer);
    JsonArray& setpoint = alarm_in.createNestedArray("setpoints");
    for ( byte j = 0; j < numSetpoint; j++) {
      float setPointAl = alarmMem[i][j];
      setpoint.add(double_with_n_digits(setPointAl, 1));
    }
    alarmID.add(alarm_in);
  }
  // backup.prettyPrintTo(Serial);
  return backup;
}
//-----------------------------------------------------------
void backup() {
  StaticJsonBuffer<4000> jsonBuffer;
  JsonObject& json = JSONbackup(jsonBuffer);
  //json.prettyPrintTo(Serial);
  File log;
  SD.remove("backup/BACKUP.txt"); //delete old file to reset data
  log = SD.open("backup/BACKUP.txt", FILE_WRITE);
  if (!log) Serial.println("FAIL TO OPEN BACKUP");
  json.printTo(log);
  log.close();
  Serial.println("Backup done.");
}

//-----------------------------------------------------------
void restore() {
  char* json = bckup.get();
  StaticJsonBuffer<4000> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }
  // root.prettyPrintTo(Serial);
  JsonObject& config = root["configs"][0];
  K = config["K"];
  vs = config["vs"];
  smm = config["smm"];

  JsonArray& configs = config["inputs"];
  for (JsonArray::iterator it = configs.begin(); it != configs.end(); ++it) {
    JsonObject& input = *it;
    byte chan = input["canal"];
    inChannelID[chan].offset = input["offset"];
  }

  JsonArray& inputs = root["inputs"];
  byte i = 0;
  for (JsonArray::iterator it = inputs.begin(); it != inputs.end(); ++it) {
    JsonObject& input = *it;
    strcpy(inChannelID[i].channelName, input["name"]);
    outChannelID[i].channelSwitch = input["status"];
    outChannelID[i].sp = input["setPoint"];
    i++;
  }

  JsonArray& alarms = root["alarms"];
  //alarms.prettyPrintTo(Serial);
  byte  alarmTag = 0;
  for (JsonArray::iterator it = alarms.begin(); it != alarms.end(); ++it) {
    JsonObject& alarm = *it;
    //alarm.prettyPrintTo(Serial);
    SPAlarm.restoreAlarm(alarmTag, alarm);
    byte alarmType = alarm["type"];
    bool alarmSwitch = alarm["switch"];
    byte wHour = alarm["hour"];
    byte wMin = alarm["minute"];
    SPAlarm.set(alarmTag, alarmType, alarmSwitch, wHour, wMin);

    JsonArray& setpoints = alarm["setpoints"];
    //setpoints.prettyPrintTo(Serial);

    byte spTag = 0;
    for (JsonArray::iterator jt = setpoints.begin(); jt != setpoints.end(); ++jt) {
      float setpoint = *jt;
      //Serial.println(setpoint);
      alarmMem[alarmTag][spTag] = setpoint;
      spTag++;
    }
    alarmTag++;
  }
}