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
        //##webSocket.sendTXT(num, "Connected");
      }
      break;
    case WStype_TEXT:
      //Serial << num << " get Text: " << (char*)payload << endl;
      parseMessage((char*)payload, num);

      // send message to client
      //##webSocket.sendTXT(num, "message here");

      // send data to all connected clients
      //##webSocket.broadcastTXT("broadcast message here");
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
  //root.prettyPrintTo(Serial);  // for debug
  wsRequestHandler(root, num);
  
}

struct WsContainer {
  char* id;
  SuscriberPages suscriberPages;
  void (*callback)();
  int intervalTime;
};

WsContainer wsGetContainer[] = {    //resquest index : (ID, callback function)
  {"ajax_inputs", HOME, writeJSONResponse, 1000},
  {"ajax_alarms", ALARMS, writeJSON_Alarm_Response, 0},
  {"configs", CONFIGS, wsSendDataConfigs, 250}
};
#define SWSGC SizeOfArray(wsGetContainer) //Size of websocket get container

bool noSuscriber(bool suscribed[]) {
  for (int i = 0; i < sizeof(suscribed); i++) {
    if(suscribed[i]) return false;
  }
}

enum wsRequestType { GETWS, PUTWS, SUSCRIBE, UNSUSCRIBE, CONSOLE_LOG };

void wsRequestHandler(JsonObject& root, int num) {
  wsRequestType requestT;
  if (strncmp(root["request"], "get", 3) == 0 ) requestT = GETWS;
  else if (strncmp(root["request"], "put", 3) == 0 ) requestT = PUTWS;
  else if (strncmp(root["request"], "sus", 3) == 0 ) requestT = SUSCRIBE;
  else if (strncmp(root["request"], "uns", 3) == 0 ) requestT = UNSUSCRIBE;
  else if (strncmp(root["request"], "con", 3) == 0 ) requestT = CONSOLE_LOG;

  switch(requestT) {
    case GETWS:
      Serial.println("receive get request !!!");
        //root["data"] to do parse get data
    break;
    case PUTWS:
      Serial.println("receive put request !!!");
      parsePutRequest(root["data"]);
      break;
    case SUSCRIBE:
      for (int i = 0; i < SWSGC; i++){ //i for request index
        if( strcmp(root["id"], wsGetContainer[i].id) == 0 ) {
          //start sending data to client
          Serial.println("get subscription");
          suscribers[i][num] = true;
          timerWebSocket[i].interval(wsGetContainer[i].intervalTime, wsGetContainer[i].callback);
        }
      }
      break;
    case UNSUSCRIBE:
      for (int i = 0; i < SWSGC; i++){ //i for request index
        Serial.println("get unSubscription");
        suscribers[i][num] = false;
        if (noSuscriber(suscribers[i])) timerWebSocket[i].cancel(); 
      }
      break;
    case CONSOLE_LOG:
      char id[20]  { root["id"] };
      char log[200] { root["data"] };
      Serial << "WS console message From socket #" << num << "] id:[" << id << "][  " << log << " ]" << endl;
      break;
  }
}
/*
void wsRequestHandler(JsonObject& root, int num) {
  if (strcmp(root["request"], "put") ==0 ) { //handle put data request
    Serial.println("receive put request !!!");
    parsePutRequest(root["data"]);
    return;
  }
  for (int i = 0; i < SWSGC; i++){ //i for request index
    if( strcmp(root["id"], wsGetContainer[i].id) == 0 ) {
      if ( strcmp(root["request"], "suscribe") == 0 ) {
        //start sending data to client
        Serial.println("get subscription");
        suscribers[i][num] = true;
        timerWebSocket[i].interval(250, wsGetContainer[i].callback);
      }
      else if ( strcmp(root["request"], "unsuscribe") ==0 ) {
           //stop sending data to client
           Serial.println("get unSubscription");
           suscribers[i][num] = false;
           if (noSuscriber(suscribers[i])) timerWebSocket[i].cancel(); 
      }
    }
    
  }
}*/

void wsSendDataConfigs() {
  regulator_inputs();//refresh analog readings
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  json["id"] = "configs";

  //create data:
  StaticJsonBuffer<1000> dataBuffer;
  JsonObject& data = JSONconfigs__Corrected(dataBuffer);

  json["request"] = "response";
  json["data"] = data;
  char message[1000];
  json.printTo(message);
  //Serial << message << endl;

  for (int i = 0; i < WEBSOCKETS_SERVER_CLIENT_MAX; i++) {
    if (suscribers[CONFIGS][i]) { 
      webSocket.sendTXT(i, message);// from test  webSocket.sendTXT(i,testMessage);
    }
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

void parsePutRequest(JsonObject& data) {
  //data.printTo(Serial);
  int canal = data["canal"];
  int offset = data["offset"];
  inChannelID[canal].offset = offset;
}
