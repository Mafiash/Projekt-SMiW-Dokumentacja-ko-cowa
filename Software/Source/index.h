const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="pl">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Smart Schody - Panel</title>
  <style>
    body { font-family: 'Segoe UI', Arial, sans-serif; background-color: #1a1a1a; color: #f0f0f0; text-align: center; margin: 0; padding: 20px; }
    h1 { color: #00d2ff; margin-bottom: 5px; }
    h3 { color: #aaa; margin-top: 0; }
    
    .card { background-color: #2d2d2d; max-width: 400px; margin: 20px auto; padding: 25px; border-radius: 15px; box-shadow: 0 10px 20px rgba(0,0,0,0.5); }
    
    .sensor-grid { display: grid; grid-template-columns: 1fr 1fr; gap: 15px; margin-bottom: 20px; }
    .sensor-box { background: #383838; padding: 15px; border-radius: 10px; }
    .val { font-size: 1.8rem; font-weight: bold; color: #fff; }
    .label { font-size: 0.9rem; color: #888; }

    .btn-group { display: flex; justify-content: center; gap: 10px; margin: 20px 0; }
    button { padding: 12px 20px; font-size: 16px; border: none; border-radius: 8px; cursor: pointer; transition: 0.2s; font-weight: bold; width: 100%; }
    
    .active { border: 2px solid white; transform: scale(1.05); }
    .btn-auto { background-color: #007bff; color: white; }
    .btn-man { background-color: #ffc107; color: black; }
    
    .manual-controls { background: #444; padding: 15px; border-radius: 10px; margin-top: 15px; }
    .toggle-btn { padding: 15px; width: 100%; margin-bottom: 10px; font-size: 1.2rem; }
    .on { background-color: #28a745; color: white; }
    .off { background-color: #dc3545; color: white; }
    
    input[type="color"] { width: 100%; height: 50px; border: none; cursor: pointer; border-radius: 5px; }
    input[type=range] { width: 100%; margin: 10px 0; }
  </style>
</head>
<body>

  <h1>Smart Schody</h1>
  <h3>Panel Sterowania</h3>

  <div class="card">
    <div class="sensor-grid">
      <div class="sensor-box">
        <div class="label">Temperatura</div>
        <div class="val"><span id="temp">--</span> °C</div>
      </div>
      <div class="sensor-box">
        <div class="label">Jasność (LDR)</div>
        <div class="val"><span id="ldr">--</span></div>
      </div>
    </div>
    
    <div style="font-size: 0.9rem; color: #bbb; margin-bottom: 15px;">
      Status Radaru: <span id="radar" style="font-weight:bold; color:orange">--</span>
    </div>

    <hr style="border-color: #444;">

    <div class="btn-group">
      <button id="btnAuto" class="btn-auto" onclick="setMode('auto')">TRYB AUTO</button>
      <button id="btnMan" class="btn-man" onclick="setMode('manual')">TRYB RĘCZNY</button>
    </div>

    <div id="manualPanel" class="manual-controls" style="display:none;">
      <button id="mainSwitch" class="toggle-btn off" onclick="toggleLed()">LED: WYŁĄCZONE</button>
      <label style="display:block; margin: 10px 0 5px;">Wybierz Kolor:</label>
      <input type="color" id="colorPicker" value="#ffffff" onchange="updateColor()">
      <label style="display:block; margin: 15px 0 5px;">Jasność: <span id="brightVal">50</span></label>
      <input type="range" min="10" max="255" value="50" id="brightSlider" oninput="updateBrightness(this.value)" onchange="updateBrightness(this.value)">
    </div>
  </div>

<script>
  let isManualOn = false;

  setInterval(getData, 1500);

  function getData() {
    fetch('/status').then(response => response.json()).then(data => {
      document.getElementById("temp").innerText = data.temp.toFixed(1);
      document.getElementById("ldr").innerText = data.ldr;
      document.getElementById("radar").innerText = data.radar ? "Wykryto Ruch" : "Czuwanie";
      
      if (data.modeAuto) {
        document.getElementById("btnAuto").classList.add("active");
        document.getElementById("btnMan").classList.remove("active");
        document.getElementById("manualPanel").style.display = "none";
      } else {
        document.getElementById("btnAuto").classList.remove("active");
        document.getElementById("btnMan").classList.add("active");
        document.getElementById("manualPanel").style.display = "block";
      }
    });
  }

  function setMode(mode) {
    fetch('/set?mode=' + mode);
    setTimeout(getData, 200);
  }

  function toggleLed() {
    isManualOn = !isManualOn;
    let btn = document.getElementById("mainSwitch");
    if(isManualOn) {
      btn.innerText = "LED: WŁĄCZONE";
      btn.className = "toggle-btn on";
      fetch('/set?manual=1');
    } else {
      btn.innerText = "LED: WYŁĄCZONE";
      btn.className = "toggle-btn off";
      fetch('/set?manual=0');
    }
  }

  function updateColor() {
    var color = document.getElementById("colorPicker").value.substring(1);
    fetch('/set?color=' + color);
    if(!isManualOn) toggleLed();
  }

  function updateBrightness(val) {
    document.getElementById("brightVal").innerText = val;
    fetch('/set?brightness=' + val);
    if(!isManualOn) toggleLed(); 
  }
  
  window.onload = getData;
</script>
</body>
</html>
)rawliteral";