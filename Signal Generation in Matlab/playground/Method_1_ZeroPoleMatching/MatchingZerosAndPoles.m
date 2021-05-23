%% This Script creates a State-Space model for the magnetic MEMS mirror 
%  If fits the model parameters to the measurement values.

%% Measurement of X-Axis MEMS-Mirror 
clf
clear 
clc
clear directFormIIR % delets persistant data inside directFormIIR

[V, f, h] = textread('../Measurements/xAxisMeasData.txt', '%f %f %f', 'headerlines', 2);

% Scanner data
angleOfTarget = 7 
dMemsToTarget = 45

% Analysis of Measurement Data
Angle = atan2( (h / tand(angleOfTarget)), (2*dMemsToTarget))  % Calculating Angle from height 

AngleDeg = Angle * 180/pi
Vu =  AngleDeg ./ V
Vu = Vu ./ Vu(1);
VuDB = 20*log10(Vu);

% Calculating the theoretical Values
wtheo = f*2*pi;
VuTheo = calcTF(wtheo, 404.64, 23.390718, 3.11e-3);
VuTheoDB = 20*log10(VuTheo);

SquareErrorUnfitted = sum((VuTheoDB-VuDB).^2)./length(VuDB)

%% Fit theoretical Value to measurements.
dcGain = 23.390718;
dcGain = 0;
damping = 3.11e-3;

param = [dcGain damping];
paramNew = fminsearch(@calcSquareError, param, [], f*2*pi, VuDB)
dcGainNew = paramNew(1)
dampingNew = paramNew(2)

