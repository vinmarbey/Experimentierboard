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
            <div id="box" class="jxgbox" style="width:700px; height:500px;"></div>
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
    <script type="text/css" src="drucken.js"></script>

    <script>
    var value, modus, resolution, zeit, verstaerkung;
    var getdataBtn = document.querySelector("#getData");
    getdataBtn.addEventListener("click",function(){
//      $.ajax({
//      url:"/rawdata",
//      contentType: "application/json",
//          dataType: 'json',
//          success: function(json) {
//            console.log(json);
//            value = json.data;
//            console.log(value);
//            modus = json.modus;
//            resolution = json.resolution;
//            zeit = json.zeit;
//            if (modus == 2){
//              verstaerkung = json.verstaerkung;
//            }
//            
//            //document.getElementById("datacontent").innerHTML = value;
//              
//            printData();
//          
//            },
//            error: function(e) {
//            console.log("jQuery error message = "+e.message);
//          }
//      });
      
      var request_data = new XMLHttpRequest();
      request_data.open('POST','/rawdata',true);
      request_data.onreadystatechange = function () {
        if(request_data.readyState === XMLHttpRequest.DONE) {
          var status = request_data.status;
          if (status === 0 || (status >= 200 && status < 400)) {
            // The request has been completed successfully
            var body = request_data.response;
            //console.log(body);
            //console.log(JSON.parse(request_data.response));
            var obj = JSON.parse(request_data.response);
            //console.log(obj.data);
            value = obj.data;
            modus = obj.modus;
            resolution = obj.resolution;
            zeit = obj.zeit;
            if (modus == 2){
              verstaerkung = obj.verstaerkung;
            }

            printData();
            
          } else {
            console.log("Status: " + status);
          }
        }
      };
      request_data.send(null);
    });

    var example_value = [4,29,52,75,96,116,136,159,177,197,214,230,247,264,280,295,309,325,339,351,365,377,388,401,413,427,438,448,459,469,477,487,496,505,513,522,530,537,545,553,560,567,573,580,586,592,598,604,608,615,619,625,629,634,640,644,648,652,656,661,664];
    var example_resolution = 250;

    function printData() {
        
        var meas_values = [];
        for (let i = 0; i<value.length;i++){
          meas_values[i] = value[i]*3.3/255;
        }
        var example_value_max = Math.max(...meas_values);
        var example_value_length = meas_values.length;
        if (modus == 2){
          document.getElementById("parmeter").innerHTML = "Modus: " + "Regelung" + "<br>Messzeitzeit: " + zeit + " ms<br>Aufloesung: " + resolution + " ms<br>Verstaerkung: " + verstaerkung;
        } else{
          document.getElementById("parmeter").innerHTML = "Modus: " + "Sprungantwort" + "<br>Messzeitzeit: " + zeit + " ms<br>Aufloesung: " + resolution + " ms";
        }
        //document.getElementById("datacontent").innerHTML = meas_values;
            
        var board = JXG.JSXGraph.initBoard('box', {boundingbox: [-0.7, example_value_max+0.7, example_value_length*resolution/1000+1, -0.5], axis:true,defaultAxes: { x:{ name: 'Zeit [s]', withLabel: true, label:{ position: 'rt', offset: [-20, -20]}}, y:{ withLabel:true, name: 'Spannung [V]', label:{ position: 'rt',offset: [5, -15]}}}});
        var points = [];
        for (let i = 0; i<example_value_length;i++){
          points[i] = board.create('point',[i*resolution/1000,meas_values[i]],{withLabel:false, fixed:true});
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
    </script>
  </body>
</html>
)=====";
