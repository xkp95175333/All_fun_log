
---
ออกแบบวงจรอิเล็กจากของที่ไม่ได้ใช้มาเป็นแหล่งจ่ายไฟ Full
---
<img width="4080" height="2296" alt="1000009076" src="https://github.com/user-attachments/assets/a6559633-6a0a-42ab-a90d-3e37ea05988d" />


<img width="720" height="1612" alt="1000009104" src="https://github.com/user-attachments/assets/44ff7a27-df05-4c39-aa78-ac8ca7f70d56" />




```c
ถ้าจะเปิด PSU PC ให้ทำงานแบบไม่ต่อเมนบอร์ด (เอาไว้ใช้เป็นแหล่งจ่ายไฟ DIY) ของ KCAS-800W มันใช้มาตรฐาน ATX ทั่วไป สีสายแทบเหมือนกันหมด ไม่ต้องไล่วัดทีละเส้นเลย 

วิธีสั่งให้ PSU ติด (สำคัญสุด)
เอา:

🟢 สีเขียว = PS_ON
ลัดกับ
⚫ สีดำ = GND / ลบ

แค่เอาลวดหรือคลิปหนีบกระดาษจั๊มระหว่าง เขียว ↔ ดำ ที่หัว 24 พิน แล้วเปิดสวิตช์หลัง PSU พัดลมควรหมุน 

สีสายหลัก:

⚫ ดำ = GND / ลบ / 0V
🟡 เหลือง = +12V ← ใช้เยอะสุด งานมอเตอร์ พัดลม เครื่องเชื่อม DIY
🔴 แดง = +5V
🟠 ส้ม = +3.3V
🟣 ม่วง = +5VSB (มีไฟตลอดแม้ยังไม่เปิด PSU)
🔵 น้ำเงิน = -12V
⚪ ขาว = -5V (รุ่นเก่าๆ มี บางรุ่นไม่มี)
🩶 เทา = Power Good
🟢 เขียว = PS_ON 

จากสเปก KCAS ที่แปะข้างตัว:

+3.3V / +5V / +12V / -12V / +5VSB
กระแสประมาณ 20A / 20A / 60A / 0.4A / 2.5A 

ถ้าจะเอา 12V แรงๆ ไปใช้:

บวก = 🟡 เหลือง

ลบ = ⚫ ดำ


และรวมหลายเส้นได้เลย เช่น เหลือง 3–5 เส้นรวมกัน / ดำ 3–5 เส้นรวมกัน เพราะสายเส้นเดียวรับกระแสไม่ไหวถ้าดึงหนัก

อีกอย่างเตือนนิด: KCAS รุ่นเก่าเคยมีคนเอาไปใช้งาน DIY หนักๆ แล้วมีปัญหาได้บ้าง เลยถ้าจะใช้กับเครื่องเชื่อมแบต DIY หรือโหลดกระชากสูง ค่อยๆ ทดสอบก่อน 

ถ้าจะให้ชัวร์ ส่งรูป “หัว 24 พินของสายจริง” มา เดี๋ยวฉันวงตำแหน่ง เขียว/ดำ/เหลือง ให้เลย ไม่ต้องนั่งนับรูเอง 😆

ไอเดียนี้ไปได้ไกลเลยนะ โดยเฉพาะถ้าคุณมีพื้นฐานอิเล็กฯ + คอม + มีของกองอยู่แล้วอย่าง UNO + Power PC 12V 60A + XH-M604 + โมดูลต่างๆ มันสามารถทำเป็น “เครื่องมือช่างอัจฉริยะรวมศูนย์” ได้เลย ไม่ใช่แค่แหล่งจ่ายไฟธรรมดา

ลิงก์ GitHub ที่ส่งมาเปิดแบบตรงจุดยากนิดนึงจากตัวค้นหา แต่เข้าใจคอนเซปต์แล้วว่าอยากให้ UNO เป็น “สมอง” ของระบบ คอยเช็ค → ถาม → หยุด → สั่งขั้นถัดไปอัตโนมัติ

[GitHub โปรเจกต์ที่ส่งมา](https://github.com/xkp95175333/All_fun_log/blob/xkp95175333/main_sell_hide/Lan_c++/AIGenCore/Temp/README.md?utm_source=chatgpt.com)

โครงสร้างที่ผมมองว่าน่าเล่น:

KCAS-800W 12V 60A
        │
        ▼
[ฟิวส์+รีเลย์ตัดหลัก]
        │
        ▼
[DC BUS กลาง]
        │
 ┌──────┴────────┐
 │               │
 ▼               ▼
XY6020          โมดูลเสริม
ปรับ V/A         Spot weld
                ชาร์จแบต
                ESR meter
                พัดลม
                ทดสอบมอเตอร์
                ไฟทดลอง
                ฯลฯ
        │
        ▼
UNO = สมอง

UNO ไม่ควรจ่ายไฟเอง แต่เป็น "ผู้สั่ง"

UNO คุม:

รีเลย์เปิด/ปิดไฟหลัก

MOSFET

วัดอุณหภูมิ

วัดแรงดัน

วัดกระแส

ปุ่มเมนู

จอ

buzzer

พัดลม

ระบบหยุดฉุกเฉิน


เช่น:

ผู้ใช้: จะชาร์จแบต 3S

UNO:
ตรวจแรงดันก่อน
↓
แบตต่ำเกินไหม
↓
อุณหภูมิเท่าไร
↓
ถามเริ่มไหม
↓
กดปุ่ม
↓
เปิดไฟ
↓
ตรวจกระแส
↓
ร้อนเกินปิด

อันนี้คล้ายเครื่องแล็บจริงๆเลย

ของที่น่าใส่เพิ่ม:

1. INA219 หรือ ACS758



วัดกระแสแบบโหด

UNO อ่านได้

แสดง W / A / V


2. DS18B20 หลายตัว



ติด:

ฮีตซิงค์

MOSFET

สาย

หม้อแปลง


3. OLED I2C



แสดง:

12.2V
4.5A
55W
Temp:44°
Mode: Spot

4. รีเลย์รถยนต์ 70A



อย่าให้ UNO คุม 60A ตรง

ใช้:

UNO → MOSFET → รีเลย์

5. ปุ่ม Emergency Stop



สำคัญมาก

เพราะ Power PC 60A ลัดวงจรทีมีประกายแรงมาก

6. Hall sensor



ไว้เช็ค:

รอบมอเตอร์

พัดลมหมุนจริงไหม


7. เซนเซอร์ควัน



ถ้ากำลังเล่นแบตลิเธียม

8. Buck Converter



ลด

12V→5V

จ่าย UNO

ห้ามเอา VIN UNO ตรงจาก 12V ถ้าจะใช้งานยาวๆ

อีกเรื่องสำคัญมาก:

KCAS 12V 60A มันไม่ใช่ 60A ทุกเส้น

ราง 12V หลายเส้นรวมกันถึงได้เลขนั้น

อย่าเอาสายเหลืองเส้นเดียวไปยิง Spot Welder

ควรรวม:

เหลืองหลายเส้น + ดำหลายเส้น

เข้าบัสทองแดง

เพราะสาย AWG เดิมรับกระแสจำกัด

แนวคิดโหมดที่น่าทำ:

Power Supply

Spot Welder

Charger

Battery Analyzer

Wire Tester

Fan Tester

Motor Test

LED Test

Capacitor discharge

ESR check


แล้ว UNO ทำเมนู:

1.Power
2.Welder
3.Charge
4.Test

แล้วหยุดถามก่อนทุกขั้น

Voltage=12V
Current=20A
Continue?

[Y]
[N]

อันนี้เริ่มเข้าใกล้เครื่องซ่อมอเนกประสงค์แบบ DIY แล้ว

ถ้าอยาก ผมช่วยวาด “ผังต่อสายจริงจากของที่คุณมีตอนนี้” (UNO + KCAS + XY6020 + XH-M604 + พัดลม + รีเลย์ + โมดูลวัด) แบบต่อได้ทีละจุดเลยได้ เพราะของที่คุณมีตอนนี้เริ่มต่อรวมเป็นระบบใหญ่ได้แล้ว ไม่ใช่แค่โปรเจกต์เล่นๆ

ผมเช็คราคากลางแนว Shopee/Lazada ให้แบบเน้น “ทำเครื่องซ่อมรวมศูนย์ UNO + Power PC” และเอาเฉพาะของที่คุ้มก่อน ไม่ใช่ซื้อยัดทุกอย่าง

ของที่แนะนำ:

[LM2596 โมดูล Step Down]()
55 บาท
ลด 12V → 5V ให้ UNO
สำคัญมาก

[DS18B20 Stainless Steel Waterproof Temperature Probe Sensor]()
28–70 บาท/ตัว
เอา 2–3 ตัวติด:

ฮีตซิงค์

MOSFET

จุดแบต


[OLED Display I2C Module 0.96]()
~100 บาท
แสดง V/A/W/อุณหภูมิ

[INA219 DC Current Sensor Module]()
29–50 บาท
แต่มีข้อจำกัด: วัดกระแสได้ไม่แรงมาก (ประมาณ 3.2A โมดูลทั่วไป) 

[ACS758 Current Sensor Module]()
~168 บาท
อันนี้เหมาะกว่า ถ้าจะเล่นโหลดแรงหรือ Spot Welder

[Relay 12V 70A]()
~118 บาท
เอาไว้ตัดไฟหลัก

Rotary Encoder
~50–80 บาท
ใช้หมุนเมนูแทนกดหลายปุ่ม

Emergency Stop ปุ่มเห็ด
~40–80 บาท
อันนี้อย่าตัดออก


สรุปเป็นแพ็ก:

แพ็กขั้นต่ำ (ให้ระบบเริ่มฉลาด)

LM2596 ×1 = 55

DS18B20 ×2 = 60

OLED = 100

Relay 70A = 118

ปุ่มฉุกเฉิน = 50


รวม ~380–450 บาท

ทำได้:

เปิด/ปิดไฟ

เช็คอุณหภูมิ

หยุดฉุกเฉิน

มีเมนู

คุมพัดลม



---

แพ็กกลาง (เริ่มเป็นเครื่องซ่อมจริง)

เพิ่ม:

ACS758 = 168

Rotary = 60


รวม ~600–700 บาท

ทำเพิ่ม:

วัดกระแสจริง

คำนวณ Watt

เช็คโหลด

วิเคราะห์แบต



---

แพ็กบ้าพลัง

เพิ่ม:

MOSFET IRFB3207 หลายตัว

Busbar ทองแดง

พัดลม

ฟิวส์รถยนต์

Hall sensor

Spot Welder mode


รวมประมาณ 1000–1500+

อันที่อยากเตือนแรงๆ: ถ้าจะใช้ KCAS 12V 60A จริง ของที่ “ห้ามงก” คือ

1. ฟิวส์


2. ปุ่มหยุดฉุกเฉิน


3. สายใหญ่


4. เซนเซอร์กระแส



เพราะ 60A ช็อตทีไม่ใช่ระดับ Arduino ไหม้ แต่สายละลายได้

และอีกเรื่อง: ถ้าจะวัดกระแสหนัก อย่าใช้ INA219 เป็นตัวหลัก เพราะโมดูลทั่วไปรับกระแสไม่สูงมาก  ชุมชน Arduino หลายคนก็เจออ่านเพี้ยนเมื่อใช้งานหนักหรือเดินสายไม่เหมาะ 

ชุดที่ผมเลือกให้ถ้าเอาตามของที่คุณมีตอนนี้: LM2596 + ACS758 + OLED + DS18B20 + Relay 70A ก่อนเลย จบและคุ้มสุดสำหรับ V1.

```
---
ซอสโค้ด in UNO
---

