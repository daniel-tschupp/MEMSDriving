%% This Script creates a State-Space model for the magnetic MEMS mirror 
%  If fits the model parameters to the measurement values.

%% Measurement of X-Axis MEMS-Mirror 
clf
clear 
clc

[f, V, Vi, h] = textread('../Measurements/xAxisMeasData_old.txt', '%f %f %f %f', 'headerlines', 2);

R = 1.25 % Measured Resistor Value [Ohm]

I = Vi/R; % Calc Current
% Scanner data
angleOfTarget = 26*pi/180 
dMemsToTarget = 50

% Analysis of Measurement Data
Angle = atan2(h, 2*dMemsToTarget*tan(angleOfTarget)); % Calculating Angle from height 
AngleDeg = Angle * 180/pi;
Vu =  AngleDeg ./ I .* (23/7);
VuDB = 20*log10(Vu);
NInter = 1000;
fInter = linspace(f(1), f(length(f)), NInter);
VuDBInter = interp1(f,VuDB,fInter','linear');

% Calculating the theoretical Values
wtheo = logspace(2, 4, 200);
wtheo = fInter*2*pi;
VuTheo = calcTF(wtheo, 404.64, 23.390718, 3.11e-3);
VuTheoDB = 20*log10(VuTheo);

figure(1)
% semilogx(f, VuDB, '*-' , fInter, VuDBInter, '.', wtheo/(2*pi), VuTheoDB)
semilogx(f, VuDB, '*' , wtheo/(2*pi), VuTheoDB)
legend('Measured', 'Interpolation', 'Theoretical')
xlabel('Frequency / Hz')
ylabel('Amplification / dB')
title('Transferfunction of MEMS Mirror X-Axis')

finalSquareError = sum((VuTheoDB-VuDBInter).^2)./NInter

%% Fit theoretical Value to measurements.
dcGain = 23.390718;
damping = 3.11e-3;

param = [dcGain damping];
paramNew = fminsearch(@calcSquareError, param, [], fInter*2*pi, VuDBInter)

dcGainNew = paramNew(1);
dampingNew = paramNew(2);

[VuFit, sys] = calcTF(wtheo, 404.64, dcGainNew, dampingNew);
VuFitDB = 20*log10(VuFit);

figure(2)
semilogx(f, VuDB, '*' , wtheo/(2*pi), VuFitDB, '-')
legend('Measured', 'Theoretical')
xlabel('Frequency / Hz')
ylabel('Amplification / dB')
title('Transferfunction of MEMS Mirror X-Axis')

finalSquareError = sum((VuFitDB-VuDBInter).^2)./NInter

f = logspace(1, 4, 1000);
wtheo = f*2*pi;
[VuX, sysX] = calcTF(wtheo, 404.64, 1/0.042752, 3.223e-3);
VuX = 20*log10(VuX);
[VuY, sysY] = calcTF(wtheo, 243.32, 1/0.044109, 3.551e-3);
VuY = 20*log10(VuY);
figure(3)
bode(sysX, sysY)
% semilogx(f, VuX, '-' , f, VuY, '-')
 legend('X-Axis', 'Y-Axis')
% xlabel('Frequency / Hz')
% ylabel('Amplification / dB')
 title('Transferfunction of Sercalo MEMS mirror')

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
