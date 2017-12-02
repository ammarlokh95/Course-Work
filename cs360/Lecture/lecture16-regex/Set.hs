module Set (
    Set,

    empty,
    isEmpty,

    singleton,

    size,

    toList,
    fromList,

    split,

    member,
    insert,
    delete,
    union,
    intersection,
    difference,

    isSubsetOf,

    mapSet,
    filterSet
  ) where

import Data.List (sort)

-- We represent sets as *ordered* lists
data Set a = Set [a]
  deriving (Eq, Ord, Show, Read)

empty :: Set a
empty = error "empty unimplemented"

isEmpty :: Set a -> Bool
isEmpty = error "isEmpty unimplemented"

singleton :: a -> Set a
singleton = error "singleton unimplemented"

size :: Set a -> Int
size = error "size unimplemented"

toList :: Set a -> [a]
toList = error "toList unimplemented"

fromList :: Ord a => [a] -> Set a
fromList = error "fromList unimplemented"

split :: Set a -> (a, Set a)
split (Set [])     = error "Cannot split empty set"
split (Set (x:xs)) = (x, Set xs)

member :: Ord a => a -> Set a -> Bool
member = error "member unimplemented"

insert :: Ord a => a -> Set a -> Set a
insert x0 (Set ys0) = Set (ins x0 ys0)
  where
    ins :: Ord a => a -> [a] -> [a]
    ins x []                 = [x]
    ins x (y:ys) | x < y     = x : (y : ys)
                 | x == y    = y : ys
                 | otherwise = y : ins x ys

delete :: Ord a => a -> Set a -> Set a
delete x0 (Set ys0) = Set (del x0 ys0)
  where
    del :: Ord a => a -> [a] -> [a]
    del _ []                 = []
    del x (y:ys) | x < y     = y : ys
                 | x == y    = ys
                 | otherwise = y : del x ys

union :: Ord a => Set a -> Set a -> Set a
union (Set xs0) (Set ys0) = Set (uni xs0 ys0)
  where
    uni :: Ord a => [a] -> [a] -> [a]
    uni []     ys                 = ys
    uni xs     []                 = xs
    uni (x:xs) (y:ys) | x < y     = x : uni xs (y:ys)
                      | x == y    = x : uni xs ys
                      | otherwise = y : uni (x:xs) ys

intersection :: Ord a => Set a -> Set a -> Set a
intersection (Set xs0) (Set ys0) = Set (int xs0 ys0)
  where
    int :: Ord a => [a] -> [a] -> [a]
    int []     _                  = []
    int _      []                 = []
    int (x:xs) (y:ys) | x < y     = int xs (y:ys)
                      | x == y    = x : int xs ys
                      | otherwise = int (x:xs) ys

difference ::  Ord a => Set a -> Set a -> Set a
difference (Set xs0) (Set ys0) = Set (diff xs0 ys0)
  where
    diff :: Ord a => [a] -> [a] -> [a]
    diff []     _                  = []
    diff xs     []                 = xs
    diff (x:xs) (y:ys) | x < y     = x : diff xs (y:ys)
                       | x == y    = diff xs ys
                       | otherwise = diff (x:xs) ys

isSubsetOf :: Ord a => Set a -> Set a -> Bool
isSubsetOf (Set xs0) (Set ys0) = subs xs0 ys0
  where
    subs :: Ord a => [a] -> [a] -> Bool
    subs []     _                  = True
    subs _      []                 = False
    subs (x:xs) (y:ys) | x < y     = False
                       | x == y    = subs xs ys
                       | otherwise = subs (x:xs) ys

mapSet :: Ord b => (a -> b) -> Set a -> Set b
mapSet f (Set xs) = fromList (map f xs)

filterSet :: Ord a => (a -> Bool) -> Set a -> Set a
filterSet f (Set xs) = Set (filter f xs)
