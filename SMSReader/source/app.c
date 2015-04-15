
/*
*------------------------------------------------------------------------------
* Include Files
*------------------------------------------------------------------------------
*/
#include "config.h"
#include "board.h"
#include "timer.h"
#include "communication.h"
#include "app.h"
#include "lcd.h"
#include "string.h"
#include "eep.h"


//#define SIMULATION
/*
*------------------------------------------------------------------------------
* Structures
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Variables
*------------------------------------------------------------------------------
*/
 const UINT8 index[] = "+CMTI";
 const UINT8 data[]  = "+CMGR";

/*------------------------------------------------------------------------------
* Private Functions
*------------------------------------------------------------------------------
*/

typedef struct _APP
{
	UINT8 message[MAX_MESSAGE_SIZE];
	UINT8 smsIndex[50];
	UINT8 buffIndex;
	UINT8 smsCounter;
	UINT8 currIndex;
}APP;

APP app = {0};

UINT8 APP_comCallBack( far UINT8 *rxPacket, far UINT8* txCode,far UINT8** txPacket);


/*
*------------------------------------------------------------------------------
* void APP-init(void)
*------------------------------------------------------------------------------
*/

void APP_init(void)
{

	

	COM_init(CMD_SOP , CMD_EOP ,RESP_SOP , RESP_EOP , APP_comCallBack);



}


/*
*------------------------------------------------------------------------------
* void APP-task(void)
*------------------------------------------------------------------------------
*/
void APP_task(void)
{
	if(app.smsCounter > 0)
	{
		LCD_clear( );
		if(app.smsIndex[app.currIndex] != '\0')
		{
			LCD_putChar(app.smsIndex[app.currIndex]);
			app.currIndex++;
		}
		app.currIndex++;
		app.smsCounter--;	
	}

}

void APP_writeEEPROM( UINT8 *buffer )
{
	UINT8 i;
	for(i = 0; i < MAX_OUTPUT_CHARS; i++)
	{
		Write_b_eep( (EEPROM_ADD + i), buffer[i]);
		Busy_eep();
	}
}

void APP_readEEPROM( UINT8 *buffer )
{
	UINT8 i;
	for(i = 0; i < MAX_OUTPUT_CHARS; i++)
	{
		buffer[i] = Read_b_eep(EEPROM_ADD + i);
		Busy_eep();
	}
}


void APP_call( UINT8 *buffer )
{
	COM_txCMD( DEVICE_ADDRESS, buffer, 3);
}





UINT8 APP_comCallBack( far UINT8 *rxPacket, far UINT8* txCode,far UINT8** txPacket)
{

	UINT8 i;

	//UINT8 rxCode = rxPacket[0];
	UINT8 length = 0;

	if (strncmp(rxPacket,index,5) == 0 )
	{
		i =12;
		while(rxPacket[ i] != '\0')
		{
			app.smsIndex[app.buffIndex++ ] = rxPacket[ i++];
		}
		app.smsIndex[app.buffIndex++ ]= '\0';

		if(app.buffIndex >= MAX_MESSAGE_SIZE)
			app.buffIndex = 0;
		app.smsCounter++;	
	}


//		if(strcmp ( , &data) == TRUE)

	return length;

}
	
	
	

