```.html
<!DOCTYPE html>
<html lang="th">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Ohm Calculator</title>

<style>
body{
    font-family:sans-serif;
    background:#111;
    color:white;
    padding:20px;
}
.box{
    background:#1e1e1e;
    padding:20px;
    border-radius:12px;
    max-width:400px;
}
input{
    width:100%;
    padding:10px;
    margin-top:10px;
    font-size:18px;
}
button{
    margin-top:15px;
    padding:10px;
    width:100%;
    font-size:18px;
}
.result{
    margin-top:20px;
    font-size:22px;
}
</style>
</head>

<body>

<div class="box">
    <h2>คำนวณกฎโอห์ม</h2>

    <label>แรงดันไฟ (V)</label>
    <input type="number" id="volt" value="4.2">

    <label>ความต้านทาน (Ω)</label>
    <input type="number" id="res" value="7">

    <button onclick="calc()">คำนวณ</button>

    <div class="result" id="out"></div>
</div>

<script>
function calc(){

    let V = parseFloat(document.getElementById("volt").value);
    let R = parseFloat(document.getElementById("res").value);

    let I = V / R;
    let P = (V*V) / R;

    document.getElementById("out").innerHTML =
        "กระแส = " + I.toFixed(2) + " A<br>" +
        "กำลัง = " + P.toFixed(2) + " W";
}
</script>

</body>
</html>

```