```cpp
ถ้าจะทำแบบที่คุณเล่า—UNO เป็น “สมองหลัก” คุมแหล่งจ่าย, เช็คเซนเซอร์, หยุดก่อนขั้นต่อไป, มีโหมดหลายอย่าง—ผมไม่แนะนำเขียนยัดทุกอย่างใน loop() เดียว เพราะเดี๋ยวโค้ดจะเละมาก ให้ทำแบบ State Machine แยกโหมด

ตัวอย่างนี้เป็น “แกนกลาง” ก่อน:

ปุ่มเริ่ม / หยุด

รีเลย์เปิดไฟหลัก

อ่านอุณหภูมิ

อ่านแรงดัน

หยุดฉุกเฉินถ้าร้อน

ถามก่อนเริ่มงาน

มีหลายโหมดต่อยอดได้


// ===== PIN =====
#define RELAY_MAIN 8
#define FAN_PIN 9
#define BTN_START 2
#define BTN_STOP 3
#define BUZZER 4

#define TEMP_PIN A0
#define VOLT_PIN A1

// ===== SYSTEM =====
enum State
{
    IDLE,
    CHECK_SYSTEM,
    WAIT_CONFIRM,
    RUNNING,
    EMERGENCY_STOP
};

State currentState = IDLE;

float tempC = 0;
float voltage = 0;

bool confirmStart=false;


//=======================

void setup()
{
    Serial.begin(115200);

    pinMode(RELAY_MAIN,OUTPUT);
    pinMode(FAN_PIN,OUTPUT);

    pinMode(BUZZER,OUTPUT);

    pinMode(BTN_START,INPUT_PULLUP);
    pinMode(BTN_STOP,INPUT_PULLUP);

    digitalWrite(RELAY_MAIN,LOW);

    Serial.println("SMART LAB START");
}


//=======================

void loop()
{
    ReadSensors();

    if(digitalRead(BTN_STOP)==LOW)
    {
        EmergencyStop();
    }

    switch(currentState)
    {
        case IDLE:
        StateIdle();
        break;

        case CHECK_SYSTEM:
        StateCheck();
        break;

        case WAIT_CONFIRM:
        StateConfirm();
        break;

        case RUNNING:
        StateRun();
        break;

        case EMERGENCY_STOP:
        StateEmergency();
        break;
    }

}



//=======================

void ReadSensors()
{
    int rawT=analogRead(TEMP_PIN);

    tempC=(rawT*5.0/1023.0)*100.0;


    int rawV=analogRead(VOLT_PIN);

    voltage=(rawV*5.0/1023.0)*5.0;


    if(tempC>60)
    {
        EmergencyStop();
    }
}



//=======================

void StateIdle()
{
    digitalWrite(RELAY_MAIN,LOW);

    Serial.println("IDLE");

    if(digitalRead(BTN_START)==LOW)
    {
        delay(200);

        currentState=CHECK_SYSTEM;
    }

}



void StateCheck()
{
    Serial.println("CHECKING");

    Serial.print("Temp=");
    Serial.println(tempC);

    Serial.print("Volt=");
    Serial.println(voltage);


    if(voltage<10)
    {
        Serial.println("LOW POWER");

        currentState=IDLE;

        return;
    }


    currentState=WAIT_CONFIRM;
}



void StateConfirm()
{
    Serial.println("READY ?");
    Serial.println("Send Y");


    if(Serial.available())
    {
        char c=Serial.read();

        if(c=='Y')
        {
            currentState=RUNNING;
        }

        if(c=='N')
        {
            currentState=IDLE;
        }
    }

}



void StateRun()
{
    digitalWrite(RELAY_MAIN,HIGH);


    if(tempC>45)
    {
        digitalWrite(FAN_PIN,HIGH);
    }
    else
    {
        digitalWrite(FAN_PIN,LOW);
    }


    Serial.println("RUNNING");


    delay(500);
}



void EmergencyStop()
{
    currentState=EMERGENCY_STOP;
}



void StateEmergency()
{
    digitalWrite(RELAY_MAIN,LOW);

    digitalWrite(FAN_PIN,HIGH);


    tone(BUZZER,2000);

    Serial.println("!!! STOP !!!");


    while(1)
    {
    }

}

หลักการมันจะทำงานประมาณนี้:

เปิดเครื่อง
 ↓
IDLE
 ↓
กดเริ่ม
 ↓
เช็คแรงดัน
เช็คอุณหภูมิ
 ↓
ถามผ่าน Serial:
READY? Y/N
 ↓
Y
 ↓
เปิดรีเลย์
 ↓
รัน
 ↓
ร้อนเกิน
 ↓
EMERGENCY STOP

พอแกนนี้รันได้ ค่อยเสียบเพิ่มทีละอย่าง:

INA219 → วัด V/A/W จริง

DS18B20 → วัดหลายจุด

OLED I2C

Rotary Encoder

IRFB3207

Spot Welder mode

Battery Analyzer

ESR meter

XH-M604

XY6020


อีกเรื่องที่เตือนก่อน: UNO ขา I/O จ่ายกระแสได้แค่นิดเดียว ห้ามเอาไปขับรีเลย์รถ 60A ตรง ๆ ต้อง UNO → MOSFET → รีเลย์ → โหลด

และถ้าจะยิงกระแสจาก KCAS ไป Spot Welder หรือโหลดหนัก อย่าเอา RELAY_MAIN จากโค้ดนี้ไปต่อ 60A ตรง เดี๋ยวหน้าคอนแทคไหม้

ถ้าคุณจะใช้ของที่มีจริงตอนนี้ (UNO + KCAS + XH-M604 + XY6020 + IRFB3207) ผมเขียนเวอร์ชัน V2 แบบผูกเข้าของพวกนี้ตรงๆได้เลย ไม่ใช่ตัวอย่างลอยๆแล้ว.




```
---
งานซ่อมแบตให้ บ้าน
---

