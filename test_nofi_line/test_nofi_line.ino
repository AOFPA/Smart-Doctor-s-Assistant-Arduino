//vO4lyRGeL9tXsvS3RypyiXQeSLulEqYef9G8ZzXEpip
#include <TridentTD_LineNotify.h>
#include <ESP8266WiFi.h>

#define SSID        "virus"      // บรรทัดที่ 11 ให้ใส่ ชื่อ Wifi ที่จะเชื่อมต่อ
#define PASSWORD    "11111111"     // บรรทัดที่ 12 ใส่ รหัส Wifi
#define LINE_TOKEN  "vO4lyRGeL9tXsvS3RypyiXQeSLulEqYef9G8ZzXEpip"   // บรรทัดที่ 13 ใส่ รหัส TOKEN ที่ได้มาจากข้างบน

void setup() {
  Serial.begin(115200); Serial.println();
  Serial.println(LINE.getVersion());

  WiFi.begin(SSID, PASSWORD);
  Serial.printf("WiFi connecting to %s\n",  SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(400);
  }
  Serial.printf("\nWiFi connected\nIP : ");
  Serial.println(WiFi.localIP());

  // กำหนด Line Token
  LINE.setToken(LINE_TOKEN);

  // ตัวอย่างส่งข้อความ
  LINE.notify("อุณหภูมิ เกินกำหนด");
  LINE.notify("myarduino.net");
  // ตัวอย่างส่งข้อมูล ตัวเลข
  LINE.notify(2342);          // จำนวนเต็ม
  LINE.notify(212.43434, 5);  // จำนวนจริง แสดง 5 หลัก

  // เลือก Line Sticker ได้จาก https://devdocs.line.me/files/sticker_list.pdf
  LINE.notifySticker(3, 240);       // ส่ง Line Sticker ด้วย PackageID 3 , StickerID 240
  LINE.notifySticker("Hello", 1, 2); // ส่ง Line Sticker ด้วย PackageID 1 , StickerID 2  พร้อมข้อความ

  // ตัวอย่างส่ง รูปภาพ ด้วย url
  LINE.notifyPicture("https://preview.ibb.co/j6G51n/capture25610417181915334.png");
  LINE.notifyPicture("จตุธาตุ", "https://www.fotoaparat.cz/storage/pm/09/10/23/670915_a5351.jpg");
}

void loop() {
  delay(1);
}
