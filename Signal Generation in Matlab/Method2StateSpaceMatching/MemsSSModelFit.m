%% Measurement of X-Axis MEMS-Mirror 
clf
clear 
clc

[V, f, h] = textread("../Measurements/xAxisMeasData.txt", "%f %f %f", "headerlines", 2);

% Scanner data
angleOfTarget = 7
dMemsToTarget = 45

% Analysis of Measurement Data
Angle = atan2( (h / tand(angleOfTarget)), (2*dMemsToTarget))  % Calculating Angle from height 

AngleDeg = Angle * 180/pi;
Vu =  AngleDeg ./ V;
VuDB = 20*log10(Vu)';
w=f*2*pi;

%% fit state space model to measurement data
% data from datasheet
R = 11.46;
r = 0.0062/2;
dcGain = 0.042752;

% assumptions
bf = 0.0000001;     % friction of mirror movement
wbreak_el = 3500;   % break frequency of electronics
J = 4.565e-9;       % Inertia of mems mirror


% Initial conditions for omptimization
c = 100;            % Spring coefficient of mems mirror
gain = 1000;        % Frequency independent gain of mems mirror
param = [c, gain];  % Parameter set to optimize

% Do optimization to find c and gain.
options = optimset("PlotFcns",@optimplotfval,"TolFun",1e-12);
[paramNew, error] = fminsearch(@calcSquareError, param, [], w, wbreak_el, VuDB, dcGain, bf, R, J, r);

% Overwrite the newly found parameters
c = paramNew(1)
gain = paramNew(2)

% Calculate the amplitude response as well as the state space plant
[VuDBFit2, sys] = createSSModel(w, wbreak_el, dcGain, bf, R, J, r, c, gain);
wSys = linspace(10*2*pi,1000*2*pi, 100000); 
[VuDBFit, sys] = createSSModel(wSys, wbreak_el, dcGain, bf, R, J, r, c, gain);
MEMS = sys;
save MEMS.mat MEMS

% Visualize Model
figure(1)
subplot(3,1,1)
semilogx(w, VuDB, "x", wSys, VuDBFit, "-");
title("Amplitude Response of MEMS mirror")
xlabel("Circular Frequency w / Hz")
ylabel("Amplification / dB")
legend("Measurement data", "Created model", "Location","northwest")

subplot(3,1,2)
pzplot(sys)
axis([-5000, 5000, -3000, 3000])
legend("Created model", "Location","northeast")

subplot(3,1,3)
step(sys)
legend("Created model", "Location","northeast")
fig = gcf;
fig.PaperPositionMode = "auto";
fig.PaperUnits = "inches";
fig.PaperPosition = [0 0 18 9];
saveas(gcf,"MEMS_ModelPlots","png");

