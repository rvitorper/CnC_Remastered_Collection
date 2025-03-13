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

/* $Header: /counterstrike/INTERNET.CPP 6     3/17/97 1:05a Steve_tall $ */
/*************************************************************************************
 **   C O N F I D E N T I A L --- W E S T W O O D    S T U D I O S                  **
 *************************************************************************************
 *                                                                                   *
 *                 Project Name : Command & Conquer - Red Alert                      *
 *                                                                                   *
 *                    File Name : INTERNET.CPP                                       *
 *                                                                                   *
 *                   Programmer : Steve Tall                                         *
 *                                                                                   *
 *                   Start Date : March 11th, 1996                                   *
 *                                                                                   *
 *                  Last Update : August 5th, 1996 [ST]                              *
 *                                                                                   *
 *-----------------------------------------------------------------------------------*
 * Overview:                                                                         *
 *                                                                                   *
 *  Miscellaneous junk related to H2H internet connection.                           *
 *                                                                                   *
 *-----------------------------------------------------------------------------------*
 * Functions:                                                             	 			*
 *  Check_From_WChat -- Interprets start game packet from WChat           	 			*
 *  Read_Game_Options -- Read the game setup options from the wchat packet	 			*
 *  Is_User_WChat_Registered -- retrieve the users wchat entry from registry 			*
 *  Spawn_WChat -- spawns or switches focus to wchat                         			*
 *  Spawn_Registration_App -- spawns the C&C/Planet westwood registration app			*
 *  Do_The_Internet_Menu_Thang -- Handle case where user clicks on 'Internet' button *
 *                                                                         				*
 *                                                                         				*
 *                                                                         				*
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#ifdef WIN32

#include "function.h"
#include "tcpip.h"
#include "ccdde.h"


extern bool SpawnedFromWChat;

#ifndef WOLAPI_INTEGRATION
int Read_Game_Options(void);
#endif

#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
extern bool Is_Mission_126x126 (char *file_name);
extern bool Is_Mission_Aftermath (char *file_name);
extern bool Is_Mission_Counterstrike (char *file_name);
#endif

/***************************************************************************
** Internet specific globals
*/
char PlanetWestwoodHandle[] = {"Handle"};				//Planet WW user name
char PlanetWestwoodPassword[] = {"Password"};			//Planet WW password
char PlanetWestwoodIPAddress[IP_ADDRESS_MAX] = {"206.154.108.87"};	//IP of server or other player
long PlanetWestwoodPortNumber = 1234;					//Port number to send to
bool PlanetWestwoodIsHost = false;						//Flag true if player has control of game options
unsigned long PlanetWestwoodGameID;						//Game ID
unsigned long PlanetWestwoodStartTime;					//Time that game was started
HWND	WChatHWND = 0;											//Handle to Wchat window.
bool	GameStatisticsPacketSent;							//Flag that game stats have been sent to wchat
bool	ConnectionLost;										//Flag that the connection to the other player was lost
int		WChatMaxAhead;
int		WChatSendRate;
bool	SpawnedFromWChat;
int		ShowCommand;

#ifdef FRENCH
#define TXT_HACKHACK  "Connexion En Cours..."
#endif
#if defined(ENGLISH) || defined(GERMAN)
#define TXT_HACKHACK  Text_String(TXT_CONNECTING)
#endif

/***********************************************************************************************
 * Check_From_WChat -- This function reads in C&CSPAWN.INI and interprets it.                  *
 *                     C&CSPAWN.INI is now sent to us by WCHAT via DDE                         *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Name of C&CSPAWN.INI file. If NULL then get file from DDE Server.                 *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    6/9/96 1:44PM ST : Created                                                               *
 *=============================================================================================*/
