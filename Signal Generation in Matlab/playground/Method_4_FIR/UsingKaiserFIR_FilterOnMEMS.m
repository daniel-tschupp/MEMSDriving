%% This Script creates a State-Space model for the magnetic MEMS mirror 
%  If fits the model parameters to the measurement values.

%% Measurement of X-Axis MEMS-Mirror 
clf
clear 
clc

[f, V, Vi, h] = textread('../Measurements/xAxisMeasData.txt', '%f %f %f %f', 'headerlines', 2);

R = 1.25 % Measured Resistor Value [Ohm]

I = Vi/R; % Calc Current
% Scanner data
angleOfTarget = 26*pi/180 
dMemsToTarget = 50

% Analysis of Measurement Data
Angle = atan2(h, 2*dMemsToTarget*tan(angleOfTarget)); % Calculating Angle from height 
AngleDeg = Angle * 180/pi
Vu =  AngleDeg ./ I .* (23/7)
VuDB = 20*log10(Vu);
NInter = 1000;
fInter = linspace(f(1), f(length(f)), NInter);
VuDBInter = interp1(f,VuDB,fInter,'linear');

% Calculating the theoretical Values
wtheo = logspace(2, 4, 200);
wtheo = fInter*2*pi;
VuTheo = calcTF(wtheo, 404.64, 23.390718, 3.11e-3);
VuTheoDB = 20*log10(VuTheo);


%% Fit theoretical Value to measurements.
dcGain = 23.390718;
damping = 3.11e-3;

test = VuDB(1:5);
testInt = poly(test);

param = [dcGain damping];

paramNew = fminsearch(@calcSquareError, param, [], fInter*2*pi, VuDBInter)

dcGainNew = paramNew(1);
dampingNew = paramNew(2);

[VuFit, sys] = calcTF(wtheo, 404.64, dcGainNew, dampingNew);
VuFitDB = 20*log10(VuFit);

finalSquareError = sum((VuFitDB-VuDBInter).^2)./NInter;

fs = 10000;
sysZ = c2d(sys, 1/fs, 'zoh')

%% Filter with Kaiser FIR Filter

num = load('kaiser60_0_55_10k_300_500.mat').kaiser60_0_55_10k_300_500
driver = dfilt.dffir(num)

nPer = 100;
fsig = 100;
N = fs/fsig;

t = linspace(0,nPer/fsig, N*nPer);
x = 5*sawtooth(2*pi*fsig*t-N/4, 0.5);

% driveSig = filt
driveSig = filter(driver, x) ./23;
respSig = lsim(sys, driveSig, t);
bareRespSig = lsim(sys, x, t) ./23;

figure(1)
subplot(3,1,1)
plot(t, x)
title('Reference Signal')
axis([0.45, 0.5, -5.5, 5.5])
xlabel('time / s')
ylabel('Angle /deg')

subplot(3,1,2)
plot(t, driveSig)
title('Driving Signal')
axis([0.45, 0.5, -0.3, 0.3])
xlabel('time / s')
ylabel('Current / I')

subplot(3,1,3)
plot(t, respSig, t, bareRespSig)
title('Resulting Motion')
axis([0.45, 0.5, -5.5, 5.5])
xlabel('time / s')
ylabel('Angle /deg')
legend('Response to Driving Signal', 'Response to triangle Signal')

figure(2)
driverSys = tf(num, 1, 1/fs)
bode(sysZ, driverSys, driverSys*sysZ)
legend('MEMS Amplitude Response', 'Driver Amplitude Response', 'Combination')

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