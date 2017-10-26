/*======================================================================================================================================
												Header file
  ======================================================================================================================================*/

#include <stdio.h>
#include "coffee_buy.h"
#include "coffee_max.h"
#include "coke_buy.h"
#include "coke_max.h"
#include "sold_out.h"
#include "vending.h"

/*======================================================================================================================================
												Static variable
  ======================================================================================================================================*/

volatile unsigned int *textlcd_ctrl_base 	= (unsigned int *) 0x43c03000;
volatile unsigned int *sevenseg_ctrl_base	= (unsigned int *) 0x43C01000;
volatile unsigned int *led_ctrl_base		= (unsigned int *) 0x43C00004;
volatile unsigned int *sw_ctrl_base			= (unsigned int *) 0x43C00008;
volatile unsigned int *tftlcd_ctrl_base		= (unsigned int *) 0x43C02000;
volatile unsigned int *BRAM 				= (unsigned int *) 0x83C00000;


/*======================================================================================================================================
												Function Prototype
  ======================================================================================================================================*/

void textlcd(char *, char *);
void manager(int *max1, int *max2);
void loading();

/*======================================================================================================================================
												Main Function
  ======================================================================================================================================*/

int main()
{
	char up_line[16];
	char down_line[16];
	int coffee = 5, coke = 5;
	int x, y, z;

	/* LED Test - LODING */
	loading();

	while(1)
	{
		*tftlcd_ctrl_base = 0x1;
		*(tftlcd_ctrl_base+1) = 0x1;

		sprintf(up_line,"                ");
		sprintf(down_line,"                ");
		textlcd(up_line, down_line);

		for (y=0; y<272; y++)
		{
			for( x=0; x<240; x++)
			{
				*(BRAM+y*240+x) = vending[x*2 + 1 + y*480]<<16 | vending[x*2 + y*480];
			}
		}

		if(coffee < 1 && coke > 0)
		{
			sprintf(up_line,"    coffee    ");
			sprintf(down_line,"    sold out    ");
			textlcd(up_line, down_line);

			for (y=0; y<272; y++)
			{
				for( x=0; x<240; x++)
				{
					*(BRAM+y*240+x) = coffee_max[x*2 + 1 + y*480]<<16 | coffee_max[x*2 + y*480];
				}
			}
		}
		if(coke < 1 && coffee > 0)
		{
			sprintf(up_line,"     coke     ");
			sprintf(down_line,"    sold out    ");
			textlcd(up_line, down_line);

			for (y=0; y<272; y++)
			{
				for( x=0; x<240; x++)
				{
					*(BRAM+y*240+x) = coke_max[x*2 + 1 + y*480]<<16 | coke_max[x*2 + y*480];
				}
			}
		}
		if(coke < 1 && coffee < 1)
		{
				sprintf(up_line,"       all      ");
				sprintf(down_line,"    sold out    ");
				textlcd(up_line, down_line);

				for (y=0; y<272; y++)
				{
					for( x=0; x<240; x++)
					{
						*(BRAM+y*240+x) = sold_out[x*2 + 1 + y*480]<<16 | sold_out[x*2 + y*480];
					}
				}
		}

		if(*sevenseg_ctrl_base > 0x98000000)
		{
			*sevenseg_ctrl_base = 0x99000000;
			sprintf(up_line,"  no more coin  ");
			sprintf(down_line,"   full  coin   ");
			textlcd(up_line, down_line);
		}
		if(*sw_ctrl_base == 0x00FE && *sevenseg_ctrl_base >= 0x05000000 && coffee > 0)
		{
				for (y=0; y<272; y++)
				{
					for( x=0; x<240; x++)
					{
						*(BRAM+y*240+x) = coffee_buy[x*2 + 1 + y*480]<<16 | coffee_buy[x*2 + y*480];
					}
				}
				sprintf(up_line,"     coffee     ");
				sprintf(down_line,"      500       ");
				textlcd(up_line, down_line);
				if((*sevenseg_ctrl_base & 0x0F000000) < 0x05000000 )
				{
					*sevenseg_ctrl_base = *sevenseg_ctrl_base - 0x10000000;
					*sevenseg_ctrl_base += 0x0A000000;
				}
				*sevenseg_ctrl_base -= 0x05000000;
				while(*sw_ctrl_base == 0x00FE);
				sleep(1);
				coffee--;
		}
		else if(*sw_ctrl_base == 0x00FE && *sevenseg_ctrl_base == 0x00000000)
		{
				for (y=0; y<272; y++)
				{
					for( x=0; x<240; x++)
					{
						*(BRAM+y*240+x) = coffee_buy[x*2 + 1 + y*480]<<16 | coffee_buy[x*2 + y*480];
					}
				}
				*sevenseg_ctrl_base = 0x00000500;
				while(*sw_ctrl_base == 0x00FE);
				*sevenseg_ctrl_base = 0x00000000;
				sleep(1);
		}
		else if(*sw_ctrl_base == 0x00FD && *sevenseg_ctrl_base >= 0x07000000 && coke > 0)
		{
				for (y=0; y<272; y++)
				{
					for( x=0; x<240; x++)
					{
						*(BRAM+y*240+x) = coke_buy[x*2 + 1 + y*480]<<16 | coke_buy[x*2 + y*480];
					}
				}
				sprintf(up_line,"      coke      ");
				sprintf(down_line,"      700       ");
				textlcd(up_line, down_line);
				if((*sevenseg_ctrl_base & 0x0F000000) < 0x07000000 )
				{
					*sevenseg_ctrl_base = *sevenseg_ctrl_base - 0x10000000;
					*sevenseg_ctrl_base += 0x0A000000;
				}
				*sevenseg_ctrl_base -= 0x07000000;
				while(*sw_ctrl_base == 0x00FD);
				sleep(1);
				coke--;
		}
		else if(*sw_ctrl_base == 0x00FD && *sevenseg_ctrl_base == 0x00000000)
		{
				for (y=0; y<272; y++)
				{
					for( x=0; x<240; x++)
					{
						*(BRAM+y*240+x) = coke_buy[x*2 + 1 + y*480]<<16 | coke_buy[x*2 + y*480];
					}
				}
				*sevenseg_ctrl_base = 0x00000700;
				while(*sw_ctrl_base == 0x00FD);
				*sevenseg_ctrl_base = 0x00000000;
				sleep(1);
		}
		else if(*sw_ctrl_base == 0x00FB)
		{
			while(*sw_ctrl_base == 0x00FB)
			{
				manager(&coffee, &coke);
			}
			loading();
		}
		else if(*sw_ctrl_base == 0x00DF)
		{
			sprintf(up_line,"  return coin   ");
			sprintf(down_line,"    wait....    ");
			textlcd(up_line, down_line);
			while(*sevenseg_ctrl_base >= 0x01000000)
			{
				if((*sevenseg_ctrl_base & 0x0F000000) < 0x01000000 )
				{
					*sevenseg_ctrl_base = *sevenseg_ctrl_base - 0x10000000;
					*sevenseg_ctrl_base += 0x09000000;
				}
				*sevenseg_ctrl_base -= 0x01000000;
				usleep(100000);
			}
		}
		else if(*sw_ctrl_base == 0x00BF)
		{
			*sevenseg_ctrl_base += 0x10000000;
			if((*sevenseg_ctrl_base & 0xF0000000) == 0xA0000000)
			{
				*sevenseg_ctrl_base = *sevenseg_ctrl_base - 0x1000000;
			}
			while(*sw_ctrl_base == 0x00BF);
			sleep(1);
		}
		else if(*sw_ctrl_base == 0x007F)
		{
			*sevenseg_ctrl_base  += 0x01000000;
			if((*sevenseg_ctrl_base & 0x0F000000) == 0x0A000000)
			{
				*sevenseg_ctrl_base = *sevenseg_ctrl_base & 0xF0FFFFFF;
				*sevenseg_ctrl_base += 0x10000000;
				if(*sevenseg_ctrl_base > 0x99000000) *sevenseg_ctrl_base = 0x99000000;
			}
			while(*sw_ctrl_base == 0x007F);
			sleep(1);
		}
	}
}

