
/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */

#include <ESP8266WiFi.h>


const char* ssid     = "KDA";
const char* password = "mniKiriNeruGonta!";

const char* host = "192.168.8.100";
String id   = "1";
String location = "home";

#define DHT11_PIN 14
#define pir_Sensor 12
#define motor 13

int pir_Sensor_Read;
int temp_Sensor;

void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(motor,OUTPUT);
  pinMode(pir_Sensor,INPUT);
  pinMode(DHT11_PIN,INPUT);
  pinMode(5,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(2,OUTPUT);
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

int value = 0;

void loop() {
  delay(5000);
  ++value;
  
  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 8084;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URI for the request
  String url = "/Lumos/ServletURLPattern?location="+location+"&id="+id;
  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host+":"+httpPort + "\r\n" +
               "Accept:*/*\r\n"+
               "Connection: Keep-Alive\r\n\r\n");
  unsigned long timeout = millis();
 Serial.println(client.available());
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  String line;
  int count = 0;
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    line = client.readStringUntil('\r');
    
    Serial.print(line);
if(count == 5){
  Serial.println();
    String autoMode = getValue(line, ',', 1);
    String z = getValue(line, ',', 2);
    String r = getValue(line, ',', 3);
    String g = getValue(line, ',', 4);
    String b = getValue(line, ',', 5);
    
    int speeds = strToInt(z);
    int red = strToInt(r);
    int green = strToInt(g);
    int blue = strToInt(b);

    analogWrite(5,red);
    analogWrite(4,green);
    analogWrite(2,blue);
    
    pir_Sensor_Read = digitalRead(pir_Sensor);
    Serial.println(pir_Sensor_Read);
    //temp_Sensor = DHT.read11(DHT11_PIN);
    //int temp = DHT.temperature;
    //int humidity = DHT.humidity;
    if(pir_Sensor_Read){
    analogWrite(motor,speeds);
    }else{
      analogWrite(motor,0);
      }
    //Serial.println(temp);
    //Serial.println(humidity);
  //  Serial.println(yvalue);
    //Serial.println(zvalue);

}
  count++;
  }
  
  Serial.println("closing connection");
}




int strToInt(String number) {
  int out = 0;

  for(int i = 0; i < number.length(); i++) {
    out = out * 10 + (number.charAt(i) - '0');
  }

  return out;
}

String getValue(String data, char separator, int index) {
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

