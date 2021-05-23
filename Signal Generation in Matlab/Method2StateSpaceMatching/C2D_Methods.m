figure(1)
H = tf([0.1 1],[1 4 5 5],'InputDelay', 0.3); 

% zoh
Hd = c2d(H,0.1,'zoh');
subplot(2,3,1)
step(H,'-',Hd,'--')
title('Method: ZOH')
axis([0, 2.2, 0, 0.20]);
legend('Continuouse Time Step Response', 'Discrete Time Step Response', 'Location','northwest');

% foh
Hd = c2d(H,0.1,'foh');
subplot(2,3,2)
step(H,'-',Hd,'--')
title('Method: FOH')
axis([0, 2.2, 0, 0.20]);
legend('Continuouse Time Step Response', 'Discrete Time Step Response', 'Location','northwest');

% tustin
Hd = c2d(H,0.1,'tustin');
subplot(2,3,3)
step(H,'-',Hd,'--')
title('Method: Tustin')
axis([0, 2.2, 0, 0.20]);
legend('Continuouse Time Step Response', 'Discrete Time Step Response', 'Location','northwest');

% impulse
Hd = c2d(H,0.1,'impulse');
subplot(2,3,4)
step(H,'-',Hd,'--')
title('Method: Impulse Response Matching')
axis([0, 2.2, 0, 0.20]);
legend('Continuouse Time Step Response', 'Discrete Time Step Response', 'Location','northwest');

% matched (pole zero match
Hd = c2d(H,0.1,'matched');
subplot(2,3,5)
step(H,'-',Hd,'--')
title('Method: Zero-Pole matching')
axis([0, 2.2, 0, 0.20]);
legend('Continuouse Time Step Response', 'Discrete Time Step Response', 'Location','northwest');

fig = gcf;
fig.PaperPositionMode = 'auto';
fig.PaperUnits = 'inches';
fig.PaperPosition = [0 0 18 9];
saveas(gcf,'StepResp_Methods','png');

figure(2)
% zoh
Hd = c2d(H,0.1,'zoh');
subplot(2,3,1)
pzplot(Hd)
title('Method: ZOH')

% foh
Hd = c2d(H,0.1,'foh');
subplot(2,3,2)
pzplot(Hd)
title('Method: FOH')

% tustin
Hd = c2d(H,0.1,'tustin');
subplot(2,3,3)
pzplot(Hd)
title('Method: Tustin')

% impulse
Hd = c2d(H,0.1,'impulse');
subplot(2,3,4)
pzplot(Hd)
title('Method: Impulse Response Matching')

% matched (pole zero match
Hd = c2d(H,0.1,'matched');
subplot(2,3,5)
pzplot(Hd)
title('Method: Zero-Pole matching')

fig = gcf;
fig.PaperPositionMode = 'auto';
fig.PaperUnits = 'inches';
fig.PaperPosition = [0 0 18 9];
saveas(gcf,'PZ-Plots_Methods','png');