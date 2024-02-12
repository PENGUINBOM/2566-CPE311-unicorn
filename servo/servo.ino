#include <Servo.h>

Servo moton;
int inputPin = 8;      // กำหนดหมายเลขของ pin ที่เชื่อมกับ STM32L152RB
int val = 0;           // ตัวแปรสำหรับเก็บค่าที่ได้อ่านจาก pin
int direction = HIGH;  // ตัวแปรสำหรับกำหนดทิศทางการหมุนของมอเตอร์

void setup() {
  moton.attach(9);
  pinMode(inputPin, INPUT);    // กำหนดให้ pin ที่ต่อกับ STM32L152RB เป็นขาในโหมดอินพุต
}

    void loop() {
         val = digitalRead(inputPin); // อ่านค่าจาก pin ที่ต่อกับ STM32L152RB
         if (val == HIGH){            // ถ้าได้อ่านค่า High
            moton.write(150);          // หมุนมอเตอร์
            delay(500);
            direction = !direction;    
         }
         if (direction == HIGH){      // ปรับทิศทางการหมุน
             moton.write(20);
             delay(500);
         }                      
         else{                        // ถ้าได้อ่านค่า Low
             moton.write(0);            // หยุดหมุนมอเตอร์
        }
 }
