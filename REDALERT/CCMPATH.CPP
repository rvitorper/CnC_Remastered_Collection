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

/***************************************************************************
 *                                                                         *
 *                 Project Name : Command & Conquer                        *
 *                                                                         *
 *                    File Name : CCMPATH.CPP                              *
 *                                                                         *
 *                   Programmer : Bill R. Randolph                         *
 *                                                                         *
 *                   Start Date : 01/09/96                                 *
 *                                                                         *
 *                  Last Update : January 11, 1996 [BRR]                   *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 *   Init_MPATH -- Performs MPATH-specific initialization                  *
 *   Shutdown_MPATH -- Shuts down MPATH connections                        *
 *   Connect_MPATH -- Waits for connections to other players               *
 *   Destroy_MPATH_Connection -- Destroys the given connection             *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include "function.h"


/***************************************************************************
 * Init_MPATH -- Performs MPATH-specific initialization                    *
 *                                                                         *
 * INPUT:                                                                  *
 *		none.																						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		1 = OK, 0 = error																		*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   01/09/1996 BRR : Created.                                             *
 *=========================================================================*/
int Init_MPATH(void)
{

	return (1);
}	// end of Init_MPATH


/***************************************************************************
 * Shutdown_MPATH -- Shuts down MPATH connections                          *
 *                                                                         *
 * INPUT:                                                                  *
 *		none.																						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		none.																						*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   01/09/1996 BRR : Created.                                             *
 *=========================================================================*/
void Shutdown_MPATH(void)
{
}	// end of Shutdown_MPATH


/***************************************************************************
 * Connect_MPATH -- Waits for connections to other players                 *
 *                                                                         *
 * INPUT:                                                                  *
 *		none.																						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		none.																						*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   01/10/1996 BRR : Created.                                             *
 *=========================================================================*/
void Connect_MPATH(void)
{
	return;
}


/***************************************************************************
 * Destroy_MPATH_Connection -- Destroys the given connection               *
 *                                                                         *
 * INPUT:                                                                  *
 *		id			connection ID to destroy												*
 *		error		0 = user signed off; 1 = connection error; otherwise, 		*
 *					no error is shown.		  												*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		none.																						*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   01/11/1996 BRR : Created.                                             *
 *=========================================================================*/
void Destroy_MPATH_Connection(int id, int error)
{
	id = id;
	error = error;
}	// end of Destroy_MPATH_Connection


/***************************** end of ccmpath.cpp **************************/
