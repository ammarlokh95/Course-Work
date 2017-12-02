// To determine whether a year is a leap year or not

import java.io.*;

public class prob3 
{
	static boolean isLeap(int a)
	{
		if (a%4==0)
			if(a%100==0)
				if(a%400==0)
					return true;
		return false;
	}
	public static void main(String [] args)
	{
		if(isLeap(Integer.parseInt(args[0])))
			System.out.println("leap year!");
		else System.out.println("not a leap year!");
	}
}

