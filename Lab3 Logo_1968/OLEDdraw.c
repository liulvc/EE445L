#include "globals.h"
#include "RIT128x96x4.h"


float coslut[60] = {
	1.000, 0.995, 0.978, 0.951, 0.914, 0.866, 0.809, 0.743, 0.669, 0.588, 0.500, 0.407, 0.309, 0.208, 0.105, 0.000, -0.105, -0.208, -0.309, -0.407, -0.500, -0.588, -0.669, -0.743, -0.809, -0.866, -0.914, -0.951, -0.978, -0.995, -1.000, -0.995, -0.978, -0.951, -0.914, -0.866, -0.809, -0.743, -0.669, -0.588, -0.500, -0.407, -0.309, -0.208, -0.105, 0.000, 0.105, 0.208, 0.309, 0.407, 0.500, 0.588, 0.669, 0.743, 0.809, 0.866, 0.914, 0.951, 0.978, 0.995
};

float sinlut[60] = {
	0.000, 0.105, 0.208, 0.309, 0.407, 0.500, 0.588, 0.669, 0.743, 0.809, 0.866, 0.914, 0.951, 0.978, 0.995, 1.000, 0.995, 0.978, 0.951, 0.914, 0.866, 0.809, 0.743, 0.669, 0.588, 0.500, 0.407, 0.309, 0.208, 0.105, 0.000, -0.105, -0.208, -0.309, -0.407, -0.500, -0.588, -0.669, -0.743, -0.809, -0.866, -0.914, -0.951, -0.978, -0.995, -1.000, -0.995, -0.978, -0.951, -0.914, -0.866, -0.809, -0.743, -0.669, -0.588, -0.500, -0.407, -0.309, -0.208, -0.105
};

void drawCircle(int x, int y, int radius);

int miniteRadius = 40;
int secondRadius = 20;
int hourRadius = 30;
unsigned char color  = 15;

void analogClockDraw(){
	int x_s = (int)(coslut[seconds]*secondRadius + 64);
	int y_s = (int)(sinlut[seconds]*secondRadius + 96);
	int x_m = (int)(coslut[minutes]*miniteRadius + 64);
	int y_m = (int)(sinlut[minutes]*miniteRadius + 96);
	int x_h = (int)(coslut[hours*5]*hourRadius + 64);
	int y_h = (int)(sinlut[hours*5]*hourRadius + 96);
	
	DisableInterrupts();
	RIT128x96x4_ClearImage();
	drawCircle(64, 48, 45);

	//draws clock hands
	RIT128x96x4_Line(64, 48, x_s, y_s, color);
	RIT128x96x4_Line(64, 48, x_m, y_m, color);
	RIT128x96x4_Line(64, 48, x_h, y_h, color);
	RIT128x96x4_ShowImage();
	EnableInterrupts();
}

//hh:mm:ss
void digitalClockDraw(){
	char time[20];
	DisableInterrupts();
	sprintf(time, "%d:%d:%d\n", hours, minutes, seconds);
//	printf("time ");
//	printf("%d:%d:%d\n", hours, minutes, seconds);
//	printf("time end");
 	RIT128x96x4StringDraw(time, 52, 44, color);
	EnableInterrupts();
}


//draws circle with radius r centered at (x,y)
void drawCircle(int x, int y, int r) {
	int i;
	int x_1 = r*coslut[0]+x;
	int y_1 = r*sinlut[0]+y;

	for(i = 1; i < 60; i++){
		int x_2 = r*coslut[i]+x;
		int y_2 = r*sinlut[i]+y;
		RIT128x96x4_Line(x_1, y_1, x_2, y_2, color);
		x_1 = x_2;
		y_1 = y_2;
	}
}