#include <ESP8266WiFi.h>
const char* ssid = "Nasc A71";
const char* password = "nasc1234";

const char* host = "fatec-linhas-itapira.loca.lt";

String lat = "1";
String lng = "1";

void setup() {
  Serial.begin(9600);
  delay(100);

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  delay(5000);

  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  String data = "{\"linhaId\":\"1\",\"latitude\":\"" + lat + "\",\"longitude\":\"" + lng + "\"}";

  Serial.print("Requesting POST: ");
  // Send request to the server:
  client.println("POST / HTTP/1.1");
  client.println("Host: fatec-linhas-itapira.loca.lt");
  client.println("Accept: */*");
  client.println("Content-Type: application/json");
  client.print("Content-Length: ");
  client.println(data.length());
  client.println();
  client.print(data);

  delay(500);  // Can be changed
  if (client.connected()) {
    //if (client.available()) {
      String line = client.readStringUntil('\n');
      Serial.print(line);
    //}
    client.stop();  // DISCONNECT FROM THE SERVER
  }
  Serial.println();
  Serial.println("closing connection");
  delay(5000);
}