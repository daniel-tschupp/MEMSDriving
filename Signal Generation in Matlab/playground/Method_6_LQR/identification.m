clf; clc; clear;

[V, f, h] = textread('../Measurements/xAxisMeasData.txt', '%f %f %f', 'headerlines', 2);

% Scanner data
angleOfTarget = 7
dMemsToTarget = 45

% Analysis of Measurement Data
Angle = atan2( (h / tand(angleOfTarget)), (2*dMemsToTarget))  % Calculating Angle from height 

AngleDeg = Angle * 180/pi;
Vu =  AngleDeg ./ V;
VuDB = 20*log10(Vu)';
w=f*2*pi;


fs = 10000;
t = linspace(0,1, fs);
in = zeros(1,length(t));
out = zeros(1,length(t));
for i=1:length(f)
    in = in +  V(i) * sin(2*pi*f(i)*t);
    out = out + AngleDeg(i) * sin(2*pi*f(i)*t);
end


ti = linspace(0, 0.1, 10000)';
u = cos(ti .* ti * 2*pi*10000);
ufl = flipud(u);
u = [u;ufl;u;ufl];
t = (0:length(u)-1)*(ti(2)-ti(1));

in = u;
s = tf('s');
G = 50/(50+s) * 800/(800 + s) ;

out = lsim(G, in, t);

figure(1)
subplot(1,2,1)
plot(t, in)
subplot(1,2,2)
plot(t, out)

out = out';
% out = out(1:6);
n = 6; %nPoles
m = 6; %mZeros
nParam = n+m;

Y = zeros(length(out)-n, 1);
Theta = zeros(length(Y), nParam);

for i=n+1:length(out)
    outIdx = i-n;
    Y(outIdx) = out(i);
    for k=1:n
        y_i = out(i-k);
        Theta(outIdx,k) = -y_i;
    end;
    for l=1:m
        x_i = in(i-l);
        Theta(outIdx,l+n) = x_i;
    end;
end
out;
Theta;
size(Theta)
size(out)
size(in)

Phi = Theta\Y

A = Phi(1:n)
B = Phi(n+1:n+m)
tst = filter(B,A, in);
 
figure(2)
plot(t, tst)