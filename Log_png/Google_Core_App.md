---
ฟังชั่นทำฝั่งดึงตารางExcler Google 
---

```.md
ได้ครับ เราจะทำเวอร์ชันแรกให้ ไม่บันทึกลิงก์ลง Google Sheets รับลิงก์มาแล้วแสดงตัวอย่าง ตรวจสอบก่อน จากนั้นเลือกคัดลอกหรือเปิด LINE เพื่อส่งเอง

มีข้อแตกต่างที่ควรรู้ก่อน: เวอร์ชันนี้จะใช้วิธี กดแชร์ → คัดลอกลิงก์ → เปิดเครื่องมือ → วางลิงก์ เพราะ Web App ของ Apps Script ธรรมดายังไม่ใช่แอปที่รับรายการแชร์จาก Android Share Sheet ได้โดยตรง ส่วนการส่ง LINE จะเปิดหน้าส่งต่อให้เลือกห้องแชตเอง ไม่ส่งเข้ากลุ่มโดยอัตโนมัติ

### 1\. สร้าง Google Apps Script

เปิด [Google Apps Script](https://script.google.com/?utm_source=chatgpt.com) แล้วกด โปรเจกต์ใหม่

จะมีไฟล์ `Code.gs` ให้ลบโค้ดเดิม แล้ววางโค้ดนี้:

```javascript

function doGet() {
  return HtmlService.createHtmlOutputFromFile('Index')
    .setTitle('รับลิงก์และส่ง LINE')
    .addMetaTag('viewport', 'width=device-width, initial-scale=1');
}
```

### 2\. สร้างหน้าเครื่องมือ

กดเครื่องหมาย `+` ข้าง Files → เลือก HTML → ตั้งชื่อ `Index`

วางโค้ดทั้งหมดนี้:

```html

