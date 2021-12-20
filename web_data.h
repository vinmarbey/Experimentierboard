const char WEBDATA[] PROGMEM = R"=====(
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
        padding: 0px 15px 0;
      }
      .footer {
        position: absolute;
        bottom: 0;
        width: 100%;
        height: 60px;
        line-height: 60px;
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
      .btn, .button{
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
      .btn:hover, .button:hover{
        color: #080708;
        background-color: #3772FF;
        border-color: #252627;
      }
      .btn:active, .button:active{
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
            <h2>DATA</h2>
            <p>get data and handle it</p>
            <button id="getData" type="button" class="btn">get and print measured data</button>
            <button id="printData" type="button" onclick="printData()" class="btn">reprint data</button>
            <button id="gotosettings" type="button" class="btn"><a href="/admin">go to settings</a></button>
            <div id="capture">
            <p>Parameter der Messung:</p>
            <div id="datacontent"></div>
            <div id="parmeter"></div>
            <p>Verlauf der Messung:</p>
            <div id="box" class="jxgbox" style="width:700px; height:500px;">Wenn kein blauer Rand gezeigt wird, Seite neu laden.</div>
            </div>
             <a><button id="pbtn" class="button">Drucken</button></a>
             <button id="printData" type="button" onclick="dataToArrayWithTimestamp()" class="btn">Export to CSV</button>
    </main>
    
    <footer class="footer">
    </footer>
   
    <!--<link rel="stylesheet" type="text/css" href="https://jsxgraph.org/distrib/jsxgraph.css" />-->
    <!--<script type="text/javascript" src="https://jsxgraph.org/distrib/jsxgraphcore.js"></script>-->
    <link rel="stylesheet" type="text/css" href="jsxgraph.css" />
    <script type="text/javascript" src="jsxgraphcore.js"></script>
    <!--<script type="text/css" src="drucken.js"></script>-->

    <script>
    var value, modus, resolution, zeit, verstaerkung,Regelstruktur, sollwert, nachstellzeit;
    var getdataBtn = document.querySelector("#getData");
    getdataBtn.addEventListener("click",function(){

      
      var request_data = new XMLHttpRequest();
      request_data.open('POST','/rawdata',true);
      request_data.onreadystatechange = function () {
        if(request_data.readyState === XMLHttpRequest.DONE) {
          var status = request_data.status;
          if (status === 0 || (status >= 200 && status < 400)) {
            // The request has been completed successfully
            var body = request_data.response;
            console.log(body);
            //console.log(JSON.parse(request_data.response));
            var obj = JSON.parse(request_data.response);
            //console.log(obj.data);
            value = obj.data;
            value1 = obj.data1;
            value2 = obj.data2;
            value3 = obj.data3;
            value4 = obj.data4;
            //console.log(value);
            modus = obj.modus;
            resolution = obj.resolution;
            zeit = obj.zeit;
            if (modus == 2){
//              regelstruktur = obj.Regelstruktur;
//              verstaerkung = obj.verstaerkung;
//              sollwert = obj.sollwert;
//              nachstellzeit = obj.nachstellzeit;
              
            }

            printData();
            
          } else {
            console.log("Status: " + status);
          }
        }
      };
      request_data.send(null);
    });



    function printData() {
        
        var value_max = [];
        value_max[0] = Math.max(...value);
        value_max[1] = Math.max(...value1);
        value_max[2] = Math.max(...value2);
        value_max[3] = Math.max(...value3);
        value_max[4] = Math.max(...value4);
        var value_maximum = Math.max(...value_max);
        var value_length = zeit/resolution;
        console.log(value_length);
        if (modus == 2){
          //console.log(regelstruktur);
//          if(regelstruktur == 1){
//           document.getElementById("parmeter").innerHTML = "Modus: " + "Regelung" + "<br>Reglerstruktur: " + "P-Regler" + "<br>Messzeitzeit: " + zeit + " ms<br>Aufloesung: " + resolution + " ms<br>Verstaerkung: " + verstaerkung + "<br>Sollwert: " + sollwert; 
//          }else if(regelstruktur == 2){
//           document.getElementById("parmeter").innerHTML = "Modus: " + "Regelung" + "<br>Reglerstruktur: " + "PI-Regler" + "<br>Messzeitzeit: " + zeit + " ms<br>Aufloesung: " + resolution + " ms<br>Verstaerkung: " + verstaerkung + "<br>Sollwert: " + sollwert  + "<br>Nachstellzeit: " + nachstellzeit; 
//          }
        } else{
          document.getElementById("parmeter").innerHTML = "Modus: " + "Sprungantwort" + "<br>Messzeitzeit: " + zeit + " ms<br>Aufloesung: " + resolution + " ms";
        }
        //document.getElementById("datacontent").innerHTML = meas_values;
            
        var board = JXG.JSXGraph.initBoard('box', {boundingbox: [-0.7, value_maximum*3.3/255+0.7, value_length*resolution/1000+1, -0.5], axis:true,defaultAxes: { x:{ name: 'Zeit [s]', withLabel: true, label:{ position: 'rt', offset: [-20, -20]}}, y:{ withLabel:true, name: 'Spannung [V]', label:{ position: 'rt',offset: [5, -15]}}}});
        var points = [];
        for (let i = 0; i<value_length;i++){
          if (i < 60){
          points[i] = board.create('point',[i*resolution/1000,value[i]*3.3/255],{withLabel:false, fixed:true});
          }else if (i <120){
            points[i] = board.create('point',[i*resolution/1000,value1[i-60]*3.3/255],{withLabel:false, fixed:true});
          }else if (i <180){
            points[i] = board.create('point',[i*resolution/1000,value2[i-120]*3.3/255],{withLabel:false, fixed:true});
          }else if (i <240){
            points[i] = board.create('point',[i*resolution/1000,value3[i-180]*3.3/255],{withLabel:false, fixed:true});
          }else if (i <300){
            points[i] = board.create('point',[i*resolution/1000,value4[i-240]*3.3/255],{withLabel:false, fixed:true});
          }
        }
        
    }
  
    function dataToArrayWithTimestamp(){
      var dataArrayTimestamp = [];
      var ArrayTimestamp_temp = [];
      for (let i = 0; i<value.length;i++) {
        ArrayTimestamp_temp[0] = i*resolution/1000;
        ArrayTimestamp_temp[1] = value[i]*3.3/255;
        dataArrayTimestamp.push([ArrayTimestamp_temp[0],ArrayTimestamp_temp[1]]);
      }
      exportToCsv('export.csv', dataArrayTimestamp);
    }

    function exportToCsv(filename, rows) {
    var processRow = function (row) {
        var finalVal = '';
        for (var j = 0; j < row.length; j++) {
            var innerValue = row[j] === null ? '' : row[j].toString();
            if (row[j] instanceof Date) {
                innerValue = row[j].toLocaleString();
            };
            var result = innerValue.replace(/"/g, '""');
            if (result.search(/("|,|\n)/g) >= 0)
                result = '"' + result + '"';
            if (j > 0)
                finalVal += ',';
            finalVal += result;
        }
        return finalVal + '\n';
    };

    var csvFile = '';
    for (var i = 0; i < rows.length; i++) {
        csvFile += processRow(rows[i]);
    }

    var blob = new Blob([csvFile], { type: 'text/csv;charset=utf-8;' });
    if (navigator.msSaveBlob) { // IE 10+
        navigator.msSaveBlob(blob, filename);
    } else {
        var link = document.createElement("a");
        if (link.download !== undefined) { // feature detection
            // Browsers that support HTML5 download attribute
            var url = URL.createObjectURL(blob);
            link.setAttribute("href", url);
            link.setAttribute("download", filename);
            link.style.visibility = 'hidden';
            document.body.appendChild(link);
            link.click();
            document.body.removeChild(link);
        }
    }
    }

    var el = document.getElementById("pbtn");
    el.addEventListener("click", function (eve) {
      //console.log("click");
      ausgabe();
    });
    function ausgabe() {
      let node = document.querySelector("#capture");

      let dupNode = node.cloneNode(true);

      let printHTML = '<!DOCTYPE html>' +
        '<html lang="de" dir="ltr">' +
        '' +
        '<head>' +
        '  <meta charset="utf-8">' +
        '  <meta name="viewport" content="width=device-width, initial-scale=1.0" />' +
        '  <title>Druckvorschau</title>' +
        '  <link rel="stylesheet" href="CSS/stylesheet.css">' +
        '  <link rel="stylesheet" href="CSS/stylesheetP.css">' +
        '</head>' +
        '' +
        '<body>' +
        '</body>' +
        '' +
        '</html>';

    let win = window.open('', 'printwindow');
    win.document.write(printHTML);
    win.document.body.appendChild(dupNode);

    setTimeout(function () {
      win.focus();
      win.print();
    }, 450);

    win.onfocus = function () { setTimeout(function () { win.close(); }, 500); }
  }
    </script>
  </body>
</html>
)=====";