#ifndef WOLAPI_INTEGRATION
void Check_From_WChat(char *wchat_name)
{

	char	default_string[] = {"Error"};
	char	key_string[256];
	char *ini_file;
	RawFileClass wchat_file;

	/*
	** Get a pointer to C&CSPAWN.INI either by reading it from disk or getting it from
	** the DDE server.
	*/
	if (wchat_name){
		ini_file = new char [8192];
	}else{
		ini_file = DDEServer.Get_MPlayer_Game_Info();

#ifdef NEVER
		/*
		** Save it to disk as well so I can see it
		*/
		RawFileClass anotherfile ("FROMCHAT.TXT");
		anotherfile.Write(ini_file, DDEServer.Get_MPlayer_Game_Info_Length());
#endif

	}

	if (wchat_name){
		wchat_file.Set_Name(wchat_name);
	}

	if (!wchat_name || wchat_file.Is_Available()){

		/*
		** Read the ini file from disk if we founf it there
		*/
		if (wchat_name){
			wchat_file.Read(ini_file, wchat_file.Size());
		}

		/*
		** Get the IP address
		*/
		key_string[0] = 0;

		WWGetPrivateProfileString("Internet",
										"Address",
										default_string,
										key_string,
										sizeof(key_string),
										ini_file);


		if (!strcmp (key_string, default_string)) {
			if (wchat_name) delete ini_file;
			return;
		}
		strcpy (PlanetWestwoodIPAddress, key_string);



		/*
		** Get the port number
		*/
		key_string[0] = 0;

		WWGetPrivateProfileString("Internet",
										"Port",
										default_string,
										key_string,
										sizeof(key_string),
										ini_file);


		if (!strcmp (key_string, default_string)) {
			if (wchat_name) delete ini_file;
			return;
		}

		PlanetWestwoodPortNumber = atol(key_string);


		/*
		** Get host or client
		*/
		key_string[0] = 0;

		WWGetPrivateProfileString("Internet",
										"Host",
										default_string,
										key_string,
										sizeof(key_string),
										ini_file);


		if (!strcmp (key_string, default_string)) {
			if (wchat_name) delete ini_file;
			return;
		}

		if (strchr (key_string, '1')){
			PlanetWestwoodIsHost = true;
		}else{
			PlanetWestwoodIsHost = false;
		}


		Special.IsFromWChat = true;
	}

	if (wchat_name) delete ini_file;

}
#endif	//	!WOLAPI_INTEGRATION


/***************************************************************************
 * Read_Game_Options -- reads multiplayer game options from disk           *
 *                                                                         *
 * This routine is used for multiplayer games which read the game options	*
 * from disk, rather than through a connection dialog.							*
 *                                                                         *
 * INPUT:                                                                  *
 *		name of C&CSPAWN.INI file. Null if data should be got from DDE server*                                                                         *
 * OUTPUT:                                                                 *
 *		1 = OK, 0 = error																		*
 *                                                                         *
 * WARNINGS:                          \                                     *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   01/11/1996 BRR : Created.                                             *
 *=========================================================================*/
