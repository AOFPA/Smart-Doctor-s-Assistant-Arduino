
int potpin = A0;
float temp;
void setup() {
  Serial.begin(115200);
}

void loop() {
  // >37.80 มีไข้
  temp = analogRead(potpin);         // อ่านค่า Analog ขาA0 เก็บไว้ในตัวแปร temp
  temp = map(temp, 0, 1023, 350, 400);  // แปลงค่า ระหว่าง 0-1023 เป็น 0-180
  Serial.print("temp = ");           // พิมพ์ข้อมความส่งเข้าคอมพิวเตอร์ "temp = "
  Serial.println(temp / 10);            // พิมพ์ค่าของตัวแปร temp
  delay(100);                        // Delay 15mS
}
