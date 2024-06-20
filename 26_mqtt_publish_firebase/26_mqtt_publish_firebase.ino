// shiftr.io sitesindeki projemize veri gönderme
// sıcaklığı olcup olculen degeri stringe cevirip gönderme
#include <ESP8266WiFi.h>
#include <MQTTClient.h>

int ADC_okunan=0;           // adc'den okunana ham değer
float sensor_gerilim=0.0;   // sensorden okunan degerin gerilim karsiligi mV cinsinden
float sicaklik=0.0;         // okunan sıcaklık değeri

const char *ssid = "xxxxxxxxxxxxx";                 // bağlanılacak olan kablosuz ağın SSID si
const char *password = "xxxxxxxx";                  // bağlanılacak olan kablosuz ağın şifresi

WiFiClient wifi_istemci;
MQTTClient mqtt_istemci;
//------------------------------------------------------------------------------------
void setup() 
{
  Serial.begin(9600);                             //Seri iletişim hızı 9600 bps olarak ayarlanarak başlatılıyor.
  delay(1000);
  Serial.println("Wifi agina baglaniliyor");

  WiFi.mode(WIFI_STA);                           //ESP8266 istasyon moduna alınıyor. 
  WiFi.begin(ssid, password);                    //SSID ve şifre girilierek wifi başlatılıyor.

  //------- Wifi ağına bağlanıncaya kadar beklenilen kısım ------------
  while (WiFi.status() != WL_CONNECTED) 
  { 
    delay(500);                                 // Bu arada her 500 ms de bir seri ekrana yan yana noktalar yazdırılarak
    Serial.print(".");                          // görsellik sağlanıyor.
  }
  //-------------------------------------------------------------------

  Serial.println("");                           //Bir alt satıra geçiliyor.
  Serial.println("Bağlantı sağlandı...");       //seri ekrana bağlantının kurulduğu bilgisi gönderiliyor.

  Serial.print("Alınan IP addresi: ");          // kablosuz ağdan alınan IP adresi
  Serial.println(WiFi.localIP());               // kablosuz ağdan alınan IP adresi
 
  mqtt_istemci.begin("xxxxx.cloud.shiftr.io", wifi_istemci); // site üzerindeki projeye linkine bağlan
}
//------------------------------------------------------------------------------------
void loop() 
{
  mqtt_istemci.loop();							    // broker a veri gönderebilmek ve gelen veriyi okuyabilmek için
  
  if (!mqtt_istemci.connected()) 				// broker a bağlanmadı mı?
    baglan();									          // bağlı değil baglan fonkisyonuna git
  
  sicaklik_olc();                       // sıcaklıgı ölc
  
  mqtt_istemci.publish("tttt",String(sicaklik));	// ttt konusuna olculen sıcaklık degerini stringe cevir gönder
  delay(3000);										// 3sn bekle
}
//*********************************************************************
void baglan()
{
  // baglanana kadar bekle ekrana . yazdır
  while (!mqtt_istemci.connect("xxx", "yyy", "zzz")) // xxx=sizin client adınız, yyy=proje adı, zzz= token
  {
    Serial.print(".");
    delay(1000);
  }
}
//*********************************************************************
void sicaklik_olc()
{
  ADC_okunan=analogRead(A0);              // ADC yi oku, A0 girişine bağlı LM35
  sensor_gerilim=(ADC_okunan*1000.0)/1023.0; //bu kismin açıklaması en alttadır
  sicaklik=sensor_gerilim/10.0;  
}
