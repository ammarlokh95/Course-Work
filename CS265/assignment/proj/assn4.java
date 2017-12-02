import java.util.*;
import java.io.*;

public class assn4 {
	public void init (String [] arg, String f,String f1)
	{
		try{
		String input="";
		BufferedWriter writer = new BufferedWriter(new FileWriter(f1,false));
		writer.write("0");
		writer.close();
		int [] deno= {0,0,0,0};
	   int amt;
		if(arg.length <= 7 && arg.length>3)
		{
			if(Integer.parseInt(arg[1])>=0)
				amt=Integer.parseInt(arg[1]);
			else {System.exit(1);return;}
			for(int i=3; i< arg.length; i++)
			{ 
			  	if(Integer.parseInt(arg[i])>=0)
					deno[i-3]=Integer.parseInt(arg[i]);
				else {System.exit(1); return;}
			}
			for(int i =0;i<4;i++)
			{
				input+=deno[i] + " ";
			}
			if(amt== deno[0]+deno[1]*5+deno[2]*10+deno[3]*20)
			{
		      writer = new BufferedWriter(new FileWriter(f,false));
            writer.write(amt+" = "+ input);
				writer.close();
				System.exit(0);
			}
			else {System.exit(2);}
		}
		else {System.exit(1);}
		} catch(IOException e) {System.exit(4);}
	}

	public void purchase(String []arg, String f,String f1)
	{
	   try {	
		BufferedReader buf = new BufferedReader(new FileReader(f));
		String line;
		int []input={0,0,0,0};
		line = buf.readLine();
		buf.close();
		String [] lines= line.split("\\s+");
		int price;
		if( arg.length <=7 && arg.length >3)
		{
			if(Integer.parseInt(arg[1])>=0)
				price = Integer.parseInt(arg[1]);
			else {System.exit(1);return;}
			int c=1;
			for(int i=3;i<arg.length; i++)
			{
			  	if(Integer.parseInt(arg[i])>=0)
				{
					input[i-3]=Integer.parseInt(arg[i]);
					lines[i-1]=String.valueOf(Integer.parseInt(lines[i-1])+input[i-3]);
				}
				else { System.exit(1); return;}
			}
			
			int amt_tend=(input[0]+input[1]*5+input[2]*10+input[3]*20);
			int balance = amt_tend - price;
			int [] change = {0,0,0,0};
			if(balance>=0)
			{
				while(balance>0)
				{
					if(balance >= 20 && Integer.parseInt(lines[5])>0)
					{
						change[3]++;
						balance = balance-20;
						lines[5]=String.valueOf(Integer.parseInt(lines[5])-1);
					}
					else if(balance >=10 && Integer.parseInt(lines[4])>0)
					{
						change[2]++;
						balance = balance-10;
						lines[4]=String.valueOf(Integer.parseInt(lines[4])-1);
					}
					else if(balance >=5 && Integer.parseInt(lines[3])>0)
					{
						change[1]++;
						balance = balance-5;
                  lines[3]=String.valueOf(Integer.parseInt(lines[3])-1);
					}
					else if(balance>=1 && Integer.parseInt(lines[2])>0)
					{
						change[0]++;
						balance = balance-1;
                  lines[2]=String.valueOf(Integer.parseInt(lines[2])-1);
					}
					else 
					{ 
					  System.exit(3);
					}
				}
				for(int i=0; i<4;i++)
				{
					System.out.print(change[i]+" ");
				}
				System.out.println();
				lines[0]=String.valueOf(Integer.parseInt(lines[0])+price);
			   BufferedWriter writer = new BufferedWriter(new
						FileWriter(f,false));
				for(int i=0;i<lines.length;i++)
				{
				writer.write(lines[i]+" ");
				}
				writer.close();
				buf=new BufferedReader(new FileReader(f1));
				String sale = buf.readLine();
				buf.close();
				writer=new BufferedWriter(new FileWriter(f1,false));
				int sal=Integer.parseInt(sale) + price;
				writer.write(sal+"");
				writer.close();
			   System.exit(0);
			}
			else { System.exit(2); return;
			}
		}
		else {System.exit(1); return;}
			} catch(IOException e) {System.exit(4);}
	}
	
	public void change(String [] arg, String f)
	{
		if(arg.length>3 && arg.length<=10)
		{
			try {
			BufferedReader buf = new BufferedReader(new FileReader(f));
			String line;
			String[] lines;
			int flag=0;
			try {
			line=buf.readLine();
			buf.close();
			lines=line.split("\\s+");
			int []tendered={0,0,0,0};
			int [] bill_req ={0,0,0,0};
			for(int i=1,j=0;i<arg.length;i++)
			{
				if(arg[i].equals("="))
					flag=1;
				else if(Integer.parseInt(arg[i])<0)
				{ 
					System.exit(1);
					return;
				}
				else if(flag==0)
					tendered[i-1]=Integer.parseInt(arg[i]);
				else if(flag==1)
				{
					bill_req[j]=Integer.parseInt(arg[i]);
					j++;
				}
			}
			int check=tendered[0]+tendered[1]*5+tendered[2]*10+tendered[3]*20;
			if(check==(bill_req[0]+bill_req[1]*5+bill_req[2]*10+bill_req[3]*20))
			{
				for(int i=0; i<4;i++)
				{
					if(bill_req[i]<=(Integer.parseInt(lines[i+2])))
					{
						lines[i+2]=String.valueOf(Integer.parseInt(lines[i+2])-bill_req[i]+tendered[i]);
					}
					else {System.exit(3);return;}
				}
				for(int i=0; i<4;i++)
				{
					System.out.print(bill_req[i]+ " ");
				}
				System.out.println();
				BufferedWriter writer = new BufferedWriter(new FileWriter(f,false));
				for(int i=0;i<lines.length;i++)
					writer.write(lines[i] +" ");
				writer.close();
				System.exit(0);
			}
			else {System.exit(2);}
			} catch (IOException e) {System.exit(4);}
			} catch(FileNotFoundException e) {System.exit(4);}
		}
		else{ System.exit(1);}
	}
	public void report(String [] arg, String f, String f1)
	{
		if(arg.length<2)
		{
		try{
			FileReader fr = new FileReader(f);
			BufferedReader buf = new BufferedReader(fr);
			fr=new FileReader(f1);
      	BufferedReader buf1 = new BufferedReader(fr);
			String line;
			line=buf1.readLine();
		   line+=	" : ";
			line+=buf.readLine();
			System.out.println(line);
			buf.close();
			buf1.close();
			System.exit(0);		
		}catch(IOException e) {System.exit(4);
					System.out.println("Exception");}
		}
		else {System.exit(1);}
		
	}
	public static void main(String [] arg)
	{
		String f="till.txt";
		String f1="sale.txt";
		assn4 a=new assn4();
		if(arg.length>0)
		{
			if(arg[0].equals("init"))
				a.init(arg,f,f1);
			else if(arg[0].equals("purchase"))
				a.purchase(arg,f,f1);
			else if(arg[0].equals("change"))
				a.change(arg,f);
			else if(arg[0].equals("report"))
				a.report(arg,f,f1);
			else System.exit(1);
		}
	else {System.exit(1);}
	}
}
