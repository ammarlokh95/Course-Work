package hw3;

public class Airplane implements Runnable {

	private boolean isAvail;
	private boolean isFlying;
	private int fuel;
	private IOClass io;
	private String name;
	private int seatsAvail;
	private boolean isCrashed;
	private Student pilot;
	private Instructor instructor;
	public Airplane(String n)
	{
		isAvail = true;
		isFlying = false;
		fuel = 10;
		name = n;
		io = new IOClass();
		seatsAvail = 3;
	}
	
	@Override
	public void run() {
		while(true)
		{
			if(!isAvail)
			{
				/*sleep(5000);
				fuel -= 1;
				if(!hasFuel()) {
					hasCrashed();
				}*/
			}
		}
	}
	public void hasCrashed()
	{
		isCrashed = false;
		
	}
	private void sleep(int rT) {
		try {
			Thread.sleep(rT);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		
	}
	public boolean seatsAvailable()
	{
		if (seatsAvail == 0)
			return false;
		else return true;
	}
	public void occupySeat(int i)
	{
		seatsAvail-=i;
	}
	public boolean isAvailable()
	{
		return isAvail;
	}
	public void setCopilot(Instructor I)
	{
		instructor = I;
	}
	public Airplane airplaneAquire(Student p)
	{
		isAvail = false;
		pilot = p;
		return this;
	}
	public String getName()
	{
		return name;
	}
	
	public void takeOff()
	{
		isFlying = true;
	}
	public void land()
	{
		isFlying = false;
	}
	public boolean hasFuel()
	{
		if(fuel==0)
			return false;
		return true;
	}
	public void refuel() {
		fuel = 10;
	}

	public void park() {
		isAvail = true;
		seatsAvail = 3;
	}
}
