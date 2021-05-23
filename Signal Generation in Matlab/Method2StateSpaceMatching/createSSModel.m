function [Vu, sys] = createSSModel(w, wbreak_el, dcGain, bf, R, J, r, c, gain)
    L = R/wbreak_el;        % Calculate Induction to meet el. break frequency
    K = c*r^2/(4*dcGain);   % Calculate generator coefficient to meet dc response of data sheet
    
    % Create State Space matrices
    A = [   -bf/J,              -r*c/(J*2),      K/J;
            1,                  0,             0;
            -K/L,               0,              -R/L    ];
        
    B = [   0,                  0,              gain/L     ]';
    
    C = [   0,                  1,          0       ];
    D = [   0                                      ];

    sys = ss(A, B, C, D, "StateName", {"Velocity", "Position", "Current"}, "InputName", "Voltage");
    
    [mag, phase] = bode(sys, w);
    Vu = 20*log10(squeeze(mag));
end
