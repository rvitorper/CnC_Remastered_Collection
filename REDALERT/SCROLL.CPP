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

/* $Header: /CounterStrike/SCROLL.CPP 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : SCROLL.CPP                                                   *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 01/08/95                                                     *
 *                                                                                             *
 *                  Last Update : August 25, 1995 [JLB]                                        *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   ScrollClass::AI -- Handles scroll AI processing.                                          *
 *   ScrollClass::ScrollClass -- Constructor for the scroll class object.                      *
 *   ScrollClass::Set_Autoscroll -- Turns autoscrolling on or off.                             *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"

#define	SCROLL_DELAY	1

CDTimerClass<SystemTimerClass> ScrollClass::Counter;


/***********************************************************************************************
 * ScrollClass::ScrollClass -- Constructor for the scroll class object.                        *
 *                                                                                             *
 *    This is the constructor for the scroll class object.                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/10/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
ScrollClass::ScrollClass(void) :
	IsAutoScroll(true)
{
	Counter = SCROLL_DELAY;
	Inertia = 0;
}


/***********************************************************************************************
 * ScrollClass::AI -- Handles scroll AI processing.                                            *
 *                                                                                             *
 *    This routine is called every game frame for purposes of input processing.                *
 *                                                                                             *
 * INPUT:   input    -- Reference to the keyboard/mouse event that just occurred.              *
 *                                                                                             *
 *          x,y      -- The mouse coordinates.                                                 *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/10/1995 JLB : Created.                                                                 *
 *   08/10/1995 JLB : Revamped for free smooth scrolling.                                      *
 *   08/25/1995 JLB : Handles new scrolling option.                                            *
 *=============================================================================================*/
#define	EVA_WIDTH		80
void ScrollClass::AI(KeyNumType &input, int x, int y)
{
	HelpClass::AI(input, x, y);
}


/***********************************************************************************************
 * ScrollClass::Set_Autoscroll -- Turns autoscrolling on or off.                               *
 *                                                                                             *
 *    This routine controls the autoscrolling setting. Autoscroll, when active, will cause the *
 *    map to scroll if the mouse is held over the scroll region. This is regardless of whether *
 *    any mouse button is held down or not.                                                    *
 *                                                                                             *
 * INPUT:   control  -- Should the autoscroll be turned on?                                    *
 *                      0  = turn off                                                          *
 *                      1  = turn on                                                           *
 *                      -1 = toggle current setting                                            *
 *                                                                                             *
 * OUTPUT:  Returns with the old setting of the autoscroll flag.                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/10/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool ScrollClass::Set_Autoscroll(int control)
{
	bool old = IsAutoScroll;

	switch (control) {
		case -1:
			IsAutoScroll = !IsAutoScroll;
			break;

		default:
			IsAutoScroll = control;
			break;
	}
	return(old);
}


