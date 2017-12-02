-- 30 mins
module Interpreter (
    emptyState,

    sStep
  ) where

import Prelude hiding (lookup)

import While

type Z = Integer

-- | In lecture, we represented the state as a function. Here we represent state
-- as an association list instead.
type State = [(Var,Z)]

-- | The empty state contains no bindings for any variables---it as an empty
-- association list.
emptyState :: State
emptyState = []

-- | 'lookup' finds a variable in the current state. Note that it is a partial
-- function---we don't gracefully handle use of an undefined variable.
lookup :: Var -> State -> Z
lookup v []                      = error $ "undefined variable " ++ show v
lookup v ((v',n):vs) | v' == v   = n
                     | otherwise = lookup v vs

-- | 'extend' adds or replaces a binding in the current state. To avoid an
-- exploding association list, we prune any previous binding for @v@.
extend :: State -> Var -> Z -> State
extend vs v n = (v,n) : filter (\(v', _) -> v' /= v) vs

-- | The semantic function for arithmetic expressions
aSem :: Aexp -> State -> Z
aSem (Const x)   _ = x
aSem (Var v)     s = lookup v s
aSem (Add a1 a2) s = aSem a1 s + aSem a2 s
aSem (Sub a1 a2) s = aSem a1 s - aSem a2 s
aSem (Mul a1 a2) s = aSem a1 s * aSem a2 s

-- | The semantic function for boolean expressions
bSem :: Bexp -> State -> Bool
bSem BTrue       _ = True
bSem BFalse      _ = False
bSem (Eq a1 a2)  s = aSem a1 s == aSem a2 s
bSem (Le a1 a2)  s = aSem a1 s <= aSem a2 s
bSem (Not b)     s = not (bSem b s)
bSem (And b1 b2) s = bSem b1 s && bSem b2 s

-- | The semantic function for statements
sStep :: Stm -> State -> State
sStep Skip          s = s
sStep (Out _)       s = s
sStep (Assign x xs) s = extend s x z
							 where z = aSem xs s
sStep (Seq s1 s2)   s = sStep s2 s'
						 	where s' = sStep s1 s
sStep (If b s1 s2)  s | (bSem b s) == True = sStep s1 s
							 | otherwise  = sStep s2 s
sStep (While b s1)  s | (bSem b s) = sStep (While b s1) s' 
							 | otherwise  = s
							 where s' = sStep s1 s




