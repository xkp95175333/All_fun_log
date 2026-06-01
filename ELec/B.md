 ```md                
                 +BATT (54~63V)
                      |
                      |
              +-------+--------+
              |                |
              |              [BMS]
              |                |
              |     B0-B15 + NTC1..4
              |                |
              |                +---- P-
              |                      |
              |                  [MOSFET]
              |                      |
              +----------------------+
                                     |
                                     |
                            +--------+-------+
                            | Controller ECU |
                            | กล่องคุมมอเตอร์ |
                            +--------+-------+
                                     |
              +----------------------+----------------+
              |                      |                |
          Throttle             Speed SW         Battery sense
        (คันเร่ง)              L/M/H              ADC
              |                      |                |
              +----------------------+----------------+
                                     |
                               MCU / Logic
                                     |
                           เตือนแบตต่ำ / ตัดกำลัง
                                     |
                                 Driver
                                     |
                                 Motor 500W

```

---
2
---
```md

จากข้อมูลที่นายให้:

รถ  Angie

แบต 48V

BMS รหัส BZB15-5070-D4C4-V4

มี PC-

มีสายบาลานซ์ 2 แถวหลายสี


ฉันวาดแบบที่ “ใกล้ของจริงสุด” ของรถจีน AOI ได้ประมาณนี้:

┌────────────────────┐
                         │     CHARGER        │
                         │      54.6V         │
                         └───────┬────────────┘
                                 │
                      Charger +  │
                                 │
                ┌────────────────┴─────────────┐
                │                              │
                │           BATTERY            │
                │          13S Li-ion          │
                │      (48V nominal pack)      │
                │                              │
                │ B1 B2 B3 ... B13            │
                └┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬────┘
                 │ │ │ │ │ │ │ │ │ │ │ │ │
                 │ │ │ │ │ │ │ │ │ │ │ │ │
         Balance │ │ │ │ │ │ │ │ │ │ │ │ │
           wires │ │ │ │ │ │ │ │ │ │ │ │ │
                 ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼

        ┌─────────────────────────────────────┐
        │     BMS : BZB15-5070-D4C4-V4       │
        │                                     │
        │  B-  <──── Battery Negative         │
        │                                     │
        │  MOSFET Charge/Discharge            │
        │                                     │
        │  PC- / P-  ───────┬──────────────┐  │
        └───────────────────┘              │
                                           │
                           Load Negative   │
                                           │
                         ┌─────────────────▼────────────┐
                         │        CONTROLLER            │
                         │         48V 15A             │
                         │                              │
 Battery + ─────────────►│ B+                           │
 PC-/P-  ───────────────►│ B-                           │
                         │                              │
                         │ Phase U/V/W ──────┐          │
                         │ Hall 5pin ───────┼──►Motor  │
                         │ Brake cut ───────┤          │
                         │ Throttle ────────┤          │
                         │ Key/ACC ─────────┤          │
                         │ Alarm lock ──────┘          │
                         └─────────────────────────────┘

ระบบจริงของ AOI Angie จะมีประมาณนี้:

1. BMS

หน้าที่:

เช็คแรงดันแต่ละ cell

ตัดชาร์จเต็ม

ตัดไฟต่ำ

ตัดลัดวงจร

balance cell


สายหลัก:

ชื่อ	หน้าที่

B-	ลบแบตตรง
P- / PC-	ลบโหลด/ชาร์จ
B+	บวกตรงจากแบต


หลายรุ่น “ไม่มี P+” เพราะใช้ B+ ตรงจากแบตเลย


---

2. Controller

จะรับ:

B+

PC-/P-


แล้วแปลงไปขับมอเตอร์

สายสำคัญ:

สีที่พบบ่อย	หน้าที่

เหลือง/เขียว/น้ำเงินใหญ่	Phase motor
แดงดำเล็ก	ไฟเลี้ยง
Hall 5 เส้น	เซนเซอร์มอเตอร์
น้ำตาล/ดำ	Brake cut
แดงเล็ก	ACC/key



---

3. จุดที่รถ “เช็คแบต”

AOI Angie มักเช็ค 2 จุด:

ที่ BMS

เช็ค:

cell voltage

current

overcharge

undervoltage


ที่ Controller

เช็ค:

แรงดันรวม pack

แรงดันตกตอนเร่ง


ดังนั้น:

แบตเต็มแต่เตือน

เร่งแล้วตัด


อาจเกิดจาก:

BMS ตัด

แรงดันตก

MOSFET รั่ว

balance เพี้ยน



---

จุดวัดที่ควรได้

ถ้าแบตเต็ม:

วัด	ค่าปกติ

B+ ↔ B-	54.6V
B+ ↔ PC-	54.6V
แต่ละ cell	~4.15-4.2V


ถ้า:

B+ ↔ B- มีไฟ

แต่ B+ ↔ PC- ไม่มี


= BMS ตัด หรือ MOSFET ฝั่ง output เสีย


---

จุดเสียบ่อยของ BZB15

[ MOSFET ]
     ↑
ร้อน / ลัดวงจร

[ Shunt resistor ]
     ↑
ไหม้แล้ววัดกระแสผิด

[ Balance resistor ]
     ↑
cell เพี้ยน

[ PC- copper trace ]
     ↑
ลายขาดจากกระแสสูง


---

ถ้าจะ bypass ชั่วคราวเพื่อเทส

Battery -
   │
   └────► Controller -

จะทำให้รถติดแม้ BMS เสีย
แต่:

ไม่มีระบบป้องกัน

ห้ามชาร์จแบบ bypass

ใช้แค่เทสสั้นๆ



---

ถ้าจะเปลี่ยน BMS

ควรหา:

13S

48V

15A ขึ้นไป

same port ถ้าเดิมใช้ PC-

มี balance


ใกล้ของเดิมสุด:

13S 48V 15A Li-ion BMS

หรืออัปเกรด:

20A

30A MOSFET เยอะกว่า

ร้อนน้อยกว่าเดิม


ถ้านายส่งรูป:

หน้า/หลัง BMS

จุด PC-

จำนวน MOSFET

หัวปลั๊กสาย 2 แถว


ฉันจะวาด “pinout รายเส้น” ให้ได้เลยว่า:

เส้นไหน B1-B13

เส้น temp sensor

จุด charge/discharge

และจุด bypass เทสแบบปลอดภัยสุดของ AOI Angie จริงๆ ได้เลย


```
---
3
---
```md
วงจรที่คุณคิด “แบต 2 ก้อน → boost เพิ่มแรงดัน → MOSFET spot welder” ทำได้ แต่ต้องเข้าใจก่อนว่า spot welder ต้องการ “กระแสสูงมากช่วงสั้นๆ” ไม่ใช่แค่แรงดันสูง

ดังนั้น:

Step-up ทั่วไปเพิ่ม V ได้จริง

แต่ส่วนมาก “A ตก”

ยิงไม่ติด หรือ MOSFET ระเบิด


แนวที่ดีกว่าคือ:

ใช้ 2S โดยตรง (7.4–8.4V)

เพิ่ม capacitor bank

ใช้ MOSFET low Rds(on)


มากกว่าปั่นไฟขึ้นสูง


---

โครงสร้างที่ “ใช้งานได้จริง”

18650 x2 (2S)
   │
 BMS 2S
   │
 Capacitor Bank
   │
 TVS Protection
   │
 MOSFET Bank
   │
 Copper Probe


---

ถ้าจะ “boost ไป 12V”

ทำได้ แต่:

ต้องใช้ boost ที่จ่าย 30–50A pulse

XL6009 / MT3608 ไม่ไหว


ต้องเล่น:

XL4016

LTC3780

หรือ supercapacitor



---

แบบที่แนะนำจริงกว่า

ใช้:

2S high drain

capacitor ช่วยยิง pulse


แทนการ boost

เพราะ:

P = V x A

ถ้า boost V สูง:

กระแสฝั่งแบตจะพุ่งหนักกว่าเดิมอีก



---

วงจรแนะนำ (DIY ใช้ได้จริง)

Battery

ใช้:

Samsung 25R

Sony VTC5

Molicel


2 ก้อนแบบ 2S


---

BMS

2S 20A–40A


---

Capacitor Bank

เช่น:

4700uF–22000uF Low ESR

16V หรือ 25V


ต่อขนานหลายตัว


---

MOSFET

นิยม:

เบอร์	ดี

IRLB3034	ดีมาก
HY4008	ถูก
AOI518	แรง
IRF1404	นิยม


ต่อขนาน 4–8 ตัว


---

Driver

ใช้:

TC4420

IR2110

หรือ NE555 ยิง pulse



---

TVS Diode

กัน spike:

1.5KE15A

SMBJ16A



---

สาย

ต้องสั้นมาก:

8AWG

10AWG



---

ผังวงจร

+8.4V
         │
      Fuse 30A
         │
   Capacitor Bank
         │
  +------+------+
  |             |
 MOSFET       TVS
 Bank         Diode
  |             |
  +------+- ----+
         │
      Probe +
         
Battery -
         │
      Probe -


---

ถ้าจะใช้ boost จริง

โครงสร้าง:

2S Battery
   │
Boost Converter
(8.4V → 12V)
   │
Capacitor Bank
   │
MOSFET Welder

แต่ปัญหา:

boost ร้อนหนัก

efficiency ตก

ยิง pulse ไม่ทัน

ช็อตแล้วพังง่าย



---

สิ่งที่ “ดีกว่า boost”

คนทำจริงนิยม:

supercapacitor

capacitor bank ใหญ่

MOSFET แรงๆ


แทน step-up


---

หน้าตาวงจรที่นิยม

MOSFET Spot Welder

Capacitor Bank + Probe

รายการอะไหล่ที่แนะนำ

ส่วน	ค่า

แบต	18650 High Drain 2S
BMS	2S 30A
MOSFET	IRLB3034 x4
Capacitor	4700–22000uF Low ESR
TVS	SMBJ16A
Driver	TC4420
Fuse	30A รถยนต์
สาย	8AWG
Probe	Copper rod



---

จุดสำคัญสุด

แบต “มือถือธรรมดา” แม้เขียน 5600mAh:

ไม่ได้แปลว่าจ่ายกระแสสูง

หลายก้อนจ่ายได้แค่ 5–10A


spot welder ต้องการ pulse สูงมาก

ดังนั้น:

high drain สำคัญกว่า mAh

resistance ต่ำสำคัญกว่า voltage สูง


ถ้าคุณอยาก ผมสามารถวาด:

PCB layout

wiring diagram จริง

วิธีต่อ MOSFET ขนาน

วงจร NE555 pulse

แบบ auto trigger

แบบ supercapacitor


ให้เป็นภาพต่อสายได้เลยด้วย

ถ้าจะทำ spot welder จาก “แบตมือถือ 2–3 ก้อน” (Li-ion 4.2V เต็ม) มันทำได้ แต่ต้องเข้าใจก่อนว่า spot welder ต้องใช้ “กระแสพุ่งสูงมาก” ช่วงสั้นๆ ไม่ใช่แค่ mAh เยอะ

แบตมือถือทั่วไป:

จ่ายต่อเนื่องไม่แรงมาก

ถ้าช็อตตรงๆ อาจบวม/ไหม้ได้


ดังนั้นวงจรที่นิยมคือ:

ใช้ MOSFET ยิง pulse

ใช้ capacitor ช่วย

หรือใช้แบต high drain



---

แบบพื้นฐานสุด (นิยมมาก)

ใช้:

แบต 2S หรือ 3S

MOSFET หลายตัวขนาน

ปุ่มยิง pulse


โครงสร้าง:

แบต Li-ion
   │
BMS
   │
Capacitor
   │
MOSFET bank
   │
หัวเชื่อมทองแดง


---

ถ้าใช้แบต 2 ก้อน

2S

ได้:

8.4V เต็ม

แรงพอเชื่อม nickel บางๆ


เหมาะกว่า 1S เยอะ


---

MOSFET ที่นิยม

พวก:

IRF1404

IRLB3034

AOI518

HY4008


ต้องใช้หลายตัวขนานกัน


---

วงจรที่นิยม DIY มาก

“Mini Battery Spot Welder”

ใช้:

2S/3S Li-ion

super capacitor บางที

MOSFET trigger


ตัวอย่างหน้าตา


---

ทำไมต้องใช้ MOSFET

เพราะ:

ปุ่มธรรมดารับกระแสไม่ไหว

จะอาร์คไหม้

MOSFET เปิดได้เร็วมากระดับ ms



---

ถ้าจะ “ปั่น A”

จริงๆ คือ:

ดึงกระแสสูงช่วงสั้น

ใช้ internal resistance ต่ำ

ใช้สายสั้นมาก


ดังนั้น:

แบตมือถือบางก้อนไม่เหมาะ

18650 high drain ดีกว่า



---

ถ้าจะให้เชื่อมได้จริง

แนะนำ:

2S หรือ 3S

18650 high drain เช่น:

Samsung 25R

Sony VTC5

Molicel



---

แบบปลอดภัยกว่า

ใช้บอร์ดสำเร็จ:

คำค้น:

12V spot welder board
MOSFET spot welder module
DIY battery spot welder board

พวกนี้มี:

timer

auto trigger

pulse control

MOSFET bank มาแล้ว



---

ตัวอย่างบอร์ด


---

สรุปแบบใช้งานได้จริง

แบบ	แนะนำไหม

แบตมือถือ 1S	❌ อ่อน
แบตมือถือ 2S	⚠️ พอได้
18650 high drain 2S	✅ ดี
MOT microwave	✅ แรงมาก
super capacitor	✅ โปรกว่า



---

ถ้าคุณจะทำจริง

ผมแนะนำแนวนี้:

2S 18650 high drain
+
MOSFET spot welder board
+
สายเชื่อมสั้นมาก
+
หัวทองแดง

จะง่ายและปลอดภัยกว่า MOT เยอะ และแรงพอเชื่อม nickel 0.1–0.15mm ได้เลย
```
<img width="465" height="284" alt="1000009314" src="https://github.com/user-attachments/assets/66949483-6023-4e9e-bf09-f36931fbe858" />
<img width="465" height="260" alt="1000009316" src="https://github.com/user-attachments/assets/57bd2c80-7128-4e13-bc0c-b18a746d2138" />


