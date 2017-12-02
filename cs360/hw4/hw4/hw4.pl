
% Find the sum of all number in a list
sum(0, []).
sum(Total, [L | LS]) :- sum(Y, LS), Total is L + Y.

%Find the smallest number in a list
min(X,[X]).
min(X,[L,S|Ls]) :- L =< S, min(X,[L|Ls]).
min(X,[L,S|Ls]) :- L > S, min(X, [S|Ls]).

%delete an element from a list
del(0,Xs,Xs). 
del(X,[L|Xs],[L | Y]):- X =\= L, del(X,Xs,Y).
del(X,[L|Xs],Y) :- X == L, del(0,Xs,Y).

%sort a list of numbers
min_sort([],[]).
min_sort(X,[Y|Ys]) :- min(Y,X), del(Y,X,L), min_sort(L,Ys).

%Check if the list inputted follows the grammar
sentence(X) :- noun_phrase(X,Xs),verb_phrase(Xs,Ys),fullstop(Ys).
noun_phrase([X,Y|Xs],Xs) :- article(X),noun(Y).
verb_phrase([L|Ls],Ys) :- verb(L),noun_phrase(Ls,Ys).
article(a).
article(the).
noun(girl).
noun(dog).
verb(sees).
verb(pets).
fullstop(X):- equal(X,'.').
equal([X],X).

% Can you use the same program to generate all possible sentences tha%t can be
% derived from the grammar? Does the order of the subgoals in your rules make a
% difference?
%Yes, you can generate all possible sentences that can be derived from the
%grammar using the same program.
%Yes, the order of the subgoals does matter. 

% Problem 1: 30-40 mins
% Problem 2: 1 hour
% Problem 3: 3-4 hours
% Problem 4: 3 hours
