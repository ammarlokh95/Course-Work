/*
  Problem 4.12: Frog puzzle
*/

#define noOfStones 7

#define success ((stones[0]==female) && (stones[1]==female) && (stones[2]==female)  && (stones[4]==male)   && (stones[5]==male)   && (stones[6]==male))

mtype = { male, female, empT }
mtype stones[noOfStones];

ltl { []!success }

proctype moveMale(byte pos) {
end:do
	:: 	atomic {
			(pos < noOfStones-1) && 
			(stones[pos+1] == empT) -> 
			stones[pos] = empT; 
			stones[pos+1] = male;
			pos = pos + 1;
		}
	:: atomic {
			(pos < noOfStones-2) && 
	   		(stones[pos+1] != empT) && 
			(stones[pos+2] == empT) -> 
			stones[pos] = empT; 
			stones[pos+2] = male;
			pos = pos + 2;
		}
	od
}

proctype moveFemale(byte pos) {
end:do
	:: atomic {
			(pos > 0) && 
			(stones[pos-1] == empT) -> 
			stones[pos] = empT; 
			stones[pos-1] = female;
			pos = pos - 1;
		}
	:: atomic {
			(pos > 1) && 
	   		(stones[pos-1] != empT) && 
			(stones[pos-2] == empT) -> 
			stones[pos] = empT; 
			stones[pos-2] = female;
			pos = pos - 2;
		}
	od
}

init {
	atomic {
		stones[noOfStones/2] = empT;
		byte i = 0;
        do      :: i == noOfStones/2 -> break;
   				:: else -> 
            	 	stones[i] = male;
					run moveMale(i);
    				stones[noOfStones-i-1] = female;
					run moveFemale(noOfStones-i-1);	
					i++
        od
	}
}
/*
	atomic {
	byte i = 0;
	byte procNo = 0; 
	do	:: success -> break;
		:: procNo==255 ->break;
		:: else ->
			if :: (i==noOfStones) -> i=0;
			   :: else -> i=i;
			fi;	
			if :: (stones[i]==male) -> run moveMale(i); i++;
			   :: (stones[i]==female) -> run moveFemale(i); i++;
			   :: else -> i++;
			fi;
			procNo++;
	od
	}
}
*/
