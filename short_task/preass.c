

int bitpr(long x, char* ptr)
{ 
	int i = 0;
	int k;								
	for (i; i < 32; ++i)						
	{									
		if (i % 8 == 0 && i != 0)													
			*ptr++ = '|';				
		else	
		{							
			if (i % 4 == 0 && i != 0)	
				*ptr++ = ' ';
		}		
		if (x & (1 << (31 - i)))	
				*ptr++ = '1';			
			else				
				*ptr++ = '0';			
	}									
	*ptr++ = '\n';
	*ptr++ = ' ';								
	*ptr++ = ' ';
	*ptr++ = ' ';							
	for (i = 0; i < 8; ++i) 					
	{	
		k = (x & (15 << (28 - i * 4))) >> (28 - i * 4);				 				
		if (k < 10)
			*ptr++ = k + '0';
		else 
			switch(k){
				case 10:*ptr++ = 'A';break;
				case 11:*ptr++ = 'B';break;
				case 12:*ptr++ = 'C';break;
				case 13:*ptr++ = 'D';break;
				case 14:*ptr++ = 'E';break;
				case 15:*ptr++ = 'F';break;
			}
		*ptr++ = ' ';
		*ptr++ = ' ';
		*ptr++ = ' ';
		*ptr++ = ' ';									
	}									
	*ptr = '\n';
	++ptr;								
	return 0;									
}
