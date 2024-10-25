// ESP32
// run with php -S 0.0.0.0:8000

#include <WiFi.h>
#include <HTTPClient.h>

// set network ssid and key
const char* ssid = "TMOBILE-23E1";
const char* password = "widen.wrought.shrunk.darkening";

const char* serverName = "http://192.168.12.204:8000/data_receiver.php"; 

#define RXD1 18
#define TXD1 17

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600, SERIAL_8N1, RXD1, TXD1);


  // Connect to wifi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void serialTransmit(String data) {
  Serial1.print(data);
}

String serialRead() {
  String receivedData = "";
  while (Serial1.available()) {
    char incomingByte = Serial1.read();
    receivedData += incomingByte;
  }
  return receivedData;
}

void loop() {
  // Read from Serial1 and send to Serial
  String received = serialRead();
  if (received.length() > 0) {
    Serial.print(received);

  sendCsvLine(received);
  }

  // Read from Serial and send to Serial1
  if (Serial.available()) {
    String inputString = "";
    while (Serial.available()) {
      char incomingByte = Serial.read();
      inputString += incomingByte;
    }
    serialTransmit(inputString);
  }
}



void sendCsvLine(String csvLine) {
  // Check wifi connection
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName);  // set destitnation for http request

    // set the post request content type
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // make the post request payload
    String postData = "csv_line=" + csvLine;

    // Send the post request
    int httpResponseCode = http.POST(postData);

    // check the response
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Response from server: " + response);
    } else {
      Serial.println("Error on sending POST: " + String(httpResponseCode));
    }

    // close the connection
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}

