#include <ESP8266WiFi.h>
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>

//const char* ssid = "Nasc A71";
//const char* password = "nasc1234";
const char* ssid = "Nasc A71";
const char* password = "nasc1234";

const char* host = "fatec-linhas-itapira.loca.lt";

static const int RXPin = 4, TXPin = 5;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

SoftwareSerial ss(RXPin, TXPin);

String lat = "";
String lng = "";

unsigned long lastTime = 0;
unsigned long timerDelay = 10000;

void setup() {
  Serial.begin(GPSBaud);
  Serial.print("Iniciando GPS");
  ss.begin(GPSBaud);
  Serial.print("GPS Iniciado");
  //delay(100);

  // We start by connecting to a WiFi network
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  while (ss.available() > 0) {
    gps.encode(ss.read());
    if (gps.location.isValid()) {
      lat = String(gps.location.lat(), 6);
      lng = String(gps.location.lng(), 6);
    }
  }

  if ((millis() - lastTime) > timerDelay) {
    Serial.println("lat: " + lat);
    Serial.println("lng: " + lng);

    //return;

    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
      Serial.println("connection failed");
      return;
    }

    if (lat.isEmpty() && lng.isEmpty()) {
      Serial.println("falha na leitura do gps");
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
    //delay(10000);
    lastTime = millis();
  }
}