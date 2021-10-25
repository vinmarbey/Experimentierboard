const char WEBDATA[] PROGMEM = R"=====(
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
            <h2>DATA</h2>
            <p>get data and handle it</p>
            <button id="getData" type="button" class="btn">get data</button>
            <button id="printData" type="button" onclick="printData()" class="btn">print data</button>
            <p>gemessene Daten:</p>
            <div id="datacontent"></div>
            
    </main>
    
    <footer class="footer">
    </footer>
    
        <script src="./jquery-3.3.1.min.js"></script>
    <script src="./popper.min.js"></script>
    <script src="./bootstrap.min.js"></script>
    <script type="text/javascript" src="https://jsxgraph.org/distrib/jsxgraphcore.js"></script>
    
    <script>
    $("#getData" ).click(function() {
//            $.get("/rawdata", function(data, status){
//    alert("Data: " + data + "\nStatus: " + status);
//  });

   $.ajax({
    url:"/rawdata",
    contentType: "application/json",
        dataType: 'json',
        success: function(json) {
          console.log(json);   // needs to match the payload (i.e. json must have {value: "foo"}
          value = json.data;
          console.log(value);
          document.getElementById("datacontent").innerHTML = value;
            
            var board = JXG.JSXGraph.initBoard('box', {boundingbox: [-0.5, 3.5, value.length/10+0.5, -0.5], axis:true});
            var points = []
            for (let i = 0; i<value.length;i++){
            points[i] = board.create('point',[i/10,value[i]]);
            }
          },
          error: function(e) {
          console.log("jQuery error message = "+e.message);
        }
      });
      

  
//      $.ajax("/api/v1/state", { data: JSON.stringify(json), dataType: "json", method: "POST", contentType: "application/json", cache: false, timeout: 2000})
//        .done(function( data ) {
//          console.log( "Response: " );
//          console.log( data );
//          //alert(data);
//        })
//        .fail(function( data ) {
//          console.log( "Error: " );
//          console.log( data );
//        //  alert(data);
//        });
    });
    </script>
      <div id="box" class="jxgbox" style="width:500px; height:500px;"></div>
    <script>
      function printData() {
            document.getElementById("datacontent").innerHTML = value;
            
            var board = JXG.JSXGraph.initBoard('box', {boundingbox: [-0.5, 3.5, value.length/10+0.5, -0.5], axis:true});
            var points = []
            for (let i = 0; i<value.length;i++){
            points[i] = board.create('point',[i/10,value[i]]);
            }
        }
    </script>
    
    <script type="text/javascript">
      
    </script>
  </body>
</html>
)=====";
