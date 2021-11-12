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
        padding: 30px 15px 0;
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
              <div id="conficSprungantwort" class="form-group" style="visibility:hidden">
                <div class="form-group">
                  <label>Sprungantwort Messzeitraum [ms] max: 15000ms</label>
                  <input id="MesszeitraumSprungantwort" placeholder="0" value="10000"><br>
                  <label>Auflösung Messung [ms]</label>
                  <input id="MessaufloesungSprungantwort" placeholder="0" value="250">
                </div>
              </div>
              <div id="conficRegelung" class="form-group" style="visibility:hidden">
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
    <script src="./popper.min.js"></script>
    <script src="./bootstrap.min.js"></script>
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
      const chanceColorClickedSprungantwort = () => btnSprungantwort.style.Color = '#3772FF';
      const chanceBackgroundUnclickedRegelung = () => btnRegelung.style.backgroundColor = '#F2EFE9';
      const chanceColorUnclickedRegelung = () => btnRegelung.style.Color = '#3772FF';
      const showConfigSprungantwort = () => divSprungantwort.style.visibility='visible';
      const hideConfigRegelung = () => divRegelung.style.visibility='hidden';

      const chanceBackgroundClickedRegelung = () => btnRegelung.style.backgroundColor = '#DF2935';
      const chanceColorClickedRegelung = () => btnRegelung.style.Color = '#3772FF';
      const chanceBackgroundUnclickedSprungantwort = () => btnSprungantwort.style.backgroundColor = '#F2EFE9';
      const chanceColorUnclickedSprungantwort = () => btnSprungantwort.style.Color = '#3772FF';
      const showConfigRegelung = () => divRegelung.style.visibility='visible';
      const hideConfigSprungantwort = () => divSprungantwort.style.visibility='hidden';

      let btnSprungantwort = document.querySelector('#Sprungantwort');
      let btnRegelung = document.querySelector('#Regelung');
      let divSprungantwort = document.querySelector('#conficSprungantwort');
      let divRegelung = document.querySelector('#conficRegelung');
      let btnSubmit = document.querySelector('#submitValues');
      let btnStop = document.querySelector('#stopMessung');
      btnSprungantwort.addEventListener('click', () => {
      chanceColorClickedSprungantwort();
      chanceBackgroundClickedSprungantwort();
      chanceColorUnclickedRegelung();
      chanceBackgroundUnclickedRegelung();
      showConfigSprungantwort();
      hideConfigRegelung();
      Messmodus=1;
      showSubmit();
      });

      btnRegelung.addEventListener('click', () => {
      chanceColorClickedRegelung();
      chanceBackgroundClickedRegelung();
      chanceColorUnclickedSprungantwort();
      chanceBackgroundUnclickedSprungantwort();
      showConfigRegelung();
      hideConfigSprungantwort();
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
    
    $("#submitValues" ).click(function() {
      //var Messmodus = parseInt($('#inputRValue').val());
      //var Messobjekt = parseInt($('#inputGValue').val());
      var MesszeitraumSprung = parseInt($('#MesszeitraumSprungantwort').val());
      var MessaufloesungSprung = parseInt($('#MessaufloesungSprungantwort').val());
      
      var VerstaerkungReg = parseInt($('#VerstaerkungRegelung').val());
      var MesszeitraumReg = parseInt($('#MesszeitraumRegelung').val());
      var MessaufloesungReg = parseInt($('#MessaufloesungRegelung').val());
      
      var json = {Start:1,Stop:0,Modus:Messmodus,Objekt:Messobjekt,Zeit:MesszeitraumSprung,Aufloesung:MessaufloesungSprung,Verstaerkung2:VerstaerkungReg,Zeit2:MesszeitraumReg,Aufloesung2:MessaufloesungReg};
      console.log(json);
      console.log(JSON.stringify(json));
      
      $.ajax("/api/v1/state", { data: JSON.stringify(json), dataType: "json", method: "POST", contentType: "application/json", cache: false, timeout: 2000})
        .done(function( data ) {
          console.log( "Response: " );
          console.log( data );
          //alert(data);
        })
        .fail(function( data ) {
          console.log( "Error: " );
          console.log( data );
        //  alert(data);
        });
    });
    $("#stopMessung" ).click(function() {
      var json = {Start:0,Stop:1};
      console.log(json);
      console.log(JSON.stringify(json));
      
      $.ajax("/api/v1/state", { data: JSON.stringify(json), dataType: "json", method: "POST", contentType: "application/json", cache: false, timeout: 2000})
        .done(function( data ) {
          console.log( "Response: " );
          console.log( data );
          //alert(data);
        })
        .fail(function( data ) {
          console.log( "Error: " );
          console.log( data );
        //  alert(data);
        });
    });

    </script>
  </body>
</html>
)=====";
