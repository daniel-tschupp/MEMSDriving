//var wsUri = "wss://echo.websocket.org/";

var wsUri = "ws://127.0.0.1:5000";
function init()
{
	websocket = new WebSocket(wsUri);
	websocket.onopen = function(evt) { onOpen(evt) };
	websocket.onclose = function(evt) { onClose(evt) };
	websocket.onmessage = function(evt) { onMessage(evt) };
	websocket.onerror = function(evt) { onError(evt) };
}

function onOpen(evt)
{
	document.getElementById("ledIndicator").className = 'led-green';
}

function onClose(evt)
{
	document.getElementById('ledIndicator').className = 'led-red';
	websocket.close();
	init();
}

function onMessage(evt)
{
	var reader = new FileReader();
  	reader.onload = function(event){
    		document.getElementById("txtRec").value = reader.result;		
  	}
	reader.readAsText(evt.data);	
}

function onError(evt)
{
	document.getElementById('ledIndicator').className = 'led-red';
    	document.getElementById("txtRec").value = evt.data;
}

function doSend(message)
{
	document.getElementById("txtSend").value = message;
    	document.getElementById("txtRec").value = "";
	var enc = new TextEncoder();
    	websocket.send(enc.encode(message));
}

function StartStopAction()
{	
	var b = document.getElementById("bStartStop");
	if(b.value == "Start"){
		var contScan = document.getElementById("iContScan");
		if(contScan.checked == true){
			b.value = "Stop";
		}
		var msg = "{ \"RunScan\"}";
		doSend(msg);
	}
	else{
		b.value = "Start";
		var msg = "{ \"StopScan\"}";
		doSend(msg);
	}
} 

function SendMCUConfig()
{
	var OctType = "";
	var ScanMode = "";
	if(document.getElementById("iSpectrometer").checked == true)
		OctType = "SpectrometerOCT";
	if(document.getElementById("iSweptSource").checked == true)
		OctType = "SSOCT";

	if(document.getElementById("iSingleScan").checked == true)
		ScanMode = "SingleScan";
	if(document.getElementById("iContScan").checked == true)
		ScanMode = "ContinuousScan";
	var obj = {
		OCTType: OctType,
		AScanFrequency: document.getElementById("iAScanFrec").value,
		ScanningMode: ScanMode
	}
	var configObj = {
		MCUConfig: obj
	}
	var msg = JSON.stringify(configObj);
	doSend(msg);
}

function SendFastConfig()
{
	var filter = "NoFilter";
	if(document.getElementById("iFastBessel").checked == true)
		filter = "Bessel6thOrder";
	if(document.getElementById("iFastIIR").checked == true)
		filter = "IIR_X";


	if(document.getElementById("iFastSine").checked == true){
		var obj = {
			SineSignal: {
				OutputStage: "XAxis",
				Frequency: Number(document.getElementById("iFastFrec").value),
				Amplitude: Number(document.getElementById("iFastAmp").value),
				Phase: Number(document.getElementById("iFastPhase").value),
				Offset: Number(document.getElementById("iFastOffset").value),
				Filter: filter
			}
		}
		var msg = JSON.stringify(obj);
		doSend(msg);
		return;
	}

	if(document.getElementById("iFastSawtooth").checked == true)
	{
		var obj = {
			TriangleSignal: {
				OutputStage: "XAxis",
				Frequency: Number(document.getElementById("iFastFrec").value),
				Amplitude: Number(document.getElementById("iFastAmp").value),
				Phase: Number(document.getElementById("iFastPhase").value),
				Offset: Number(document.getElementById("iFastOffset").value),
				Width: Number(document.getElementById("iFastWidth").value),
				Filter: filter
			}
		}
		var msg = JSON.stringify(obj);
		doSend(msg);
		return;

	}
	if(document.getElementById("iFastConstant").checked == true)
	{
		var obj = {
			ConstantSignal: {
				OutputStage: "XAxis",
				Duration: Number(document.getElementById("iFastDuration").value),
				DCValue: Number(document.getElementById("iFastDC").value),
				Phase: Number(document.getElementById("iFastPhase").value),
				Offset: Number(document.getElementById("iFastOffset").value),
				Filter: filter
			}
		}
		var msg = JSON.stringify(obj);
		doSend(msg);
		return;
	}
	return;
}


