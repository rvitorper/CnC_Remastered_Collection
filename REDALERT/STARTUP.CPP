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

/* $Header: /counterstrike/STARTUP.CPP 6     3/15/97 7:18p Steve_tall $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : STARTUP.CPP                                                  *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : October 3, 1994                                              *
 *                                                                                             *
 *                  Last Update : September 30, 1996 [JLB]                                     *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Prog_End -- Cleans up library systems in prep for game exit.                              *
 *   main -- Initial startup routine (preps library systems).                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
#include	<conio.h>
#include	<io.h>

#include  "ccdde.h"
#include	"ipx95.h"

#ifdef MCIMPEG // Denzil 6/15/98
#include "mcimovie.h"
#endif



extern char RedAlertINI[_MAX_PATH];

bool Read_Private_Config_Struct(FileClass & file, NewConfigType * config);
void Print_Error_End_Exit(char * string);
void Print_Error_Exit(char * string);

//WinTimerClass * WinTimer;
extern void Create_Main_Window ( HANDLE instance , int command_show , int width , int height);
extern bool RA95AlreadyRunning;
HINSTANCE	ProgramInstance;
void Check_Use_Compressed_Shapes (void);
void Read_Setup_Options(RawFileClass *config_file);
bool VideoBackBufferAllowed = true;

const char* Game_Registry_Key();

#if (ENGLISH)
#define WINDOW_NAME		"Red Alert"
#endif

#if (FRENCH)
#define WINDOW_NAME		"Alerte Rouge"
#endif

#if (GERMAN)
#define WINDOW_NAME		"Alarmstufe Rot"
#endif

// Added. ST - 5/14/2019
bool ProgEndCalled = false;

extern "C"{
	extern char		*BigShapeBufferStart;
	extern char		*TheaterShapeBufferStart;
	extern BOOL		UseBigShapeBuffer;
	extern bool		IsTheaterShape;
}

extern void Free_Heaps(void);
extern void DLL_Shutdown(void);

BOOL WINAPI DllMain(HINSTANCE instance, unsigned int fdwReason, void *lpvReserved)
{
	lpvReserved;

	switch (fdwReason) {

	case DLL_PROCESS_ATTACH:
		ProgramInstance = instance;
		break;

	case DLL_PROCESS_DETACH:
		if (WindowsTimer) {
			delete WindowsTimer;
			WindowsTimer = NULL;
		}
		
		/*
		** Red Alert doesn't clean up memory. Do some of that here.
		*/
		MFCD::Free_All();

		if (BigShapeBufferStart) {
			Free(BigShapeBufferStart);
			BigShapeBufferStart = NULL;
		}

		if (TheaterShapeBufferStart) {
			Free(TheaterShapeBufferStart);
			TheaterShapeBufferStart = NULL;
		}

		if (_ShapeBuffer) {
			delete [] _ShapeBuffer;
			_ShapeBuffer = NULL;
		}

		DLL_Shutdown();
		Free_Heaps();
		break;

	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	  
	return true;
}



/***********************************************************************************************
 * main -- Initial startup routine (preps library systems).                                    *
 *                                                                                             *
 *    This is the routine that is first called when the program starts up. It basically        *
 *    handles the command line parsing and setting up library systems.                         *
 *                                                                                             *
 * INPUT:   argc  -- Number of command line arguments.                                         *
 *                                                                                             *
 *          argv  -- Pointer to array of command line argument strings.                        *
 *                                                                                             *
 * OUTPUT:  Returns with execution failure code (if any).                                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/20/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
//int PASCAL WinMain(HINSTANCE, HINSTANCE, char *, int )
//PG int PASCAL WinMain ( HINSTANCE instance , HINSTANCE , char * command_line , int command_show )
int DLL_Startup(const char * command_line_in)
{
	RunningAsDLL = true;
	int command_show = SW_HIDE;
	HINSTANCE instance = ProgramInstance;
	char command_line[1024];
	strcpy(command_line, command_line_in);



	#ifndef MPEGMOVIE // Denzil 6/10/98
	//PG DDSCAPS	surface_capabilities;
	#endif

	/*
	** First thing to do is check if there is another instance of Red Alert already running
	**  and if so, switch to the existing instance and terminate ourselves.
	*/
	SpawnedFromWChat = false;