---
El
---
```md
ถ้าคุณทำแนว:

ซ่อม BMS

USB-C

controller

บอร์ด 2 หน้า

แบต

MOSFET

ลมร้อน


จริงๆ “กาว/flux/สารเคมี” จะมีหลายสายมาก และแต่ละตัวหน้าที่คนละแบบเลย

ผมสรุปเป็น “ชุดโต๊ะซ่อมครบ” ให้เลย


---

1. Flux (สำคัญสุด)

แบบเหนียว Tacky Flux

ใช้:

ลมร้อน

SMD

USB-C

PCB 2 หน้า


ตัวนิยม

AMTECH NC-559-ASM

Mechanic XG-Z40

RELIFE RL-559


หน้าตา


---

2. Flux น้ำ

ใช้:

ลากตะกั่ว

งานทั่วไป

through-hole


ข้อดี:

ถูก

ไหลง่าย


ข้อเสีย:

แห้งไว

เป่าลมร้อนสู้ tacky ไม่ได้



---

3. Low Melt Alloy

ตัวเทพถอดของยาก

ใช้:

USB-C

ground plane

HDMI

BMS


ตัวดัง

ChipQuik


หน้าตา


---

4. ตะกั่วบัดกรี

แบบที่ควรมี

63/37

ดีที่สุดสายซ่อม

ข้อดี:

ละลายง่าย

ไหลสวย

ไม่หนืด


ยี่ห้อดี

Kester 63/37

Mechanic HX-T100



---

5. Solder Paste

ใช้:

reflow

stencil

SMD ทั้งแผง


ต้องแช่เย็นบางรุ่น


---

6. UV Mask

กาวเขียว/ดำเคลือบบอร์ด

ใช้:

ซ่อมลายปริ้น

กันลัดวงจร


ตัวนิยม

Mechanic UV Solder Mask


หน้าตา


---

7. กาวแดงล็อกน็อต

ใช้:

กันคลาย

spot welder

โครงเครื่อง


ตัวดัง

Loctite 243



---

8. Silicone / RTV

ใช้:

ยึดสาย

กันสั่น

กันไฟรั่ว


ดีสุดสายอิเล็กทรอนิกส์

Neutral Cure RTV


อย่าใช้กรดอะซิติกกับ PCB


---

9. Epoxy

ใช้:

ยึดหนัก

ซ่อมพลาสติก

ยึด heatsink


ตัวดัง

Araldite

JB Weld



---

10. Thermal Paste

ใช้:

MOSFET

regulator

CPU


ตัวดัง

Arctic MX-4



---

11. Kapton Tape

สำคัญมากกับลมร้อน

ใช้:

กันร้อน

บังชิ้นส่วน


หน้าตา


---

12. Copper Wick

ดูดตะกั่ว

ตัวดี

Goot Wick

Mechanic Solder Wick



---

ชุด “ควรมีจริง” ถ้าซ่อมบอร์ด

ประเภท	ควรมี

Flux	NC-559
ตะกั่ว	63/37
wick	Goot
UV mask	Mechanic
Kapton	10–20mm
RTV	neutral cure
Low melt	ChipQuik



---

ถ้าจะจัด “ชุดโต๊ะซ่อมคุ้มสุด”

ผมจะเลือก:

NC-559
63/37 solder
Copper wick
Kapton tape
UV mask
ChipQuik

แค่นี้ซ่อม BMS / USB-C / controller / PCB 2 หน้า ได้เกือบหมดแล้ว

```
