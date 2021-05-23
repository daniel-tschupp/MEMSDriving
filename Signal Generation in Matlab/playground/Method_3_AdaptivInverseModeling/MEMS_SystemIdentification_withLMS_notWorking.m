%% NLF Problem 1, System identification

 clear all               % clears all active variables
% close all
% clc;

% Defining Unknown FIR Filter function for testing the adaptive LMS Filter
z = tf('z');
% X-Axis Params
xfres = 404.64;     % Hz
xRcoil = 11.46;     % Ohm
xDCGain = 0.042752; % A/�
% xDamping = 3.223e-3 % 1
xDamping = 3.223e-3 % 1
xwres = 2*pi*xfres % s^-1
s = tf('s')

fs = 10000;
Ts = 1/fs;
% s = 2/Ts * (1-z^(-1))/(1-z^(-1));
H = 1/xDCGain / ( s^2/xwres^2 + 2*xDamping * s/xwres + 1 )
Hd = c2d(H, Ts, 'zoh')
figure(3)
pzplot(Hd)

bode(H, Hd)
grid on
return
%x = randn(1,100);
fsig = 100;
nPer = 100;
nPer/fsig
fs/fsig*nPer
t = linspace(0, nPer/fsig, fs/fsig*nPer);
%x = square(2*pi*50*t*100);

x = randn(1,length(t));     % Teaching signal containing all frequencies
d = lsim(Hd, x);

% LMS
order = 30;
w = zeros(1, order)';
xs = zeros(1, order);
ds = zeros(1, order);
%w = [0;0;0];
for i=1:length(x)
    w = [w, LMS(x(i), d(i), 0.001, order)];
end
figure(1)
plot(w')

coef = w(:, length(w))
% Hlearned = 0;
% for k=1:1:length(coef)
%     k
%     Hlearned = Hlearned + coef(k)*z^(-(k-1));
% end

Hlearned = c2d(tf([0 0 0 1], coef'), Ts, 'zoh');

Hlearned
dz = lsim(Hlearned, x);
figure(4)
bode(Hlearned)
figure(2)
plot(t, d, t, dz)
title('Simulated signal')
xlabel('time / s')
ylabel('amplitude / 1')
legend('Original', 'FIR')
% subplot(4,1,1)
% plot(t, x)
% 
% subplot(4,1,2)
% plot(t, d)


function w = LMS(xi, di, u, order)
    persistent xs
    persistent ws
    
    if(isempty(ws))
       ws = zeros(order, 1);
       xs = zeros(order, 1);
    end
    
    % adding the new values
    xs = [xi; xs];
    xs(order+1) = [];
    
    % Calculate estimated output
    y = ws'*xs;
    
    % Calculate Error to measured output
    e = di - y;
    
    % Correct Weightings
    ws = ws + u / ( 0.01 + xs'*xs ) * e * xs;
    
    % Return weighting
    w=ws;
end