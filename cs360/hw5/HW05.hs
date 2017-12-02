{-# OPTIONS_GHC -fwarn-tabs #-}
{-
Name: Ammar Lokhandwala
Notes: Problem 1
1.1: 15 mins
1.2: 20 mins
1.3: 25 mins
1.4: 15 mins
1.5: 5 mins

Problem 2: 15 mins
-}

module HW05 where

isThisWorking :: String
isThisWorking = "Yes"
-- Load this file into GHCi (say, with `ghci HW05.hs`) and type
-- `isThisWorking` at the prompt. GHCi will tell you whether it's working!

-- Put your work below.
lastDigit :: Integer -> Integer
lastDigit a = mod a 10

dropLastDigit :: Integer -> Integer
dropLastDigit a = div a 10

toDigits :: Integer -> [Integer]
toDigits a | a <= 0 = []
			  | a > 0  =  toDigits (dropLastDigit a) ++ [lastDigit a]
 
doubleEveryOther :: [Integer] -> [Integer]
doubleEveryOther [] = []
doubleEveryOther xs = reverse(double (reverse xs))

double :: [Integer] -> [Integer]
double [] = []
double [x] = [x]
double (x:xs) = x : (2 * head xs) : double (tail xs)

sumDigits :: [Integer] -> Integer
sumDigits [] = 0
sumDigits (x:xs) | x >= 10 = sumDigits(toDigits x) + sumDigits xs
					  | x <  10 = x + sumDigits xs

validate :: Integer -> Bool
validate a | a <= 0 = False
			  | mod (sumDigits (doubleEveryOther (toDigits a))) 10 == 0 = True
			  | otherwise = False

type Peg = String
type Move = (Peg, Peg)

hanoi :: Integer -> Peg -> Peg -> Peg -> [Move]
hanoi 0 x y z = []
hanoi a x y z = hanoi (a-1) x z y ++ (x,y) : hanoi (a-1) z y x