![1000009107](https://github.com/user-attachments/assets/23360c39-389a-4b61-b404-8e96af850160)

![1000009041](https://github.com/user-attachments/assets/d0a4025b-1bc7-493c-9e8f-aa1c8842eb37)

![1000009065](https://github.com/user-attachments/assets/ab1e63aa-38ba-4803-8e2a-0fd092738dc6)

<img width="2296" height="4080" alt="1000009082" src="https://github.com/user-attachments/assets/24e050c6-6f8f-41fb-b29e-df4e9abf0b5c" />
<img width="2296" height="4080" alt="1000009084" src="https://github.com/user-attachments/assets/db4d58b1-8818-449a-9833-38f0e5f35f46" />


<img width="3060" height="4080" alt="1000009043" src="https://github.com/user-attachments/assets/e5dc7628-9053-41d9-93c4-e33f667e6de6" />

<img width="2296" height="4080" alt="1000009084" src="https://github.com/user-attachments/assets/471d6965-4a26-42e6-8fda-671b27a01171" />

<img width="2296" height="4080" alt="1000009085" src="https://github.com/user-attachments/assets/d7520ab2-a565-4aae-a419-ba398c3396fb" />

![1000009088](https://github.com/user-attachments/assets/11100f64-a67a-4870-9d7a-1bc6070cb404)
<img width="2296" height="4080" alt="1000009092" src="https://github.com/user-attachments/assets/8406f6e6-0dd1-4328-a91d-c69c85be6602" />
