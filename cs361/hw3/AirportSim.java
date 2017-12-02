package hw3;

import java.util.concurrent.Semaphore;

public class AirportSim {
	public static void main(String []args)
	{
		IOClass io = new IOClass();
		int N = 3, k = 2, s = 6, I=2;
		if(args.length>0)
		{
			N = (args.length>0)? Integer.parseInt(args[0]):3;
			k = (args.length>1)? Integer.parseInt(args[1]):2;
			s = (args.length>2)? Integer.parseInt(args[2]):6;
			I = (args.length>3)? Integer.parseInt(args[3]):2;
		}
		Semaphore airAcc = new Semaphore(N);
		Semaphore runAcc = new Semaphore(k);
		Semaphore fuelAcc = new Semaphore(1);
		Semaphore instructAcc = new Semaphore(I);
		Airplane airObject [] = new Airplane[N];		
		Student studentObject[] = new Student[s];
		Instructor instructObject[] = new Instructor[I];
		Thread students[] = new Thread[s];
		Thread instructors[] = new Thread[I];
		for(int i=0;i<N;i++)
			airObject[i]= new Airplane("<Plane "+ (i+1) +">");
		for(int i =0; i<I;i++)
		{
			instructObject[i] = new Instructor(instructAcc, "<Instructor " + (i+1)+">");
			instructors[i] = new Thread(instructObject[i], "<Instructor " + (i+1)+">");
			instructors[i].start();
		}
		for(int i=0;i<s;i++) {
			studentObject[i] = new Student(airAcc, runAcc, fuelAcc, instructAcc, airObject, instructObject);
			students[i] = new Thread(studentObject[i], "<Student " + (i+1) +">");
			students[i].start();
		}
		
		for(int i=0;i<s;i++)
		{
			try {
				students[i].join();
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		io.print("ALL STUDENTS ARE DONE");
	}
}
