// -To replicate the working of a classical phone T9

import java.io.*;
import java.util.*;


public class PhoneDict{
	private Map<String, LinkedList<String>> hm = new HashMap<String, LinkedList<String>>();

	private String getCode(String s) //Gets the code for string
	{
 		String code="";
		for (int i=0; i < s.length(); i++) 
		{
         String a =Character.toString( s.charAt(i));
			a=a.toLowerCase();
			char c = a.charAt(0); 		
		 	if ( c==  'a'  || c== 'b' || c== 'c' )
   	 {			
			 code = code + '2';     
		 }
		  else if ( c == 'd' || c == 'e' || c == 'f')
       {		
			  code += '3';	
		 }				 
		 else if ( c == 'g' || c == 'h' || c == 'i')		
		 {			             
			 code += '4';			
		 }             
		 else if ( c == 'j' || c == 'k' || c == 'l')	     
		 {     
  			 code += '5';       
		 }           
		 else if ( c == 'm' || c == 'n' || c == 'o')        
		 {     
 			 code += '6';     
		 }    
		 else if ( c == 'p' || c == 'q' || c == 'r' || c == 's' ) 		 
  		 {		
  			 code += '7'; 		  
  		 }   
		 else if ( c == 't' || c == 'u' || c == 'v' )       
		 {       
			 code += '8';     
		 } 
		 else if ( c == 'w' || c == 'x' || c == 'y' || c == 'z' )	   
		 {
			 code += '9';
		 }
		}
		return code;
	}
		                                                                 				
		public void wordMapping( String w) //Maps all the words from a file to	correspounding code
	{
		
		try{
			FileReader f = new FileReader(w);
			int i=0;
			String line = null;
			String code = null;
			LinkedList<String> ll;
			BufferedReader buf= new BufferedReader(f);
			try{
				while( (line = buf.readLine()) != null)
				{
					code= getCode(line);
					ll= hm.get(code);
					if(ll == null)
						ll=new LinkedList<String>();
					ll.addLast(line);
					hm.put(code,ll);
				}
				}
			catch(IOException e){}
		}
		catch(FileNotFoundException e){
			System.out.println("File not Found");}
	}
	
	private void displayLine(ArrayList<String> wordlist)
	//creates and displays the string line
	{
		String disp="";
		LinkedList<String> code=new LinkedList<String>();
		for(int l=0;l<wordlist.size();l++) 
		{
			 code=hm.get(wordlist.get(l));													        
			 if(code!=null)																
			 {								
				 int m=code.size();					
				 if(m>1)										
				 {						
					 disp+="(";				
					 for(int	i=0;i<m;i++)									
					 {												
						 disp+= code.get(i);												
						 if(i!=(m-1))
							 disp+="|";		
						 else
							 disp+=")";
					 }
				 }					
				 else				
					 disp+=code.get(0);					
			 }											
			 else			
				 for(int i=0;i<wordlist.get(l).length();i++)							
					 disp+="*";																		
			 disp+="	";						
		}					     
  		System.out.println(disp);		
	}
	public void codeList(String line)
	// gets all the codes from stdin input stripping the 0s
	{	
		int j=0;	
		ArrayList<String> wordlist= new ArrayList<String>();
		int count=1;
		String word;		
		word="";						
		for(int i=0;i<line.length();i++)											         
		{																	           
		  	count=0;		
			if(line.charAt(i)!='0')					
			{										
				word+=Character.toString(line.charAt(i));			
			}																	
			else			
			{				
				if(i!=0)			
				{						
					wordlist.add(j,word);				
					word="";												
					j++;							
				}						
				for(count=i;count<line.length();)								
				{															
					if(line.charAt(count)=='0')				
						count++;														
					else								
						break;						
				}								
				i=count-1;				
				count=1;							
			}						
		}			
		if(count!=1)		
			wordlist.add(j++,word);
		displayLine(wordlist);
	}
	
	public static void main(String [] arg)
	{	
		PhoneDict pd= new PhoneDict();
		pd.wordMapping("words");
	   Scanner s= new Scanner(System.in);
		String line;
		while(s.hasNextLine())	//reads from stdin and displays line
		{
			line=s.nextLine();
			pd.codeList(line);
		}			
	}
}


