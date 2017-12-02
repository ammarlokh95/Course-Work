package hw3;

import java.time.Clock;
import java.util.Random;
import java.util.concurrent.Semaphore;

public class Student implements Runnable {
	private Semaphore airplaneAcc; 
	private Semaphore runwayAcc;
	private Semaphore fuelAcc;
	private Semaphore instructAcc;
	private Airplane planes[];
	private Instructor instructors[];
	private IOClass io;
	private Airplane myAirplane;
	private Instructor myInstructor;
	private String name;
	private boolean isAlive = true;
	private long startTime =0;
	private long endTime = 0;
	public Student(Semaphore as, Semaphore rs, Semaphore fs, Semaphore is, Airplane planes[], Instructor i[])
	{
		airplaneAcc = as;
		runwayAcc = rs;
		fuelAcc = fs;
		instructAcc = is;
		this.planes = planes;
		instructors = i;
		myAirplane = null;
		myInstructor = null;
		io = new IOClass();
	}
	@Override
	public void run() {
		name = getName();
		while(true) {
			int rT = generateRandom(10000);
			sleep(rT);
			acquireAirplane();
			getInstructor();
			int practiceNo = generateRandom(5);
			waitForRunway();	
			for(int i = 0;i<practiceNo;i++)
			{
				takeOff();
				freeRunway();
				flyAround();
				waitForRunway();
				land();
				if(!myAirplane.hasFuel())
					break;
			}
			freeRunway();
			refuel();
			parkAirplane();
			releaseInstructor();
		}
	}
	public void hasCrashed()
	{
		io.print(name +" has died in crash of " + myAirplane.getName());
		isAlive = false;
		
	}
	private void releaseInstructor()
	{
		myInstructor.isWithStudent(false);
		io.print(name + " is releasing " +myInstructor.getName());
		myInstructor=null;
		instructAcc.release();
	}
	private void getInstructor() {
		io.print(name + " is Waiting for instructor");
		try {
			startTime = System.currentTimeMillis();
			instructAcc.acquire();
			for(Instructor i:instructors) {
				if(i.isAvailable())
				{
					endTime = System.currentTimeMillis();
					myInstructor = i;
					myInstructor.isWithStudent(true);
					io.print(name + " acquired " +myInstructor.getName() +" after " + (endTime - startTime) +" ms");
				}	
			}
		} catch (Exception e) {
			io.print("Instructor Deadlock in " + name);
			e.printStackTrace();
		}
	}
	private String getName() {
		return Thread.currentThread().getName();
	}
	private void parkAirplane() {
		myAirplane.park();
		io.print(name + " parked and got off " +myAirplane.getName());
		myAirplane = null;
		airplaneAcc.release();
	}
	private void refuel() {
		try {
			io.print(name + " is waiting to refuel plane");
			startTime = System.currentTimeMillis();
			fuelAcc.acquire();
			endTime = System.currentTimeMillis();
			myAirplane.refuel();
			io.print(name + " refuled " +myAirplane.getName()+" after " + (endTime - startTime) + " ms");
			fuelAcc.release();
		} catch (InterruptedException e) {
			io.print("Fuel DEADLOCK");
			e.printStackTrace();
		}
	}
	private void land() {
		myAirplane.land();
		io.print(name + " landed " +myAirplane.getName());
	}
	private void freeRunway() {
		runwayAcc.release();
	}
	private void takeOff() {
		myAirplane.takeOff();
		io.print(name + " took off " +myAirplane.getName());
	}
	private void sleep(int rT) {
		try {
			Thread.sleep(rT);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		
	}
	private void flyAround() {
		int fT = generateRandom(10000);
		io.print(name + " is flying " + myAirplane.getName());
		sleep(fT);
	}
	private void waitForRunway() {
		try {
			io.print(name + " waiting for runway");
			startTime = System.currentTimeMillis();
			runwayAcc.acquire();
			endTime = System.currentTimeMillis();
			io.print(name + " got runway after " + (endTime - startTime) +" ms");
		} catch (InterruptedException e) {
			io.print("Runway DEADLOCK!");
			e.printStackTrace();
		}
		
	}
	private void acquireAirplane() {
		try {
			io.print(name + " waiting for plane");
			startTime = System.currentTimeMillis();
			airplaneAcc.acquire();
			endTime = System.currentTimeMillis();
			for(Airplane a : planes) {
				if(a.isAvailable())
				{
					myAirplane = a.airplaneAquire(this);
					myAirplane.occupySeat(2);
					io.print(name + " got " +myAirplane.getName()+" after " + (endTime - startTime) +" ms");
					return;
				}
			}
		} catch (InterruptedException e) {
			io.print("Airplane DEADLOCK");
			e.printStackTrace();
		}
	}
	private int generateRandom(int num) {
		Random rand = new Random();
		int n = rand.nextInt(num) + 1;
		return n;
	}

}
