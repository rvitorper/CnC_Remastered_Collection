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

/* $Header:   F:\projects\c&c0\vcs\code\comqueue.cpv   4.1   11 Apr 1996 18:28:16   JOE_BOSTIC  $ */
/***************************************************************************
 **   C O N F I D E N T I A L --- W E S T W O O D    S T U D I O S        **
 ***************************************************************************
 *                                                                         *
 *                 Project Name : Command & Conquer                        *
 *                                                                         *
 *                    File Name : COMQUEUE.CPP                             *
 *                                                                         *
 *                   Programmer : Bill Randolph                            *
 *                                                                         *
 *                   Start Date : December 19, 1994                        *
 *                                                                         *
 *                  Last Update : May 31, 1995 [BRR]                       *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 *   CommQueueClass::Add_Delay -- adds a new delay value for response time *
 *   CommQueueClass::Avg_Response_Time -- returns average response time    *
 *   CommQueueClass::CommQueueClass -- class constructor                   *
 *   CommQueueClass::Configure_Debug -- sets up special debug values       * 
 *   CommQueueClass::Get_Receive -- gets ptr to queue entry                *
 *   CommQueueClass::Get_Send -- gets ptr to queue entry                   *
 *   CommQueueClass::Init -- initializes this queue                        *
 *   CommQueueClass::Max_Response_Time -- returns max response time        *
 *   CommQueueClass::Mono_Debug_Print -- Debug output routine              * 
 *   CommQueueClass::Mono_Debug_Print2 -- Debug output; alternate format   * 
 *   CommQueueClass::Next_Receive -- gets ptr to next entry in send queue  *
 *   CommQueueClass::Next_Send -- gets ptr to next entry in send queue     *
 *   CommQueueClass::Queue_Receive -- queues a received message            *
 *   CommQueueClass::Queue_Send -- queues a message for sending            *
 *   CommQueueClass::Reset_Response_Time -- resets computations            *
 *   CommQueueClass::UnQueue_Receive -- removes next entry from send queue *
 *   CommQueueClass::UnQueue_Send -- removes next entry from send queue    *
 *   CommQueueClass::~CommQueueClass -- class destructor                   *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

