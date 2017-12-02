// To seperate a csv files into different fields

import java.io.*;
import java.util.*;
import java.util.regex.*;
class Csv {
	private File in;
	private String line;
	private Vector<String> field;
	private int nfield;
	private String fieldsep;
	public Csv (String f, String sep)
	{
	//	try{
			this.in = new File(f);
	//}
	//	catch( FileNotFoundException e){ System.out.println("Invalid file");}
		this.fieldsep=sep;
		field = new Vector<String>();
	}
	public int getnfield()
	{
		return nfield;
	}
	public boolean getline( String s)
	{
		String c;
		try {
		Scanner buf = new Scanner (in);
		try{
			line = buf.nextLine();
			s= line;
		return (buf.hasNextLine());
		} catch(NoSuchElementException e) {return false;}
		} catch(FileNotFoundException e) {return false;}
	}
	private int advquoted(String s, String fld, int i)
		{
			int j,k;
			Pattern pat = Pattern.compile(fieldsep);
			Matcher matcher = pat.matcher(s);
			fld="";
			for(j=i;j<s.length();j++)
			{
				if ( s.charAt(j) == '"' && s.charAt(++j) != '"')
				{
					if(matcher.find())
						 k = matcher.start();
					else k=s.length();
					for(k -= j; k-- >0;)
						fld += s.charAt(j++);
					break;
				}
				fld += s.charAt(j);
			}
			return j;
		}
	private int advplain(String s, String fld, int i)
	{
		int j;
		Pattern pat = Pattern.compile(fieldsep);
		Matcher matcher = pat.matcher(s);
		if ( matcher.find())
			j= matcher.start();
		else j = s.length();
		fld = s.substring(i, j-i);
		return j;
	}

	public String getfield(int n)
	{
		if(n<0 || n >= nfield)
			return "";
		else return field.elementAt(n);
	}
	private int split()
	{
		String fld="";
		int i, j;
		nfield = 0;
		if( line.length() == 0)
			return 0;
		i = 0;
		do {
				if ( i <line.length() && line.charAt(i) == '"')
					j = advquoted(line, fld, ++i);
				else
					j = advplain(line,fld,i);
				if (nfield >= field.size())
					field.add(0,fld);
				else
					field.add(nfield, fld);
				nfield++;
				i=j+1;
		}while (j < line.length());
		return nfield;
	}
	public static void main(String [] arg)
	{
		String l="";
		Csv cs= new Csv("test.csv",",");
		while(cs.getline(l) != false)
		{
			System.out.println("line= " + l);
			for(int i = 0; i <cs.getnfield(); i++)
				System.out.println( "field[" + i + "] = " + cs.getfield(i));
		}
	}
}
		