//printf("in program.\n");getch();
//printf("ram free = %ld\n",Ram_Free(MEM_NORMAL));getch();
	if (Ram_Free(MEM_NORMAL) < 7000000) {
		printf (TEXT_NO_RAM);
		return(EXIT_FAILURE);
	}


	if (strstr(command_line, "f:\\projects\\c&c0") != NULL ||
			strstr(command_line, "F:\\PROJECTS\\C&C0") != NULL) {
		MessageBox(0, "Playing off of the network is not allowed.", "Red Alert", MB_OK|MB_ICONSTOP);
		return(EXIT_FAILURE);
	}

	int		argc;				//Command line argument count
	unsigned	command_scan;
	char		command_char;
	char *	argv[20];		//Pointers to command line arguments
	char		path_to_exe[132];

	ProgramInstance = instance;

	/*
	** Get the full path to the .EXE
	*/
	GetModuleFileName (instance, &path_to_exe[0], 132);

	/*
	** First argument is supposed to be a pointer to the .EXE that is running
	**
	*/
	argc=1;							//Set argument count to 1
	argv[0]=&path_to_exe[0];	//Set 1st command line argument to point to full path

	/*
	** Get pointers to command line arguments just like if we were in DOS
	**
	** The command line we get is cr/zero? terminated.
	**
	*/

	command_scan=0;

	do {
		/*
		** Scan for non-space character on command line
		*/
		do {
			command_char = *( command_line+command_scan++ );
		} while ( command_char==' ' );

		if ( command_char!=0 && command_char != 13 ) {
			argv[argc++]=command_line+command_scan-1;

			/*
			** Scan for space character on command line
			*/
			bool in_quotes = false;
			do {
				command_char = *( command_line+command_scan++ );
				if (command_char == '"') {
					in_quotes = !in_quotes;
				}
			} while ( (in_quotes || command_char!=' ') && command_char != 0 && command_char!=13 );
		*( command_line+command_scan-1 ) = 0;
		}

	} while ( command_char != 0 && command_char != 13 && argc<20 );


	/*
	**	Remember the current working directory and drive.
	*/

	if (Parse_Command_Line(argc, argv)) {
		WindowsTimer = new WinTimerClass(60, FALSE);

		////////////////////////////////////////
		// The editor needs to load the Red Alert ini file from a different location than the real game. - 7/18/2019 JAS 
		char* red_alert_file_path = nullptr;
		if (RunningFromEditor)
		{
			red_alert_file_path = RedAlertINI;
		}
		else
		{
			red_alert_file_path = CONFIG_FILE_NAME;
		}

		RawFileClass cfile(red_alert_file_path);
		//RawFileClass cfile(CONFIG_FILE_NAME);
		// end of change - 7/18/2019 JAS
		////////////////////////////////////////

#ifdef NEVER
		Keyboard->IsLibrary = true;
		if (Debug_Flag) {
			Keyboard_Attributes_On(DEBUGINT);
		}
#endif

		Keyboard = new KeyboardClass();

		/*
		** If there is loads of memory then use uncompressed shapes
		*/
		Check_Use_Compressed_Shapes();

		/*
		** If there is not enough disk space free, don't allow the product to run.
		*/
		if (Disk_Space_Available() < INIT_FREE_DISK_SPACE) {
		}

		if (cfile.Is_Available()) {

			Read_Private_Config_Struct(cfile, &NewConfig);
			
			/*
			** Set the options as requested by the ccsetup program
			*/
			Read_Setup_Options( &cfile );

			Create_Main_Window( instance , command_show , ScreenWidth , ScreenHeight );
			SoundOn = Audio_Init ( MainWindow , 16 , false , 11025*2 , 0 );

			#ifdef MPEGMOVIE // Denzil 6/10/98
			if (!InitDDraw())
				return (EXIT_FAILURE);
			#else
			BOOL video_success = FALSE;
			/*
			** Set 640x400 video mode. If its not available then try for 640x480
			*/
			if (ScreenHeight == 400) {
				if (Set_Video_Mode (MainWindow, ScreenWidth, ScreenHeight, 8)) {
					video_success = TRUE;
				} else {
					if (Set_Video_Mode (MainWindow, ScreenWidth, 480, 8)) {
						video_success = TRUE;
						ScreenHeight = 480;
					}
				}
			} else {
				if (Set_Video_Mode (MainWindow, ScreenWidth, ScreenHeight, 8)) {
					video_success = TRUE;
				}
			}

			if (!video_success) {
				MessageBox(MainWindow, TEXT_VIDEO_ERROR, TEXT_SHORT_TITLE, MB_ICONEXCLAMATION|MB_OK);
				if (WindowsTimer) delete WindowsTimer;
				//if (Palette) delete Palette;
				return (EXIT_FAILURE);
			}

			if (ScreenWidth==320) {
				VisiblePage.Init( ScreenWidth , ScreenHeight , NULL , 0 , (GBC_Enum)0);
				ModeXBuff.Init( ScreenWidth , ScreenHeight , NULL , 0 , (GBC_Enum)(GBC_VISIBLE | GBC_VIDEOMEM));
			} else {
				VisiblePage.Init(ScreenWidth, ScreenHeight, NULL, 0, (GBC_Enum)0);
				HiddenPage.Init(ScreenWidth, ScreenHeight, NULL, 0, (GBC_Enum)0);
			}

			ScreenHeight = 3072;

			if (VisiblePage.Get_Height() == 480) {
				SeenBuff.Attach(&VisiblePage, 0, 40, 3072, 3072);
				HidPage.Attach(&HiddenPage, 0, 40, 3072, 3072);
			} else {
				SeenBuff.Attach(&VisiblePage, 0, 0, 3072, 3072);
				HidPage.Attach(&HiddenPage, 0, 0, 3072, 3072);
			}
			#endif // MPEGMOVIE - Denzil 6/10/98
			
			Options.Adjust_Variables_For_Resolution();

			/*
			** Install the memory error handler
			*/
			Memory_Error = &Memory_Error_Handler;

			WindowList[0][WINDOWWIDTH] 	= SeenBuff.Get_Width();
			WindowList[0][WINDOWHEIGHT]	= SeenBuff.Get_Height();
			WindowList[WINDOW_EDITOR][WINDOWWIDTH] = SeenBuff.Get_Width();
			WindowList[WINDOW_EDITOR][WINDOWHEIGHT]= SeenBuff.Get_Height();

			////////////////////////////////////////
			// The editor needs to not start the mouse up. - 7/22/2019 JAS 
			if (!RunningFromEditor)
			{
				WWMouse = new WWMouseClass(&SeenBuff, 48, 48);
				MouseInstalled = TRUE;
			}

			//PG CDFileClass::Set_CD_Drive (CDList.Get_First_CD_Drive());

			/*
			** See if we should run the intro
			*/
			INIClass ini;
			ini.Load(cfile);

			/*
			**	Check for forced intro movie run disabling. If the conquer
			**	configuration file says "no", then don't run the intro.
			** Don't do this for TEN & MPath.
			*/
			if (!Special.IsFromInstall) {
				Special.IsFromInstall = ini.Get_Bool("Intro", "PlayIntro", true);
			}
			SlowPalette = ini.Get_Bool("Options", "SlowPalette", false);

			/*
			** Regardless of whether we should run it or not, here we're
			** gonna change it to say "no" in the future.
			*/
			if (Special.IsFromInstall) {
				BreakoutAllowed = true;
//				BreakoutAllowed = false;
				ini.Put_Bool("Intro", "PlayIntro", false);
				ini.Save(cfile);
			}

			/*
			** If WChat has been trying to send us a game start packet then receive it and
			** flag that we were spawned from WCHat so we dont play the into movie.
			*/
			if (DDEServer.Get_MPlayer_Game_Info()){
				Check_From_WChat(NULL);
			}else{
				//Check_From_WChat("C&CSPAWN.INI");
				//if (Special.IsFromWChat){
				//	DDEServer.Disable();
				//}
			}
			/*
			**	If the intro is being run for the first time, then don't
			**	allow breaking out of it with the <ESC> key.
			*/
			if (Special.IsFromInstall) {
				BreakoutAllowed = true;
//				BreakoutAllowed = false;
			}

			Memory_Error_Exit = Print_Error_End_Exit;

			Main_Game(argc, argv);

			if (RunningAsDLL) {	//PG
				return (EXIT_SUCCESS);
			}

			#ifdef MPEGMOVIE // Denzil 6/15/98
			if (MpgSettings != NULL)
				delete MpgSettings;
	
			#ifdef MCIMPEG
			if (MciMovie != NULL)
				delete MciMovie;
			#endif
			#endif
			
			VisiblePage.Clear();
			HiddenPage.Clear();
			Memory_Error_Exit = Print_Error_Exit;

			/*
			** Flag that this is a clean shutdown (not killed with Ctrl-Alt-Del)
			*/
			ReadyToQuit = 1;

			/*
			** Post a message to our message handler to tell it to clean up.
			*/
			PostMessage(MainWindow, WM_DESTROY, 0, 0);

			/*
			** Wait until the message handler has dealt with the message
			*/
			do
			{
				Keyboard->Check();
			}while (ReadyToQuit == 1);

			return (EXIT_SUCCESS);
		} else {
			if (!RunningFromEditor)
			{
				puts(TEXT_SETUP_FIRST);
				Keyboard->Get();
			}
		}

		if (WindowsTimer) {
		 	delete WindowsTimer;
		 	WindowsTimer = NULL;
		}
	}
	/*
	**	Restore the current drive and directory.
	*/
	return(EXIT_SUCCESS);
}


