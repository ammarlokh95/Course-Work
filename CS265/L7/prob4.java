// to calculate the time since jan 1 1970

import java.io.*;
import java.util.Date;

public class prob4 
{
	public static void main(String [] arg)
	{
		int a =Integer.parseInt(arg[0]);
		Date d = new Date();
		long n= d.getTime();
		switch(a)
		{
			case 0: System.out.println("Milliseconds since Jan 1, 1970: " + n);
					  break;
			case 1: n=n/1000;
					  System.out.println("Seconds since Jan 1, 1970: " + n);
					  break;
			case 2: n=n/(1000*60*60*24);
					  System.out.println("Days since Jan 1, 1970:" + n);
					  break;
			default: System.out.println(d.toString());
						break;
		}
	}
}

