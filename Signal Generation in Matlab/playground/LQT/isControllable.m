%% Parameters 
%% A	(type:matrix --> State Matrix of System)
%% B	(type:matrix --> Input Matrix of System)
%%
%% Return 
%% Pc 	(type:matrix --> Controllability matrix)
function [Pc]=isControllable(A, B)
	
	Aa = 1;
	Pc = B;

	for i=2:length(B')
		Aa = A*Aa;
		Pc = [Pc Aa*B];
	end

	Rank_Pc = rank(Pc)
	if Rank_Pc == length(B')
		controllability = 'System is controllable'
	else
		controllability = 'System is NOT controllable'
	end
end
