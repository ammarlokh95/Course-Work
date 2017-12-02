-- 2-3 hours
module NfaMatch (nfaMatch) where

import Nfa
import Set

nfaMatch :: Nfa Int -> String -> Bool
nfaMatch (Nfa qs moves q0 fin) str = not (empty == (nfaTransition (Nfa qs moves q0 fin) str `intersection` fin))


nfaFold :: (Set a -> Char -> Set a) -> Set a -> String -> Set a
nfaFold f r [] = r
nfaFold f r (c:cs) = nfaFold f (f r c) cs

nfaTransition :: Ord a => Nfa a -> String -> Set a
nfaTransition (Nfa qs moves q0 fin) str = nfaFold step start str
		 where
		 step set ch = onetrans (Nfa qs moves q0 fin) set ch
		 start = epsilonClosure (Nfa qs moves q0 fin) (singleton q0)	  
