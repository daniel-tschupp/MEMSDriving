%% FIR Filter test
clear 
clf
clc 

% X-Axis Params
xfres = 404.64;     % Hz
xRcoil = 11.46;     % Ohm
xDCGain = 0.042752; % A/°
xDamping = 3.223e-3 % 1
xwres = 2*pi*xfres % s^-1

s = tf('s')
xTPlant = 1/xDCGain / ( s^2/xwres^2 + 2*xDamping * s/xwres + 1 )

fs = 10000;
Hd = mems_iir_tscheb_6order;
nP = 10;
iFig = 1;

N = 10; % Number of harmonics
for fsig = 34:10:34
     wsig = 2*pi*fsig;
     Ns = floor(fs/fsig*nP);
     
%     t = linspace(0,nP*1/fsig, fs/fsig*nP);
%     x = sawtooth(2*pi*fsig*t, 0.5);
%     
%     xfilt = filter(Hd, x);
%     [y,t]=lsim(xTPlant, xfilt, t);
%     
%     xfilt = xfilt(0.9*Ns:Ns);
%     y = y(0.9*Ns:Ns);
%     t = t(0.9*Ns:Ns);
    
    fourier_coef = zeros(1, floor(N/2))
    i = 1;
    aMax = 1.1;
    for n = 1:2:(N*2+1)
       fourier_coef(i) = aMax*8/pi^2*(-1)^((n-1)/2) / n^2; 
       i = i + 1;
    end
    fourier_w = 1:2:(N*2+1);
    fourier_w = fourier_w * wsig;
    C = ControlSig(fourier_w, xwres, wsig, xDamping, 1/xDCGain);
    T = 1/fsig;
    t = linspace(0, nP*T*10, 10*Ns);
    wave = zeros(1, length(t));
    i = 1;
    for n = 1:2:(N*2+1)
        if( n*fsig <=310 || n*fsig > 450 )
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
        saveas(gcf,strcat(num2str(nFig), '_PowerSpectrum'),'epsc');
    end
end


function v = ControlSig(w, wres, wsig, damp, k)
    damp = damp * 0.1;
    w1 = 2*pi*500
    w2 = 2*pi*300
    wn = w/wres;
    
    A = 1 - wn.^2;
    B = 2*damp*wn;
    C = 1- w.^2 ./ ( w1 .* w2 );
    D = w ./ w1 + w ./ w2;
    
    Treal = (A.*C + B.*D) ./ (C.^2 + D.^2);
    Timag = (B.*C - A.*D) ./ (C.^2 + D.^2);
    v = 1/k* sqrt(Treal.^2 + Timag.^2);
end