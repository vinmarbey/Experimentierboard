const char WEBADMIN[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="de">
  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>WifiRGB Controller Admin</title>
    
    <style>
      /* Sticky footer styles
      -------------------------------------------------- */
      html {
        position: relative;
        min-height: 100%;
      }
      body {
        /* Margin bottom by footer height */
        margin-bottom: 60px;
        padding: 30px 15px 0;
      }
      .footer {
        position: absolute;
        bottom: 0;
        width: 100%;
        /* Set the fixed height of the footer here */
        height: 60px;
        line-height: 60px; /* Vertically center the text there */
        background-color: #f5f5f5;
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
      }
      .btn:hover{
        color: #fff;
        background-color: #0069d9;
        border-color: #0062cc;
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
                <p>Messmodus: 1 - Sprungantwort; 2 - Regelung</p>
                <label>Modus</label>
                <input id="inputRValue" type="number" min="1" max="2">
              </div>
              <div class="form-group">
                <p>Messobjekte: Hier sind die Objekte 1-5 auswählbar</p>
                <label>Messobjekte</label>
                <input id="inputGValue" type="number" min="1" max="5">
              </div>
              <div class="form-group">
                <label>Messzeitraum</label>
                <input id="inputBValue" placeholder="0" value="0">
              </div>
              <button id="submitValues" type="button" class="btn">Start</button>
            </form>
    </main>
    
    <footer class="footer">
    </footer>
    
        <script src="./jquery-3.3.1.min.js"></script>
    <script src="./popper.min.js"></script>
    <script src="./bootstrap.min.js"></script>
    <script>
    $("#submitValues" ).click(function() {
      var Messmodus = parseInt($('#inputRValue').val());
      var Messobjekt = parseInt($('#inputGValue').val());
      var Messzeitraum = parseInt($('#inputBValue').val());
      
      var json = {Modus:Messmodus,Objekt:Messobjekt,Zeit:Messzeitraum};
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
