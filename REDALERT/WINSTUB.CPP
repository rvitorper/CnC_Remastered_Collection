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

/* $Header: /CounterStrike/WINSTUB.CPP 3     3/13/97 2:06p Steve_tall $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : WINSTUB.CPP                                                  *
 *                                                                                             *
 *                   Programmer : Steve Tall                                                   *
 *                                                                                             *
 *                   Start Date : 10/04/95                                                     *
 *                                                                                             *
 *                  Last Update : October 4th 1995 [ST]                                        *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Overview:                                                                                   *
 *   This file contains stubs for undefined externals when linked under Watcom for Win 95      *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 *                                                                                             *
 * Functions:                                                                                  *
 *   Assert_Failure -- display the line and source file where a failed assert occurred         *
 *   Check_For_Focus_Loss -- check for the end of the focus loss                               *
 *   Create_Main_Window -- opens the MainWindow for C&C                                        *
 *   Focus_Loss -- this function is called when a library function detects focus loss          *
 *   Memory_Error_Handler -- Handle a possibly fatal failure to allocate memory                *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"

#include "tcpip.h"
#include "ipx95.h"

void output(short,short)
{}


unsigned long CCFocusMessage = WM_USER+50;	//Private message for receiving application focus
extern	void VQA_PauseAudio(void);
extern	void VQA_ResumeAudio(void);

//#include "WolDebug.h"


/***********************************************************************************************
 * Focus_Loss -- this function is called when a library function detects focus loss            *
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
 *    2/1/96 2:10PM ST : Created                                                               *
 *=============================================================================================*/

void Focus_Loss(void)
{
	Theme.Suspend();
	Stop_Primary_Sound_Buffer();
	if (WWMouse) WWMouse->Clear_Cursor_Clip();
}

void Focus_Restore(void)
{
	Restore_Cached_Icons();
	Map.Flag_To_Redraw(true);
	Start_Primary_Sound_Buffer(TRUE);
	if (WWMouse) WWMouse->Set_Cursor_Clip();
	VisiblePage.Clear();
	HiddenPage.Clear();
}



/***********************************************************************************************
 * Check_For_Focus_Loss -- check for the end of the focus loss                                 *
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
 *    2/2/96 10:49AM ST : Created                                                              *
 *=============================================================================================*/

void Check_For_Focus_Loss(void)
{

}



extern bool InMovie;





HANDLE DebugFile = INVALID_HANDLE_VALUE;

/***********************************************************************************************
 * WWDebugString -- sends a string to the debugger and echos it to disk                        *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    string                                                                            *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    10/28/96 12:48PM ST : Created                                                              *
 *=============================================================================================*/
void WWDebugString (char *string)
{
}









/***********************************************************************************************
 * Create_Main_Window -- opens the MainWindow for C&C                                          *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    instance -- handle to program instance                                            *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    10/10/95 4:08PM ST : Created                                                             *
 *=============================================================================================*/

#define CC_ICON 1

#if (ENGLISH)
#define WINDOW_NAME		"Red Alert"
#endif

#if (FRENCH)
#define WINDOW_NAME		"Alerte Rouge"
#endif

#if (GERMAN)
#define WINDOW_NAME		"Alarmstufe Rot"
#endif


void Create_Main_Window ( HANDLE instance , int command_show , int width , int height )

{
	MainWindow = NULL;
	return;
}


void Window_Dialog_Box(HANDLE  hinst, LPCTSTR  lpszTemplate, HWND  hwndOwner, DLGPROC  dlgprc)
{
}



typedef struct tColourList {

	char	Red;
	char	Green;
	char	Blue;
} ColourList;

ColourList	ColourLookup[9]={
	0,0,0,
	63,0,0,
	0,63,0,
	0,0,63,
	63,0,63,
	63,63,0,
	0,63,63,
	32,32,32,
	63,63,63
};




int	DebugColour=1;


extern "C" void Set_Palette_Register(int number, int red, int green, int blue);
//#pragma off (unreferenced)
void Colour_Debug (int call_number)
{
}

//#pragma on (unreferenced)





