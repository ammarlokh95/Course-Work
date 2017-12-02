module PowerSet where

import Set

powerSet :: Ord one => Set one -> Set (Set one)
powerSet st
	| isEmpty st = insert empty empty
	| otherwise = let (first, rest) = split st
                  in let powerSetRest = powerSet rest
                     in union powerSetRest (mapSet (insert first) powerSetRest)