/* Initialize DirectDraw and surfaces */
bool InitDDraw(void)
	{

	VisiblePage.Init(ScreenWidth, ScreenHeight, NULL, 0, (GBC_Enum)(GBC_VISIBLE | GBC_VIDEOMEM));
	HiddenPage.Init(ScreenWidth, ScreenHeight, NULL, 0, (GBC_Enum)GBC_VIDEOMEM);
	VisiblePage.Attach_DD_Surface(&HiddenPage);
	SeenBuff.Attach(&VisiblePage, 0, 0, 3072, 3072);
	HidPage.Attach(&HiddenPage, 0, 0, 3072, 3072);
	return true;
	}


/***********************************************************************************************
 * Prog_End -- Cleans up library systems in prep for game exit.                                *
 *                                                                                             *
 *    This routine should be called before the game terminates. It handles cleaning up         *
 *    library systems so that a graceful return to the host operating system is achieved.      *
 *                                                                                             *
 * INPUT:   why - text description of exit reason                                              *
 *          fatal - indicates a fatal error                                                    *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/20/1995 JLB : Created.                                                                 *
 *   08/07/2019  ST : Added why and fatal params                                               *
 *=============================================================================================*/
void __cdecl Prog_End(const char *why, bool fatal)
{
	GlyphX_Debug_Print("Prog_End()");
	
	if (why) {
		GlyphX_Debug_Print(why);
	}
	if (fatal) {
		*((int*)0) = 0;
	}

	Sound_End();
	if (WWMouse) {
		delete WWMouse;
		WWMouse = NULL;
	}
	if (WindowsTimer) {
		 delete WindowsTimer;
		 WindowsTimer = NULL;
	}

	ProgEndCalled = true;
}