finalSquareError = sum((VuDBFit2'-VuDB).^2)./length(w)
%% Calc Sensitivity

figure(99)
s=tf("s")
%bode( 1/(1+5*sys), 1*s^0, 5*sys / (1+5*sys), 5*sys)

w = linspace(1,10000,10000)
[Smag, Sangle] = bode(1/(1+sys), w)
[Tmag, Tangle] = bode(sys/(1+sys),w)
Smag = 20*log10(squeeze(Smag));
Tmag = 20*log10(squeeze(Tmag));
plot(w, Smag, w, Tmag)
title("Sensitivity plot")
legend("S(jw)", "T(jw)")
grid on
xlabel("frequency w / rad/s")
ylabel("magnitude / dB")

A = sys.A
B = sys.B
C = [0 0 1];%sys.C

Po = [C; C*A; C*A*A]
Pc = [B A*B A*A*B]

if(rank(Po) == 3)
	SystemIsObservable = "yes"
else
	SystemIsObservable = "no"
end
if(rank(Pc) == 3)
	SystemIsControllable = "yes"
else
	SystemIsControllable = "no"
end
if(rank(Po)==3 && rank(Pc)==3)
	SystemIsMinimal = "yes"
else
	SystemIsMinimal = "no"
end

%% Match poles
Tsys = tf(sys);
fs = 10000;
sysZ = c2d(Tsys, 1/fs, "impulse")
iir1 = 1/sysZ

nPer = 200;
fsig = 150;
N = fs/fsig;

t = linspace(0,nPer/fsig, N*nPer+1);
x = 1.5*sawtooth(2*pi*fsig*t+pi/2, 0.5);


s = tf("s");
TP = c2d(1/(1+s/6000)*1/(1+s/10000)*1/(1+s/10000), 1/fs, "impulse");
iir = iir1 * TP

figure(2)
subplot(1,2,1)
pzplot(iir,TP)
title("Discrete Time IIR Filter Pole-Zero plot")
subplot(1,2,2)
bode(iir)
title("Discrete Time IIR Filter Bode Diagram")
yDirect = zeros(1, length(x));
fig = gcf;
fig.PaperPositionMode = "auto";
fig.PaperUnits = "inches";
fig.PaperPosition = [0 0 18 9];
saveas(gcf,"IIR_TP_Discrete_Bode_impulse_fs10k","png");

%% Using direct form implementation of iir
clear directFormIIR % delets persistant data inside directFormIIR
den = iir.den{1}
a = den / abs(den(1))
num = iir.num{1};
b = num / abs(den(1))
figure(3)
bode(iir)


%% creating std vector with coefficients
aStr = "std::vector<float> den = {/*a*/ ";
for i=1:length(a)
    if(i<length(a))
        aStr = aStr + a(i) + "f, ";
    else
        aStr = aStr + a(i)+ "f";
    end
end
aStr = aStr + "};"
bStr = "std::vector<float> num = {/*b*/	";
for i=1:length(b)
    if(i<length(b))
        bStr = bStr + b(i) + "f, ";
    else
        bStr = bStr + b(i)+ "f";
    end
end
bStr = bStr + "};"

%% Using coefficients to simulate driver
FilterGain = 3.5;
for i=1:length(x)
   yDirect(i) = single(directFormIIR(b,a,single(x(i))))*FilterGain;
   if(yDirect(i) > 1.5)
       yDirect(i) = 1.5;
   end
   if(yDirect(i) < -1.5)
       yDirect(i) = -1.5;
   end
end

% Quantization to a 12 bit resolution
yDirect = yDirect * 4096;
yDirect = floor(yDirect);
yDirect = yDirect / 4096;

%% Eighter calc electronic gain and use datasheet dc gain of mirror
% electronicGainCompensation = 8.2/62; % (8.2Ohm / 62Ohm = gain)
% FilterGain = 1;  % (62Ohm / 8.2Ohm) * (1V / 1.5V) = 5.04
% yDirect = yDirect*FilterGain;
yMD = lsim(sys, yDirect, t);
yMU = lsim(sys, x, t);

PerVis = 15;
startx = (nPer-PerVis)*N/fs;
stopx = startx + PerVis/fs*N;
figure(4)
subplot(3,1,1)
[hAx, hline1, hline2] = plotyy(t, x, t , yMD)
hline1.LineStyle = "-";
hline1.Marker = ".";
hline2.LineStyle = "-";
hline2.Marker = ".";
legend("Reference Signal / V","MEMS response on driving signal / deg")
xlabel("time / s")
title("Simulated mirror response on driving signal")
ylabel(hAx(1),"DAC Voltage / V") % left y-axis 
ylabel(hAx(2),"Mirror Angle / deg") % right y-axis
xlim(hAx, [startx, stopx]);
ylim(hAx(1), [-5.5, 5.5]);
ylim(hAx(2), [-5.5, 5.5]);

subplot(3,1,2)
plot(t, yDirect+1.65)
title("MCU DAC Driving Signal")
xlabel("time / s")
ylabel("DAC Voltage / V")
legend( "Driving Signal simulation", "Driving Signal iir implementation")
axis([startx, stopx, 0, 3.3]);

subplot(3,1,3)
plot(t, x, ".-", t, yMU, ".-")
legend("Triangle input Signal", "MEMS response on triangle signal")
xlabel("time / s")
ylabel("Angle / deg")
title("Simulated mirror response on pure triangle signal")
axis([startx, stopx, -20, 20]);
fig = gcf;
fig.PaperPositionMode = "auto";
fig.PaperUnits = "inches";
fig.PaperPosition = [0 0 18 9];
saveas(gcf,"MEMS_Response","png");

%% Plotting Power Spectrum of Triangle Signal
TransferFunction = Tsys*d2c(iir,"tustin");
[mag, phase] = bode(TransferFunction, wSys);
AmpResp = 20*log10(squeeze(mag));

function err = calcSquareError(param, w, wbreak_el, VuDB, dcGain, bf, R, J, r)
    VuDBModel = createSSModel(w, wbreak_el, dcGain, bf, R, J, r, param(1), param(2))';
    err = sum((VuDB - VuDBModel).^2);
end
