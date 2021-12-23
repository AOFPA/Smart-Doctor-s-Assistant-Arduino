#define Delay 1000
unsigned long time_1 = 0;
long bpm;

void setup() {
  Serial.begin(115200);

}

void loop() {
  /*
    <60 เกิดอาการหน้ามืด หมดสติ มีความเสี่ยงที่จะเป็นโรคความดันโลหิตต่ำได้
    60-100 ครั้ง = ปกติ
    > 100 หัวใจเต้นเร็วกว่าปกติ เสี่ยงที่จะเกิดโรคหัวใจและหลอดเลือด
  */
  // พิมพ์หมายเลขสุ่มจาก 40 ถึง 120
  if (millis() - time_1 > Delay) {
    time_1 = millis();
    Serial.print("Bpm = ");
    bpm = random(40, 120);
    Serial.println (bpm);
    if (bpm >= 60 && bpm <= 100) {
      Serial.print ("status(BPM) : ปกติ [");
      Serial.print (bpm);
      Serial.println ("]");
    } else if (bpm > 100) {
      Serial.print ("status(BPM) : สูงกว่าปกติ [");
      Serial.print (bpm);
      Serial.println ("] วินิจฉัย : เสี่ยงที่จะเกิดโรคหัวใจและหลอดเลือด");
    } else {
      Serial.print ("status(BPM) : ต่ำกว่าปกติ [");
      Serial.print (bpm);
      Serial.println ("] วินิจฉัย : หน้ามืด หมดสติ ความดันโลหิตต่ำ");
    }
  }

}
