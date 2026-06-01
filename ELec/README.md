

---
AI ออกแบบทั้งหมด
---

<div align="center">

# ⚡ RvA Calculator

Voltage Drop / Ohm Law Tool

<br>

<a href="https://xkp95175333.github.io/All_fun_log/ELec/RvA.html">

<img
src="https://img.shields.io/badge/OPEN-LIVE_APP-00ff88?style=for-the-badge">

</a>

<br><br>

<a href="https://xkp95175333.github.io/All_fun_log/ELec/RvA.html">

<img
width="900"
src="https://dummyimage.com/1200x500/111111/00ff88&text=RvA+CALCULATOR">

</a>

</div>

---

<details>

<summary>📦 Open Preview</summary>

<br>

| Function | Support |
|---|---|
| Voltage Drop | ✅ |
| Ohm Law | ✅ |
| TH / EN | ✅ |
| Mobile UI | ✅ |
| GitHub Pages | ✅ |

<br>

Click image or button above to open calculator.

</details>

---


##🌐 Live Web

```txt
https://xkp95175333.github.io/All_fun_log/ELec/RvA.html
```
---

📁 Files

ELec/RvA.html
ELec/README.md


https://github.com/xkp95175333/All_fun_log/tree/xkp95175333/main_sell_hide/ELec



[![Open](https://img.shields.io/badge/Open-Web-green?style=for-the-badge)](https://github.com/xkp95175333/All_fun_log/blob/xkp95175333/main_sell_hide/ELec/index.html))


<div align="center">

# ⚡ RvA Calculator

<a href="https://xkp95175333.github.io/All_fun_log/ELec/RvA.html">

<img
width="900"
alt="RvA Calculator"
src="https://github.com/user-attachments/assets/961c3088-792f-40c2-a081-2a449a3b1849" />

</a>

<br><br>

<a href="https://xkp95175333.github.io/All_fun_log/ELec/RvA.html">

<img
src="https://img.shields.io/badge/OPEN-LIVE_APP-00ff88?style=for-the-badge">

</a>

</div>





https://github.com/xkp95175333/All_fun_log/blob/xkp95175333/main_sell_hide/ELec/index.html
---
สเต็ปอัพMini Dc
---
![1000009260](https://github.com/user-attachments/assets/2578167f-bec0-4191-98da-7b125155c38b)

![1000009261](https://github.com/user-attachments/assets/21090ac3-dc83-4141-aa52-f78f39ba533d)


<img width="1402" height="1122" alt="1000009259" src="https://github.com/user-attachments/assets/961c3088-792f-40c2-a081-2a449a3b1849" />

---
ชาร์จไร้สาย
---
![1000009262](https://github.com/user-attachments/assets/597c65da-1b3a-40b3-8c12-9c5c7c7994fe)

---
spot Welder
---

<img width="1536" height="1024" alt="1000009257" src="https://github.com/user-attachments/assets/865979e5-7dd2-4454-8a24-bee356d7ccfa" />
<img width="1402" height="1122" alt="1000009264" src="https://github.com/user-attachments/assets/2bfc09ff-835c-4779-9c67-8764f3169747" />

'Pc Power supply '
<img width="1536" height="1024" alt="1000009267" src="https://github.com/user-attachments/assets/11fc30ec-3351-4bf2-836c-f1ef6f837619" />
<img width="1331" height="1181" alt="1000009266" src="https://github.com/user-attachments/assets/a56347c0-b25f-48d9-8914-9b300cf327b0" />


```c
/*
 DIY SPOT WELDER
 Arduino UNO + IR2110 + MOSFET
 PSU 12V 800W

 D9  -> Gate Q1
 D10 -> Gate Q2
 D11 -> Gate Q3
 D12 -> Gate Q4

 D2 -> Fire Button
 A0 -> Potentiometer

 LCD I2C 16x2
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

#define Q1 9
#define Q2 10
#define Q3 11
#define Q4 12

#define FIRE_BTN 2
#define POT A0

int weldTime = 10;

void setup()
{
  pinMode(Q1, OUTPUT);
  pinMode(Q2, OUTPUT);
  pinMode(Q3, OUTPUT);
  pinMode(Q4, OUTPUT);

  pinMode(FIRE_BTN, INPUT_PULLUP);

  digitalWrite(Q1, LOW);
  digitalWrite(Q2, LOW);
  digitalWrite(Q3, LOW);
  digitalWrite(Q4, LOW);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("SPOT WELDER");
  delay(1000);
}

void loop()
{
  int pot = analogRead(POT);

  // ปรับเวลา 1-100ms
  weldTime = map(pot,0,1023,1,100);

  lcd.setCursor(0,0);
  lcd.print("TIME:");
  lcd.print(weldTime);
  lcd.print("ms   ");

  lcd.setCursor(0,1);
  lcd.print("READY        ");

  if(digitalRead(FIRE_BTN)==LOW)
  {
    firePulse();

    lcd.setCursor(0,1);
    lcd.print("WELDING...   ");

    delay(500);
  }
}

void firePulse()
{
  // เปิด MOSFET
  digitalWrite(Q1,HIGH);
  digitalWrite(Q2,HIGH);
  digitalWrite(Q3,HIGH);
  digitalWrite(Q4,HIGH);

  delay(weldTime);

  // ปิด MOSFET
  digitalWrite(Q1,LOW);
  digitalWrite(Q2,LOW);
  digitalWrite(Q3,LOW);
  digitalWrite(Q4,LOW);

  delay(100);
}

```

---
HW-586 Lib เสริม Unload BP
---

<img width="1402" height="1122" alt="1000009265" src="https://github.com/user-attachments/assets/5075c769-010a-40cd-9301-0a55bdf4e061" />




<img width="1536" height="1024" alt="1000009258" src="https://github.com/user-attachments/assets/a497bc15-48c6-4f0b-8bfb-cf5288f6b8dc" />



---
จุดด้วยหม้อแปลงไมโครเวฟ
---

<img width="720" height="1612" alt="1000009310" src="https://github.com/user-attachments/assets/e22e2824-995f-4c17-8ee4-f88651648063" />

