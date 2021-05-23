load MEMS.mat

A = MEMS.A
B = MEMS.B
C = [1 0 0; 0 1 0; 0 0 1];
MEMS.C = C;
D = MEMS.D


% Building augmented model for LQR Controlled Model
Q = [1 0 0; 0 100 0; 0 0 0.5];
R = 0.01;

Klqr = lqr(A, B, Q, R)

Alqr = A-B*Klqr;
Kvf = 1/(C/(-Alqr+B*Klqr)*B)
Kvf = Kvf(3);
memsLQR = ss(Alqr, B, C, D);
%memsLQR = Kvf * memsLQR;

memsLQR.C
memsLQR.C = [180/pi 0 0; 0 180/pi 0; 0 0 1]

opt = stepDataOptions('StepAmplitude',0.2);
figure(1)
subplot(3,1,1)
step( MEMS(1,1), Kvf*memsLQR(1,1), 0.1, opt)
title('Anglular velocity Step Plot');
legend('MEMS', 'MEMS_LQR')
yaxis([0 10]);

subplot(3,1,2)
step( MEMS(2,1), Kvf*memsLQR(2,1), 0.1, opt)
title('Angle Step Plot');
legend('MEMS', 'MEMS_LQR')
yaxis([0 5]);

subplot(3,1,3)
step( MEMS(3,1), Kvf*memsLQR(3,1), 0.1, opt)
title('Current Step Plot');
legend('MEMS', 'MEMS_LQR')
yaxis([0 0.3]);


figure(2)
bode(MEMS, memsLQR)

figure(3)
subplot(3,1,1)
nyquist(MEMS(1,1), memsLQR(1,1))
axis([-6 6, -2 2])
subplot(3,1,2)
nyquist(MEMS(2,1), memsLQR(2,1))
axis([-6 6 -2 2])
subplot(3,1,3)
nyquist(MEMS(3,1), memsLQR(3,1))
axis([-6 6 -2 2])
