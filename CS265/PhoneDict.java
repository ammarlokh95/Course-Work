// -To replicate the working of a classical phone T9

import java.io.*;
import java.util.*;


class PhoneDict{
	public HashMap  wordMapping( String w)
	{
		FileReader f = new FileReader(w);
		Map<String, ArrayList<String>> hm = new HashMap<String,
			ArrayList<String>>();
		String line = null;
		String code = null;
		BufferReader buf= new BufferReader(f);
		while( (line = buf.readline()) != null)
		{
			for (int i=0; i < line.length(); i++) {
				char c = line.charAt(i);
				if ( c== 'a' || c== 'b' || c== 'c' )
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
				else 
				{
					hm.put(code , );
