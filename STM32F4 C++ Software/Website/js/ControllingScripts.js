
function JSFastSawToothSelected()
{
	
	document.getElementById('fastFrec').style.display = 'table-row';
	document.getElementById('fastDuration').style.display = 'none';
	document.getElementById('fastAmp').style.display = 'table-row';
	document.getElementById('fastDC').style.display = 'none';
	document.getElementById('fastWidth').style.display = 'table-row';
	drawScanPattern();
}
function JSFastSineSelected()
{
	
	document.getElementById('fastFrec').style.display = 'table-row';
	document.getElementById('fastDuration').style.display = 'none';
	document.getElementById('fastAmp').style.display = 'table-row';
	document.getElementById('fastDC').style.display = 'none';
	document.getElementById('fastWidth').style.display = 'none';
	drawScanPattern();
}

function JSFastConstSelected()
{
	document.getElementById('fastFrec').style.display = 'none';
	document.getElementById('fastDuration').style.display = 'table-row';
	document.getElementById('fastAmp').style.display = 'none';
	document.getElementById('fastDC').style.display = 'table-row';
	document.getElementById('fastWidth').style.display = 'none';
	drawScanPattern();
}

function JSSlowSawToothSelected()
{
	document.getElementById('slowFrec').style.display = 'table-row';
	document.getElementById('slowDuration').style.display = 'none';
	document.getElementById('slowAmp').style.display = 'table-row';
	document.getElementById('slowDC').style.display = 'none';
	document.getElementById('slowWidth').style.display = 'table-row';
	drawScanPattern();
}
function JSSlowSineSelected()
{
	document.getElementById('slowFrec').style.display = 'table-row';
	document.getElementById('slowDuration').style.display = 'none';
	document.getElementById('slowAmp').style.display = 'table-row';
	document.getElementById('slowDC').style.display = 'none';
	document.getElementById('slowWidth').style.display = 'none';
	drawScanPattern();
}

function JSSlowConstSelected()
{
	document.getElementById('slowFrec').style.display = 'none';
	document.getElementById('slowDuration').style.display = 'table-row';
	document.getElementById('slowAmp').style.display = 'none';
	document.getElementById('slowDC').style.display = 'table-row';
	document.getElementById('slowWidth').style.display = 'none';
	drawScanPattern();
}

function McuConfigClick()
{
	document.getElementById("bMCUConfig").className = 'buttonClicked';
	document.getElementById("bFilterConfig").className = 'buttonUnclicked';
	document.getElementById("bScanPreview").className = 'buttonUnclicked';

	document.getElementById("iMCUConfigBox").style.display = 'block';
	document.getElementById("iFilterConfigBox").style.display = 'none';
	document.getElementById("iScanPreviewBox").style.display = 'none';
}

function FilterClick()
{
	
	document.getElementById("bMCUConfig").className = 'buttonUnclicked';
	document.getElementById("bFilterConfig").className = 'buttonClicked';
	document.getElementById("bScanPreview").className = 'buttonUnclicked';
	
	document.getElementById("iMCUConfigBox").style.display = 'none';
	document.getElementById("iFilterConfigBox").style.display = 'block';
	document.getElementById("iScanPreviewBox").style.display = 'none';
}

function ScanPreviewClick()
{
	
	document.getElementById("bMCUConfig").className = 'buttonUnclicked';
	document.getElementById("bFilterConfig").className = 'buttonUnclicked';
	document.getElementById("bScanPreview").className = 'buttonClicked';
	
	document.getElementById("iMCUConfigBox").style.display = 'none';
	document.getElementById("iFilterConfigBox").style.display = 'none';
	document.getElementById("iScanPreviewBox").style.display = 'block';

	drawScanPattern();
}

function SaveXFiltClick()
{
	alert('Yes');
}
function LoadXFiltClick()
{

}
function SaveYFiltClick()
{

}
function LoadYFiltClick()
{

}


