#define LED_BUILTIN 2

// ---> WIFI <---

#include <ESP8266WiFi.h>

const char* ssid = "net virtuia apt 44"; //Rede WIFI
const char* password = "Sandira13"; //Senhra

WiFiClient nodemcuClient;

// ---> MQTT <---

#include <PubSubClient.h>

const char* mqtt_broker = "192.168.0.7";
const char* mqtt_ClientID = "DispositivoTU";

PubSubClient client(nodemcuClient);

const char*topicoTemp = "dispositivos/temperatura";
const char*topicoUmid = "dispositivos/umidade";

// ---> DHT <---

#include <DHT.h>

#define DHTPIN D3
#define DHTTYPE DHT22//Seleciona o sensor tipo 22

DHT dht(DHTPIN,DHTTYPE);

int umidade;
int temperatura;

// ---> OLED <---

#include <Adafruit_SSD1306.h>

#define OLED_RESET LED_BUILTIN
Adafruit_SSD1306 display(OLED_RESET);

// ---> SETUP <---

void setup() {
  configuraDisplay();
  conectaWifi();
  client.setServer(mqtt_broker, 1883);
}


// ---> LOOP <---

void loop() {

  if (!client.connected()) {
    reconMQTT();
  }
  
  medeTempUmid();
  processaTempUmid();
  publicaTempUmidMQTT();
}

// ---> FUNÇÕES PERIFÉRICOS <---

void configuraDisplay() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //Obtido com o I2C scanner
  display.setTextColor(WHITE); //Monocromático
  display.clearDisplay(); 
}

void medeTempUmid() {
  umidade = dht.readHumidity();
  temperatura = dht.readTemperature(false); //Habilita a utilização em Graus Celsius
  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.print(" *C");
  Serial.print ("    ");
  Serial.print("Umidade: ");
  Serial.println(umidade);
  delay(2000);
}

void processaTempUmid () {
  exibeTempUmid("Temperatura:", (temperatura), " C", "Umidade:", (umidade), " %");
}


void exibeTempUmid(const char* texto1A, int medicaoA, const char* texto2A, const char* texto1B, int medicaoB, const char* texto2B) {
  //TEMPERATURA
  display.clearDisplay(); //Limpa
  display.setTextSize(1); //Tamanho da Fonte
  display.setCursor(0,0); //Deslocamento
  display.print(texto1A); //Conteúdo 

  display.setTextSize(2);
  display.setCursor(10,12);
  display.print(medicaoA);

  display.setTextSize(1);
  display.print(texto2A);

  //UMIDADE
  display.setTextSize(1); 
  display.setCursor(0,32); 
  display.print(texto1B);  

  display.setTextSize(2);
  display.setCursor(10,45);
  display.print(medicaoB);

  display.setTextSize(1);
  display.print(texto2B);

  display.display(); //Imprime
  delay(2000);
}

// ---> FUNÇÕES REDE <---

void conectaWifi(){
  delay(10);

  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0,0);
  display.print("Conectando");
  display.display();

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    display.print(".");
    display.display();
  }
  
}

void reconMQTT(){
   while (!client.connected()) {
    client.connect(mqtt_ClientID);
   }
}

// ---> FUNÇÕES PUBLISH <---

void publicaTempUmidMQTT() {
  client.publish(topicoTemp, String(temperatura).c_str(), true); //true retém o dado publicado até que seja enviado o próximo
  client.publish(topicoUmid, String(umidade).c_str(), true);
  delay(300000);
}

