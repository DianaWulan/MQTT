//library
#include "AdafruitIO_WiFi.h"

//pengaturan adafruit.io
#define IO_USERNAME "dianawulandari" //username pada web adafruit io
#define IO_KEY "aio_qlhx37WdJq8JPtBlRvrm8W9EeOXt" //key dashboard

//wifi
#define WIFI_SSID "Bunga"
#define WIFI_PASS "Bunga12"

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);

//pin
#define SOIL_PIN A0
#define LED_PIN 5

int current = 0; //variable penampung data soil
int last = 0; //-1
int led=0;

//pengaturan feed dashboard
AdafruitIO_Feed *soil = io.feed("Soilpercent");
AdafruitIO_Feed *light = io.feed("testcontrol");

//menerima perintah kontrol led dari dashboard
void handleMessage(AdafruitIO_Data *data) {
 //menampilkan ke layar status tombol yang ditekan pada feed yang baru saja dibaca
  Serial.print("received <- ");
 
  if(data->toPinLevel() == HIGH)
    Serial.println("HIGH");
  else
    Serial.println("LOW");

  //mengatur status LED on/off pada board sesuai dengan tombol yang ditekan
  // tulis keadaan saat ini ke led
  digitalWrite(LED_PIN, data->toPinLevel());
}

//setup
void setup() {
  Serial.begin(115200);

  while(! Serial);
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  pinMode(LED_PIN, OUTPUT);

  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println(io.statusText());

  //membaca perintah dari dashboard > kontrol led
  light->get(); 
  light->onMessage(handleMessage);
}

//loop
void loop() 
{
  io.run();
  //current menampung data pembacaan sensor soil moisture
  current = analogRead(SOIL_PIN);
  //led menampung perintah pada led
  led = digitalRead(5);

  //jika data yg terbaca pada soil = -1, maka kembalikan
  if(current == last)
    return;

  Serial.print("sending -> ");
  Serial.println(current);
  Serial.print("LED STATE -> ");
  Serial.println(led);
  soil->save(current);
  light->save(led);//send data
  last = current; //memulai nilai data current dari awal kembali / 0
  delay(4000);
}
