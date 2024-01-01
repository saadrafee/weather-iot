

#include <Wire.h>
#include <PubSubClient.h>
#include <HTTPClient.h>
#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT11 // DHT 11
DHT dht(DHTPIN, DHTTYPE);
#define wifi_ssid "Enter hotspot name"
#define wifi_password "Enter hotspot password"
#define mqtt_server "test.mosquitto.org"
#define hum_topic "Enter_humidity_topic"
#define temp_topic "Enter_temperatuer_topic"

#define fire_topic "fire_topic"
#define gas_topic "Gas_topic"

#define rain_topic "rain_topic"
#define ldr_topic "ldr_topic"

const char *apiKey = "Enter your api key of think space";
const char *thingSpeakAddress = "api.thingspeak.com";

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(9600);
  setupWiFi();
  setupMQTT();
}

void setupWiFi() {
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    attempts++;
    if (attempts > 20) {
      Serial.println("\nFailed to connect to WiFi. Check your credentials.");
      return;
    }
  }

  Serial.println("");

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setupMQTT() {
  client.setServer(mqtt_server, 1883);
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    if (client.connect("ESP8266Client")) {
      Serial.println("connected");

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  bool isRain = false;
  bool isFire = false;
  bool isGas = false;
  bool isldr = false;
float h = dht.readHumidity();
float t = dht.readTemperature();
  Serial.println(t);
char receivedChar;
Serial.println(h);

char humidity[7];  // Assuming a reasonable buffer size

   Serial.println(client.publish(hum_topic,String(h).c_str()));
delay(1000);
for (int i =0 ;i<200;i++){
  if (Serial.available()>0)
  {
      receivedChar=Serial.read();
      Serial.println(receivedChar);
    if (receivedChar == 'R')
    {isRain = true;

      Serial.println("Raining");
      
      }
     
    if (receivedChar == 'L')
    {
      isldr = true;
      Serial.println("LED: ON");
      Serial.println(client.publish(ldr_topic,"Light ON"));
      }
    if (receivedChar == 'F')
    {
      Serial.println(client.publish(fire_topic,"Alert!! Fire"));
   isFire = true;
  
    }
    if (receivedChar == 'G')
    {Serial.println(client.publish(gas_topic,"Alert!! Gas Leak"));
      isGas = true;
    }
     
     
  }
}

sendDataToThingSpeak(t, h,isRain,isFire ,isGas,isldr);
  client.publish(temp_topic, String(t).c_str());

  delay(1000);
  Serial.println("SEEEEEEE");

  
}
void publishCheck(float t)
{
  Serial.println("publish hum");
Serial.println(client.publish(hum_topic,String(t).c_str()));
}
void publishTemp(float t)
{Serial.println("publish remp");
  client.publish(temp_topic, String(t).c_str());

}
void sendDataToThingSpeak(float data1, float data2,bool rain,bool fire ,bool gas,bool isladr) {

 int ra= rain ? 1:0;
 int fi= fire ? 1:0;
 int gs= gas ? 1:0;
int ladr = isladr ? 1:0;
  String url = "/update?api_key=" + String(apiKey) +
               "&field1=" + String(data1) +
               "&field2=" + String(data2) +
               "&field3=" + String(ra) +
               "&field4=" + String(fi) +
               "&field5=" + String(gs) +
               "&field6=" + String(ladr) 
               ;

  HTTPClient http;
  http.begin(thingSpeakAddress, 80, url);

  int httpResponseCode = http.GET();
  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
  } else {
    Serial.print("Error in HTTP request. Code: ");
    Serial.println(httpResponseCode);
  }

  http.end();
}

