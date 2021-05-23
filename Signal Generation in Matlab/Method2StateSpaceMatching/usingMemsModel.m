% use MEMS Model
function sys = usingMemsModel(w)
% data from datasheet
R = 11.46;
r = 0.0062/2;
dcGain = 0.042752;

% assumptions
bf = 0.0000001;     % friction of mirror movement
wbreak_el = 5000;   % break frequency of electronics
J = 4.565e-9;       % Inertia of mems mirror

% fitted parameters
c = 19.2133;
gain = 2.2294e+03;

% Calculate the amplitude response as well as the state space plant
[VuDBFit, sys] = createSSModel(w, wbreak_el, dcGain, bf, R, J, r, c, gain);

end