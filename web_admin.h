const char WEBADMIN[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="de">
  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Regelungstechnik Experimentierboard</title>
    
    <style>
      html {
        position: relative;
        min-height: 100%;
      }
      body {
        margin-bottom: 60px;
        padding: 15px 15px 0;
      }
      .footer {
        position: absolute;
        bottom: 0;
        width: 100%;
        height: 60px;
        line-height: 60px;
        background-color: #F2EFE9;
      }
      h2 {
        font-size: 2rem;
        margin-bottom: .5rem;
        font-family: inherit;
        font-weight: 500;
        line-height: 1.2;
        color: inherit;
      }
      .form-group{
        margin-bottom: 1rem;
      }
      .btn{
        cursor: pointer;
        display: inline-block;
        font-weight: 400;
        text-align: center;
        white-space: nowrap;
        vertical-align: middle;
        padding: .375rem .75rem;
        font-size: 1rem;
        line-height: 1.5;
        border-radius: .25rem;
        transition: color .15s ease-in-out,background-color .15s ease-in-out,border-color .15s ease-in-out,box-shadow .15s ease-in-out;
        background-color: #F2EFE9;
      }
      .btn:hover{
        color: #080708;
        background-color: #3772FF;
        border-color: #252627;
      }
      .btn:active{
        background-color:#904E55;
      }
      input,
    textarea {
      background: none;
      color: $muted-color;
      font-size: 18px;
      padding: 10px 10px 10px 5px;
      display: block;
      width: $width;
      border: none;
      border-radius: 0;
      border-bottom: 1px solid $muted-color;
      &:focus {
        outline: none;
      }
      &:focus ~ label,
      &:valid ~ label {
        top: -14px;
        font-size: 12px;
        color: $hl-color;
      }
      &:focus ~ .bar:before {
        width: $width;
      }
    }
      code {
        font-size: 80%;
      }
    </style>
  </head>
  <body>
    <header>
      
  </header>
  
  <!-- Begin page content -->
  <main >
            <h2>Controller</h2>
            <form>
              <div class="form-group">
                <p>Bitte wählen Sie den gewünschten Messmodus aus.</p>
                <p>Ein erneuter Klick blendet die Parametrierung ein bzw. aus.</p>
                <button id="Sprungantwort" type="button" class="btn" onclick="chooseSprungantwort()">Sprungantwort</button>
                <button id="Regelung" type="button" class="btn" onclick="chooseRegelung()" >Regelung</button>
                <p>gewählter Messmodus: <span id="choosedMode"></span></p>
              </div>
              <!--
              <div class="form-group">
                <p>Messobjekte: Hier sind die Objekte 1-5 auswählbar</p>
                <label>Messobjekte</label>
                <input id="inputGValue" type="number" min="1" max="5"  value="1">
              </div>
          -->
            <table>
              <div id="conficSprungantwort" class="form-group" style="display:none;">
                
                  <label>Sprungantwort Messzeitraum [ms] </label>
                  <input id="MesszeitraumSprungantwort" placeholder="0" value="10000" onchange="check_number_elements_s()"><br>
                  <label>Auflösung Messung [ms]</label>
                  <input id="MessaufloesungSprungantwort" placeholder="0" value="250" onchange="check_number_elements_s()">
                
              </div>
              <div id="conficRegelung" class="form-group" style="display:none;">
                <div class="form-group">
                  <p>Nachfolgend lassen sich folgende Regler mit ihrer jeweiligen Struktur auswählen:</p>
                  <ul>
                  <li>P-Regler u(t)=K_p * e(t)</li>
                  <li>PI-Regler u(t)=K_p * (e(t) + 1/T_N *  &#x222b; e(&#x3C4;) d&#x3C4;</li>
                  
                  <br>
                  <label for="pregler">P-Regler</label>
                  <input type="radio" id="pregler" name="regler" value="1" checked>
                  <label for="piregler">PI-Regler</label>
                  <input type="radio" id="piregler" name="regler" value="2" >
                  </ul>
                  <label>Sollwert [mV] </label>
                  <input id="SollwertRegelung" placeholder="0" value="2000"><br>
                  <label>Verstärkung </label>
                  <input id="VerstaerkungRegelung" placeholder="0" value="1"><br>
                  <label>Nachstellzeit [ms]</label>
                  <input id="NachstellzeitRegelung" placeholder="0" value="5000"><br>
                  <label>Sprungantwort Messzeitraum [ms] </label>
                  <input id="MesszeitraumRegelung" placeholder="0" value="10000" onchange="check_number_elements_r()"><br>
                  <label>Auflösung Messung [ms]</label>
                  <input id="MessaufloesungRegelung" placeholder="0" value="250" onchange="check_number_elements_r()">
                </div>
              </div>
              </table>
              <button id="submitValues" type="button" class="btn" style="visibility:hidden">Start</button>
              <button id="stopMessung" type="button" class="btn" style="visibility:hidden">Stop</button>
              <button id="gotosettings" type="button" class="btn"><a href="/data">go to data</a></button>
              <p>Achtung: Erst nach Ablauf der Messzeit zur Datenseite wechseln! Wenn die Zeit abgelaufen ist, werden Sie informiert.</p>
              <h2 id="start_alert"></h2>

              
            </form>
    </main>
    
    <footer class="footer">
    </footer>
    
    <script type="text/javascript">
    function check_number_elements_s(){
      var time_temp1 = document.getElementById("MesszeitraumSprungantwort").value;
      var res_temp1 = document.getElementById("MessaufloesungSprungantwort").value;
      if ((time_temp1/res_temp1) > 254){
        alert("Es sind zu viele Messpunkte ausgewählt worden. Die maximale Anzahl an Messpunkten liegt bei 254.");
        hideSubmit();
      }else {
        showSubmit();
      }
    }
    function check_number_elements_r(){
      var time_temp1 = document.getElementById("MesszeitraumRegelung").value;
      var res_temp1 = document.getElementById("MessaufloesungRegelung").value;
      if ((time_temp1/res_temp1) > 254){
        alert("Es sind zu viele Messpunkte ausgewählt worden. Die maximale Anzahl an Messpunkten liegt bei 254.");
        hideSubmit();
      }else {
        showSubmit();
      }
    }
    
    var Messmodus = 1;
    var Messobjekt = 1;

    const showSubmit = () => btnSubmit.style.visibility='visible';
    const hideSubmit = () => btnSubmit.style.visibility='hidden';
    const showStop = () => stopMessung.style.visibility='hidden';
    const chanceBackgroundClickedSubmit = () => btnSubmit.style.backgroundColor = '#DF2935';
    const chanceBackgroundUnclickedSubmit = () => btnSubmit.style.backgroundColor = '#F2EFE9';
    const chanceBackgroundClickedStop = () => stopMessung.style.backgroundColor = '#DF2935';
    const chanceBackgroundUnclickedStop = () => stopMessung.style.backgroundColor = '#F2EFE9';

      const chanceBackgroundClickedSprungantwort = () => btnSprungantwort.style.backgroundColor = '#DF2935';
      const chanceBackgroundUnclickedRegelung = () => btnRegelung.style.backgroundColor = '#F2EFE9';
      

      const chanceBackgroundClickedRegelung = () => btnRegelung.style.backgroundColor = '#DF2935';
      const chanceBackgroundUnclickedSprungantwort = () => btnSprungantwort.style.backgroundColor = '#F2EFE9';
      

      let btnSprungantwort = document.querySelector('#Sprungantwort');
      let btnRegelung = document.querySelector('#Regelung');
      let divSprungantwort = document.querySelector('#conficSprungantwort');
      let divRegelung = document.querySelector('#conficRegelung');
      let btnSubmit = document.querySelector('#submitValues');
      let btnStop = document.querySelector('#stopMessung');
      btnSprungantwort.addEventListener('click', () => {
        chanceBackgroundClickedSprungantwort();
        chanceBackgroundUnclickedRegelung();
        toggleConfigSprungantwort();
        Messmodus=1;
        showSubmit();
      });

      btnRegelung.addEventListener('click', () => {
        chanceBackgroundClickedRegelung();
        chanceBackgroundUnclickedSprungantwort();
        toggleConfigRegelung();
        Messmodus=2;
        showSubmit();
      });

      btnSubmit.addEventListener('click', () => {
        showStop();
        chanceBackgroundClickedSubmit();
        chanceBackgroundUnclickedStop();
      });

      btnStop.addEventListener('click', () => {
        chanceBackgroundUnclickedSubmit();
        chanceBackgroundClickedStop();
      });

      function myGreeting() {
          document.getElementById("start_alert").innerHTML = "Messzeit vorbei. Wechseln Sie nun zur Datenseite"
      }

      function chooseSprungantwort() {
        document.getElementById("choosedMode").innerHTML = "Sprungantwort";
      }
      function chooseRegelung() {
        document.getElementById("choosedMode").innerHTML = "Regelung";
      }

      function toggleConfigSprungantwort() {
        
        var x = document.getElementById("conficSprungantwort");
        var y = document.getElementById("conficRegelung");
        if (x.style.display === "none") {
          x.style.display = "block";
        } else {
          x.style.display = "none";
        }
        y.style.display = "none";
      }

      function toggleConfigRegelung() {
        var x = document.getElementById("conficRegelung");
        var y = document.getElementById("conficSprungantwort");
        if (x.style.display === "none") {
          x.style.display = "block";
        } else {
          x.style.display = "none";
        }
        y.style.display = "none";
      }

    var submitBtn = document.querySelector("#submitValues");
    submitBtn.addEventListener("click",function(){
      
      var MesszeitraumSprung = parseInt(document.getElementById("MesszeitraumSprungantwort").value);
      var MessaufloesungSprung = parseInt(document.getElementById("MessaufloesungSprungantwort").value);

      var checked_piregler = document.getElementById("piregler").checked;
      if (checked_piregler == true){
        var Reglerstruktur = 2;}
      else{
        var Reglerstruktur = 1;}
      var VerstaerkungReg = parseInt(document.getElementById("VerstaerkungRegelung").value*100);
      var SollwertReg = parseInt(document.getElementById("SollwertRegelung").value);
      var NachstellzeitReg = parseInt(document.getElementById("NachstellzeitRegelung").value);
      var MesszeitraumReg = parseInt(document.getElementById("MesszeitraumRegelung").value);
      var MessaufloesungReg = parseInt(document.getElementById("MessaufloesungRegelung").value);
      
      if (Messmodus == 1){
          const myTimeout1 = setTimeout(myGreeting, MesszeitraumSprung);}
      if (Messmodus == 2){
          const myTimeout2 = setTimeout(myGreeting, MesszeitraumReg);}
          
      var json_start = {Start:1,Stop:0,Modus:Messmodus,Objekt:Messobjekt,Zeit:MesszeitraumSprung,Aufloesung:MessaufloesungSprung,Zeit2:MesszeitraumReg,Aufloesung2:MessaufloesungReg,Regelstruktur:Reglerstruktur,Sollwert:SollwertReg,Verstaerkung:VerstaerkungReg,Nachstellzeit:NachstellzeitReg};
      console.log(json_start);
      console.log(JSON.stringify(json_start));
      
//      $.ajax("/api/v1/state", { data: JSON.stringify(json_start), dataType: "json", method: "POST", contentType: "application/json", cache: false, timeout: 2000})
//        .done(function( data ) {
//          console.log( "Response: " );
//          console.log( data );
//        })
//        .fail(function( data ) {
//          console.log( "Error: " );
//          console.log( data );
//        });
      var request_start = new XMLHttpRequest();
      request_start.open('POST','/api/v1/state',true);
      request_start.send(JSON.stringify(json_start));
      
    });
    
    var stopBtn = document.querySelector("#stopMessung");
    stopBtn.addEventListener("click",function(){
      var json_stop = {Start:0,Stop:1};
      
//      $.ajax("/api/v1/state", { data: JSON.stringify(json_stop), dataType: "json", method: "POST", contentType: "application/json", cache: false, timeout: 2000})
//        .done(function( data ) {
//          console.log( "Response: " );
//          console.log( data );
//        })
//        .fail(function( data ) {
//          console.log( "Error: " );
//          console.log( data );
//        });
      var request_stop = new XMLHttpRequest();
      request_stop.open('POST','/api/v1/state',true);
      request_stop.send(JSON.stringify(json_stop));
    });

    </script>
  </body>
</html>
)=====";