BOOL Any_Locked (void)
{
	if (SeenBuff.Get_LockCount() ||
		HidPage.Get_LockCount()) {
		return (TRUE);
	} else {
		return(FALSE);
	}
}








//
// Miscellaneous stubs. Mainly for multi player stuff
//
//
//

//IPXAddressClass::IPXAddressClass(void) {
//	int i;
//	i++;
//}
//int IPXManagerClass::Num_Connections(void) { return (0); }
//int IPXManagerClass::Connection_ID( int ) { return (0); }
//IPXAddressClass * IPXManagerClass::Connection_Address( int ) { return ((IPXAddressClass*)0); }
//char * IPXManagerClass::Connection_Name( int ) { return ((char*)0); }
//int IPXAddressClass::Is_Broadcast() { return (0); }
//int IPXManagerClass::Send_Global_Message( void *, int, int, IPXAddressClass * ) { return (0); }
//int IPXManagerClass::Service() { return (0); }
//int IPXManagerClass::Get_Global_Message( void  *, int  *, IPXAddressClass  *, short unsigned  * ) { return (0); }
//int IPXAddressClass::operator ==( IPXAddressClass  & ) { return (0); }
//IPXManagerClass::IPXManagerClass( int, int, int, int, short unsigned, short unsigned ) {}
//IPXManagerClass::~IPXManagerClass() {
//	int i;
//	i++;
//	}
//int  IPXManagerClass::Delete_Connection( int ) { return (0); }
//IPXAddressClass::IPXAddressClass( char unsigned  *, char unsigned  * ) {}
//void  IPXManagerClass::Set_Socket( short unsigned ) {}
//int  IPXManagerClass::Is_IPX() { return (0); }
//int  IPXManagerClass::Init() { return (0); }
//void  IPXAddressClass::Get_Address( char unsigned  *, char unsigned  * ) {}
//void  IPXManagerClass::Set_Bridge( char unsigned  * ) {}
//int  IPXManagerClass::Global_Num_Send() { return (0); }
//void  IPXManagerClass::Set_Timing( long unsigned, long unsigned, long unsigned ) {}
//unsigned long IPXManagerClass::Global_Response_Time() { return (0); }
//int  IPXManagerClass::Create_Connection( int, char  *, IPXAddressClass  * ) { return (0); }
//int  IPXAddressClass::operator !=( IPXAddressClass  & ) { return (0); }
//int  IPXManagerClass::Send_Private_Message( void  *, int, int, int ) { return (0); }
//int  IPXManagerClass::Get_Private_Message( void  *, int  *, int  * ) { return (0); }
//int  IPXManagerClass::Connection_Index( int ) { return (0); }
//void  IPXManagerClass::Reset_Response_Time() {}
//long unsigned  IPXManagerClass::Response_Time() { return (0); }
//int  IPXManagerClass::Private_Num_Send( int ) { return (0); }

//_VQAHandle  *  VQA_Alloc(void) { return ((_VQAHandle *)0); }
//void  VQA_Init( _VQAHandle  *, long ( *)()) {}
//long  VQA_Open( _VQAHandle  *, char const  *, _VQAConfig  * ) { return (0); }
//void  VQA_Free( _VQAHandle  * ) {}
//void  VQA_Close( _VQAHandle  * ) {}
//long  VQA_Play( _VQAHandle  *, long ) { return (0); }

//void VQA_Init(VQAHandle *, long(*)(VQAHandle *vqa, long action,	void *buffer, long nbytes)) {}

//long VQA_Open(VQAHandle *, char const *, VQAConfig *)
//{
//	return (0);
//}

//void VQA_Close(VQAHandle *) {}

//long VQA_Play(VQAHandle *, long)
//{
//	return (0);
//}


unsigned char	*VQPalette;
long				VQNumBytes;
unsigned long	VQSlowpal;
bool				VQPaletteChange = false;


extern "C"{
	void __cdecl SetPalette(unsigned char *palette, long numbytes, unsigned long slowpal);
}



void Flag_To_Set_Palette(unsigned char *palette, long numbytes, unsigned long slowpal)
{
	VQPalette = palette;
	VQNumBytes = numbytes;
	VQSlowpal = slowpal;
	VQPaletteChange = true;
}



