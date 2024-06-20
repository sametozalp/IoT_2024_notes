// shiftr.io sitesindeki projemizden veri alma
// alınan veriyi firebase e yazma, firebaseden ayarlanan veriyi okuyup
// alınan veri ile ayarlanan veriyi karşılaştırıp led yakma
#include <ESP8266WiFi.h>
#include <MQTTClient.h>
#include <FirebaseArduino.h>   

const char *ssid = "xxxxxxxxxxxx";                 // bağlanılacak olan kablosuz ağın SSID si
const char *password = "xxxxxxx";                  // bağlanılacak olan kablosuz ağın şifresi

#define FIREBASE_HOST "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"     // veritabanının linki
#define FIREBASE_AUTH "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"  // veritabanı secret key

WiFiClient wifi_istemci;
MQTTClient mqtt_istemci;
//*********************************************************************
void setup() 
{
  pinMode(D0,OUTPUT);                             // D0 pinini çıkış yap, LED bağlı
  
  Serial.begin(9600);                             //Seri iletişim hızı 9600 bps olarak ayarlanarak başlatılıyor.
  delay(1000);
  Serial.println("Wifi agina baglaniliyor");

  WiFi.mode(WIFI_STA);                           //ESP8266 istasyon moduna alınıyor. 
  WiFi.begin(ssid, password);                    //SSID ve şifre girilierek wifi başlatılıyor.

  //------- Wifi ağına bağlanıncaya kadar beklenilen kısım ------------
  while (WiFi.status() != WL_CONNECTED) 
  { 
    delay(500);                                 // Bu arada her 500 ms de bir seri ekrana yan yana noktalar yazdırılarak
    Serial.print(".");                          //görsellik sağlanıyor.
  }
  //-------------------------------------------------------------------

  Serial.println("");                           //Bir alt satıra geçiliyor.
  Serial.println("Bağlantı sağlandı...");       //seri ekrana bağlantının kurulduğu bilgisi gönderiliyor.

  Serial.print("Alınan IP addresi: ");          // kablosuz ağdan alınan IP adresi
  Serial.println(WiFi.localIP());               // kablosuz ağdan alınan IP adresi
 
  mqtt_istemci.begin("xxxxxxxx.cloud.shiftr.io", wifi_istemci); // site üzerindeki projeye linkine bağlan
  mqtt_istemci.onMessage(mesaj_geldi);			// mesaj gelince mesaj_geldi fonksiyonunu çagır
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); 
}
//*********************************************************************
void loop() 
{
  mqtt_istemci.loop();							// broker a veri gönderebilmek ve gelen veriyi okuyabilmek için
  if (!mqtt_istemci.connected()) 
    baglan();

  delay(1000);
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
  
  mqtt_istemci.subscribe("sicaklik/kabin");		//sicaklik/kabin konusuna abone ol
}
//*********************************************************************
void mesaj_geldi(String &konu, String &veri)		// gelen veriyi oku
{
  Serial.println(konu + ": "+ veri);

 // ogrenci nosu etiket olacak şekilde veriyi flaot olarak yaz
 Firebase.setFloat("ogrenci no", veri.toFloat());
  // handle error
  if (Firebase.failed()) {
      Serial.print("yazdirma hatasi:");
      Serial.println(Firebase.error());  
      return;
  }

  // ayarlanan etiketi altındaki veriyi oku, Float a cevir
  float ayarlanan= (Firebase.getString("ayarlanan")).toFloat();
 
  if(ayarlanan<veri.toFloat())
    digitalWrite(D0, HIGH);
  else
   digitalWrite(D0, LOW);
    
}
