void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {

  switch (type) {
    case WStype_DISCONNECTED:
      Serial << num << " Disconnected!" << endl;
      break;
    case WStype_CONNECTED:
      {
        //IPAddress ip = webSocket.remoteIP(num);
        Serial << num << " Connected url: " << (char*)payload << endl;
        // send message to client
        webSocket.sendTXT(num, "Connected");
      }
      break;
    case WStype_TEXT:
      Serial << num << " get Text: " << (char*)payload << endl;

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