void Check_VQ_Palette_Set(void)
{
	if (VQPaletteChange) {
		SetPalette(VQPalette, VQNumBytes, VQSlowpal);
		VQPaletteChange = false;
	}
}





void __cdecl SetPalette(unsigned char *palette, long, unsigned long)
{
	for (int i=0 ; i<256*3 ; i++) {
		*(palette+i)&=63;
	}
	Increase_Palette_Luminance(palette , 15 , 15 , 15 ,63);

	if (PalettesRead) {
		memcpy (&PaletteInterpolationTable[0][0] , InterpolatedPalettes[PaletteCounter++] , 65536);
	}
	Set_Palette(palette);
}


#ifndef NDEBUG
/***********************************************************************************************
 * Assert_Failure -- display the line and source file where a failed assert occurred           *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    line number in source file                                                        *
 *           name of source file                                                               *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    4/17/96 9:58AM ST : Created                                                              *
 *=============================================================================================*/

void Assert_Failure (char *expression, int line, char *file)
{
	char	assertbuf[256];
	char   timebuff[512];
	SYSTEMTIME	time;

	sprintf (assertbuf, "assert '%s' failed at line %d in module %s.\n", expression, line, file);

	if (!MonoClass::Is_Enabled()) MonoClass::Enable();

	Mono_Clear_Screen();
	Mono_Printf("%s", assertbuf);

	WWDebugString(assertbuf);

	GetLocalTime(&time);

	sprintf (timebuff, "%02d/%02d/%04d %02d:%02d:%02d - %s", time.wMonth, time.wDay, time.wYear,
																		time.wHour, time.wMinute, time.wSecond,
																		assertbuf);


	HMMIO handle = mmioOpen("ASSERT.TXT", NULL, MMIO_WRITE);
	if (!handle) {
		handle = mmioOpen("ASSERT.TXT", NULL, MMIO_CREATE | MMIO_WRITE);
		//mmioClose(handle, 0);
		//handle = mmioOpen("ASSERT.TXT", NULL, MMIO_WRITE);
	}

	if (handle) {

		mmioWrite(handle, timebuff, strlen(timebuff));
		mmioClose(handle, 0);
	}

	WWMessageBox().Process(assertbuf);
//	WWMessageBox().Process("Red Alert demo timed out - Aborting");
	//Get_Key();

	Prog_End(assertbuf, false);
	Invalidate_Cached_Icons();
	//PostQuitMessage( 0 );
	//ExitProcess(0);
}
#endif







/***********************************************************************************************
 * Memory_Error_Handler -- Handle a possibly fatal failure to allocate memory                  *
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
 *    5/22/96 3:57PM ST : Created                                                              *
 *=============================================================================================*/
void Memory_Error_Handler(void)
{
	VisiblePage.Clear();
	CCPalette.Set();
	while (Get_Mouse_State()) {Show_Mouse();};
	WWMessageBox().Process(TEXT_MEMORY_ERROR, TEXT_ABORT, false);

	ReadyToQuit = 1;

	PostMessage(MainWindow, WM_DESTROY, 0, 0);
	do
	{
		Keyboard->Check();
	}while (ReadyToQuit == 1);

	ExitProcess(0);
}








GraphicBufferClass* Read_PCX_File(char* name, char* Palette, void *Buff, long Size);
void Load_Title_Screen(char *name, GraphicViewPortClass *video_page, unsigned char *palette)
{

	GraphicBufferClass *load_buffer;


	load_buffer = Read_PCX_File (name, (char*)palette, NULL, 0);

	if (load_buffer) {
		load_buffer->Blit(*video_page);
		delete load_buffer;
	}
}



#include "filepcx.h"