[VuFit, sys] = calcTF(wtheo, 404.64, 1, dampingNew); % generating 
% System with a dc gain of 1 as the resulting fir filter shall not
% compensate for the dc gain. (For simulation the system will be
% recalculated with the accurate dc gain later on.

figure(1)
subplot(3,1,1)
wSys = linspace(10*2*pi,1000*2*pi, 100000);
[VuFitVis, sys2] = calcTF(wSys, 404.64, dcGainNew, dampingNew);
semilogx(f*2*pi, VuDB, wSys, VuFitVis);
title('Amplitude Response of MEMS mirror')
xlabel('Circular Frequency w / Hz')
ylabel('Amplification / dB')
legend('Linearly interpolated measurement data', 'Created model', 'Location','northwest')

subplot(3,1,2)
pzplot(sys2)

subplot(3,1,3)
step(sys2)

%% Match poles
fs = 10000;
sysZ = c2d(sys, 1/fs, 'zoh')
iir = 1/sysZ

nPer = 200;
fsig = 10;
N = fs/fsig;

t = linspace(0,nPer/fsig, N*nPer+1);
x = 1.5*sawtooth(2*pi*fsig*t+pi/2, 0.5);

s = tf('s');
TP = c2d(1/(1+s/8000)*1/(1+s/10000), 1/fs, 'zoh');
iir = iir * TP

%% Using direct form implementation of iir
den = iir.den{1};
a = -(den / abs(den(1)));
num = iir.num{1};
b = num / abs(den(1));
iir2 = filt(b, a, 1/fs);
figure(2)
bode(iir, iir2)
yDirect = zeros(1, length(x));
a
b

for i=1:length(x)
   yDirect(i) = directFormIIR(b,a,x(i)); 
end
% yDirect = filter(b, a, x); % filter function for testing purposes

% Quantization to a 12 bit resolution
yDirect = yDirect * 4096;
yDirect = floor(yDirect);
yDirect = yDirect / 4096;

%% Eighter calc electronic gain and use datasheet dc gain of mirror
[VuFit, sys] = calcTF(wtheo, 404.64, dcGain, dampingNew); 
electronicGainCompensation = 8.2/62; % (8.2Ohm / 62Ohm = gain)
yMD = lsim(sys, yDirect * electronicGainCompensation, t);
yMU = lsim(sys, x * electronicGainCompensation, t);
%% OR use directly the new DC Gain from the optimization
% [VuFit, sys] = calcTF(wtheo, 404.64, dcGainNew, dampingNew);
% yMD = lsim(sys, yDirect, t);
% yMU = lsim(sys, x, t);
VuFitDB = 20*log10(VuFit);
finalSquareError = sum((VuFitDB-VuDB).^2)./length(f)
figure(6)
pzplot(sys)

plotPS(x, t, fs, 100, 3, [0, 3000, -200, 10]);
plotPS(yMD, t, fs, 100, 4, [0, 3000, -200, 10]);

figure(5)
subplot(3,1,1)
plot(t, x, '.-', t , yMD, '.-')
legend('Reference Signal / V','MEMS response on driving signal / °')
xlabel('time / s')

% subplot(3,1,1)
% [hAx, hline1, hline2] = plotyy(t, yDirect, t , yMD)
% hline1.LineStyle = '-';
% hline1.Marker = '.';
% hline2.LineStyle = '-';
% hline2.Marker = '.';
% legend('Driving Signal','MEMS response')
% xlabel('time / s')
% ylabel(hAx(1),'Current / A') % left y-axis 
% ylabel(hAx(2),'Angle / deg') % right y-axis
% xlim(hAx, [0.0, 0.07]);
title('Simulated mirror response on driving signal')
fig = gcf;
fig.PaperPositionMode = 'auto';
fig.PaperUnits = 'inches';
fig.PaperPosition = [0 0 18 9];
saveas(gcf,'MEMS_RespOnDrive','png');

subplot(3,1,2)
plot(t, yDirect+1.65)
%plot(t, yDirect)
xlabel('time / s')
ylabel('DAC Voltage / V')
legend( 'Driving Signal simulation', 'Driving Signal iir implementation')
% axis([0.0, 0.05, -0.25, 0.25]);

subplot(3,1,3)
plot(t, x, '.-', t, yMU, '.-')
legend('Triangle input Signal', 'MEMS response on triangle signal')
xlabel('time / s')
% ylabel(hAx(1),'Current / A') % left y-axis 
% ylabel(hAx(2),'Angle / deg') % right y-axis
% xlim(hAx, [0.0, 0.07]);
title('Simulated mirror response on triangle signal')
axis([0.0, 0.04, -5, 5]);

fig = gcf;
fig.PaperPositionMode = 'auto';
fig.PaperUnits = 'inches';
fig.PaperPosition = [0 0 18 9];
% saveas(gcf,'MEMS_RespOnTriangle','png');
print('MEMS_RespOnTriangle','-dpng','-r0')

clear directFormIIR
return
%% Finding frequency linear correction curve
freqs = 1:10:300;

corrFactors = zeros(1, length(freqs))

for i=1:length(freqs)
    fsig = freqs(i);
    nPer = 100;
    N = fs/fsig;
    t1 = linspace(0,nPer/fsig, N*nPer+1);
    x1 = 5*sawtooth(2*pi*fsig*t1+pi/2, 0.5);
    
    yDirect1 = zeros(1, length(x1));
    for k=1:length(x1)
        yDirect1(k) = directFormIIR(b,-a,x1(k)); 
    end
    yMD1 = lsim(sys, yDirect1, t1);
    yMax = max(yMD1);
    corrFactors(i) = 5 / yMax;
    clear directFormIIR
end

param = [1, 1];
paramNew = fminsearch(@calcSErrorLin, param, [],  freqs, corrFactors)

figure(2)
plot(freqs, corrFactors, freqs, paramNew(1)*freqs+paramNew(2))
title('Frequency dependent correction')
xlabel('Frequency / Hz')
ylabel('Correction Factor')

function [Vu, sys] = calcTF(w, fres, dcGain, damping)
    s = i*w;
    wres = fres*2*pi;
    Vu = abs(dcGain ./ ( s.^2./wres.^2 + 2*damping * s./wres + 1 ));
    s = tf('s');
    sys = dcGain / ( s^2/wres^2 + 2*damping * s/wres + 1 );
end

function err = calcSquareError(param, w, VuDB)
    VuTheo = 20*log10(calcTF(w, 404.64, param(1), param(2)));
    err = sum((VuDB - VuTheo).^2);
end

function res = calcSErrorLin(param, f, corr)
    a = param(1);
    b = param(2);
    res = sum((a*f+b - corr).^2);
end

function [xAbs, f] = plotPS(x, t, fs, zPadFactor, nFig, ax, print)
    xN = length(x);
    N = xN*zPadFactor;
    xfft = fft(x, N);
    xfft = xfft .* conj(xfft) / xN;
    f = linspace(-fs/2, fs/2, N+1);
    f(N+1) = [];
    
    xAbs = abs(fftshift(xfft));
    if(nFig > 0)
        figure(nFig)
        subplot(1,2,1)
        plot(t, x, '.-')
        title('Time Domain Signal')
        xlabel('Time / s')
        ylabel('Amplitude / 1')
        grid on
        
        subplot(1,2,2)
        %plot(f, abs(fftshift(xfft)), f, 200+real(fftshift(xfft)), f, imag(fftshift(xfft)-100*i));
        plot(f,20*log10(xAbs/max(xAbs)), '-');
        title('Power Spectrum')
        xlabel('Frequency / Hz')
        ylabel('Amplitude / 1')
        axis(ax)
        grid on
        %saveas(gcf,strcat(num2str(nFig), '_PowerSpectrum'),'epsc');
    end
end
