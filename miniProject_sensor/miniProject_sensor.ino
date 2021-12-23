#include <FirebaseESP8266.h>
#include <TridentTD_LineNotify.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>  // ดึงเวลาปัจจุบันจาก internet (epoch time)
#include <TimeLib.h>    // เพื่อ convert epoch time เป็นตัวเลขที่เข้าใจได้
#include <WiFiUdp.h>

WiFiUDP ntpUDP;
// 3600*7 เพราะประเทศไทย GMT+7 ชั่วโมง
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600 * 7);
unsigned long epochTime = 0;

//#define WIFI_SSID "virus"
//#define WIFI_PASSWORD "11111111"

#define WIFI_SSID "Phanthila_2.4G"
#define WIFI_PASSWORD "15062543"

#define FIREBASE_HOST "tranquil-rite-321405-default-rtdb.firebaseio.com"
#define FIREBASE_KEY "oJxQ6DVfBcXLOwknyzXr8Ku91nJcMIw8KIN8dvEz"
#define LINE_TOKEN  "vO4lyRGeL9tXsvS3RypyiXQeSLulEqYef9G8ZzXEpip"

#define TRIGGER_PIN  D1
#define ECHO_PIN     D2

#define LED_Temp D5 //เหลือง
#define LED_Bpm  D6 //แดง

#define Delay 5000
unsigned long time_1 = 0;
int bpm;

int potpin = A0;
double temp;


FirebaseData firebaseData;


void setup() {
  connectWifi();
  LINE.setToken(LINE_TOKEN);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUILTIN_LED, OUTPUT);

  pinMode(LED_Temp,OUTPUT);
  pinMode(LED_Bpm,OUTPUT);
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_KEY);
  timeClient.begin();
  
  LINE.notifySticker(6370, 11088021); //Sticker HELLO
  LINE.notify("สถานะระบบ : เริ่มทำงาน !! ");
}

void loop() {
  timeClient.update();
  epochTime = timeClient.getEpochTime();
  double distance;
  if (millis() - time_1 > Delay) {
    time_1 = millis();

    distance = get_distance();

    Serial.print("Bpm = ");
    bpm = random(58, 102);
    Serial.println (bpm);

    temp = analogRead(potpin);
    temp = map(temp, 0, 1023, 350, 400);
    Serial.print("temp = ");
    temp = temp / 10 ;
    Serial.println(temp);

    check_distance(distance);
    check_bpm();
    check_temp();

    String dateTime = String(day(epochTime)) + "/" + String(month(epochTime)) + "/" + String(year(epochTime)) + " " + String(timeClient.getFormattedTime())  ;

    FirebaseJson data;
    data.set("Distance", distance);
    data.set("BPM", bpm);
    data.set("Temp", temp);
    data.set("DateTime", dateTime);

    if (Firebase.pushJSON(firebaseData, "/patient", data)) {
      //      Serial.println("Added");
      Serial.println("Pushed : " + firebaseData.pushName());
    } else {
      Serial.println("Error : " + firebaseData.errorReason());
    }
    Serial.println("-------------------------------------------------");
  }
}

bool put_item = true;
void check_distance(double value) {
  if (value >= 5 && put_item == true) {
    Serial.print("ผู้ป่วยถอดสายรัดข้อมือออก");
    LINE.notifySticker(446, 2011); //Sticker ตกใจ
    LINE.notify("ผู้ป่วยถอดสายรัดข้อมือออก");
    put_item = false;
  } else if (value < 5 && put_item == false) {
    Serial.print("ผู้ป่วยส่วมสายรัดข้อมืออยู่");
    LINE.notifySticker(446, 1993); //Sticker ดีใจ
    LINE.notify("ผู้ป่วยส่วมสายรัดข้อมืออยู่");
    put_item = true;
  }
}

double old_temp ;
bool temp_change = true;
bool temp_normal = false;

void check_temp() {
  if (temp >= 37.80) {
    if (temp_change == true) {
      old_temp = temp;
      String text = "แจ้งเตือน!! ผู้ป่วยมีอุณหภูมิสูงกว่าปกติ ผลการวินิจฉัย : มีไข้ขึ้นสูง อุณหภูมิ : " + String(temp) + " องศา";
      LINE.notifySticker(446, 2011);
      LINE.notify(text);
      Serial.println(text);
      digitalWrite(LED_Temp,HIGH);

      temp_change = false ;
      temp_normal = false ;
    }

    if (old_temp != temp) {
      temp_change = true;
    }

  } else {
    if (temp_normal == false) {
      String text = "แจ้งเตือน!! ผู้ป่วยมีอุณหภูมิปกติ อุณหภูมิ : " + String(temp) + " องศา";
      LINE.notifySticker(446, 1993);
      LINE.notify(text);
      Serial.println(text);
      digitalWrite(LED_Temp,LOW);

      temp_normal = true ;
    }


  }
}

bool bpm_normal = true ;
void check_bpm() {
  if (bpm >= 60 && bpm <= 100 && bpm_normal == false) {
    String text = "แจ้งเตือน!! status(BPM) : ปกติ [" + String(bpm) + "]" ;
    LINE.notifySticker(446, 1993); //Sticker ดีใจ
    LINE.notify(text);
    Serial.println (text);
    digitalWrite(LED_Bpm,LOW);
    bpm_normal = true ;
  } else if (bpm > 100 && bpm_normal == true) {
    String text = "อันตราย!! status(BPM) : สูงกว่าปกติ [" + String(bpm) + "] วินิจฉัย : เสี่ยงที่จะเกิดโรคหัวใจและหลอดเลือด" ;
    LINE.notifySticker(446, 2011); //Sticker ตกใจ
    LINE.notify(text);
    Serial.println (text);
    digitalWrite(LED_Bpm,HIGH);
    bpm_normal = false ;
  } else if (bpm < 60 && bpm_normal == true) {
    String text = "อันตราย!! status(BPM) : ต่ำกว่าปกติ [" + String(bpm) + "] วินิจฉัย : หน้ามืด หมดสติ ความดันโลหิตต่ำ" ;
    LINE.notifySticker(446, 2011); //Sticker ตกใจ
    LINE.notify(text);
    Serial.println (text);
    digitalWrite(LED_Bpm,HIGH);
    bpm_normal = false ;
  }
}

double get_distance() {
  double duration, distance;
  digitalWrite(TRIGGER_PIN, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(TRIGGER_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = (duration / 2) / 29.1;
  Serial.print("distance : ");
  Serial.print(distance);
  Serial.println(" cm");
  return distance ;
}

void connectWifi() {
  Serial.begin(115200);
  Serial.println(WiFi.localIP());
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
}
