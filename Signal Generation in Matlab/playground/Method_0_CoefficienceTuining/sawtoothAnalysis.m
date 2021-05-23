iFig = 1;
%% Sawtooth signal analysis

fsig = 100;
wsig = 2*pi*fsig;
T = 1/fsig;

nP = 5;
Ns = nP*100;
fs = Ns / (nP*T)

t = linspace(0, nP*T, Ns);
x = -sawtooth(wsig*t,1/2);

plotPS(x, t, fs, 1000, iFig, [0, 300, -120, 10]);
iFig=iFig+1;

%% Creating the model of the magnetic MEMS mirror

% X-Axis Params
xfres = 404.64;     % Hz
xRcoil = 11.46;     % Ohm
xDCGain = 0.042752; % A/�
xDamping = 3.223e-3 % 1
xwres = 2*pi*xfres % s^-1
% Y-Axis Params 
yfres = 243.32;     % Hz
yRcoil = 8.16;      % Ohm
yDCGain = 0.044109; % A/�
yDamping = 3.551e-3;% 1
ywres = 2*pi*yfres; % s^-1

s = tf('s')

xTPlant = 1/xDCGain / ( s^2/xwres^2 + 2*xDamping * s/xwres + 1 )
yTPlant = 1/yDCGain / ( s^2/ywres^2 + 2*yDamping * s/ywres + 1 )

figure(iFig)
iFig=iFig+1;
bode(xTPlant, yTPlant)
legend("xTPlant", "yTPlant")
title("Amplitude response of MEMS mirror")

saveas(gcf,strcat(num2str(iFig), '_TheoreticalAmplitudeResponseOfMEMSmirror'),'epsc');

%% Transfer Function of actuator
f = linspace(0, 10000, 100000);
w = 2*pi*f;
Cont = ControlSig(w, xwres, wsig, xDamping, 1/xDCGain);
figure(iFig)
iFig=iFig+1;
semilogx(f, 20*log10(Cont))
legend("xTPlant", "yTPlant")
title("Amplitude response of actuator")

saveas(gcf,strcat(num2str(iFig), '_AmplitudeResponseOfActuator'),'epsc');

%% Fourier Series Generation of Triangle singal

% hd = MEMS_IIR

for N=1:2:7
%N = 1;
fourier_coef = zeros(1, floor(N/2));
i = 1;
aMax = 1.1;
for n = 1:2:(N*2+1)
   fourier_coef(i) = aMax*8/pi^2*(-1)^((n-1)/2) / n^2; 
   i = i + 1;
end

%% creating wave from coeficients

% Creating wave from fourier coefficients
t = linspace(0, nP*T*10, Ns*10);
wave = zeros(1, length(t));
i = 1;
for n = 1:2:(N*2+1)
   wave = wave + 1 * fourier_coef(i) * sin(n*wsig*t);
   i = i + 1;
end
[y,t]=lsim(xTPlant, wave, t);

wave = wave(9*Ns:10*Ns);
y = y(9*Ns:10*Ns)
t = t(9*Ns:10*Ns);

% plotPS(wave, t, fs, 100, iFig, [0, 3000, -200, 10]);
% iFig=iFig+1;
% 
% plotPS(y, t, fs, 100, iFig, [0, 3000, -200, 10]);
% iFig=iFig+1;

% Creating wave from corrected fourier coefficients
fourier_w = 1:2:(N*2+1);
fourier_w = fourier_w * wsig;
C = ControlSig(fourier_w, xwres, wsig, xDamping, 1/xDCGain);

t = linspace(0, nP*T*10, 10*Ns);
wave = zeros(1, length(t));
i = 1;
for n = 1:2:(N*2+1)
    if( n*wsig <=350 || n*wsig > 450 )
        wave = wave + 1 * fourier_coef(i) * sin(n*wsig*t)*C(i);
    end
    i = i + 1;
end

[y,t]=lsim(xTPlant, wave, t);

wave = wave(9*Ns:10*Ns);
y = y(9*Ns:10*Ns)
t = t(9*Ns:10*Ns);

plotPS(wave, t, fs, 100, iFig, [0, 3000, -200, 10]);
iFig=iFig+1;

plotPS(y, t, fs, 100, iFig, [0, 3000, -200, 10]);
iFig=iFig+1;


%filter approach
% t = linspace(0, nP*T*10, Ns*10);
% wave = zeros(1, length(t));
% i = 1;
% for n = 1:2:(N*2+1)
%    wave = wave + 1 * fourier_coef(i) * sin(n*wsig*t);
%    i = i + 1;
% end
% wavefilt = filter(hd, wave)
% [y,t]=lsim(xTPlant, wavefilt, t);
% 
% wave = wave(9*Ns:10*Ns);
% y = y(9*Ns:10*Ns)
% t = t(9*Ns:10*Ns);
% 
% plotPS(wave, t, fs, 100, iFig, [0, 3000, -200, 10]);
% iFig=iFig+1;
% 
% plotPS(y, t, fs, 100, iFig, [0, 3000, -200, 10]);
% iFig=iFig+1;

end










function [xAbs, f] = plotPS(x, t, fs, zPadFactor, nFig, ax, print)
    xN = length(x)
    N = xN*zPadFactor
    xfft = fft(x, N);
    xfft = xfft .* conj(xfft) / xN;
    length(xfft)
    f = linspace(-fs/2, fs/2, N+1);
    f(N+1) = [];
    
    xAbs = abs(fftshift(xfft));
    
    length(f)
    length(xAbs)
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

function v = ControlSig(w, wres, wsig, damp, k)
    damp = damp * 0.1;
    w1 = 5000
    w2 = 50000
    wn = w/wres;
    
    A = 1 - wn.^2;
    B = 2*damp*wn;
    C = 1- w.^2 ./ ( w1 .* w2 );
    D = w ./ w1 + w ./ w2;
    
    Treal = (A.*C + B.*D) ./ (C.^2 + D.^2);
    Timag = (B.*C - A.*D) ./ (C.^2 + D.^2);
    v = 1/k* sqrt(Treal.^2 + Timag.^2);
end