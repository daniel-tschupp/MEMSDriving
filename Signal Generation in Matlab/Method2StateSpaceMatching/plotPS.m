function [xAbs, f] = plotPS(x, t, fs, zPadFactor, nFig, ax, name, wGs, Gs)
    xN = length(x);
    N = xN*zPadFactor;
    xfft = fft(x, N);
    xfft = xfft .* conj(xfft) / xN;
    f = linspace(-fs/2, fs/2, N+1);
    f(N+1) = [];
    xAbs = abs(fftshift(xfft));
    [Amp, I] = max(xAbs);
    T = 1/abs(f(I))
    if(nFig > 0)
        figure(nFig)
        subplot(1,2,1)
        plot(t, x, '.-')
        title('Time Domain Signal')
        xlabel('Time / s')
        ylabel('Amplitude / 1')
        xlim([0, 5*T])
        grid on
        
        subplot(1,2,2)
        %plot(f, abs(fftshift(xfft)), f, 200+real(fftshift(xfft)), f, imag(fftshift(xfft)-100*i));
        if exist('wGs','var')
            plot(f, 20*log10(xAbs/max(xAbs)), '-', wGs/(2*pi), Gs, '-');
            legend('Power Spectrum Sawtooth', 'System Transfer Function', 'Location','southwest')
        else
            plot(f, 20*log10(xAbs/max(xAbs)), '-');
        end
        
        title(name)
        xlabel('Frequency / Hz')
        ylabel('Amplitude / 1')
        axis(ax)
        grid on
        if(name ~= "none")
            fig = gcf;
            fig.PaperPositionMode = 'auto';
            fig.PaperUnits = 'inches';
            fig.PaperPosition = [0 0 18 9];
            saveas(gcf,name,'png');
        end
    end
end