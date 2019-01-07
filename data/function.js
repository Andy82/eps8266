var xmlHttp=createXmlHttpObject();
function createXmlHttpObject(){
 if(window.XMLHttpRequest){
  xmlHttp=new XMLHttpRequest();
 }else{
  xmlHttp=new ActiveXObject('Microsoft.XMLHTTP');
 }
 return xmlHttp;
}


function load(){
 if(xmlHttp.readyState==0 || xmlHttp.readyState==4){
  xmlHttp.open('PUT','/configs.json',true);
  xmlHttp.send(null);
  xmlHttp.onload = function(e) {
   jsonResponse=JSON.parse(xmlHttp.responseText);
   loadBlock();
  }
 }
 socket();
}

function socket()
{
	var socket = new WebSocket("ws://" + window.location.host + ":81");
	socket.onopen = function() {
	  socket.send('Connect ' + new Date());
	  console.log('Connect ' + new Date());
	};

	socket.onclose = function(event) {
	  if (event.wasClean) {
		console.log('Connection closed');
	  } else {
		console.log('Server disconnected'); 
	  }
	};

	socket.onmessage = function(event) {
	  console.log('Server: ', event.data);
	  //socket.send( "Thank you, Mr. server!" );
	  
	    var socket_data=JSON.parse(event.data);
		document.getElementById('timeX').innerHTML = socket_data.time;
		
		console.log('Time: ', socket_data.time);
	  
	  setInterval(function(){ $.getJSON('config.json', function(socket_data) { document.getElementById("timeX").innerHTML=socket_data.time; }); },1000);
	  
	};

	socket.onerror = function(error) {
	  console.log('WebSocket Error ', error);
	};	
}


function loadBlock(data2) {
 data2 = JSON.parse(xmlHttp.responseText);
 data = document.getElementsByTagName('body')[0].innerHTML;
 var new_string;
for (var key in data2) {
 new_string = data.replace(new RegExp('{{'+key+'}}', 'g'), data2[key]);
 data = new_string;
}
 document.getElementsByTagName('body')[0].innerHTML = new_string;
 handleServerResponse();
}

function val(id,val){
 if (val) {
  document.getElementById(id).value=(val==' '?'':val);
 } else {
  var v = document.getElementById(id).value;
  return v;
 }
}

function send_request(submit,server){
 request = new XMLHttpRequest();
 request.open("GET", server, true);
 request.send();
 save_status(submit,request);
}

function save_status(submit,request){
 old_submit = submit.value;
 request.onreadystatechange = function() {
  if (request.readyState != 4) return;
  submit.value = request.responseText;
  setTimeout(function(){
   submit.value=old_submit;
   submit_disabled(false);
  }, 1000);
 }
 submit.value = 'Подождите...';
 submit_disabled(true);
}

function submit_disabled(request){
 var inputs = document.getElementsByTagName("input");
 for (var i = 0; i < inputs.length; i++) {
  if (inputs[i].type === 'submit') {inputs[i].disabled = request;}
 }
}

function html(id,val){
    if (val) {
     document.getElementById(id).innerHTML=(val==' '?'':val);
    } else {
     var v = document.getElementById(id).innerHTML;
     return v;
    }
}

function toggle(target,status) {
    var curVal = document.getElementById(target).classList;
    if (curVal.contains('hidden')) {
     if (status != 'show') {
      curVal.remove('hidden');
      curVal.add('show');
     }
    } else {
     if (status != 'hidden') {
      curVal.remove('show');
      curVal.add('hidden');
     }
    }
   }


function loadWifi(ssids,hiddenIds){
    var xmlHttp=createXmlHttpObject();
    document.getElementById(ssids).innerHTML ='<li><a href="#">Загрузка...</a></li>';
    xmlHttp.open('GET','/wifi.scan.json',true);
    xmlHttp.send(null);
    xmlHttp.onload = function() {
     var jsonWifi=JSON.parse(xmlHttp.responseText);
     jsonWifi.networks.sort(function(a,b){return (a.dbm < b.dbm) ? 1 : ((b.dbm < a.dbm) ? -1 : 0);});
     var html = '';
     for(var i = 0;i<jsonWifi.networks.length;i++) {
      var wifiSignal = '';
      if (jsonWifi.networks[i].dbm <= -0) { wifiSignal = '<i class="wifi wifi-0-60"></i>';}
      if (jsonWifi.networks[i].dbm <= -60) { wifiSignal = '<i class="wifi wifi-60-70"></i>';}
      if (jsonWifi.networks[i].dbm <= -70) { wifiSignal = '<i class="wifi wifi-70-80"></i>';}
      if (jsonWifi.networks[i].dbm <= -80) { wifiSignal = '<i class="wifi wifi-80-90"></i>';}
      if (jsonWifi.networks[i].dbm <= -90) { wifiSignal = '<i class="wifi wifi-90-100"></i>';}
      html += '<li><a href="#" onclick="val(\''+hiddenIds+'\',\''+jsonWifi.networks[i].ssid+'\');toggle(\'ssid-select\');html(\'ssid-name\',\''+jsonWifi.networks[i].ssid+'\');return false"><div style="float:right">'+(jsonWifi.networks[i].pass?'<i class="wifi wifi-key"></i>':'')+' '+wifiSignal+' <span class="label label-default">'+jsonWifi.networks[i].dbm+' dBm</span></div><b>'+jsonWifi.networks[i].ssid+'</b></a></li>';
     }
     document.getElementById(ssids).innerHTML = (html?html:'<li><a href="#">Not found WiFi</a></li>')+'<li><a href="#" onclick="toggle(\'ssid-group\');toggle(\'ssid\');return false"><b>Скрытая сеть</b></a></li>';
    }
   }