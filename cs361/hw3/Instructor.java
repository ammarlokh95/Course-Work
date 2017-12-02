package hw3;

import java.util.Random;
import java.util.concurrent.Semaphore;

public class Instructor implements Runnable {
	private boolean isAvailable;
	private Semaphore instructAcc;
	private long startTime = 0;
	private long endTime = 0;
	private String name;
	private IOClass io;
	private boolean isAlive = true;
	public Instructor(Semaphore is, String n)
	{
		instructAcc = is;
		isAvailable = true;
		name = n;
		io = new IOClass();
	}
	@Override
	public void run() {
		while(true) {
			if(isAvailable)
			{
				int breakTime = generateRandom(10000);
				goOnBreak(breakTime);
				backFromBreak();
				lookForStudent();
			}
		}
	}
	public void hasCrashed()
	{
		isAlive = false;
	}
	private void lookForStudent() {
		sleep(1);
	}
	public String getName()
	{
		return name;
	}
	private void backFromBreak() {
		isAvailable = true;
		io.print(Thread.currentThread().getName() + " is back from break");
		instructAcc.release();
	}
	private void goOnBreak(int breakTime) {
		try {
			if(isAvailable == true) {
				instructAcc.acquire();
				isAvailable = false;
				io.print(Thread.currentThread().getName() + " is on break");
				sleep(breakTime);
			}
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	private void sleep(int rT) {
		try {
			Thread.sleep(rT);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}
	private int generateRandom(int num) {
		Random rand = new Random();
		int n = rand.nextInt(num);
		return n;
	}
	public boolean isAvailable() {
		return isAvailable;
	}
	public void isWithStudent(boolean is) {
		isAvailable = !is;
	}
}
