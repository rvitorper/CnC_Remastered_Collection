//
// Copyright 2020 Electronic Arts Inc.
//
// TiberianDawn.DLL and RedAlert.dll and corresponding source code is free 
// software: you can redistribute it and/or modify it under the terms of 
// the GNU General Public License as published by the Free Software Foundation, 
// either version 3 of the License, or (at your option) any later version.

// TiberianDawn.DLL and RedAlert.dll and corresponding source code is distributed 
// in the hope that it will be useful, but with permitted additional restrictions 
// under Section 7 of the GPL. See the GNU General Public License in LICENSE.TXT 
// distributed with this program. You should have received a copy of the 
// GNU General Public License along with permitted additional restrictions 
// with this program. If not, see https://github.com/electronicarts/CnC_Remastered_Collection

/* $Header: /CounterStrike/MAPSEL.CPP 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : MAPSEL.CPP                                                   *
 *                                                                                             *
 *                   Programmer : Barry W. Green                                               *
 *                                                                                             *
 *                   Start Date : April 17, 1995                                               *
 *                                                                                             *
 *                  Last Update : April 27, 1995   [BWG]                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Bit_It_In -- Pixel fade graphic copy.                                                     *
 *   Map_Selection -- Starts the whole process of selecting next map to go to                  *
 *   Print_Statistics -- Prints statistics on country selected                                 *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"

void Cycle_Call_Back_Delay(int time, PaletteClass &pal);
extern int ControlQ;

int Mouse_Over_Spot(int house, int scenario);
void Set_Mouse(MouseType shape, int &start, int &count, int &delay, int &xspot, int &yspot);
//VG for ant mission progression
const char* antmission[] = {NULL, "SCA01EA.INI", "SCA02EA.INI", "SCA03EA.INI", "SCA04EA.INI"};

struct point {
	int x;
	int y;
} const MapCoords[2][14][3] = {
 {
 {{185,123},{ -1, -1},{ -1, -1}},
 {{173,112},{ -1, -1},{ -1, -1}},
 {{196,100},{200,112},{ -1, -1}},
 {{175,113},{ -1, -1},{ -1, -1}},
 {{187, 91},{202, 93},{206,105}},
 {{207,161},{212,172},{ -1, -1}},
 {{172, 92},{ -1, -1},{ -1, -1}},
 {{132,119},{146,125},{ -1, -1}},
 {{199, 73},{205, 86},{ -1, -1}},
 {{236,114},{ -1, -1},{ -1, -1}},
 {{219, 64},{225, 76},{ -1, -1}},
 {{256, 69},{ -1, -1},{ -1, -1}},
 {{262, 77},{ -1, -1},{ -1, -1}},
 {{249, 97},{ -1, -1},{ -1, -1}}
 },
// Soviet coords
 {
 {{178,105},{ -1, -1},{ -1, -1}},
 {{163,101},{163,113},{ -1, -1}},
 {{160, 89},{ -1, -1},{ -1, -1}},
 {{142,101},{142,117},{ -1, -1}},
 {{212,163},{ -1, -1},{ -1, -1}},
 {{155,133},{171,144},{ -1, -1}},
 {{216,103},{ -1, -1},{ -1, -1}},
 {{132,145},{154,154},{ -1, -1}},
 {{122,117},{ -1, -1},{ -1, -1}},
 {{117,130},{ -1, -1},{ -1, -1}},
 {{ 99,107},{109,146},{ -1, -1}},
 {{134,125},{ -1, -1},{ -1, -1}},
 {{ 32,156},{ 46,171},{ -1, -1}},
 {{108, 97},{ -1, -1},{ -1, -1}}
 }
};

#ifndef WIN32
extern short StreamLowImpact;
#endif  //WIN32

/***********************************************************************************************
 * Map_Selection -- Starts the whole process of selecting next map to go to                    *
 *                                                                                             *
 *                                                                                             *
 * INPUT:                                                                                      *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1996 BWG : Created.                                                                 *
 *=============================================================================================*/
extern int CopyType;
#ifndef WIN32
extern short StreamLowImpact;
#endif
char const * Map_Selection(void)
{
	return NULL;
}

int Mouse_Over_Spot(int house, int scenario)
{
	int retval = -1;
	for (int selection = 0; selection < 3 && MapCoords[house][scenario][selection].x != -1; selection++) {
		int mousex = Get_Mouse_X() / RESFACTOR;
		int mousey = Get_Mouse_Y() / RESFACTOR;
		if (mousex >= MapCoords[house][scenario][selection].x &&
			 mousey >= MapCoords[house][scenario][selection].y &&
			 mousex <= MapCoords[house][scenario][selection].x+11 &&
			 mousey <= MapCoords[house][scenario][selection].y+9) {

			retval = selection;
			break;
		}
	}
	return(retval);
}
void Cycle_Call_Back_Delay(int time, PaletteClass &pal)
{
	static CDTimerClass<SystemTimerClass> _ftimer;
	static bool _up = false;
	static int val = 255;

	while(time--) {
		/*
		**	Process the fading white color.
		*/
		if (!_ftimer) {
			_ftimer = TIMER_SECOND/6;

			#define	STEP_RATE	20
			if (_up) {
				val += STEP_RATE;
				if (val > 150) {
					val = 150;
					_up = false;
				}
			} else {
				val -= STEP_RATE;
				if (val < 0x20) {
					val = 0x20;
					_up = true;
				}
			}

			/*
			**	Set the pulse color as the proportional value between white and the
			**	minimum value for pulsing.
			*/
			pal[254] = GamePalette[WHITE];
			pal[254].Adjust(val, BlackColor);

			pal.Set();
		}
		Call_Back_Delay(1);
	}
}

void Set_Mouse(MouseType shape, int &start, int &count, int &delay, int &xspot, int &yspot)
{
	switch(shape) {
		case MOUSE_NORMAL:
			start = 0;
			count = 1;
			delay = 0;
			xspot = 0;
			yspot = 0;
			break;
		default:
			start = 21;
			count = 8;
			delay = 4;
			xspot = 14;
			yspot = 11;
			break;
	}
}
