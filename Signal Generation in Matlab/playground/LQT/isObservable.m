%% Parameters 
%% A	(type:matrix --> State Matrix of System)
%% C	(type:matrix --> Outpur Matrix of System)
%%
%% Return 
%% Po 	(type:matrix --> Observability matrix)
function [Po]=isObservable(A, C)
	
	Aa = 1;
	Po = C;

	for i=2:length(C)
		Aa = A*Aa;
		Po = [Po; C*Aa];
	end
	
	Rank_Po = rank(Po)
	if Rank_Po == length(C)
		observability = 'System is observable'
	else
		observability = 'System is NOT observable'
	end
end
