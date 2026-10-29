```
เข้าใจแล้ว แบบที่คุณต้องการไม่จำเป็นต้องสร้างระบบใหญ่หรือเปิดให้ใช้งานผ่านอินเทอร์เน็ตเลยครับ เพราะเครื่องลูกค้า Windows 10 อยู่ข้าง ๆ คุณ และคุณใช้ Windows 11 บน Asus Nitro 16 ต่อกับ Canon G2010 ผ่าน USB

ผมแนะนำให้ทำเป็น ระบบรับงานพิมพ์ผ่านเว็บในวง LAN พร้อมระบบสแกนเอกสารและ OCR โดยเริ่มจากทำให้ไดรเวอร์ G2010 ทำงานครบก่อน แล้วค่อยต่อยอดเว็บ

### 1\. แก้ไดรเวอร์ให้ครบก่อน

จากที่คุณเล่า หลังติดตั้งไดรเวอร์จาก USB แล้วมีอุปกรณ์ขึ้น Error อีก 4–5 ตัว อาจเกิดจากไดรเวอร์ยังไม่ครบ หรือ Windows ตรวจพบอุปกรณ์ที่ยังไม่มีไดรเวอร์ แต่ยังฟันธงไม่ได้ว่าเป็นส่วนของ Canon ทั้งหมด

ทำตามนี้ก่อน:

![Solved: missing several drivers after clean install - HP Support Community - 9511586](https://images.openai.com/static-rsc-4/H-UHJG3dPOXhsO8vTR2rv8UiKxSy-hD-LKTG6tXow87FHjifUjs4zLxGtVbrP1hoJ997I-OSKZXf8_bNZZPKBj5Mnq2Lu8aBNyJ5sgnZ8X6jML22PJhKGSNtV2OvxKar-yyAp5Oa44gSGeSpfc2RRCZrHB6SC0r9QWp7hHt5h-MhPVz_7xhxswLfyO-GJSxe?purpose=fullsize)

ขั้นที่ 1 — ตรวจอุปกรณ์ที่มี Error

* กด `Win + X`

* เลือก Device Manager (ตัวจัดการอุปกรณ์)

* มองหาเครื่องหมายสามเหลี่ยมสีเหลือง

* คลิกขวาอุปกรณ์ที่มีปัญหา เลือก Properties แล้วดูแท็บ General

อย่าเพิ่งถอนอุปกรณ์ทั้งหมด เพราะบางตัวอาจเป็นอุปกรณ์ของโน้ตบุ๊กเอง

![Hướng dẫn cài đặt driver Canon PIXMA G2010](https://images.openai.com/static-rsc-4/ZDUnRGROQN5pnVhQUHgk8yf6xOHJJSiQd261YqlK2s-d65-RrwU7N4wx1u2RPjp11eIuDB7tqEwB5weKMatK_pMemSrgNB-CKUt_5kHCKx6CNAVyWFPjNjYYvDaC4e_fby31pKhuOQNys0czRrvAfRwdjReGphTKZ48lzGxHORjxbi0Q4Ht7QxgX-ebV4ZiE?purpose=fullsize)

ขั้นที่ 2 — ติดตั้งไดรเวอร์ Canon ที่ถูกต้อง

ดาวน์โหลดจากเว็บทางการที่ให้ไว้ก่อนหน้า เลือก MP Drivers หรือแพ็กเกจติดตั้ง G2010 สำหรับ Windows 11 แล้วทำตามตัวติดตั้ง

หากเครื่องเคยลงไดรเวอร์จาก USB มาแล้ว ไม่จำเป็นต้องลบทุกอย่างก่อน ให้ตรวจว่าติดตั้ง MP Drivers สำเร็จหรือยัง

![Jual FixPrint Printer Canon G2010 - Ink Tank All In One Print Scan Copy USB Windows - Garansi 3 Tahun - Roller Kertas Super Kuat | Shopee Indonesia](https://images.openai.com/static-rsc-4/L_3Uz1FN1App-KjH2_Icuz7muvfVhs2CyZ_Mmw_1aQ7mX0rYxGwEbIFKD2orJ-LopZbGG9AfuqDqVSSDKdW5rjz5X9aSLjPzZ_QPw7QVPkOoZULf514UMZ6t1iFz_Sf8LsVJ7Hx4jx3Q-SlXSZS8luCavWy6pqmBqNwlumDqg32P13wYQcsGLQ7jDIUMDluS?purpose=fullsize)

ขั้นที่ 3 — ทดสอบทั้งพิมพ์และสแกน

* พิมพ์หน้าทดสอบจาก Windows

* เปิด Windows Scan หรือโปรแกรมสแกนของ Canon แล้วลองสแกนเอกสาร

* หากสแกนได้แต่พิมพ์ไม่ได้ ให้ตรวจส่วนไดรเวอร์เครื่องพิมพ์และคิวงานแยกกัน

### 2\. ระบบส่งงานพิมพ์: ไม่ต้องเขียนเว็บเองตั้งแต่ต้น

มี 2 วิธีที่เหมาะกับคุณ

![Hướng dẫn bạn cách chia sẻ máy in trên Windows 11](https://images.openai.com/static-rsc-4/k8Ed8t5xkF9kpPArtqV554oYQoRFlOr-YOoIQ1QgtVFCWtaeU7WYFUPX2SGBzZAP9Toosts5fGPBpTP41qGObjKEGTMl3gnm5o90R5_R14FUB4cdfVtACioBBPeU_A7vszmL-nlGYZxpV7j2eW-U3SACbbgxRdCOCtTfvxXaDohOHDlRyfRvuiyj_CIVrgc6?purpose=fullsize)

วิธี A — แชร์เครื่องพิมพ์ผ่าน Windows

เริ่มได้เร็วที่สุด

แชร์ Canon G2010 จาก Nitro 16 ให้เครื่อง Windows 10 มองเห็นผ่านเครือข่ายเดียวกัน จากนั้นลูกค้าก็สั่งพิมพ์จาก Word, PDF หรือเว็บได้เหมือนเครื่องพิมพ์ทั่วไป Microsoft Support

[คู่มือแชร์เครื่องพิมพ์จาก Microsoft](https://support.microsoft.com/en-us/windows/hardware/printer/share-a-printer-as-a-network-printer?utm_source=chatgpt.com)

![Web Online Help - OP Print Release Station](https://images.openai.com/static-rsc-4/WQlKxOaapFzZxbw45d2jthalMmx9tduWBODW1PC4rbhWI_8Lr0pjSufDbo7OKAJMKepsxI7T9TkcDmdQDRikmlmrcyv40dHv-GZxh8hOPAFrXbs6-cQO1SBvXBMw_MSoQyyXiz-GckuvQjWmyueMfu3uUn1YUVnuwc6eF5ftL7Z4BD-PW_yRyfLA7kQ7thKI?purpose=fullsize)

วิธี B — เว็บรับงานผ่าน QR Code

เหมาะกับการทำเป็นบริการของคุณเอง ลูกค้าสแกน QR Code ส่งไฟล์ PDF หรือรูปภาพ แล้วคุณตรวจตัวอย่างก่อนกดพิมพ์ มีประวัติงานและสถานะคิวได้

เหมาะเป็นขั้นที่สอง เพราะต้องมีเว็บรับไฟล์และส่วนควบคุมคิวพิมพ์เพิ่ม

ข้อสำคัญ: G2010 ต่อ USB กับ Nitro 16 ได้เครื่องเดียว ดังนั้นเครื่อง Nitro ต้องเปิดอยู่และเชื่อมต่อเครือข่ายตลอดเวลาที่ลูกค้าจะพิมพ์ ส่วนการสแกนให้ใช้โปรแกรมบน Nitro แล้วบันทึกเอกสารไว้ในโฟลเดอร์ของลูกค้า

### 3\. สแกนเอกสารแล้วแก้ข้อความได้ รวมถึงบัตรประชาชนหน้า–หลัง

ผมแนะนำโปรแกรมฟรี NAPS2 สำหรับสแกนและ OCR มีความสามารถแปลงเอกสารเป็น PDF ที่ค้นหาข้อความได้ และสแกนหลายรอบต่อเนื่องได้ NAPS2 +1

[ดาวน์โหลด NAPS2 จากเว็บไซต์ทางการ](https://www.naps2.com/?utm_source=chatgpt.com)

ลำดับการทำงานที่เหมาะกับคุณ:

* สแกนเอกสารด้วย G2010

* ทำ OCR ภาษาไทยและอังกฤษ

* คัดลอกข้อความไปแก้ไขใน Word หรือโปรแกรมแก้ไขข้อความ

* บันทึกต้นฉบับเป็น PDF และเก็บข้อความที่แก้ไขแล้วแยกไฟล์

* หากเป็นบัตรประชาชน ให้สแกนด้านหน้าและด้านหลังแยกกัน แล้วรวมเป็น PDF สองหน้าได้

หมายเหตุ: OCR อาจอ่านเลขหรือภาษาไทยผิดได้ ต้องตรวจทานก่อนนำข้อมูลไปใช้งาน และควรเก็บเอกสารส่วนบุคคลในโฟลเดอร์ที่จำกัดสิทธิ์

### 4\. ขอข้อมูลจากหน้าจอคุณอีกอย่างเดียว

เพื่อแก้ Error 4–5 ตัวได้ตรงจุดที่สุด ช่วยเปิด Device Manager แล้วถ่ายภาพหน้าจอให้เห็นรายชื่ออุปกรณ์ที่มีเครื่องหมายสามเหลี่ยมสีเหลือง หรือเปิด Properties ของตัวหนึ่งแล้วถ่ายหน้า General ที่มี Device status และรหัส Error มาให้ผมดู

ผมจะช่วยแยกว่าเป็นไดรเวอร์ Canon, USB หรืออุปกรณ์ของ Nitro 16 เอง แล้วบอกว่าตัวไหนควรติดตั้งเพิ่ม ตัวไหนไม่ควรไปยุ่งครับ

```
