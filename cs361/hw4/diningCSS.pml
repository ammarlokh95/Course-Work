/*
	problem 2, Assignment 4
*/

#define N 4
 // Bit 0 = not blocked, bit 1 = blocked
bit forks[N];
bit phoneCard[N];
byte bowl = 10;

proctype students(byte i)
{
	 do	
		:: skip -> printf("\nCODE: student %d",i);
		:: skip -> printf("\nGET HUNGRY: student %d",i);
			if
				:: bowl<=0 -> atomic{ printf("\nBowl empty");
									 phoneCard[i]==0;
										phoneCard[i]=1;
								printf("\nGOT PHONE: student %d",i);
								byte j=0;
								if :: (i+1)==N -> j=0;
											::	else -> j=i+1;
										fi;
										phoneCard[j] == 0;
										phoneCard[j] = 1;
				
								printf("\nPAYING: student %d",i);
								bowl=10;
								phoneCard[i]=0;
								phoneCard[j]=0;
								}		
				:: else -> atomic { 
								forks[i]==0;
								forks[i]=1;
							}	
							byte j=0;
							printf("\nGot left fork: student %d",i);
							atomic {
								if :: (i+1)==N -> j=0;
													forks[0]==0;
													forks[0]=1;
									:: else -> j=i+1;
												forks[i+1] == 0;
												forks[i+1]=1;
								fi
							}
							printf("\nGot right fork: student %d",i);
							printf("\nEATS: student %d",i);
						atomic { forks[i] = 0; 
								forks[j] = 0;
								bowl--;
								}			
			fi
	 od
}

init{
	atomic {
		byte i=1;
		do	:: i==N -> break;
			:: else ->
					run students(i);
					i++;
		od
	}
}
