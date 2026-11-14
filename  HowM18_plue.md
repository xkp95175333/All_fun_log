---
วิธีใช้แชร์จอ
---
```md
ได้ครับ รอบนี้ผมจะจัดใหม่เป็นคู่มือใช้งานจริงสำหรับ Nitro 16-A41 + Windows 11 Pro + Vivo Y17s + MiraAdapter + HDMI USB 3.0 Capture Card โดยแยกให้ชัดว่าโปรแกรมไหนใช้ทำอะไร โปรแกรมไหนต้องติดตั้ง และโปรแกรมไหนเป็นตัวเลือก ไม่ต้องลงซ้ำซ้อน

เป้าหมายคือให้คุณสามารถรับภาพมือถือเข้าคอม เปิดดูเต็มจอ บันทึกภาพ ส่งภาพให้คนอื่นดู และสร้างจอเสมือน Display 2 ได้ โดยไม่ต้องซื้อจอเพิ่ม

### 1\. รายชื่อโปรแกรมทั้งหมดและหน้าที่

### 2\. โปรแกรมฝั่ง Windows ที่ทำได้

ผมแบ่งเป็น 5 กลุ่ม เพื่อไม่ให้ลงโปรแกรมที่ทำงานซ้ำกัน

### A. รับภาพจากมือถือเข้า Nitro

![Hướng dẫn cách chiếu màn hình điện thoại lên máy tính](https://images.openai.com/static-rsc-4/sGnecPzqfjpKITsq3IkgCS8pNh87wBfLSAHf5ql5CgPNkRk7bx5UuDb00YWhwoqzaJgu4vCJuz5XcK1EUvapFdcJkNeC5YnvHmzyKJaF8fBIaRNsDsGgJy0GeC7Ojh_ffib9k5dIcBX6VNOHkQ0Tx1hOqIQdeIADOQJ-27Vjc4n8dH6BHfb4CnSx3erchQY5?purpose=fullsize)

1\. scrcpy

แนะนำที่สุดสำหรับ USB

* รับภาพมือถือผ่าน USB หรือ Wi-Fi

* ควบคุมมือถือด้วยเมาส์และคีย์บอร์ด

* บันทึกวิดีโอได้

* ไม่ต้องติดตั้งแอปเพิ่มในมือถือ

* รองรับการทำงานถึง 120 FPS ตามความสามารถอุปกรณ์

    [ดาวน์โหลดจากผู้พัฒนา](https://github.com/Genymobile/scrcpy?utm_source=chatgpt.com)

![Article: OBS Basic Setup Guide - Cards, Drive Accessories, Tools - The IT Pro StarTech.com Community](https://images.openai.com/static-rsc-4/ezm8eYAClYqOqazGCjSC6i31BS651gGCWxBmbNgl94UnAcs_uYZ1-rq8f1BfgZFa8TU5pzdWCOMItT6OkaDdYDFHctJaru1Qmw-FqjIkymrRKdJVF1F7H3YA5bC7RUTNB6sZLyHU8-m430I1EhBQyPMwjJAu4nT6P5sqa4i2G5VGnkh5q1qd6imDOQcXvMN8?purpose=fullsize)

2\. OBS Studio

แนะนำสำหรับ HDMI Capture

* รับภาพจาก Capture Card

* จัดขนาดภาพและตำแหน่ง

* แสดงภาพเต็มหน้าต่าง

* บันทึกวิดีโอและเสียง

* สร้าง Virtual Camera เพื่อส่งภาพให้โปรแกรมอื่น

    [ดาวน์โหลด OBS](https://obsproject.com/download?utm_source=chatgpt.com)

![MP4 Player – 10 Best MP4 Video Players for Windows 10/8/7 PC and Mac](https://images.openai.com/static-rsc-4/lil_eJbgy8qEni2k4M18lMRHK0mcMxLj0m8Tp4cNbFHRSm3HxygRu2oU4OpCoEIpzQ-igahSO0DJY5_gbkcgnssjF44HLWY7twRhYsHFK0HY7ZcPM0iiMpeqihtYfSAEyKj_FwalzgPo0oquqDZrKWg4paqHgr37DqqctFkzm0jhRqnbwY5-9tJeMqZrW1GZ?purpose=fullsize)

3\. VLC Media Player

* ทดสอบภาพและเสียง

* เปิดวิดีโอที่บันทึกไว้

* ใช้เป็นโปรแกรมสำรอง

    [ดาวน์โหลด VLC](https://www.videolan.org/vlc/?utm_source=chatgpt.com)

![PotPlayer 1.7 - Multimedijski sposoban - Besplatni programi @
        Bug.hr](https://images.openai.com/static-rsc-4/LhugkyZ1gvm60rSt1RS0vdDwnuWuJk6IX7G7QT7abKz2VC_bI7OpMHpAAW8hDgj8YlNfKvq-mnCukmlZQGU3EDT5uRdP6Y70HWBk3u3JcUN8Nj9bfXtFXqBg1Zyb_ACrUsvuafKaNyBg3u72iIPDFFoKzzQSVXzlHZLpgAvh5QZhLB7qkzskpjFRhA9I6CCt?purpose=fullsize)

4\. PotPlayer

* โปรแกรมเล่นวิดีโอ

* ปรับขนาดและสัดส่วนภาพ

* ใช้เปิดไฟล์บันทึกจากมือถือ

    [เว็บไซต์ทางการ](https://potplayer.daum.net/?utm_source=chatgpt.com)

    ตัวเลือก ไม่จำเป็นต้องลงพร้อม VLC

### B. สร้างจอเสมือน Display 2

![Virtual Display Driver - PyroSoft Productions](https://images.openai.com/static-rsc-4/P0PBePdRBVb2dhqBDU3lr4Tm42p9SwegqICsJLgtoP6XKmAGah0ON8VTNgy5CQ4SHkTmDML8stnC5ztOxGeUKFeg5IXw6QQpgPrz7DqnHTeFYVTASXOGtzzgdJOUedNAGD9UwpgxG5OuZ8-f8SpVoAgQ_5M0QLfcLH76NVPEI9NAA3cZfvSqikFDWTuBOIT_?purpose=fullsize)

5\. Virtual Display Driver (VDD)

ตรงกับความต้องการจอปลอม

* เพิ่มจอเสมือนใน Windows

* ตั้ง 1920×1080 / 60Hz

* ใช้เป็นพื้นที่วางหน้าต่าง OBS

* ใช้กับระบบส่งภาพหน้าจอไปเครื่องอื่น

    [ดาวน์โหลด VDD](https://github.com/VirtualDrivers/Virtual-Display-Driver/releases?utm_source=chatgpt.com)

![Virtual Display Driver: cómo crear un monitor virtual para hacer streaming de juegos o usar realidad virtual](https://images.openai.com/static-rsc-4/jFIfW0hLWv1UI1fwe3eHvNLMLx26jyGJd-Q1TZkZvE3yAACXDClV0mIzT5CrI3JupxpDrP-RwOT3PuOc0PBEvswrORK-81fj1XlsdAkOkEAKHXWIfadhbNNryFiQZj6GJZZByNmsF2ZCkX1vpKuL8y7jMcU7cNefr2k360asGaDLzgCD2rMycrUf7MJt_uHZ?purpose=fullsize)

6\. Virtual Driver Control

* โปรแกรมจัดการ VDD

* ติดตั้งและเปิด/ปิดจอจำลอง

* ตรวจสอบจอเสมือน

    เป็นเครื่องมือควบคุมของโครงการ VDD ไม่ใช่ไดรเวอร์จออีกตัวหนึ่ง

    [รายละเอียดและดาวน์โหลด](https://github.com/VirtualDrivers/Virtual-Display-Driver?utm_source=chatgpt.com)

### C. รับภาพไร้สายและส่งภาพไปยังอุปกรณ์อื่น

![How to Use an Additional Computer as a Secondary Display | Microsoft Community Hub](https://images.openai.com/static-rsc-4/RnxiTabmrYhsGPpTzlYwYKNd5kk5goQYzYgP53ynTnEtOB21ulkdoqHdxUCOdlO6v0I0fSwNp9gOUNQ0lS9kdgu-6niv-Sp8yM29i18BA30UcOBrU0HuAzkk5v_a6Rn5yCgA1Oigk7QNkOvUVxdVcEe7wrB23aKkmXfWfoFMKQ1i8QGNNCLhDfRdg4Euj7FB?purpose=fullsize)

7\. Wireless Display

* ให้ Windows รับภาพผ่าน Miracast

* ไม่ต้องใช้ HDMI Capture Card

* ต้องรองรับ Miracast ทั้งมือถือและคอม

    เป็นฟีเจอร์เสริมของ Windows ไม่ใช่โปรแกรมรับ HDMI

    [คู่มือ Microsoft](https://support.microsoft.com/en-us/windows/hardware/display-graphics/screen-mirroring-and-projecting-to-your-pc-or-wireless-display?utm_source=chatgpt.com)

![Guide: Stream PC Games to TV with Sunshine & Moonlight | NerdZap](https://images.openai.com/static-rsc-4/E9hvHioowlBPZNrEd8WLzRWS-FdcW0DXMEq7M9nGZYo4vnz5HO5_wXizMjHzoW1HAdK5Cb69Ks0oXh0v2QHNxm2kq4fix-7J_O_u_W5HHfPdz2sCT4i8xWjMphTOox0QyCTs3UhkqFGLls-4myQ11SEhZAVyopHxY7aKnVpCoqSYit_O6SPin7EbP9yqPVyR?purpose=fullsize)

8\. Sunshine

* ส่งภาพจาก Windows ไปอีกเครื่อง

* ใช้ร่วมกับ Virtual Display

* เหมาะกับการทำเครื่อง Nitro เป็นเครื่องต้นทาง

    [เว็บไซต์โครงการ](https://github.com/LizardByte/Sunshine?utm_source=chatgpt.com)

![Top 8 Remote Desktop Software for Gaming in 2024 - Avica](https://images.openai.com/static-rsc-4/gkvJExcpJjSDcMbU2giI9_Z6TzxP8mJvxj0Ez2G6r2OHDvKXPTXzI8HTU0l97oGs1IfeGPn-OffAzFdGMDHbQIaj4PPm5PxaQzNDcu4JpIgjS7oDVAeyoKd6S0olakn1S59TmtUvA8x6omV9psvcxBNtPQYMbLrT0naRQi146qBL3GtrpNP6nIttkxzm3s_7?purpose=fullsize)

9\. Moonlight

* รับภาพจาก Sunshine

* เปิดดูภาพ Nitro บนอุปกรณ์ปลายทาง

* รองรับการส่งภาพผ่านเครือข่าย

    [ดาวน์โหลด Moonlight](https://moonlight-stream.org/?utm_source=chatgpt.com)

### D. โปรแกรมเกี่ยวกับ MiraAdapter

| โปรแกรม               | หน้าที่                                                | ติดตั้งที่    |
| --------------------- | ------------------------------------------------------ | ------------- |
| EZMira                | ค้นหาและตั้งค่า MiraAdapter                            | Vivo          |
| EZCastLite            | โปรแกรมฝั่ง Windows สำหรับอุปกรณ์ที่รองรับ             | Nitro         |
| Miracast / Smart View | ส่งภาพหน้าจอมือถือแบบไร้สาย                            | Vivo          |
| DLNA                  | ส่งสื่อ เช่น วิดีโอและรูป โดยไม่จำเป็นต้องสะท้อนทั้งจอ | Vivo / ตัวรับ |

ข้อสำคัญ: เว็บไซต์ผู้ผลิตปัจจุบันแยก EZMira สำหรับ Android/iOS และ EZCastLite สำหรับ Windows แล้ว ไม่ควรนำ EZMira Windows รุ่นเก่ามาเป็นโปรแกรมหลักโดยไม่ตรวจรุ่นอุปกรณ์ก่อน MiraScreen +1

### E. โปรแกรมดูแลไดรเวอร์และตรวจอุปกรณ์

| โปรแกรม                | หน้าที่                                      | คำแนะนำ           |
| ---------------------- | -------------------------------------------- | ----------------- |
| Driver Booster 14 Pro  | สแกนและช่วยอัปเดตไดรเวอร์                    | มีแล้ว ใช้ตรวจสอบ |
| Device Manager         | ดูอุปกรณ์ที่ Windows ตรวจพบ                  | จำเป็น            |
| Windows Update         | อัปเดตระบบและไดรเวอร์ที่ Windows จัดให้      | ใช้เป็นหลัก       |
| Camera (Windows)       | ทดสอบอุปกรณ์ที่ Windows มองเห็นเป็นกล้อง     | ควรมี             |
| USB Device Tree Viewer | ตรวจสอบว่า USB เชื่อมต่อแบบ USB 2.0 หรือ 3.x | ตัวเลือก          |

### 3\. โปรแกรมไหนต้องลงจริงบ้าง

ชุดเริ่มต้นที่ผมเลือกให้

| ลำดับ | โปรแกรม               | ความจำเป็น                                    |
| ----- | --------------------- | --------------------------------------------- |
| 1     | OBS Studio            | ต้องมีถ้าใช้ Capture Card                     |
| 2     | scrcpy                | ควรมี ถ้าต้องการต่อ Vivo ผ่าน USB             |
| 3     | EZMira                | ต้องมีสำหรับตั้งค่า MiraAdapter ถ้ารุ่นรองรับ |
| 4     | VDD                   | ลงเมื่อจะสร้าง Display 2                      |
| 5     | VLC                   | สำรองไว้ทดสอบ                                 |
| 6     | Driver Booster 14 Pro | มีอยู่แล้ว ไม่ต้องลงใหม่                      |
| 7     | Wireless Display      | ทดลองก่อนว่า Nitro รองรับ Miracast หรือไม่    |
| 8     | Sunshine + Moonlight  | ลงเมื่อจะส่งภาพไปอีกเครื่อง                   |

ไม่จำเป็นต้องติดตั้งทุกตัว โดยเฉพาะ Sunshine, Moonlight และ VDD หากแค่ต้องการดูภาพมือถือใน OBS บน Nitro เครื่องเดียว

### 4\. แปลคู่มือ MiraAdapter เป็นภาษาไทยแบบเข้าใจง่าย

ผมค้นพบคู่มือ EZMira สำหรับ Windows ที่มีภาพหน้าจอและลำดับเมนูตรงกับตระกูล MiraScreen ที่คุณถ่ายมา จึงนำมาเทียบกับเอกสารของผู้ผลิตด้วย

* [เปิดคู่มือ EZMira Windows ฉบับ PDF](https://informatika.uma.ac.id/wp-content/uploads/2019/11/EZMira_User_Manual_for_Windows_v4.pdf?utm_source=chatgpt.com)

* [เว็บไซต์ผู้ผลิต MiraScreen](https://mirascreen.com/pages/download?utm_source=chatgpt.com)

* [คู่มือฉบับอ่านข้อความง่าย](https://manuals.plus/m/50c22abc1719dcd0892c7243f3bb4cdff8b12230e5e8fe9f7ec798accbe10ce6?utm_source=chatgpt.com)

คู่มือ Windows ที่ค้นพบเป็นคู่มือรุ่นเก่า ไม่สามารถยืนยันว่าเฟิร์มแวร์ของกล่องคุณตรงกันทุกเมนู

### ขั้นตอนที่ 1 — ติดตั้ง EZMira

ต้นฉบับ: Download EZMira for Windows and follow the installation instructions.

แปล: ดาวน์โหลดโปรแกรมสำหรับ Windows แล้วติดตั้งตามขั้นตอน

ในคู่มือเก่า โปรแกรมจะเปิดทำงานอยู่ตรงมุมขวาล่างของ Taskbar (ใกล้นาฬิกา Windows)

### ขั้นตอนที่ 2 — เชื่อมต่อ Wi-Fi ของ MiraAdapter

ต้นฉบับ: Find matching SSID of MiraScreen in your Windows Laptop's Wi-Fi List.

แปล: เปิดรายการ Wi-Fi ในคอม แล้วค้นหาชื่อเครือข่ายของ MiraAdapter

ตัวอย่างชื่อที่คู่มือแสดง:

* Mirascreen BFC84BD8

* Mirascreen 364C7D5B

ชื่อจริงของกล่องคุณอาจแตกต่างกัน

### ขั้นตอนที่ 3 — ค้นหาอุปกรณ์

ต้นฉบับ: Click Search Device.

แปล: คลิกขวาที่ไอคอน EZMira ตรง Taskbar แล้วเลือก Search Device เพื่อค้นหาเครื่องรับภาพ

เมื่อพบชื่ออุปกรณ์ ให้เลือกชื่อที่ตรงกับกล่องของคุณ

### ขั้นตอนที่ 4 — เปิดการส่งภาพ

ต้นฉบับ: Once connected, you may see the green light. Start casting by clicking Mirror On.

แปล: เมื่อเชื่อมต่อสำเร็จ จะมีสัญลักษณ์สีเขียวแสดงสถานะ ให้เลือก Mirror On เพื่อเริ่มส่งภาพ

คำสั่งอื่นในเมนู:

| คำสั่ง        | ความหมาย               |
| ------------- | ---------------------- |
| Volume Up     | เพิ่มเสียง             |
| Volume Down   | ลดเสียง                |
| Mirror On/Off | เปิด/ปิดการส่งภาพ      |
| Search Device | ค้นหาเครื่องรับภาพ     |
| Setting       | ตั้งค่าเครื่อง         |
| Update        | ตรวจสอบเวอร์ชันโปรแกรม |
| Quit          | ปิดโปรแกรม             |

### ขั้นตอนที่ 5 — ตั้งค่าอินเทอร์เน็ต

ต้นฉบับ: Click Setting, then enter Internet section and connect to preferred Wi-Fi.

แปล: เข้า Setting แล้วเลือก Internet เพื่อให้ MiraAdapter เชื่อมต่อ Wi-Fi บ้านหรือ Wi-Fi Router ที่ต้องการ

จุดประสงค์คือให้ตัวรับภาพเชื่อมต่ออินเทอร์เน็ตได้ โดยต้องตั้งรหัสผ่านให้ถูกต้อง

### ขั้นตอนที่ 6 — ตั้งค่าภาพและระบบ

![EZMira for Android -  Download](https://images.openai.com/static-rsc-4/22eZgaBFc5l1aAutIapRd2FVzdB2GTB-NqDwS-PJaiqEm-mq0inas6kNYJ99zzHzCo3u8dSvXqin0SXXx3DsM9qybTgdsRTVSRZYqHSd5glrTt4grssxuteJ9iaBNe7DwDGIhNnHRjzDzcXTluxRN8cjnTEot0t8ML6L5R1ei3-NGq4r-jbObB-a4reBMCsZ?purpose=fullsize)

| เมนู                     | ความหมายแบบใช้งานจริง              |
| ------------------------ | ---------------------------------- |
| Internet                 | เชื่อมต่อ Wi-Fi Router             |
| Resolution               | เลือกความละเอียดภาพที่ HDMI ส่งออก |
| Language                 | เลือกภาษา                          |
| Password                 | เปลี่ยนรหัส Wi-Fi ของตัวรับ        |
| Wi-Fi Channel            | เลือกช่องสัญญาณไร้สาย              |
| AirPlay                  | ตั้งโหมดรับภาพจากอุปกรณ์ Apple     |
| Upgrade                  | อัปเดตระบบภายในกล่อง               |
| Reboot                   | ปิดแล้วเปิดกล่องใหม่               |
| Reset to Factory Default | คืนค่าเริ่มต้นจากโรงงาน            |

คู่มือแสดงตัวอย่างความละเอียด `1920×1080_60P` ซึ่งหมายถึง Full HD 60 เฟรมต่อวินาที ไม่ใช่ 60Hz ของจอ Nitro โดยตรง informatika.uma.ac.id +1

### 5\. ผังการทำงานที่เลือกได้ 3 วิธี

วิธีที่ 1 — USB โดยตรง

Vivo → USB → Nitro → scrcpy

![How to mirror using scrcpy | Mirror Android Screen to PC](https://images.openai.com/static-rsc-4/glFBAF3ogJA8kU81O7SKfg-AsEFTnCTxirDvoPq0BXvF_4_RdMp96Dq5FsO-5xnj6s9Njq5tUMAk17oqtkqhv5vexaV1HJ7AZ0-U7k7yeZglQm7dYd_izsfdFTZKg2aTd7DxQ3CwiT0ai_XPraiIJTDxYAreebvBTyOJtwejxlRlLi7JtupB2fTaltVVcq4c?purpose=fullsize)

* ไม่ต้องใช้ MiraAdapter

* ไม่ต้องใช้ Capture Card

* เหมาะกับภาพสดและงานตรวจ PCB

* ต้องเปิด USB debugging

    วิธีที่ 2 — ไร้สายผ่าน MiraAdapter

    Vivo → MiraAdapter → HDMI → Capture Card → Nitro

    ![HDMI Video Capture Card,KEHIPI 4K HDMI to USB Capture Card Full HD Video Capture Device with Audio via DSLR, Camcorder, Action Cam for Streaming, Compatible with Switch, PS4/5, Xbox One, More (4K 30fp - Newegg.com](https://images.openai.com/static-rsc-4/hZt_ZIL57zYjQbZTfRzb_eVoBVu2e4IiH85M_kS6cGUSUcdg5aXEHWYqubO0x_x8d2WSLZ8ui62tkqs_JbPOjNoswhk7pcDDOEtE5NBd-lwgM93n1W0L-ArGlv2DwOWziG8Te8_iirXHKtESbOb2lERX3DcOB35ofhfbxPIcZ7ok2Nzs6gFasW0gVkfyTK4I?purpose=fullsize)

* ใช้ Capture Card ที่คุณมี

* รับภาพเข้าผ่าน OBS

* มือถือไม่ต้องเสียบสาย USB กับคอม

* ต้องมีไฟเลี้ยงให้ MiraAdapter

    วิธีที่ 3 — Miracast เข้าคอมโดยตรง

    Vivo → Wi-Fi Miracast → Nitro

    ![Androidスマホ画面をWindowsで表示 (ワイヤレスディスプレイ プロジェクション) - がとらぼ](https://images.openai.com/static-rsc-4/ydBifemmXZ7c2LmpJccW-HfKHyW692TCK0fgvZ3ZDjmbnVUDkS0YU7uU0meKo8vBzvAGrVko0NAXpEmM3dCOOlBJJNOJ5hAkJtLorFP6vFh1_qDJssbpplGEOaqWQ37iy8nIlI4ts8Cs88aCAwFweM4hV3TtigKmndi_xyYuueRYfaQZJVcprrUqR_TVYDcE?purpose=fullsize)

* ไม่ต้องใช้ HDMI Capture

* ต้องตรวจว่า Nitro รองรับ Miracast หรือไม่

* Windows ต้องติดตั้ง Wireless Display

* ลองกด `Win + K` ตรวจสอบก่อน Microsoft Support +1

### 6\. ลำดับติดตั้งที่ผมแนะนำสำหรับเครื่องคุณ

อย่าเพิ่งติดตั้งทั้งหมดในครั้งเดียว ผมแนะนำลำดับนี้:

* ตรวจ Capture Card ใน Device Manager

* ทดสอบด้วย Windows Camera

* ติดตั้ง OBS และรับภาพ HDMI

* ติดตั้ง scrcpy เพื่อทดสอบ USB Vivo

* ตั้ง EZMira และทดสอบ MiraAdapter

* เมื่อตัวรับภาพทำงานแล้ว ค่อยติดตั้ง VDD

* ตั้ง Virtual Display เป็น 1920×1080 60Hz

* หากต้องการส่งภาพไปอีกเครื่อง ค่อยเพิ่ม Sunshine + Moonlight

ข้อควรระวัง: ก่อนอัปเดตไดรเวอร์ GPU หรือ Chipset ครั้งใหญ่ ให้ถอน VDD ออกก่อน แล้วค่อยติดตั้งกลับหลังอัปเดต เพราะผู้พัฒนาแจ้งว่าอาจเกิดปัญหาจอดำหรือการจัดลำดับจอผิดได้ GitHub

### 7\. ขอให้เริ่มจากการตรวจอุปกรณ์จริงก่อน

ผมอยากแยกให้ได้ว่าเครื่องคุณเห็น Capture Card เป็นกล้อง USB แล้วหรือยัง และไม่ให้ Driver Booster ไปแก้ไดรเวอร์ที่ไม่เกี่ยวข้อง

สถานะ Capture Card

ยังไม่ได้เสียบ

เสียบแล้ว แต่ไม่พบใน Device Manager

พบเป็น USB Video / USB Camera

พบใน OBS และเห็นภาพแล้ว

วิธีรับภาพที่ต้องการ

USB โดยตรงผ่าน scrcpy

ไร้สายผ่าน MiraAdapter

ต้องการใช้ทั้งสองวิธี

จอเสมือน

ต้องการ Display 2 แบบเสมือน

แค่หน้าต่างภาพลอยบนจอหลัก

ส่งภาพไปอีกเครื่อง

ส่งข้อมูลเพื่อจัดชุดติดตั้ง

ผมจะยึดหลักให้ใช้โปรแกรมน้อยที่สุด แต่ได้ทั้งภาพ USB, ภาพไร้สาย, จอเสมือน และการบันทึกภาพ โดยไม่กระทบจอ 1920×1200 165Hz เดิมของ Nitro ครับ



ประมวลผล 24s

จัดให้ครับ เราจะติดตั้งแบบเป็นขั้นตอน โดยให้ Nitro 16-A41 เป็นเครื่องหลัก และรักษาจอเดิม 1920×1200 165Hz ไว้เหมือนเดิม ไม่ไปเปลี่ยนไดรเวอร์การ์ดจอโดยไม่จำเป็น

ผมจะให้ทำทีละช่วง และมีจุดตรวจสอบก่อนผ่านไปช่วงต่อไป เพื่อป้องกันจอดำหรือไดรเวอร์ชนกัน

### แผนติดตั้งทั้งหมด

### ระบบที่เราจะสร้าง

จอหลัก Nitro

### 1920 × 1200 / 165Hz

คงค่าจอเดิม ไม่เปลี่ยนสัดส่วน

Display 2 — Virtual Monitor

### 1920 × 1080 / 60Hz

จอเสมือน 16:9 สำหรับวางภาพและส่งต่อ

ช่องรับภาพ 2 ทาง

USB

Vivo → scrcpy

ไร้สาย

Vivo → MiraAdapter

OBS Studio

รับภาพจาก Capture Card หรือ scrcpy จัดภาพโดยไม่ยืด ไม่ตัดขอบ และบันทึกได้

ข้อสำคัญ: USB และ MiraAdapter เป็นช่องทางรับภาพสองแบบ เราสามารถเตรียมให้ใช้สลับกันได้ และเพิ่มเป็นสองแหล่งภาพใน OBS ได้ด้วย แต่ไม่ได้หมายความว่าภาพเดียวกันจะรวมกันเองโดยอัตโนมัติ

### ขั้นที่ 1 — เตรียม Windows ก่อนติดตั้ง

เริ่มจากขั้นนี้ก่อน

### 1.1 ตรวจสอบเวอร์ชัน Windows

กด `Win + R` แล้วพิมพ์

```text
winver
```

ตรวจว่าเป็น Windows 11 Pro และจดเวอร์ชันกับ OS Build ไว้

### 1.2 สร้างจุดคืนค่าระบบ

* กด Start

* ค้นหา `Create a restore point`

* เลือกไดรฟ์ C:

* กด Configure หากยังไม่ได้เปิด System Protection

* กด Create

* ตั้งชื่อ `Before Virtual Display`

### 1.3 สำรองข้อมูลไดรเวอร์

เปิด Terminal หรือ CMD แบบ Administrator แล้วใช้คำสั่ง:

```cmd
mkdir C:\DriverBackup
pnputil /export-driver * C:\DriverBackup
```

เป็นการสำรองไดรเวอร์ที่ติดตั้งอยู่ใน Windows ไว้ก่อน

### 1.4 ตรวจสอบไดรเวอร์เดิม

เปิด Device Manager แล้วตรวจ:

* Display adapters

* Cameras

* Sound, video and game controllers

* Universal Serial Bus controllers

* Network adapters

ยังไม่ต้องกด Update All ใน Driver Booster 14 Pro

ตอนนี้ยังไม่ต้องแก้ AMD Graphics, RTX 4050, Chipset หรือ Killer Wi-Fi

### ขั้นที่ 2 — ติดตั้งระบบรับภาพ Vivo ผ่าน USB

ไม่ต้องใช้ Capture Card

ใช้ scrcpy เป็นตัวรับภาพโดยตรง

![androidとパソコンをUSBでつなぐお手軽ミラーリング【Windows編】 - K06のBLOG](https://images.openai.com/static-rsc-4/Qqm0XLi409X_0qje7cVOfrCG87zgVP3rSsVG5NcBc4pXCoHFzvH-jbQq_ftNSpWngEj0PVQaYNPV7jB1XUt6gJhqvMSAvDdF__t6Q3aruuITjgyRMwXMlCBJOOYNcRRk8nlZs6yYwT6JZWjOH0rsSY9jywYbdXa25YNcBYeIKo6OpxyAwoDNtsPvcpMpjs4m?purpose=fullsize)

ดาวน์โหลด

[scrcpy รุ่นล่าสุดจาก GitHub ทางการ](https://github.com/Genymobile/scrcpy/releases?utm_source=chatgpt.com)

ฟรี ไม่ต้องลงแอปบน Vivo และรองรับการควบคุมมือถือผ่านคอมพิวเตอร์

### 2.1 เปิด USB Debugging บน Vivo Y17s

บนมือถือ:

* เข้า Settings

* About phone

* กด Build number 7 ครั้ง

* กลับไป Settings

* เข้า System management หรือ Additional settings (ชื่อเมนูอาจต่างตาม Funtouch OS)

* Developer options

* เปิด USB debugging

ถ้าเจอหน้าต่าง Allow USB debugging ให้กด Allow เฉพาะคอมเครื่องที่เชื่อถือได้

### 2.2 ต่อ USB เข้าคอม

![Fans aren't happy with the latest Samsung DeX changes, but it's still glorious - PhoneArena](https://images.openai.com/static-rsc-4/1XWSC0fvRAlFKCdO1wcBY-vF5TAbMNc72Q1BujT82F_8mm7mY9Pb40OV6-WaZlYaB5phDiHS_HaGKnF0YFKWgif2pvp99BU9gkHYII7KbCGKWM0ZDYlSQZyrGedOLAVbAkhEsAYe7-ukJzaFZ3dRS1WDuxOz40fVHL48IzPk3PGgG7ywJqT3EzLC1z_tNHqb?purpose=fullsize)

* ใช้สาย USB-C ที่ส่งข้อมูลได้

* เสียบ USB 3.0 ช่องที่ 2 ของ Nitro

* เลือก File Transfer (ถ้ามีตัวเลือก USB)

* ยืนยัน USB debugging บนมือถือ

เปิดโฟลเดอร์ scrcpy ที่แตกไฟล์แล้ว เปิด Terminal ในโฟลเดอร์นั้น

ตรวจด้วย:

```cmd
adb devices
```

ถ้าขึ้นชื่อเครื่องและสถานะ `device` ถือว่าผ่าน

จากนั้นเรียก:

```cmd
scrcpy
```

ภาพ Vivo ปรากฏเป็นหน้าต่างบน Windows โดยไม่ผ่าน MiraAdapter

หากต้องการกำหนดขนาดภาพและอัตราเฟรมเพื่อทดสอบ:

```cmd
scrcpy --max-size=1920 --max-fps=60
```

นี่เป็นการจำกัดภาพที่ส่งมายังคอม ไม่ได้ทำให้จอมือถือเปลี่ยนความละเอียด

คู่มือ: [scrcpy สำหรับ Windows](https://github.com/Genymobile/scrcpy/blob/master/doc/windows.md?utm_source=chatgpt.com) และ [การเชื่อมต่อ USB](https://github.com/Genymobile/scrcpy/blob/master/doc/connection.md?utm_source=chatgpt.com) GitHub +1

### ขั้นที่ 3 — เตรียม MiraAdapter และ HDMI Capture Card

ทำหลังจาก USB ผ่านแล้ว

![MonQiQi New 2566 G2 Bluetooth Transmitter AndroidIOS Wifi Support HDMI Airplay TV Stick 2.4G 4K Wireless Receiver 1080P Anycast Mirror Link Transmitter Adapter Bangkok Ready Stock - MonQiQi Audio Professional Store - ThaiPick](https://images.openai.com/static-rsc-4/d9Q1xHN0XYkvEU_wY7roo87Esp1fumG0UICLPd3C84OptOvPU0WdHx-fDJrPH-47osU_IaM6gaBAFd4KnI_oFkFYBg7NYtEm4GCv-zWQZYwfy0Bex-CFn3AlUFThKW0DJBp7hDSyLZyCA5raKxESaevEwzfGQw5UQ6tPCtFY17HLZNbvp1BKX8ih2u8H_oOC?purpose=fullsize)

### 3.1 ผังเสียบสายจริง

Vivo Y17s

ส่งภาพไร้สายผ่าน Miracast / EZMira

MiraAdapter

HDMI OUT และไฟเลี้ยง Micro USB 5V

HDMI Capture Card

HDMI IN → USB 3.0

Nitro 16 — USB 3.0 ช่องที่ 1

Windows ตรวจพบเป็น USB Video / Camera

จุดที่ต้องระวัง:

* MiraAdapter ต้องรับไฟ 5V แยก

* HDMI จาก MiraAdapter ต้องเข้า HDMI IN ของ Capture Card

* สาย USB จาก Capture Card เข้า Nitro

* ห้ามนำ HDMI OUT ของ Nitro ไปต่อ HDMI IN ของ Capture Card เพราะจะไม่ใช่การส่งภาพจากมือถือ

### 3.2 ตรวจพบ Capture Card

เมื่อเสียบแล้ว:

* คลิกขวา Start

* เลือก Device Manager

* Action

* Scan for hardware changes

* ตรวจหมวด Cameras และ Sound, video and game controllers

ชื่ออาจ