void Print_Error_End_Exit(char * string)
{
	Prog_End(string, true);
	printf( "%s\n", string );
	if (!RunningAsDLL) {
		exit(1);
	}
}


void Print_Error_Exit(char * string)
{
	GlyphX_Debug_Print(string);
	
	printf( "%s\n", string );
	if (!RunningAsDLL) {
		exit(1);
	}
}







/***********************************************************************************************
 * Emergency_Exit -- Function to call when we want to exit unexpectedly.                       *
 *                   Use this function instead of exit(n) so everything is properly cleaned up.*
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Code to return to the OS                                                          *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    3/13/97 1:32AM ST : Created                                                              *
 *=============================================================================================*/
void Emergency_Exit ( int code )
{
	if (RunningAsDLL) {	//PG
		return;
	}
	/*
	** Clear out the video buffers so we dont glitch when we lose focus
	*/
	VisiblePage.Clear();
	HiddenPage.Clear();
	BlackPalette.Set();
	Memory_Error_Exit = Print_Error_Exit;

	/*
	** Flag that this is an emergency shut down - not a clean shutdown but
	** not killed with Ctrl-Alt-Del either.
	*/
	ReadyToQuit = 3;

	/*
	** Post a message to our message handler to tell it to clean up.
	*/
	PostMessage(MainWindow, WM_DESTROY, 0, 0);

	/*
	** Wait until the message handler has dealt with the message
	*/
	do
	{
		Keyboard->Check();
	}while (ReadyToQuit == 3);

	exit ( code );
}


