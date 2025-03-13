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

/* $Header: /counterstrike/NULLDLG.CPP 14    3/17/97 1:05a Steve_tall $ */
/***************************************************************************
 **   C O N F I D E N T I A L --- W E S T W O O D    S T U D I O S        **
 ***************************************************************************
 *                                                                         *
 *                 Project Name : Command & Conquer                        *
 *                                                                         *
 *                    File Name : NULLDLG.CPP                              *
 *                                                                         *
 *                   Programmer : Bill R. Randolph                         *
 *                                                                         *
 *                   Start Date : 04/29/95                                 *
 *                                                                         *
 *                  Last Update : Jan. 21, 1997 [V.Grippi]                     *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 *   Build_InitString_Listbox -- [re]builds the initstring entry listbox   *
 *   Build_Phone_Listbox -- [re]builds the phone entry listbox             *
 *   Com_Scenario_Dialog -- Serial game scenario selection dialog				*
 *   Com_Settings_Dialog -- Lets user select serial port settings          *
 *   Destroy_Null_Connection -- destroys the given connection					*
 *   Edit_Phone_Dialog -- lets user edit a phone book entry                *
 *   Init_Null_Modem -- Initializes Null Modem communications              *
 *   Init_String_Compare -- for qsort													*
 *   Phone_Compare -- for qsort															*
 *   Phone_Dialog -- Lets user edit phone directory & dial                 *
 *   Reconnect_Null_Modem -- allows user to reconnect								*
 *   Select_Serial_Dialog -- Serial Communications menu dialog             *
 *   Shutdown_Modem -- Shuts down modem/null-modem communications          *
 *   Test_Null_Modem -- Null-Modem test routine                            *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "function.h"

//PG
GameType Select_Serial_Dialog(void) { return GAME_NORMAL; }

