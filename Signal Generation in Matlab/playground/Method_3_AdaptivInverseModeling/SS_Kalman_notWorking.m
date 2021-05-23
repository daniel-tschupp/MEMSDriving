%% This is an exercise for kalman filtering
% It's about filtering the measured movement of a vehicle.

%% Given Values
% state vector x = [s, v]' (position, velocity)

% X-Axis Params
xfres = 404.64;     % Hz
xRcoil = 11.46;     % Ohm
xDCGain = 0.042752; % A/�
xDamping = 3.223e-3 % 1
xwres = 2*pi*xfres % s^-1

Ts = 0.0001;
A = [-2*xDamping, -1, xDCGain; 1, 0, 0; 0, 0, 0];
P0 = [1, 0, 0; 0, 1, 0; 0, 0, 1];
C = [0, 1, 0];
x_0 = [0, 0, 0]';


%% creating measurements with measurement noise Normal distributed with simga^2=100
rng(0,'twister');
sigma = 0;
measNoise = sigma.*randn(10/Ts,1) + 0;
procNoise = sigma.*randn(10/Ts,1) + 0;

%% create original movement
x = linspace(0, 10, 10/Ts)';
s_next = 0;
y = zeros(10/Ts,1);
for i=1:length(x)
   y(i) = s_next + (v+procNoise(i))*Ts;
   s_next = y(i);
end

ymeas = y + measNoise;

%% Filter the measured signal with a kalman filter
% create output data vector
yfiltered = zeros(10/Ts, 1);

% Step 1 init k_K, x_0, P_0
% P_0 = P0;
% RK = 1000;  % assumption no process noise
% QK = [0, 0; 0, 10];  % assumption no measurement noise
% I = [1 0; 0 1]; % Unity matrix

for i=1:length(ymeas)
   [yfiltered(i), p] = kalmi(x_0, ymeas(i), A, C, P0); 
   p
end

% 
% % init first loop
% P_next = P_0;
% x_next = x_0;
% for i=1:length(ymeas)
%     % get values from previous loop
%     P_prev = P_next;
%     x_prev = x_next;
%     
%     % Step 2: compute Kalman gain 
%     k_K = P_prev * C'*inv(C*P_prev*C' + RK);
%     % Step 3: update estimation of new position with measurement
%     x_act = x_prev + k_K*(ymeas(i)-C*x_prev);
%     % Step 4: Compute Error covariance matrix
%     P_act = (I-k_K*C)*P_prev;
%     % Step 5: Project ahead
%     x_next = A*x_act;
%     P_next = A*P_act*A'+QK;
%     
%     yfiltered(i) = C*x_next;
% end


%% Plotting results
figure(1) 
plot(x, y, x, ymeas, x, yfiltered)
title('Position of vehicle')
xlabel('time/s')
ylabel('position/m')
legend('real position', 'measured position', 'filtered position')


function [y, pact, qk] = kalmi(x0, yi, A, C, P0)
    I = [1 0; 0 1]; % Unity matrix
    
    % Step 1 init k_K, x_0, P_0
    persistent PNext;
    persistent XNext;
    persistent RK;
    persistent QK;
    
    if(isempty(PNext))
        RK = 1000;  % assumption no process noise
        QK = [0, 0; 0, 10];  % assumption no measurement noise
        PNext = P0;
        XNext = x0;
    end
    
    
    % get values from previous loop
    P_prev = PNext;
    x_prev = XNext;
    
    % Step 2: compute Kalman gain 
    k_K = P_prev * C'*inv(C*P_prev*C' + RK);
    % Step 3: update estimation of new position with measurement
    x_act = x_prev + k_K*(yi-C*x_prev);
    % Step 4: Compute Error covariance matrix
    P_act = (I-k_K*C)*P_prev;
    % Step 5: Project ahead
    XNext = A*x_act;
    PNext = A*P_act*A'+QK;
    
    y = C*XNext;
    pact = P_act;
    qk = QK;
end