/***********************************************************************************************
 * Read_Setup_Options -- Read stuff in from the INI file that we need to know sooner           *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Ptr to config file class                                                          *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    6/7/96 4:09PM ST : Created                                                               *
 *   09/30/1996 JLB : Uses INI class.                                                          *
 *=============================================================================================*/
void Read_Setup_Options( RawFileClass *config_file )
{
	if (config_file->Is_Available()) {

		INIClass ini;

		ini.Load(*config_file);

		/*
		** Read in the boolean options
		*/
		VideoBackBufferAllowed = ini.Get_Bool("Options", "VideoBackBuffer", true);
		AllowHardwareBlitFills = ini.Get_Bool("Options", "HardwareFills", true);
		ScreenHeight = ini.Get_Bool("Options", "Resolution", false) ? 3072 : 3072;

		/*
		** See if an alternative socket number has been specified
		*/
		int socket = ini.Get_Int("Options", "Socket", 0);
		if (socket >0 ) {
			socket += 0x4000;
			if (socket >= 0x4000 && socket < 0x8000) {
				Ipx.Set_Socket (socket);
			}
		}

		/*
		** See if a destination network has been specified
		*/
		char netbuf [512];
		memset(netbuf, 0, sizeof(netbuf));
		char * netptr = netbuf;
		bool found = ini.Get_String("Options", "DestNet", NULL, netbuf, sizeof(netbuf));

		if (found && netptr != NULL && strlen(netbuf)) {
			NetNumType net;
			NetNodeType node;

			/*
			** Scan the string, pulling off each address piece
			*/
			int i = 0;
			char * p = strtok(netbuf,".");
			int x;
			while (p != NULL) {
				sscanf(p,"%x",&x);			// convert from hex string to int
				if (i < 4) {
					net[i] = (char)x;			// fill NetNum
				} else {
					node[i-4] = (char)x;		// fill NetNode
				}
				i++;
				p = strtok(NULL,".");
			}

			/*
			** If all the address components were successfully read, fill in the
			** BridgeNet with a broadcast address to the network across the bridge.
			*/
			if (i >= 4) {
				Session.IsBridge = 1;
				memset(node, 0xff, 6);
				Session.BridgeNet = IPXAddressClass(net, node);
			}
		}
	}
}

void Get_OS_Version (void)
{
	//PG
	//WindowsNT = ((GetVersion() & 0x80000000) == 0) ? true : false;

}


