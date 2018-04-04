#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN D3
#define DHTTYPE DHT22//Seleciona o sensor tipo 22

DHT dht(DHTPIN,DHTTYPE);

void setup() {
  Serial.begin(115200);

}

void loop() {
  int umidade = dht.readHumidity();
  int temperatura = dht.readTemperature(false); //Habilita a utilização em Graus Celsius
  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.print(" *C");
  Serial.print ("    ");
  Serial.print("Umidade: ");
  Serial.println(umidade);
  delay(2000);
}
