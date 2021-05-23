% test circuit current source

V = 11.2;
R = [1 2 3 4 5 6 7 8 9 10 20 30 40 50 60 70 80 90];
Vm = [9 16 23 29.8 36.4 42.8 49 56.8 60.3 65 127.4 190 254 317 380 445 510 575]*1e-3;
R2 = [10 20 30 40 50 60 70 80 90];
Vm2 = [130 259 386 514 645 776 909 1024 1176 ]*1e-3;
Vm3 = [238 471 707 950 1194 1445 1699 1957 2226]*1e-3;

I = Vm2./R2*1000;
I2 = Vm3./R2*1000;

figure(1)
plot(R2, I, R2, I2)
xlabel('Load Resistance / Ohm')
ylabel('Current / mA');
title('Current-Load-Characteristic')
axis([10 90 0 30])

I_mean = mean(I);
Vu = I_mean/V