/***************************************************************************
 * READ_PCX_FILE -- read a pcx file into a Graphic Buffer                  *
 *                                                                         *
 *	GraphicBufferClass* Read_PCX_File (char* name, char* palette ,void *Buff, long size );	*
 *  																								*
 *                                                                         *
 * INPUT: name is a NULL terminated string of the format [xxxx.pcx]        *
 *        palette is optional, if palette != NULL the the color palette of *
 *					 the pcx file will be place in the memory block pointed	   *
 *               by palette.																*
 *			 Buff is optional, if Buff == NULL a new memory Buffer		 		*
 *					 will be allocated, otherwise the file will be placed 		*
 *					 at location pointed by Buffer;										*
 *			Size is the size in bytes of the memory block pointed by Buff		*
 *				  is also optional;															*                                                                         *
 * OUTPUT: on success a pointer to a GraphicBufferClass containing the     *
 *         pcx file, NULL otherwise.                                       *
 *																									*
 * WARNINGS:                                                               *
 *         Appears to be a comment-free zone                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   05/03/1995 JRJ : Created.                                             *
 *   04/30/1996 ST : Tidied up and modified to use CCFileClass             *
 *=========================================================================*/

#define	POOL_SIZE 2048
#define	READ_CHAR()  *file_ptr++ ; \
							 if ( file_ptr	>= & pool [ POOL_SIZE ]	) { \
								 file_handle.Read (pool , POOL_SIZE ); \
								 file_ptr = pool ; \
							 }


GraphicBufferClass* Read_PCX_File(char* name, char* palette, void *Buff, long Size)
{
	unsigned					i, j;
	unsigned					rle;
	unsigned					color;
	unsigned					scan_pos;
	char						*file_ptr;
	int						width;
	int						height;
	char						*buffer;
	PCX_HEADER				header;
	RGB						*pal;
	char						pool [POOL_SIZE];
	GraphicBufferClass	*pic;

	CCFileClass file_handle(name);

	if (!file_handle.Is_Available()) return (NULL);

	file_handle.Open(READ);

	file_handle.Read (&header, sizeof (PCX_HEADER));

	if (header.id != 10 &&  header.version != 5 && header.pixelsize != 8 ) return NULL ;

	width = header.width - header.x + 1;
	height = header.height - header.y + 1;

	if (Buff) {
    	buffer = (char *)Buff;
    	i = Size / width;
    	height = MIN ((int)i - 1, height);
    	pic = new GraphicBufferClass(width, height, buffer, Size);
    	if ( !(pic && pic->Get_Buffer()) ) return NULL ;
	} else {
    	pic = new GraphicBufferClass(width, height, NULL, width*(height+4));
    	if ( !(pic && pic->Get_Buffer()) ) return NULL ;
	}

	buffer = (char *) pic->Get_Buffer();
	file_ptr = pool ;
	file_handle.Read (pool , POOL_SIZE);

	if ( header.byte_per_line != width ) {

		for ( scan_pos = j = 0 ; j < (unsigned) height ; j ++, scan_pos += width ) {
			for ( i = 0 ; i < (unsigned)width ; ) {
				rle = READ_CHAR ();
				if ( rle > 192 ) {
					rle -= 192 ;
					color =	READ_CHAR (); ;
					memset ( buffer + scan_pos + i , color , rle );
					i += rle;
				} else {
					*(buffer+scan_pos + i++ ) = (char)rle;
				}
			}
      	}

		if ( i == width ) rle = READ_CHAR ();
		if ( rle > 192 )  rle = READ_CHAR ();

	} else {

		for ( i = 0 ; i < (unsigned)width * height ; ) {
  			rle = READ_CHAR ();
  			rle &= 0xff;
  			if ( rle > 192 ) {
        		rle -= 192 ;
        		color = READ_CHAR ();
  				memset ( buffer + i , color , rle );
        		i += rle ;
     		} else {
				*(buffer + i++) = (char)rle;
			}
		}
	}

	if ( palette ) {
		file_handle.Seek (- (256 * (int)sizeof ( RGB )) , SEEK_END );
		file_handle.Read (palette , 256L * sizeof ( RGB ));

		pal = ( RGB * ) palette;
		for (i = 0 ; i < 256 ; i ++) {
			pal ->red	>>= 2;
			pal ->green	>>= 2;
			pal ->blue	>>= 2;
			pal ++ ;
		}
	}

	file_handle.Close();
	return pic;
}