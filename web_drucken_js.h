const char DRUCKEN_JS[] PROGMEM = R"=====(
var el = document.getElementById("pbtn");
el.addEventListener("click", function (eve) {
  console.log("click");
  ausgabe();
});

function getImagefromZeichentool(node) {
  let iframes = node.querySelectorAll("IFRAME");
  let images = new Array;

  iframes.forEach((iframe, i) => {
    let iframeDocument = iframe.contentDocument || iframe.contentWindow.document;
    let iframeContent = iframeDocument.querySelector('#container');
    let data = iframe.contentWindow.stage.toDataURL().replace("image/png", "image/octet-stream");
    let render = {
      data: data,
      width: iframeContent.width,
      height: iframeContent.height
    }
    images.push(render);
  });

  return images;
}

function findHTMLdeleteAndReturnParrents(node, html) {
  let frames = node.querySelectorAll(html);
  let parentsTree = new Array;
  frames.forEach((frame, i) => {
    let parrent = frame.parentElement;
    parentsTree.push(parrent);
    frame.parentNode.removeChild(frame);
  });
  return parentsTree;
}

function placeImages(nodes, renders) {
  nodes.forEach((node, i) => {
    var image = new Image();
    image.src = renders[i].data;
    node.appendChild(image);
  });
}

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

  let video = dupNode.querySelector(".videoscreen");
  if (video) {
    video.parentNode.removeChild(video);
  }
  

  placeImages(findHTMLdeleteAndReturnParrents(dupNode, "IFRAME"), getImagefromZeichentool(node));

  let win = window.open('', 'printwindow');
  win.document.write(printHTML);
  win.document.body.appendChild(dupNode);

  setTimeout(function () {
    win.focus();
    win.print();
  }, 450);

  win.onfocus = function () { setTimeout(function () { win.close(); }, 500); }
}
)=====";
