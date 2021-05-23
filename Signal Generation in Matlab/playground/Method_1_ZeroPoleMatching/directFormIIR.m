function yi = directFormIIR(b, a, xi)
    persistent zi;
    
    order = length(b);
    if(isempty(zi))
       zi = zeros(order-1, 1);
    end
    
    % Direct form II merged implementation
    sumB = 0;
    sumA = 0;
    for i=2:order
       sumB = sumB + zi(i-1)*b(i);
       sumA = sumA + zi(i-1)*a(i);
    end    
    zNew = sumA+xi;
   
    % adding the new value to z
    zi = [zNew; zi];
    zi(order) = [];
    
    yi = zNew * b(1) + sumB;
end

% 
% function yi = directFormIIR(b, a, xi)
%     persistent zx;
%     persistent zy;
%     
%     order = length(b);
%     if(isempty(zx))
%        zx = zeros(order-1, 1);
%        zy = zeros(order-1, 1);
%     end
%     
%     % Direct form I implementation
%     sumForward = 0;
%     for i=2:order
%        sumForward = sumForward + zx(i-1)*b(i);
%     end  
%     
%     
%     sumBackward = 0;
%     for i=2:order
%        sumBackward = sumBackward + zy(i-1)*a(i);
%     end    
%     
%     sumForward = sumForward + xi*b(1);
%     yi = sumForward + sumBackward;
%     
%     % adding the new x value to zx
%     zx = [xi; zx];
%     zx(order) = [];
%     
%     % adding the new y value to zy
%     zy = [yi; zy];
%     zy(order) = [];
%        
% end