function drawScanPattern()
{
	if(document.getElementById("iScanPreviewBox").style.display == 'none')
		return;	
	var svgSize = document.getElementById("svgSize");
	var svg = document.getElementById("iScanPatternSVG");
	var wFactor = svgSize.clientWidth/3.5;
	var hFactor = svgSize.clientHeight/3.5;
	var wOffset = svgSize.clientWidth/2;
	var hOffset = svg.clientHeight/2;
	var tend = Number(document.getElementById("iSimEndTime").value);
	var fsampling = Number(document.getElementById("iSimDACFreq").value);
	var iSamples;
	
	var polyline = document.getElementById("pLine");
	polyline.points.clear();
	var sigSlowType = "Constant";
	if(document.getElementById("iSlowSawtooth").checked == true)
		sigSlowType = "Sawtooth";
	if(document.getElementById("iSlowSine").checked == true)
		sigSlowType = "Sine";

	var sigFastType = "Constant";
	if(document.getElementById("iFastSawtooth").checked == true)
		sigFastType = "Sawtooth";
	if(document.getElementById("iFastSine").checked == true)
		sigFastType = "Sine";
	var cfast = {
		type: sigFastType,
		freq: Number(document.getElementById("iFastFrec").value),
		amp: Number(document.getElementById("iFastAmp").value),
		phase: Number(document.getElementById("iFastPhase").value)/180*Math.PI,
		offset: Number(document.getElementById("iFastOffset").value),
		dcVal: Number(document.getElementById("iFastDC").value),
		duration: Number(document.getElementById("iFastDuration").value),
		width: Number(document.getElementById("iFastWidth").value)
	}
	var cslow = {
		type: sigSlowType,
		freq: Number(document.getElementById("iSlowFrec").value),
		amp: Number(document.getElementById("iSlowAmp").value),
		phase: Number(document.getElementById("iSlowPhase").value)/180*Math.PI,
		offset: Number(document.getElementById("iSlowOffset").value),
		dcVal: Number(document.getElementById("iSlowDC").value),
		duration: Number(document.getElementById("iSlowDuration").value),
		width: Number(document.getElementById("iSlowWidth").value)
	}

	for(iSamples = 0; iSamples < tend*fsampling; iSamples++){
		var point = svg.createSVGPoint();
		point.x = wOffset+ wFactor* calcFast(iSamples, fsampling, cfast);
		point.y = svgSize.clientHeight-(hOffset+hFactor* calcSlow(iSamples, fsampling, cslow));
		polyline.points.appendItem(point);
	}
	
}
function calcFast(i, fs, cfast)
{
	if(cfast.type == "Sawtooth")
		return calcFastSawtooth(i, fs, cfast);
	if(cfast.type == "Sine")
		return calcFastSine(i, fs, cfast);
	if(cfast.type == "Constant")
		return calcFastConst(cfast);
	return 0;
}
function calcFastConst(cfast)
{
	return cfast.offset + cfast.dcVal;
}
function calcFastSawtooth(i, fs, cfast)
{
	var deltaAmp = 2*cfast.amp;
	var fullperiods = Math.floor((i/fs)/(1/cfast.freq));
	var actPhase = (i/fs - fullperiods/cfast.freq) *cfast.freq;
	actPhase = actPhase + cfast.phase/(2*Math.PI);
	if(actPhase >= 1)
		actPhase = actPhase - 1;

	if(actPhase < cfast.width)
	{
		return cfast.offset-cfast.amp + deltaAmp/cfast.width*actPhase;
	}
	else
	{
		return cfast.offset+cfast.amp - deltaAmp/(1-cfast.width)*(actPhase-cfast.width);
	}	
}

function calcFastSine(i, fs, cfast)
{
	return 	cfast.offset + cfast.amp*Math.sin(2*Math.PI*cfast.freq*i/fs+cfast.phase);
}
function calcSlowSine(i, fs, cslow)
{
	return 	cslow.offset + cslow.amp*Math.sin(2*Math.PI*cslow.freq*i/fs+cslow.phase);
}
	
function calcSlowConst(cslow)
{
	return cslow.offset + cslow.dcVal;
}
function calcSlow(i, fs, cslow)
{
	if(cslow.type == "Sawtooth")
		return calcSlowSawtooth(i, fs, cslow);
	if(cslow.type == "Sine")
		return calcSlowSine(i, fs, cslow);
	if(cslow.type == "Constant")
		return calcSlowConst(cslow);
	return 0;
}

function calcSlowSawtooth(i, fs, cslow)
{
	var deltaAmp = 2*cslow.amp;
	var fullperiods = Math.floor((i/fs)/(1/cslow.freq));
	var actPhase = (i/fs - fullperiods/cslow.freq) *cslow.freq;
	actPhase = actPhase + cslow.phase/(2*Math.PI);
	if(actPhase >= 1)
		actPhase = actPhase - 1;
	if(actPhase < cslow.width)
	{
		return cslow.offset-cslow.amp + deltaAmp/cslow.width*actPhase;
	}
	else
	{
		return cslow.offset+cslow.amp - deltaAmp/(1-cslow.width)*(actPhase-cslow.width);
	}	
}
