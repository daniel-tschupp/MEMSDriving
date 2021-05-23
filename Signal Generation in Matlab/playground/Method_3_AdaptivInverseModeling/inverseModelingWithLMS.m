%% Inverse Modeling MEMS Mirror using LMS

clear
clc

%% Loading MEMS model
w = 2*pi* logspace(1, 5, 1000);
addpath('../Method_2_StateSpaceMatching')
sys = usingMemsModel(w);

%% Creating Sawtooth singal

fsig = 100;             % Signal frequency [Hz]
fs = 10000;             % Sampling frequency [Hz]
nP = 200;               % Number of signal periods captuered
N = fs/fsig * nP;       % Number of samples
T = 1/fsig;             % Period duration
t = linspace(0, nP * T, N);
x = sawtooth(2*pi*fsig*t, 0.5);
%x = square(t);
figure(4)
plot(t, x)
%x = randn(1, 10000);
%% inverse modeling algoritm
%
%         _______        ____/__
%  x     |       |  u   |   /   |           y
% ------>| Plant |----->| LMS   |------------> 
%   |    |_______|      |_/_____|       |
%   |                    /              |
%   |                   /              -|
%   |                  /--------------- O
%   |                    _______       +|
%   |                   |       |       |
%   |------------------>| Delay |-------|
%                       |_______|   xd
%

order = 30;
xd = delay(x, order);

u = lsim(sys, x, t);

y = zeros(1, length(u));
ws = zeros(order, 1);
for i=1:length(u)
    [w, yt] = LMS(u(i), xd(i), 1, order);
    y(i) = yt;
    ws = [ws, w];
end
figure(1)
plot(t, x, t, y)

figure(2)
plot(ws(1:5, :)')



function  xd = delay(x, n)
    xd = x;
    for k=1:n
       xd = [ 0, xd ];
       xd(length(xd)) = [];
    end
end


