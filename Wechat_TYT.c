#include<reg52.h>
unsigned char a0,b0,c0,d0,f0,e0,f0,s0,s1,s2,s3,temp,key,wei,qingling,confirm,gongneng;
unsigned int j,k,time;
sbit dula=P2^6;
sbit wela=P2^7;
sbit beep=P2^3;
sbit SG_PWM=P1 ^0;

unsigned char count=0;
unsigned char PWM_count=1;

unsigned char code table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,
                        0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,0x00};

void delay(unsigned int i)
{
	for(j=i;j>0;j--)
	for(k=114;k>0;k--);
}

void Timer_Init() 
{
  TMOD=0X01;      //T0定时方式1
  TH0=0Xfe;
  TL0=0X33;       //计数初值设置为0.5ms
  ET0=1;          //打开定时器0的中断
  TR0=1;          //打开定时器0
  EA=1;          //开总中断
}

void Timer() interrupt 1    //特别注意此处，0--外部中断0，1--定时器中断0，2--外部中断1，3--定时器中断1，4--串行口中断1
{
  TR0=0;
  TH0=0Xfe;
  TL0=0X33;       //重新赋计数初值为0.5ms
  if(count<=PWM_count)
  {
    SG_PWM=1;
  }
  else
  {
    SG_PWM=0;
  }
  count++;
  if(count>=40)
  {
    count=0;
  }
  TR0=1;
}

void display(unsigned char a,unsigned char b,unsigned char c)
{
   
   dula=0;
   P0=table[a];
   dula=1;
   dula=0;

   wela=0;
   P0=0xfe;
   wela=1;
   wela=0;
   delay(5);
   
   P0=table[b];
   dula=1;
   dula=0;
 
   P0=0xfd;
   wela=1;
   wela=0;
   delay(5);

   P0=table[c];
   dula=1;
   dula=0;
 
   P0=0xfb;
   wela=1;
   wela=0;
   delay(5);
   
}

void keyscan()
{
  {	
    P3=0xfe;
    temp=P3;
    temp=temp&0xf0;
    if(temp!=0xf0)
    {
      delay(10);
      if(temp!=0xf0)
      {	
        temp=P3;
        switch(temp)
        {
          case 0xee:
               key=0;
			   wei++;
               break;

          case 0xde:
               key=1;
			   wei++;
               break;

          case 0xbe:
               key=2;
			   wei++;
               break;

          case 0x7e:
               key=3;
			   wei++;
               break;
         }
         while(temp!=0xf0) 
        {
           temp=P3;
           temp=temp&0xf0;
           beep=0;
         }
         beep=1;
      }
    }
    P3=0xfd;
    temp=P3;
    temp=temp&0xf0;
    if(temp!=0xf0)
    {
      delay(10);
      if(temp!=0xf0)
      {
        temp=P3;
        switch(temp)
        {
          case 0xed:
               key=4;
			   wei++;
               break;

          case 0xdd:
               key=5;
			   wei++;
               break;

          case 0xbd:
               key=6;
			   wei++;
               break;

          case 0x7d:
               key=7;
			   wei++;
               break;
         }
         while(temp!=0xf0)
         {
           temp=P3;
           temp=temp&0xf0;
           beep=0;
         }
         beep=1;
      }
      }
    P3=0xfb;
    temp=P3;
    temp=temp&0xf0;
    if(temp!=0xf0)
    {
      delay(10);
      if(temp!=0xf0)
      {
        temp=P3;
        switch(temp)
        {
          case 0xeb:
               key=8;
			   wei++;
               break;

          case 0xdb:
               key=9;
			   wei++;
               break;

	      case 0xbb:
               qingling=1;
			   //key=10;
			   //wei++;
               break;
		  
		  case 0x7b:
               confirm=1;
			   //key=11;
			   //wei++;
               break;
         }
        while(temp!=0xf0)
         {
           temp=P3;
           temp=temp&0xf0;
           beep=0;
         }
         beep=1;
      }
      }
}
}

void display0()
{
	if(key!=20)
	{
	switch(wei)
		{
			case 1: a0=key;
					b0=16;
					c0=16;
					d0=16;
					e0=16;
					f0=16;
					break;
			case 2: b0=key;
					c0=16;
					d0=16;
					e0=16;
					f0=16;
					break;
			case 3: c0=key;
					d0=16;
					e0=16;
					f0=16;
					break;
	}
	key=20;
	}
	display(a0,b0,c0);
	if(a0!=16) s1=a0;
	if(b0!=16) s2=b0;
	if(c0!=16) s3=c0;
	time=s1*200+s2*20+s3*2;

}


void main()
{    
    Timer_Init();

  	while(1)
	{
	    keyscan();
		gongneng=qingling|confirm;
		if(gongneng==0)
		{
			display0();	
		}
		else
		{
	      if (qingling)
		  {
		    wei=0;
		    a0=16;b0=16;c0=16;
		    s0=0;s1=0;s2=0;
		    qingling=0;
		    display(a0,b0,c0);	
		  }
		  if (confirm==1)
		  {
		   delay(10); 
		   if(confirm==1)
		   {
		    PWM_count=0;
			delay(time);
			PWM_count=1;
			delay(time);																																																																									  
			PWM_count=0;
			delay(time);
			wei=0;
			confirm=0;
		   }
		  }
		}	
	}
}                           