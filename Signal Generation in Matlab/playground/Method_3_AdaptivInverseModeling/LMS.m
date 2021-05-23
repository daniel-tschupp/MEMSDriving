function [w, y] = LMS(xi, di, u, order)
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