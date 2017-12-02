import java.util.Random;
import java.lang.Thread;
import java.util.concurrent.locks.ReentrantLock;
/*
   Program to create a histogram representing a randomly generated data set
   By Ammar Lokhandwala
   14102456
*/
public class hw1 implements Runnable {
	private static int numbins;
	private static int datasize;
	private static int noofthreads;
	private static int[] histogram;
	private static int[][] multiBins;
	private static int[] data;
	private static long timeTaken = 0;
	private int start, end, num;
	private static ReentrantLock lock;
	public hw1(int s, int e, int n) 
	{
		start = s;
		end = e;
		num = n;
	}
	public static void main(String args[]){
		setArgValues(args);
		data = new int[datasize];
		initialize(data);
		histogram = new int[numbins];
		hw1[] threads= new hw1[noofthreads];
		Thread []t = new Thread[noofthreads];
		// initializes a multibin object
		// that holds the partial histogram for each thread
		multiBins = new int[noofthreads][];
		for(int i =0;i<noofthreads;i++)
			multiBins[i] = new int[numbins];

		int iterate = datasize/noofthreads;
		timeTaken = System.currentTimeMillis();
		// divides the data and starrs the threads
		for(int i = 0, j=0; i<noofthreads; i++)
		{
			if(i == noofthreads -1)
				threads[i] = new hw1(j, datasize, i);
			else threads[i] = new hw1(j, j=j+iterate, i);
				
			t[i] = new Thread(threads[i]);
			t[i].start();
		}	
		try{
			// Waits for threads to finish execution
		for(int i=0; i<noofthreads; i++)
		{
			t[i].join();
		}	
		}
		catch(Exception e)
		{
			System.out.println("Interrupt Exception caught");
		}
		// Computes the final histogram
		for(int i =0; i<noofthreads;i++) {
			for(int j=0; j<numbins;j++)
				histogram[j] += multiBins[i][j];
		}	
		timeTaken = System.currentTimeMillis() - timeTaken;

		System.out.println("Histogram values generated:");
		print(histogram);
		System.out.println("Execution time:" + timeTaken+ " ms");
	}
	// Starting point of thread computation
	public void run()
	{	
		computeHistogram();
	}
	private void computeHistogram()
	// Computes the values of the histogram
	{
		for(int i = start; i < end; i+=numbins)
		{
			int max = 0;
			// Finds the max values' position for this data segment
			for(int j=1; j<numbins && i+j < end;j++)
			{
				if(data[i+j]>data[i+max])
					max = j;
			}
			// Increments all histogram bins where temperature is maximum
			for(int j = 0; j<numbins && i+j < data.length; j++)
			{
				if(data[i+j] == data[i+max])
					multiBins[num][max]++;
			}
		}
	}
	private static void setArgValues(String[] args)
	// Parses the user provided comman line arguments
	{
		String[] keyArray = {"-NUMBINS", "-DATASIZE","-NOOFTHREADS"};
		try{
			if(args.length>=4)
			{
				// Assigns values from command line args
				if(args[0].equalsIgnoreCase(keyArray[0]))
					numbins=Integer.parseInt(args[1]);
				else throw new IllegalArgumentException();
				if(args[2].equalsIgnoreCase(keyArray[1]))
					datasize=Integer.parseInt(args[3]);
				else throw new IllegalArgumentException();
				if(args.length>=6)
					// Checks no. of threads requested
					// If none provided, defaults to 1
					if(args[4].equalsIgnoreCase(keyArray[2]))
						noofthreads=Integer.parseInt(args[5]);
					else throw new IllegalArgumentException();
				else noofthreads = 1;
			}
			else { 
				// If no/insufficient command line arguments passed, defaults to these
				// values
				numbins = 7;
				datasize = 70000000;
				noofthreads = 1;
			}
		}
		catch(Exception e)
		{
			System.out.println("Improper Arugments passed");
			System.out.println("Execution should be of the form:");
			System.out.println("java hw1 [-NUMBINS ### -DATASIZE ### [-NOOFTHREADS ###]]");
			System.exit(1);
		}
	}
	private static void print(int[] data)
	// Prints an int array
	{
		for(int i=0; i<data.length;i++)
			System.out.println(data[i]);
	}
	private static void initialize(int[] data)
	{
		// Generates a dataset with random values ranging from 0 to 100
		// Which is represent of the common temperature variations in
		// Philadelphia
		Random rand = new Random();
		for(int i=0;i<data.length;i++)
			data[i] = rand.nextInt(100);
	}
}