<!DOCTYPE html>
<html>
<head>
  <base target="_top">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body {
      font-family: Arial, sans-serif;
      background: #f4f6f8;
      margin: 0;
      padding: 16px;
      color: #222;
    }
    .app {
      max-width: 520px;
      margin: auto;
    }
    .panel {
      background: white;
      border-radius: 16px;
      padding: 16px;
      margin-bottom: 14px;
      box-shadow: 0 2px 8px #00000010;
    }
    h2 { margin-top: 0; }
    textarea {
      width: 100%;
      box-sizing: border-box;
      min-height: 100px;
      padding: 12px;
      border: 1px solid #ccc;
      border-radius: 10px;
      font-size: 16px;
      resize: vertical;
    }
    button {
      width: 100%;
      padding: 14px;
      border: 0;
      border-radius: 10px;
      margin-top: 10px;
      font-size: 16px;
      font-weight: bold;
      cursor: pointer;
    }
    .copy { background: #e8eef8; color: #234b83; }
    .line { background: #06c755; color: white; }
    .clear { background: #eee; color: #444; }
    .preview {
      overflow-wrap: anywhere;
      background: #f5f5f5;
      padding: 12px;
      border-radius: 10px;
    }
    img {
      width: 100%;
      border-radius: 10px;
      margin-top: 10px;
    }
    .muted { color: #777; font-size: 13px; }
    a { overflow-wrap: anywhere; }
    #status { color: #16834a; font-size: 14px; }
  </style>
</head>
<body>
<div class="app">

  <div class="panel">
    <h2>🔗 รับลิงก์เว็บ / YouTube</h2>
    <p class="muted">คัดลอกลิงก์จากเว็บหรือ YouTube แล้วนำมาวางด้านล่าง</p>

    <textarea id="urlInput"
      placeholder="วางลิงก์ที่นี่..."></textarea>

    <button class="copy" onclick="pasteLink()">
      📋 วางจากคลิปบอร์ด
    </button>

    <button class="line" onclick="makePreview()">
      🔍 ตรวจสอบตัวอย่าง
    </button>
  </div>

  <div class="panel" id="previewPanel" hidden>
    <h3>👁️ ตัวอย่างก่อนส่ง</h3>
    <div id="preview" class="preview"></div>

    <button class="copy" onclick="copyLink()">
      📋 คัดลอกลิงก์
    </button>

    <button class="line" onclick="sendLine()">
      LINE ส่งให้เพื่อน / กลุ่ม
    </button>

    <button class="clear" onclick="clearAll()">
      ล้างข้อมูล
    </button>

    <p id="status"></p>
  </div>

</div>

<script>
  let currentUrl = '';

  function validUrl(value) {
    try {
      const u = new URL(value);
      return u.protocol === 'https:' || u.protocol === 'http:';
    } catch (e) {
      return false;
    }
  }

  function getYoutubeId(url) {
    try {
      const u = new URL(url);
      const host = u.hostname.replace(/^www\./, '');

      if (host === 'youtu.be')
        return u.pathname.split('/')[1] || '';

      if (host.endsWith('youtube.com')) {
        if (u.searchParams.get('v'))
          return u.searchParams.get('v');

        const parts = u.pathname.split('/');
        if (['shorts', 'embed', 'live'].includes(parts[1]))
          return parts[2] || '';
      }
    } catch (e) {}
    return '';
  }

  async function pasteLink() {
    try {
      const text = await navigator.clipboard.readText();
      document.getElementById('urlInput').value = text;
      makePreview();
    } catch (e) {
      alert('ไม่สามารถอ่านคลิปบอร์ดได้ กรุณากดค้างในช่องแล้วเลือกวาง');
    }
  }

  function makePreview() {
    const value = document.getElementById('urlInput').value.trim();

    if (!validUrl(value)) {
      alert('กรุณาวางลิงก์เว็บที่ถูกต้องก่อน');
      return;
    }

    currentUrl = value;
    const box = document.getElementById('preview');
    const panel = document.getElementById('previewPanel');

    box.replaceChildren();

    const u = new URL(value);
    const title = document.createElement('b');
    title.textContent = 'เว็บไซต์: ' + u.hostname;
    box.appendChild(title);

    const link = document.createElement('p');
    link.textContent = value;
    box.appendChild(link);

    const yt = getYoutubeId(value);

    if (yt && /^[a-zA-Z0-9_-]{11}$/.test(yt)) {
      const img = document.createElement('img');
      img.src = 'https://img.youtube.com/vi/' +
        encodeURIComponent(yt) + '/hqdefault.jpg';
      img.alt = 'YouTube thumbnail';
      box.appendChild(img);

      const note = document.createElement('p');
      note.className = 'muted';
      note.textContent = 'พบลิงก์ YouTube พร้อมภาพตัวอย่าง';
      box.appendChild(note);
    } else {
      const note = document.createElement('p');
      note.className = 'muted';
      note.textContent =
        'ลิงก์เว็บทั่วไป: ตรวจสอบ URL แล้ว กดเปิดลิงก์เพื่อตรวจเนื้อหาได้';
      box.appendChild(note);
    }

    const open = document.createElement('a');
    open.href = value;
    open.target = '_blank';
    open.rel = 'noopener noreferrer';
    open.textContent = 'เปิดเว็บไซต์ต้นฉบับ';
    box.appendChild(open);

    panel.hidden = false;
    document.getElementById('status').textContent =
      'ตรวจสอบลิงก์แล้ว พร้อมคัดลอกหรือส่ง LINE';
  }

  async function copyLink() {
    if (!currentUrl) return;

    try {
      await navigator.clipboard.writeText(currentUrl);
    } catch (e) {
      const t = document.createElement('textarea');
      t.value = currentUrl;
      document.body.appendChild(t);
      t.select();
      document.execCommand('copy');
      t.remove();
    }

    document.getElementById('status').textContent =
      'คัดลอกลิงก์แล้ว';
  }

  function sendLine() {
    if (!currentUrl) return;

    const message = 'ลิงก์ที่แชร์มา:\n' + currentUrl;
    const lineUrl =
      'https://line.me/R/share?text=' +
      encodeURIComponent(message);

    window.open(lineUrl, '_blank');
  }

  function clearAll() {
    currentUrl = '';
    document.getElementById('urlInput').value = '';
    document.getElementById('preview').replaceChildren();
    document.getElementById('previewPanel').hidden = true;
    document.getElementById('status').textContent = '';
  }
</script>
</body>
</html>
```

### 3\. เผยแพร่เป็นเครื่องมือใช้งาน

* กดบันทึกโปรเจกต์ ตั้งชื่อ เช่น `LINE Link Helper`

* กด Deploy → New deployment

* เลือกชนิด Web app

* Execute as เลือก Me

* Who has access เลือกเฉพาะตัวเลือกที่อนุญาตให้บัญชีของคุณเข้าใช้ได้ เช่น Only myself

* กด Deploy และอนุญาตสิทธิ์ตามที่ Google แสดง

* เปิด Web app URL ที่ได้จากการ Deploy ใน Chrome บนมือถือ

อ้างอิงวิธีเผยแพร่จากคู่มือ Google: [Deploy Apps Script as a web app](https://developers.google.com/apps-script/guides/web?authuser=4&utm_source=chatgpt.com) และ [HTML Service](https://developers.google.com/apps-script/guides/html?utm_source=chatgpt.com).

### 4\. วิธีใช้งานจริงบนมือถือ

* เปิด YouTube หรือเว็บที่ต้องการ

* กดแชร์ → คัดลอกลิงก์

* เปิดเครื่องมือที่สร้างไว้ แล้วกดวางจากคลิปบอร์ด

* กดตรวจสอบตัวอย่าง

* กด LINE ส่งให้เพื่อน / กลุ่ม → เลือกกลุ่ม → กดส่งใน LINE

LINE รองรับ URL สำหรับเปิดหน้าต่างเลือกเพื่อนหรือกลุ่ม พร้อมข้อความที่เตรียมไว้ แต่ผู้ใช้ยังต้องกดยืนยันส่งเอง LINE Developers +1

### ข้อจำกัดของเวอร์ชันนี้

* YouTube: แสดงภาพปกวิดีโอได้ ถ้าลิงก์เป็นรูปแบบที่ตรวจจับได้

* เว็บทั่วไป: แสดงชื่อโดเมนและ URL แต่ยังไม่ดึงภาพปกหรือชื่อบทความอัตโนมัติ

* JPG, PNG, PDF: ถ้าเป็นลิงก์ไฟล์ จะส่ง URL ไป LINE ไม่ได้อัปโหลดไฟล์แนบจริง

* ไม่บันทึกประวัติลงชีตหรือฐานข้อมูลใด ๆ

ขั้นต่อไปที่ผมอยากทำเพิ่ม: ให้ Android มีชื่อเครื่องมือนี้ปรากฏในเมนูแชร์โดยตรง และเมื่อกดแชร์ YouTube หรือเว็บ จะเปิดหน้าพรีวิวพร้อมปุ่มส่ง LINE ทันที โดยไม่ต้องคัดลอกแล้วสลับแอปเอง ซึ่งต้องทำเป็น PWA/Share Target เพิ่มจากเวอร์ชัน Apps Script นี้ครับ


```
