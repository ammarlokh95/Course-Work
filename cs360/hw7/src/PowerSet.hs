-- 1 hour
module PowerSet where

import Set

powerSet :: Ord a => Set a -> Set (Set a)
powerSet a	| isEmpty a = insert empty empty
				| otherwise = let (first, rest) = split a
									in let pRest = powerSet rest 
										in union pRest (mapSet (insert first) pRest)