/*======================================================================================================================================
												Functions
  ======================================================================================================================================*/

void textlcd(char up[32], char down[32])
{
	int i;
	for ( i=0 ; i<4 ; i++)
		{
				*(textlcd_ctrl_base + i)     = (up[i*4]<<24) + (up[i*4+1]<<16) + (up[i*4+2]<<8) + (up[i*4+3]);
				*(textlcd_ctrl_base + i + 4) = (down[i*4]<<24) + (down[i*4+1]<<16) + (down[i*4+2]<<8) + (down[i*4+3]);
		}
}

void manager(int *max1, int *max2)
{
	int num;
	char up_line[16];
	char down_line[16];

	sprintf(up_line,"     manager    ");
	sprintf(down_line,"      mode      ");
	textlcd(up_line, down_line);

	*led_ctrl_base = 0x00FF;
	*sevenseg_ctrl_base = 0x88888888;
	usleep(500000);
	*led_ctrl_base = 0x0000;
	*sevenseg_ctrl_base = 0x00000000;

	while(*sw_ctrl_base == 0x007B || *sw_ctrl_base == 0x0073 || *sw_ctrl_base == 0x006B || *sw_ctrl_base == 0x0063) //0111 1011
	{
		num = (*max1 << 16) + *max2;
		*sevenseg_ctrl_base = num;

		if(*sw_ctrl_base == 0x007B)
		{
			sprintf(up_line,"     coffee <<  ");
			sprintf(down_line,"      coke     ");
			textlcd(up_line, down_line);
			*led_ctrl_base = *max1;
		}
		else if(*sw_ctrl_base == 0x0073)		//01110011
		{
			sprintf(up_line,"     coffee     ");
			sprintf(down_line,"      coke  <<   ");
			textlcd(up_line, down_line);
			*led_ctrl_base = *max2;
		}
		else if(*sw_ctrl_base == 0x006B)		//01101011 coffee_max up
		{
			*max1= *max1+1;
			*sevenseg_ctrl_base += 0x00010000;
			if((*sevenseg_ctrl_base & 0x000F0000) == 0x000A0000)
			{
				*sevenseg_ctrl_base = *sevenseg_ctrl_base & 0xFFF0FFFF;
				*sevenseg_ctrl_base += 0x00100000;
			}
			while(*sw_ctrl_base == 0x006B);
			sleep(1);
		}
		else if(*sw_ctrl_base == 0x0063)		//01100011 coke_max up
		{
			*max2= *max2+1;
			*sevenseg_ctrl_base += 0x00000001;
			if((*sevenseg_ctrl_base & 0x0000000F) == 0x0000000A)
			{
				*sevenseg_ctrl_base = *sevenseg_ctrl_base & 0xFFFFFFF0;
				*sevenseg_ctrl_base += 0x00000010;
			}
			while(*sw_ctrl_base == 0x0063);
			sleep(1);
		}
	}
}

void loading()
{
	char up_line[16];
	char down_line[16];

	sprintf(up_line,"     Loading    ");
	sprintf(down_line,"    wait....    ");
	textlcd(up_line, down_line);
	int z;
	for(z = 0; z < 8; z++)
	{
		*led_ctrl_base = 0x0001;
		*led_ctrl_base = *led_ctrl_base << z;
		usleep(500000);
	}
	*led_ctrl_base = 0x00FF;
	*sevenseg_ctrl_base = 0x88888888;
	usleep(500000);
	*led_ctrl_base = 0x0000;
	*sevenseg_ctrl_base = 0x00000000;
}
