#ifndef _TOOL_CONVEWRT_HEADER_H_
#define _TOOL_CONVEWRT_HEADER_H_
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <string>
#include <time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h> 
#include <math.h> 


using std::string;

class Convert
{
public:
	/*param @mac : char mac[6]; */
	static string mac_bin2str(const unsigned char *mac)
	{
		char buf[18]={0};
		for (int i = 0; i < 6; i++) 
		{
			if ( i ==5 )
			{
				sprintf(buf+(2*i+i), "%02X", mac[i]);
			}
			else
			{
				sprintf(buf+(2*i+i), "%02X:",mac[i]);
			}
		}

		return string(buf);
	}

	static unsigned char char2dec(const char ch)  
	{  
		switch(ch)  
		{  
			case '0': return 0;  
			case '1': return 1;  
			case '2': return 2;  
			case '3': return 3;  
			case '4': return 4;  
			case '5': return 5;  
			case '6': return 6;  
			case '7': return 7;  
			case '8': return 8;  
			case '9': return 9;  
			case 'a':   
			case 'A': return 10;  
			case 'b':   
			case 'B': return 11;  
			case 'c':  
			case 'C': return 12;  
			case 'd':   
			case 'D': return 13;  
			case 'e':   
			case 'E': return 14;  
			case 'f':  
			case 'F': return 15;      
			default: break;
		}  
		return 0;  
	}  

	/* param @macstr: char str[18],@machex: char mac[6] */
	static void mac_str2bin(const char *str,unsigned char *mac)
	{  
		for (int i = 0;i < 6;++i)  
		{  
			mac[i] = (char2dec(str[3*i+0]) & 0x00FF)<<4;  
			mac[i] += char2dec(str[3*i+1]) & 0x00FF;
		}     
	}  

	/* param &ip:char ip[16]; */
	static unsigned int ip_str2int(const char *ipstr)
	{
		//return ntohl(inet_addr(ipaddr));	
		if (ipstr == NULL) return 0; 

		unsigned int i = 3, total = 0, cur; 
		char *str=const_cast<char *>(ipstr);
		char *token = strtok(str, "."); 

		while (token != NULL) 
		{ 
			cur = atoi(token); 
			if (cur >= 0 && cur <= 255) 
			{ 
				total += cur * pow(256, i); 
			}
 
			i --; 
			token = strtok(NULL, "."); 
		} 

		return total; 
	}

	static string ip_int2str(const unsigned int &ip)
	{
		char temp[16]={0};
		unsigned int v=ip;//ntohl(ip);
		sprintf(temp, "%d.%d.%d.%d",  
				(v & 0xff000000) >> 24,  
				(v & 0x00ff0000) >> 16,  
				(v & 0x0000ff00) >> 8,  
				(v & 0x000000ff));  

		return string(temp);  
	}

	static string utc2str(const time_t &t)
	{
		struct tm *p = localtime(&t);
		char szFormat[128];

		sprintf(szFormat, "%d-%d-%d %d:%d:%d", p->tm_year + 1900, p->tm_mon + 1,
				p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);

		return string(szFormat);
	}

};

#endif
