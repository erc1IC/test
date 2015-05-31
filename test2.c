#include<reg51.h>
#include<math.h>

sbit beep=P2^3;
sbit dula=P2^0;		//段选信号的锁存器控制
sbit wela=P2^1;		//位选信号的锁存器控制		
sbit cs88=P2^2;		//点阵管的锁存器控制        cs88=0;//关点阵管      
sbit led_en=P2^5;
sbit DS1302=P2^7;	//定义DS1302时钟使能端，用于HJ-C52实验板复位，与本实验无关
sbit SD=P2^6;		//定义SD卡使能端，用于HJ-C52实验板复位，与本实验无关

unsigned char code numbers_display[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66,
										0x6d, 0x7d, 0x07, 0x7f, 0x6f,
										0xbf, 0x86, 0xdb, 0xcf, 0xe6,
										0xed, 0xfd, 0x87, 0xff, 0xef};
unsigned char code display_area[] = {0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f}; 
									 
//延时，i（ms）
void delay(unsigned char i)
{
	unsigned int j,k;
	for(j = i; j > 0; j--)
		for(k = 125; k > 0; k--);
}
//显示，传入字符数组
void display(unsigned char result[])
{
	unsigned char num = result[0];
	unsigned char area = 0;
	
	while(-1 < num && num < 20 && area != 9)
	{
		P0 = display_area[area++];
		wela = 1;
		wela = 0;
		P0 = numbers_display[num];
		dula = 1;
		dula = 0;
		P0=0x00;	  //什么都不显示的代码
		dula=1;
		dula=0;
		num = result[area];
	}
	
}
//键盘扫描,无输入|输入错误返回-1
unsigned char keyscan()
{
	unsigned char temp,key_click;
	P3 = 0xfe;
	temp = P3;
	temp = temp & 0xf0;
	if(temp != 0xf0)
	{
		delay(10);
		if(temp != 0xf0)
		{
			temp = P3;
			switch(temp)
			{
				case 0xee:
					key_click = 1;break;
				case 0xde:
					key_click = 2;break;
				case 0xbe:
					key_click = 3;break;
				case 0x7e:
					key_click = '+';break;
				default:
				   key_click = -1;
			}
			while(temp != 0xf0)
			{
			   temp=P3;
			   temp=temp&0xf0;
			}
			return key_click;
		}
	}
	P3 = 0xfd;
    temp = P3;
    temp = temp & 0xf0;
    if(temp != 0xf0)
    {
		delay(10);
		if(temp != 0xf0)
		{
			temp = P3;
			switch(temp)
			{
				case 0xed:
					key_click = 4;break;
				case 0xdd:
					key_click = 5;break;
				case 0xbd:
					key_click = 6;break;
				case 0x7d:
					key_click = '-';break;
				default:
				   key_click = -1;
			}
			while(temp != 0xf0)
			{
			   temp=P3;
			   temp=temp&0xf0;
			}
			return key_click;
		}
	}
	P3 = 0xfb;
    temp = P3;
    temp = temp & 0xf0;
    if(temp != 0xf0)
    {
		delay(10);
     	if(temp != 0xf0)
		{
	        temp=P3;
	        switch(temp)
	        {
				case 0xeb:
					key_click = 7;break;
				case 0xdb:
					key_click = 8;break;
				case 0xbb:
					key_click = 9;break;
				case 0x7b:
					key_click = '*';break;
				default:
				   key_click = -1;
	        }
			while(temp != 0xf0)
			{
			   temp=P3;
			   temp=temp&0xf0;
			}
			return key_click;
		}
	}
	P3 = 0xf7;
    temp = P3;
    temp = temp & 0xf0;
    if(temp != 0xf0)
    {
    	delay(10);
    	if(temp != 0xf0)
    	{
	        temp = P3;
	        switch(temp)
	        {
				case 0xe7:
					key_click = 0;break;
				case 0xd7:
					key_click = '.';break;
				case 0xb7:
					key_click = '=';break;
				case 0x77:
					key_click = '/';break;
				default:
					key_click = -1;
	         }
			 while(temp != 0xf0)
			{
			   temp=P3;
			   temp=temp&0xf0;
			}
			return key_click;
		}
	}
	return -1;
}
//重置显示
void reset(unsigned char *result)
{
	unsigned char i = 1;
	*result = 0;
	while(i < 9)
	{
		*(result + i++) = -1;
	}
}
//计算
void mathfun(float count[], unsigned char math_key, unsigned char *result)
{
	unsigned char ltemp, rtemp, point_area, result2[9];
	unsigned char k = 0, j = 0;
	float temp;
	switch(math_key)
	{
		case '+':
			temp = count[0] + count[1];
			break;
			
		case '-':
			temp = count[0] - count[1];
			break;
			
		case '*':
			temp = count[0] * count[1];
			break;
			
		case '/':
			temp = count[0] / count[1];
			break;
			
	}
	
	ltemp = (int)temp;
	do{
		result2[k] = ltemp%10;
		ltemp /= 10;
		k++;
	}while(ltemp != 0);
	do{
		result[j++] = result2[--k];
	}while(k);
	
	if(temp-(int)temp >= 0.01)
	{
		result[--j] += 10;
		k = 0; j++;
		rtemp = (int)((temp - (int)temp)*100);
		do{
			result2[k] = rtemp%10;
			rtemp /= 10;
			k++;
		}while(rtemp != 0);
		do{
			result[j++] = result2[--k];
		}while(k);
	}
}


void main()
{
	unsigned char point_area, temp, math_key, result[9] = {0, -1, -1, -1, -1, -1, -1, -1, -1};
	unsigned char flag = 0,i = 0, j = 0;
	float count[2] = {0};
	DS1302=0; //关DS1302模块，与本实验无关
    SD=0;//关SD卡模块，与本实验无关
	cs88=0;		//这是为了关闭开发板上的点阵实际应用去掉
	led_en=0;
    cs88=0;	//关点阵管
	dula=0;
	wela=0;
	while(1){
		temp = keyscan();
		switch(temp)
		{
			case -1: break;
			case '+':
			case '-':
			case '*':
			case '/':
				if(i == 0)
				{
					math_key = temp;
					flag = 0;
					i++;
				}else{
					break;
				}
				j = 0; reset(result);
				break;
				
			case '.':                        
				if(flag == 0 && j != 0)
				{
				result[--j] += 10;
				point_area = j++;
				flag = 1;
				}else{
					result[j] += 10;
					point_area = j++;
					flag = 1;
				}
				break;
				
			case '=':
				reset(result);
				mathfun(count, math_key, result);
				i = 0; j = 0; flag = 0; count[0] = 0; count[1] = 0;
				break;
				
			default:
				if(result[0] == 0 && temp == 0)break;
				if(flag == 0)
				{
					count[i] = count[i]*10 + temp;
				}else{
					count[i] = count[i] + temp/pow(10, j - point_area);
				}
				result[j++] = temp;
		}
		display(result);
	}
	
}