#ifndef WOLAPI_INTEGRATION
int Read_Game_Options(char *name)
{
	char *buffer;

	char filename[256] = {"INVALID.123"};

	if (name){
		strcpy (filename, name);
	}

	/*------------------------------------------------------------------------
	Create filename and read the file.
	------------------------------------------------------------------------*/
	CCFileClass file (filename);

	if (name && !file.Is_Available()) {
		return(0);
	} else {
		if (name){
			buffer = new char [8192];							// INI staging buffer pointer.
			memset(buffer, '\0', 8192);
			file.Read(buffer, 8192-1);
			file.Close();
		}else{
			buffer = DDEServer.Get_MPlayer_Game_Info();
		}
	}

	/*------------------------------------------------------------------------
	Get the player's name
	------------------------------------------------------------------------*/
	WWGetPrivateProfileString("Options", "Handle", "Noname", Session.Handle,
		sizeof(Session.Handle), buffer);
	strcpy(Session.GameName, Session.Handle);
	Session.ColorIdx = (PlayerColorType) WWGetPrivateProfileInt("Options", "Color", 0, buffer);
	Session.PrefColor = Session.ColorIdx;
	int temp = WWGetPrivateProfileInt("Options", "Side", 0, buffer);
	Session.House = (HousesType) ((int)HOUSE_USSR + temp);

	Session.Options.Credits = WWGetPrivateProfileInt("Options", "Credits", 0, buffer);
	Session.Options.Bases = WWGetPrivateProfileInt("Options", "Bases", 0, buffer);
	Session.Options.Tiberium = WWGetPrivateProfileInt("Options", "Tiberium", 0, buffer);
	Session.Options.Goodies = WWGetPrivateProfileInt("Options", "Crates", 0, buffer);
	Special.IsShadowGrow = WWGetPrivateProfileInt ("Options", "Shadow", 0, buffer);
	BuildLevel = WWGetPrivateProfileInt("Options", "BuildLevel", 0, buffer);
	Session.Options.UnitCount = WWGetPrivateProfileInt("Options", "UnitCount", 0, buffer);
	Seed = WWGetPrivateProfileInt("Options", "Seed", 0, buffer);
	Special.IsCaptureTheFlag = WWGetPrivateProfileInt("Options", "CapFlag", 0, buffer);
	UnitBuildPenalty = WWGetPrivateProfileInt ("Options", "BuildRate", 100, buffer);

	PlanetWestwoodGameID = WWGetPrivateProfileInt("Internet", "GameID", 0, buffer);
	PlanetWestwoodStartTime = WWGetPrivateProfileInt ("Internet", "StartTime", 0, buffer);
	WChatHWND = (HWND) WWGetPrivateProfileInt("Internet", "HWND", (int)FindWindow("OWL_Window", "Westwood Chat"), buffer);

	Session.Options.AIPlayers = WWGetPrivateProfileInt("Options", "AI", 0, buffer);		//Number of AI players
	if (Session.Options.AIPlayers){
		Session.Options.Ghosts = 1;
	}

	if (Session.Options.Tiberium) {
		Special.IsTGrowth = 1;
		Special.IsTSpread = 1;
	} else {
		Special.IsTGrowth = 0;
		Special.IsTSpread = 0;
	}

	/*
	** Read the scenario name from the .INI and try to match it with a scenario file in our list.
	*/
	WWGetPrivateProfileString("Options", "Scenario", "SCM01EA.INI",
										Session.Options.ScenarioDescription,
										sizeof (Session.Options.ScenarioDescription),
										buffer);
	//WWDebugString ("RA95I - Scenario is ");
	//WWDebugString (Session.Options.ScenarioDescription);
	//WWDebugString ("\n");

	Session.Options.ScenarioIndex = -1;
	for (int i = 0; i < Session.Scenarios.Count(); i++) {
		if (!strcmp (Session.Scenarios[i]->Description(), Session.Options.ScenarioDescription) ){
			Session.Options.ScenarioIndex = i;
			break;
		}
	}

	if (PlanetWestwoodIsHost){
		/*
		** Special new kludge for counterstrike.
		**
		** The only time the file can be unavailable is if its a counterstrike
		** mission and the CS CD is not in the drive so
		** make sure the counterstrike CD is in the drive.
		**
		** If Counterstrike is installed then force the CD
		** to be there.
		**
		*/
		if (Session.Options.ScenarioIndex == -1) {
			//WWDebugString ("RA95I - Session.Options.ScenarioIndex == -1\n");
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
			if( (Expansion_CS_Present() && Is_Mission_Counterstrike(Session.ScenarioFileName)) ||
			    (Expansion_AM_Present() && Is_Mission_Aftermath(Session.ScenarioFileName)) ) {
#else
			if ( Expansion_CS_Present() ) {
#endif

				int current_drive = CCFileClass::Get_CD_Drive();
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
				int index=Get_CD_Index(current_drive, 1*60);
				bool needcd = false;
				if (Is_Mission_Counterstrike(Session.ScenarioFileName)) {
					if (index != 2 && index != 3) {
						RequiredCD = 2;
						needcd = true;
					}
				}
				if (Is_Mission_Aftermath(Session.ScenarioFileName)) {
					if (index != 3) {
						RequiredCD = 3;
						needcd = true;
					}
				}
				if (needcd) {
#else
				if ( Get_CD_Index(current_drive, 1*60) != 2 ){
					RequiredCD = 2;
#endif
					//WWDebugString ("RA95I - CounterStrike CD not in drive\n");

					if (!Force_CD_Available (RequiredCD)){
						if (!RunningAsDLL) {	//PG
							Emergency_Exit(EXIT_FAILURE);
						}
					}
					//WWDebugString ("RA95I - Returned from Force_CD_Available()\n");

					/*
					** Update the internal list of scenarios to include the counterstrike
					** list.
					*/
					Session.Read_Scenario_Descriptions();

				}
			}

			/*
			** See if that scenario is available now. Its fatal if it isnt.
			*/
			Session.Options.ScenarioIndex = -1;
			for (int i = 0; i < Session.Scenarios.Count(); i++) {
				if (!strcmp (Session.Scenarios[i]->Description(), Session.Options.ScenarioDescription) ){
					Session.Options.ScenarioIndex = i;
					break;
				}
			}
			//if (Session.Options.ScenarioIndex == -1)
			//	WWDebugString ("RA95I - Session.Options.ScenarioIndex is still -1\n");

		}
	}


	Options.GameSpeed = 0;


	//MPlayerLocalID = Build_MPlayerID (MPlayerColorIdx, MPlayerHouse);

	Session.MaxAhead = WChatMaxAhead = WWGetPrivateProfileInt("Timing", "MaxAhead", 9, buffer);
	Session.FrameSendRate = WChatSendRate = WWGetPrivateProfileInt("Timing", "SendRate", 3, buffer);

	if (name) delete buffer;
	return (1);

}
#endif	//	!WOLAPI_INTEGRATION




/***********************************************************************************************
 * Get_Registry_Sub_Key -- search a registry key for a sub-key                                 *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    handle of key to search                                                           *
 *           text to search for                                                                *
 *           true if old key should be closed when new key opened                              *
 *                                                                                             *
 * OUTPUT:   handle to the key we found or 0                                                   *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    1/12/96 2:11PM ST : Created                                                              *
 *=============================================================================================*/

extern HKEY Get_Registry_Sub_Key (HKEY base_key, char *search_key, BOOL close);



void Just_Path(char *path, char *destpath)
{
	char *terminator = NULL;		//He'll be back.

	strcpy  (destpath, path);
	terminator = strrchr (destpath, '\\');
	if (terminator){
		*terminator = 0;
	}
}





/***********************************************************************************************
 * Is_User_WChat_Registered -- retrieve the users wchat entry from the registry                *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:  Nothing                                                                             *
 *                                                                                             *
 * OUTPUT:   TRUE if users wchat entry was found in the registry                               *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    1/12/96 2:13PM ST : Created                                                              *
 *=============================================================================================*/
bool Is_User_WChat_Registered(char *buffer, int buffer_len)
{
	// ST - 5/13/2019
	buffer; buffer_len;
	return false;
	
}



/***********************************************************************************************
 * Spawn_WChat -- spawns or switches focus to wchat                                            *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    can launch. If set then we are allowed to launch WChat if not already running     *
 *                                                                                             *
 * OUTPUT:   True if wchat was spawned                                                         *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    6/8/96 12:33PM ST : Created                                                              *
 *=============================================================================================*/
#ifndef WOLAPI_INTEGRATION
bool Poke_WChat(void);
bool Spawn_WChat(bool can_launch)
{
	// ST - 5/13/2019
	can_launch;
	return false;
	
}

#endif	//#ifndef WOLAPI_INTEGRATION

/***********************************************************************************************
 * Spawn_Registration_App -- spawns the C&C/Planet westwood registration app                   *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   True if app was spawned                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    6/8/96 12:33PM ST : Created                                                              *
 *=============================================================================================*/
#ifndef WOLAPI_INTEGRATION
bool Spawn_Registration_App(void)
{
	// ST - 5/13/2019
	return false;
}

#endif	//#ifndef WOLAPI_INTEGRATION


/***********************************************************************************************
 * Do_The_Internet_Menu_Thang -- Handle case where user clicks on 'Internet' button            *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    6/7/96 8:30PM ST : Created                                                               *
 *=============================================================================================*/
#ifndef WOLAPI_INTEGRATION
bool Do_The_Internet_Menu_Thang(void)
{
	return (false);
}

#endif	//#ifndef WOLAPI_INTEGRATION



#endif	//WIN32
