const char WEBADMIN[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="de">
  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>WifiRGB Controller Admin</title>
    
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
                
                  <label>Sprungantwort Messzeitraum [ms] max: 15000ms</label>
                  <input id="MesszeitraumSprungantwort" placeholder="0" value="10000"><br>
                  <label>Auflösung Messung [ms]</label>
                  <input id="MessaufloesungSprungantwort" placeholder="0" value="250">
                
              </div>
              <div id="conficRegelung" class="form-group" style="display:none;">
                <div class="form-group">
                  <label>Verstärkung</label>
                  <input id="VerstaerkungRegelung" placeholder="0" value="1"><br>
                  <label>Sprungantwort Messzeitraum [ms] max: 15000ms</label>
                  <input id="MesszeitraumRegelung" placeholder="0" value="10000"><br>
                  <label>Auflösung Messung [ms]</label>
                  <input id="MessaufloesungRegelung" placeholder="0" value="250">
                </div>
              </div>
              </table>
              <button id="submitValues" type="button" class="btn" style="visibility:hidden">Start</button>
              <button id="stopMessung" type="button" class="btn" style="visibility:hidden">Stop</button>
              <a href="/data"> go to data</a>

              
            </form>
    </main>
    
    <footer class="footer">
    </footer>
    
    <script src="./jquery-3.3.1.min.js"></script>
    
    <script type="text/javascript">
    var Messmodus = 1;
    var Messobjekt = 1;

    const showSubmit = () => btnSubmit.style.visibility='visible';
    const showStop = () => stopMessung.style.visibility='visible';
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
      
      var VerstaerkungReg = parseInt(document.getElementById("VerstaerkungRegelung").value);
      var MesszeitraumReg = parseInt(document.getElementById("MesszeitraumRegelung").value);
      var MessaufloesungReg = parseInt(document.getElementById("MessaufloesungRegelung").value);
      
      var json = {Start:1,Stop:0,Modus:Messmodus,Objekt:Messobjekt,Zeit:MesszeitraumSprung,Aufloesung:MessaufloesungSprung,Verstaerkung2:VerstaerkungReg,Zeit2:MesszeitraumReg,Aufloesung2:MessaufloesungReg};
      console.log(json);
      console.log(JSON.stringify(json));
      
      $.ajax("/api/v1/state", { data: JSON.stringify(json), dataType: "json", method: "POST", contentType: "application/json", cache: false, timeout: 2000})
        .done(function( data ) {
          console.log( "Response: " );
          console.log( data );
        })
        .fail(function( data ) {
          console.log( "Error: " );
          console.log( data );
        });
    });
    
    var stopBtn = document.querySelector("#stopMessung");
    stopBtn.addEventListener("click",function(){
      var json = {Start:0,Stop:1};
      
      $.ajax("/api/v1/state", { data: JSON.stringify(json), dataType: "json", method: "POST", contentType: "application/json", cache: false, timeout: 2000})
        .done(function( data ) {
          console.log( "Response: " );
          console.log( data );
        })
        .fail(function( data ) {
          console.log( "Error: " );
          console.log( data );
        });
    });

    </script>
  </body>
</html>
)=====";
