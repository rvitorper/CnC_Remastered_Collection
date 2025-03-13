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

/* $Header: /CounterStrike/NETDLG.CPP 13    10/13/97 2:20p Steve_t $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                         						  *
 *                 Project Name : Command & Conquer                        						  *
 *                                                                         						  *
 *                    File Name : NETDLG.CPP                               						  *
 *                                                                         						  *
 *                   Programmer : Bill Randolph                            						  *
 *                                                                         						  *
 *                   Start Date : January 23, 1995                         						  *
 *                                                                         						  *
 *                  Last Update : December 12, 1995 [BRR]                  						  *
 *                                                                         						  *
 *---------------------------------------------------------------------------------------------*
 *                                                                         						  *
 * These routines establish & maintain peer-to-peer connections between this system				  *
 * and all others in the game.  Each system finds out the IPX address of the others,			  *
 * and forms a direct connection (IPXConnectionClass) to that system.  Systems are				  *
 * found out via broadcast queries.  Every system broadcasts its queries, and every				  *
 * system replies to queries it receives.  At the point when the game owner signals				  *
 * 'OK', every system must know about all the other systems in the game.							  *
 *                                                                         						  *
 * How Bridges are handled:																						  *
 * Currently, bridges are handled by specifying the destination IPX address of the				  *
 * "server" (game owner's system) on the command-line.  This address is used to					  *
 * derive a broadcast address to that destination network, and this system's queries			  *
 * are broadcast over its network & the server's network; replies to the queries come			  *
 * with each system's IPX address attached, so once we have the address, we can form			  *
 * a connection with any system on the bridged net.														  *
 *                                                                         						  *
 * The flaw in this plan is that we can only cross one bridge.  If there are 3 nets				  *
 * bridged (A, B, & C), and the server is on net B, and we're on net A, our broadcasts			  *
 * will reach nets A & B, but not C.  The way to circumvent this (if it becomes a problem)	  *
 * would be to have the server tell us what other systems are in its game, not each				  *
 * individual player's system.  Thus, each system would find out about all the other systems	  *
 * by interacting with the game's owner system (this would be more involved than what			  *
 * I'm doing here).																									  *
 *                                                                         						  *
 * Here's a list of all the different packets sent over the Global Channel:						  *
 *																															  *
 *	NET_QUERY_GAME																										  *
 *							(no other data)																			  *
 *	NET_ANSWER_GAME																									  *
 *							Name:					game owner's name													  *
 *							GameInfo:			game's version & open state									  *
 *	NET_QUERY_PLAYER																									  *
 *							Name:					name of game we want players to respond for				  *
 *	NET_ANSWER_PLAYER																									  *
 *							Name:					player's name														  *
 *							PlayerInfo:			info about player													  *
 * NET_CHAT_ANNOUNCE																									  *
 *							Chat:					unique id of the local node, so I can tell				  *
 *													if this chat announcement is from myself					  *
 *	NET_QUERY_JOIN																										  *
 *							Name:					name of player wanting to join								  *
 *							PlayerInfo:			player's requested house, color, & version range		  *
 *	NET_CONFIRM_JOIN																									  *
 *							PlayerInfo:			approves player's house & color								  *
 *	NET_REJECT_JOIN																									  *
 *							Reject.Why:			tells why we got rejected										  *
 *	NET_GAME_OPTIONS																									  *
 *							ScenarioInfo:		info about scenario												  *
 *	NET_SIGN_OFF																										  *
 *							Name:					name of player signing off										  *
 *	NET_PING																												  *
 *							(no other data)																			  *
 *	NET_GO																												  *
 *							Delay:            value of one-way response time, in frames               *
 * 																														  *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                              						  *
 *   Init_Network -- initializes network stuff                             						  *
 *   Shutdown_Network -- shuts down network stuff                          						  *
 *   Process_Global_Packet -- responds to remote queries                   						  *
 *   Destroy_Connection -- destroys the given connection													  *
 *   Remote_Connect -- handles connecting this user to others              						  *
 *   Net_Join_Dialog -- lets user join an existing game, or start a new one						  *
 *   Request_To_Join -- Sends a JOIN request packet to game owner           	 					  *
 *   Unjoin_Game -- Cancels joining a game                                                     *
 *   Send_Join_Queries -- sends queries for the Join Dialog												  *
 *   Get_Join_Responses -- sends queries for the Join Dialog											  *
 *   Net_New_Dialog -- lets user start a new game															  *
 *   Get_NewGame_Responses -- processes packets for New Game dialog                            *
 *   Compute_Name_CRC -- computes CRC from char string                                         *
 *   Net_Reconnect_Dialog -- Draws/updates the network reconnect dialog                        *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
//	Warning - Most disgusting cpp file of all time. ajw

#include "function.h"

//PG Stubs
void Destroy_Connection(int, int) {}
bool Process_Global_Packet(GlobalPacketType *, IPXAddressClass *) { return false; }
bool Client_Remote_Connect(void) { return false; }
bool Server_Remote_Connect(void) { return false; }
bool Init_Network(void) { return false; }
bool Remote_Connect(void) { return false; }