function SendSlowConfig()
{
	var filter = "NoFilter";
	if(document.getElementById("iSlowBessel").checked == true)
		filter = "Bessel300Hz";
	if(document.getElementById("iSlowIIR").checked == true)
		filter = "IIR_Y";


	if(document.getElementById("iSlowSine").checked == true){
		var obj = {
			SineSignal: {
				OutputStage: "YAxis",
				Frequency: Number(document.getElementById("iSlowFrec").value),
				Amplitude: Number(document.getElementById("iSlowAmp").value),
				Phase: Number(document.getElementById("iSlowPhase").value),
				Offset: Number(document.getElementById("iSlowOffset").value),
				Filter: filter
			}
		}
		var msg = JSON.stringify(obj);
		doSend(msg);
		return;
	}

	if(document.getElementById("iSlowSawtooth").checked == true)
	{
		var obj = {
			TriangleSignal: {
				OutputStage: "YAxis",
				Frequency: Number(document.getElementById("iSlowFrec").value),
				Amplitude: Number(document.getElementById("iSlowAmp").value),
				Phase: Number(document.getElementById("iSlowPhase").value),
				Offset: Number(document.getElementById("iSlowOffset").value),
				Width: Number(document.getElementById("iSlowWidth").value),
				Filter: filter
			}
		}
		var msg = JSON.stringify(obj);
		doSend(msg);
		return;

	}
	if(document.getElementById("iSlowConstant").checked == true)
	{
		var obj = {
			ConstantSignal: {
				OutputStage: "YAxis",
				Duration: Number(document.getElementById("iSlowDuration").value),
				DCValue: Number(document.getElementById("iSlowDC").value),
				Phase: Number(document.getElementById("iSlowPhase").value),
				Offset: Number(document.getElementById("iSlowOffset").value),
				Filter: filter
			}
		}
		var msg = JSON.stringify(obj);
		doSend(msg);
		return;
	}
	return;
}

function SendFiltX()
{
	var obj = {
		SetFilter: {
			FilterName: "IIR_X",
			Fs: Number(document.getElementById("FiltSamplingFreq").value),
			Gain: Number(document.getElementById("FiltX_gain").value),
			a0: Number(document.getElementById("FiltX_a0").value),
			a1: Number(document.getElementById("FiltX_a1").value),
			a2: Number(document.getElementById("FiltX_a2").value),
			a3: Number(document.getElementById("FiltX_a3").value),
			a4: Number(document.getElementById("FiltX_a4").value),
			b0: Number(document.getElementById("FiltX_b0").value),
			b1: Number(document.getElementById("FiltX_b1").value),
			b2: Number(document.getElementById("FiltX_b2").value),
			b3: Number(document.getElementById("FiltX_b3").value),
			b4: Number(document.getElementById("FiltX_b4").value)
		}
	}
	doSend(JSON.stringify(obj));
}

function SendFiltY()
{
	var obj = {
		SetFilter: {
			FilterName: "IIR_Y",
			Fs: Number(document.getElementById("FiltSamplingFreq").value),
			Gain: Number(document.getElementById("FiltY_gain").value),
			a0: Number(document.getElementById("FiltY_a0").value),
			a1: Number(document.getElementById("FiltY_a1").value),
			a2: Number(document.getElementById("FiltY_a2").value),
			a3: Number(document.getElementById("FiltY_a3").value),
			a4: Number(document.getElementById("FiltY_a4").value),
			b0: Number(document.getElementById("FiltY_b0").value),
			b1: Number(document.getElementById("FiltY_b1").value),
			b2: Number(document.getElementById("FiltY_b2").value),
			b3: Number(document.getElementById("FiltY_b3").value),
			b4: Number(document.getElementById("FiltY_b4").value)
		}
	}
	doSend(JSON.stringify(obj));
}



  window.addEventListener("load", init, false);

