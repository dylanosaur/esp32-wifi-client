#include <WiFi.h>

WiFiClient localClient;
const char* ssid = "YourWifiName";
const char* password = "YourWifiPassword";
const uint port = 5000; // your remote server PORT
const char* ip = "192.168.4.169"; // your remote server IP
const int analogPin = 34; // sensor pin

void setup(){
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("\nSetup");

    WiFi.mode(WIFI_STA); //Optional
    WiFi.begin(ssid, password);
    Serial.println("\nConnecting");

    while(WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(100);
    }

    Serial.println("\nConnected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());
}
void sendRequest() {
  if (localClient.connect(ip, port)) {                 // Establish a connection
    if (localClient.connected()) {
      // Send the HTTP GET request
      int analogValue = analogRead(analogPin);
      String dataString = String(analogValue);
    
      // Format the GET request
      String getRequest = "GET /esp32?data=" + dataString + " HTTP/1.1";
      localClient.println(getRequest);
      localClient.println("Host: your.server.com");         // Replace with the actual host
      localClient.println("Connection: close");
      localClient.println();                                // End of headers
      
      Serial.println("[Tx] "+ getRequest);         // Log the request
    }

    while (!localClient.available());                // Wait for response
    
    // Read and print the entire response
    while (localClient.available()) {
      String line = localClient.readStringUntil('\n');
      Serial.print("[Rx] ");
      Serial.println(line);
    }
 
    localClient.stop();                            // Close the connection
  }
}
void loop(){
  
      Serial.println("\nMain Loop");
      delay(10000);
      sendRequest();

}
