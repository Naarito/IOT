#define LED_BUILTIN 2

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
  Serial.begin(115200);
  configuraDisplay();
}


// ---> LOOP <---

void loop() {
  medeTempUmid();
  processaTempUmid();
}

// ---> FUNÇÕES <---

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
  exibeTempUmid("Temperatura:", (temperatura), " C");
  exibeTempUmid("Umidade:", (umidade), " %");
}


void exibeTempUmid(const char* texto1, int medicao, const char* texto2) {
  display.clearDisplay(); //Limpa
  display.setTextSize(1); //Tamanho da Fonte
  display.setCursor(0,0); //Deslocamento
  display.print(texto1); //Conteúdo 

  display.setTextSize(5);
  display.setCursor(20,20);
  display.print(medicao);

  display.setTextSize(2);
  display.print(texto2);

  display.display(); //Imprime
  delay(2000);
}

