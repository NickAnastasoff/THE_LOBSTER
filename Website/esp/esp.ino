//widen.wrought.shrunk.darkening
// run with php -S 0.0.0.0:8000

#include <WiFi.h>
#include <HTTPClient.h>

// Replace with your network credentials
const char* ssid = "TMOBILE-23E1";
const char* password = "widen.wrought.shrunk.darkening";

// Replace with the IP address or domain of your PHP server
// Ensure to replace 'localhost' with the IP address of the server if running it locally
const char* serverName = "http://192.168.12.204:8000/data_receiver.php";  // Replace with your actual server IP

void setup() {
  Serial.begin(9600);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Send a predefined CSV line to the server
  sendCsvLine("2024-10-07 21:00:00,25.8,8.0,33.8,7,174.0");
}

void sendCsvLine(String csvLine) {
  // Check Wi-Fi connection
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName);  // Specify the destination for the HTTP request

    // Set the POST request content type
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Prepare the POST request payload
    String postData = "csv_line=" + csvLine;

    // Send the POST request
    int httpResponseCode = http.POST(postData);

    // Check the response
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Response from server: " + response);
    } else {
      Serial.println("Error on sending POST: " + String(httpResponseCode));
    }

    // Close the connection
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}

void loop() {
  // No need for repeated actions in the loop in this example
  sendCsvLine("2024-10-07 21:00:00,25.8,8.0,33.8,7,174.0");
}
