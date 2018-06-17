void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {

  switch (type) {
    case WStype_DISCONNECTED:
      Serial << num << " Disconnected!" << endl;
      cleanupDisconnected(num);
      break;
    case WStype_CONNECTED:
      {
        //IPAddress ip = webSocket.remoteIP(num);
        Serial << num << " Connected: " << (char*)payload << endl;
        // send message to client
        webSocket.sendTXT(num, "Connected");
      }
      break;
    case WStype_TEXT:
      Serial << num << " get Text: " << (char*)payload << endl;
      parseMessage((char*)payload, num);

      // send message to client
      webSocket.sendTXT(num, "message here");

      // send data to all connected clients
      webSocket.broadcastTXT("broadcast message here");
      break;
    case WStype_BIN:
      Serial << num << " get binary lenght: ";

      // send message to client
      webSocket.sendBIN(num, payload, lenght);
      break;
  }
}

void parseMessage(char* message, int num) {
    StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(message);
  
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }
  root.prettyPrintTo(Serial);  // for debug
  wsRequestHandler(root, num);
  
}

struct WsContainer {
  char* request;
  SuscriberPages suscriberPages;
  void (*callback)();
};

WsContainer wsGetContainer[] = {    //resquest index : (ID, callback function)
  {"ajax_inputs", HOME, writeJSONResponse},
  {"ajax_alarms", ALARMS, writeJSON_Alarm_Response},
  {"configs", CONFIGS, wsSendDataConfigs}
};
#define SWSGC SizeOfArray(wsGetContainer) //Size of websocket get container

bool noSuscriber(bool suscribed[]) {
  for (int i = 0; i < sizeof(suscribed); i++) {
    if(suscribed[i]) return false;
  }
}

void wsRequestHandler(JsonObject& root, int num) {
  for (int i = 0; i < SWSGC; i++){ //i for request index
    if( strcmp(root["request"], wsGetContainer[i].request) ==0 ) {
      if ( strcmp(root["subscription"], "suscribe") ==0 ) {
        //start sending data to client
        Serial.println("get subscription");
        suscribers[i][num] = true;
        timerWebSocket[i].interval(1000, wsGetContainer[i].callback);
      }
      else if ( strcmp(root["subscription"], "unsuscribe") ==0 ) {
           //stop sending data to client
           Serial.println("get unSubscription");
           suscribers[i][num] = false;
           if (noSuscriber(suscribers[i])) timerWebSocket[i].cancel(); 
      }
    }
  }
}

void wsSendDataConfigs() {
  regulator_inputs();//refresh analog readings
  StaticJsonBuffer<2000> jsonBuffer;
//  JsonObject& json = JSONconfigs(jsonBuffer);
  JsonObject& json = jsonBuffer.createObject();
  json["request"] = "configs";
  json["data"] = JSONconfigs(jsonBuffer);
  char message[1000];
  json.printTo(Serial);Serial.println();
  json.printTo(message);
  char* testMessage = "{\"request\":\"configs\",\"subscription\":\"suscribe\"}";
  for (int i = 0; i < WEBSOCKETS_SERVER_CLIENT_MAX; i++) {
    if (suscribers[CONFIGS][i]) webSocket.sendTXT(i,testMessage);//webSocket.sendTXT(i, message);
  }  
}

void cleanupDisconnected(int num){
  Serial.println("clean disconnect");
  for (int i = 0; i < SWSGC; i++){ //i for request index
    if ( suscribers[i][num] ) {
      Serial.println("found suscribed");
      suscribers[i][num] = false;//unsuscribe to any get request for disconnected client
      if (noSuscriber(suscribers[i])) {
        timerWebSocket[i].cancel(); 
        Serial.println("clean interval");
      }
    }
  }
}

