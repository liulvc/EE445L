#include "XBeeOut.h"
#include "UART2.h"
#include "globals.h"
#include "SysTick.h"
#include "RIT128x96x4.h"
#include "FIFO.h"

// delay function for testing from sysctl.c
// which delays 3*ulCount cycles
#ifdef __TI_COMPILER_VERSION__
	//Code Composer Studio Code
	void Delay(unsigned long ulCount){
	__asm (	"    subs    r0, #1\n"
			"    bne     Delay\n"
			"    bx      lr\n");
}

#else
	//Keil uVision Code
	__asm void
	Delay(unsigned long ulCount)
	{
    subs    r0, #1
    bne     Delay
    bx      lr
	}

#endif

/*
 * Initializes XBee to send output mode
 
  The XBee module is controlled by a series of AT commands. (This command set was originally developed to
control asynchronous telephone modems and was developed by the Hayes Company. The AT prefix was an
abbreviation for ATtention.) This routine sets the 16-bit module identifier of both the transmitter and receiver
modules using the MY and DL AT command suffixes. This routine also conditions the module to operate in
Application Programming Interface (API) mode 1. When operating in this mode, the module accepts data to be
transmitted as structured frames rather than as a transparent serial communication device and it permits to the
transmitter to automatically be notified if the receiver correctly received the transmission. In this protocol, all message
characters are ASCII characters. E.g., A is letter A or $41. Numbers are also in ASCII, as hex numbers. For example
the number 79 is $4F and transmitted as ASCII 4 ($34), ASCII F ($46). The ASCII character <CR> is the carriage
return (13 or $0D) and is used to terminate an AT command. The space is optional between the command and the
parameter.
LM3S to XBee XBee response to LM3S Meaning
X wait 1.1s +++ wait 1.1s OK<CR> Enter command mode
ATDL4F<CR> wait 20ms OK<CR> Sets destination address to 79
ATDH0<CR> wait 20ms OK<CR> Sets destination high address to 0
ATMY4E<CR> wait 20ms OK<CR> Sets my address to 78
ATAP1<CR> wait 20ms OK<CR> API mode 1 (sends/receive packets)
ATCN<CR> wait 20ms OK<CR> Ends command mode
Some of the default parameters are channel (CH=12), PAN (ID= 0x3332 or 13106) destination high address (DH=0),
and baud rate (BD=3, for 9600 bits/sec)
 */
 
 unsigned char ID;
 char response [10];
void XBeeInit(){
	char * commands [] = {"ATDL4F", "ATDH0", "ATMY4E", "ATAP1", "ATCN"};
	int i = 0;
	int j;
	UART_Init();
	ID = 1;
//  UART_OutString("X");
	Delay(55000000);
	 //SysTick_Wait10ms(110);		//wait waitTime number of ms;
	sendATCommand("+++", 110, 0);
	UART_InString(response, 5);
	RIT128x96x4StringDraw(response, 10, 10 , 15);
	
	for (i=1;i<6;i++){
		sendATCommand(commands[i], 20, 1);
	}
 }

/*
 * Sends output via XBee.
 This routine transmits the API transmit data frame to the XBee module via UART1.
 */
 
 void XBeeSendTxFrame(char * frame, int len){
	 
	 int i;
	 for (i=0;i<len;i++)
		UART_OutChar(frame[i]);
	 
 }
 
 /*
 * Creates frame to send.
 
 This routine creates an API transmit data frame consisting of a start delimiter, frame length, frame data, and
checksum fields. The frame data field contains destination address and transmission options information. Increment
the Frame Id (byte 5 in the figure at the bottom page 57) from 1 to 255, and then back to 1 again.
 */
 
void XBee_sendDataFrame(char * data){
	char frame [20];
	XBee_CreateTxFrame(strlen2(data), 1, data,frame);
	XBeeSendTxFrame(frame, strlen2(data) + 9);

}
void XBee_CreateTxFrame(unsigned int len, char api, char * data, char * frame){
	 int i;
	 int checkLocation = 0;
	 frame [0] = 0x7e; //start delimeter
	 frame [1] = len & 0xFF00;
	 frame [2] = len & 0x00FF; //
	 frame [3] = api; //API = 1;
	 frame[4] = ID++;
	 frame[5] = 0; //destination top
	 frame[6] = 2; //destination bottom
	 frame[7] = 0; //opt
	 for (i=0; i < (len);i++)
			frame[8 + i] = data[i];
	 checkLocation = 8+i;
	 frame[checkLocation] = 0xFF; //set initial value of checksum
	 for (i = 3; i <checkLocation; i++)
		frame[checkLocation] -= frame[i];
	 
	 if (ID==0) ID = 1;
	 
 }
 
  /*
 * Determines status (if received acknowledgment from destination module.
 
 When the XBee module transmits an API transmit data frame it will receive an acknowledgement from the
destination module if the frame was received without errors. The status of the transmission will be sent to the
LM3S1968 via an API transmit status frame. This routine returns a 1 if the transmission was successful and a 0
otherwise. The following figure shows a response the XBee returns after the transmitter sends a TxFrame that was
properly received by the other computer, measured on XBee pin 2 Dout.

 */
 void XBee_TxStatus(){
	 
	 
 }
 

void xBee_ReadFrame(char * api, char * frame ){
	 unsigned int length = 0;
	int i;
	 while (UART_InChar() != 0x7e); //start delimeter of frame
	 length = ((UART_InChar() << 8) + UART_InChar());
		*api = UART_InChar();
	 for (i=0;i<length;i++)
		frame[i] = UART_InChar();
 }
 /*
 * sends an AT command repeatedly until it receives a reply that it was correctly received
This routine receives the various parameters associated with an AT command as input then transmits the formatted
command to the XBee module. After a blind-cycle delay, the routine checks if the command has been successfully
received by determining if the module has returned the OK character string.
 */
char aa;
char bb;
	 char frame[50];

 void sendATCommand( char * command, int waitTime, char CRout){
	 char done = 0;
	 int j;
	 int size;
	 int commandLen = strlen2(command);
	 do{
		 UART_OutString(command);
		 if (CRout)
			UART_OutChar(CR);
	Delay(500000*waitTime);
	j = 0;
  size = RxFifo_Size();
	while (size>0){
		frame[j++] = UART_InChar();
	size = RxFifo_Size();
		Delay(5000);
	}

	if (frame[0] == 'O' && frame[1] == 'K' && frame[2] == CR)
		done = 1;
	} while (!done);
	 
 }
 
 