#include <SoftwareSerial.h>
#include <Ultrasonic.h>
#include <ESP8266WiFi.h>

// defines pins numbers
#define pino_trigger 2 //D4
#define pino_echo 0    //D3

/* WiFi credenciais */
char ssid[] = "NV_FROES";
char pass[] = "<SENHA>";

WiFiClient client;

/* Thinkspeak*/
String apiKey = "HHFJCWCMJL2TTL3R";                                
const char* server = "api.thingspeak.com";

//Inicializa o sensor nos pinos definidos acima
Ultrasonic ultrasonic(pino_trigger, pino_echo);

void setup() {
  Serial.begin(9600);
  Serial.println("Lendo dados do sensor...");
  
  //Conectando ao Wi-Fi
  connectWifi();
}

void loop() {
  //Le as informacoes do sensor, em cm e pol  
  float cmMsec, inMsec;
  long microsec = ultrasonic.timing();
  cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
  inMsec = ultrasonic.convert(microsec, Ultrasonic::IN);
  //Exibe informacoes no serial monitor
  Serial.print("Distancia em cm: ");
  Serial.print(cmMsec);
  Serial.print(" - Distancia em polegadas: ");
  Serial.println(inMsec);
  
  String data = String(cmMsec);
  Serial.println(data);
  sendDataTS(cmMsec);
  
  delay(2000);
}

/***************************************************
 * Conectando a rede Wi-Fi
 **************************************************/
void connectWifi(){
  Serial.print("Connecting to "+ *ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Conectado na rede ");
  Serial.println(ssid);
  Serial.print("IP: ");
  Serial.println(WiFi.localIP()); 
}

/***************************************************
 * Enviando os Dados do Sensor Ultrasonico  para o ThingSpeak
 **************************************************/
void sendDataTS(float cmMsec){  
   if (client.connect(server, 80)) { 
     Serial.println("Enviando dados para o ThingSpeak ");
     String postStr = apiKey;
     postStr += "&field1=";
     postStr += String(cmMsec);
     postStr += "\r\n\r\n";
   
     client.print("POST /update HTTP/1.1\n");
     client.print("Host: api.thingspeak.com\n");
     client.print("Connection: close\n");
     client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
     client.print("Content-Type: application/x-www-form-urlencoded\n");
     client.print("Content-Length: ");
     client.print(postStr.length());
     client.print("\n\n");
     client.print(postStr);
     delay(1000); 
   }
   client.stop();
}
