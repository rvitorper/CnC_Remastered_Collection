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

/* $Header: /counterstrike/GOPTIONS.CPP 6     3/15/97 7:18p Steve_tall $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : OPTIONS.CPP                                                  *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : June 8, 1994                                                 *
 *                                                                                             *
 *                  Last Update : July 27, 1995 [JLB]                                          *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   OptionsClass::Process -- Handles all the options graphic interface.                       *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "function.h"

#include "goptions.h"
#include "loaddlg.h"
#include "sounddlg.h"
#include "visudlg.h"
#include "gamedlg.h"
#include "textbtn.h"
#include "descdlg.h"

#include "WolStrng.h"

bool RedrawOptionsMenu;

/***********************************************************************************************
 * OptionsClass::Process -- Handles all the options graphic interface.                         *
 *                                                                                             *
 *    This routine is the main control for the visual representation of the options            *
 *    screen. It handles the visual overlay and the player input.                              *
 *                                                                                             *
 * INPUT:      none                                                                            *
 *                                                                                             *
 * OUTPUT:     none                                                                            *
 *                                                                                             *
 * WARNINGS:      none                                                                         *
 *                                                                                             *
 * HISTORY:     12/31/1994 MML : Created.                                                      *
 *   06/23/1995 JLB : Handles restating the mission objective.                                 *
 *   07/27/1995 JLB : Adjusts menu for multiplay mode.                                         *
 *=============================================================================================*/
void GameOptionsClass::Process(void)
{
	static struct {
		int ID;				// Button ID to use.
		int Text;			// Text number to use for this button.
		bool Multiplay;	// Allowed in multiplayer version?
	} _constants[] = {
		{BUTTON_LOAD,  	TXT_LOAD_MISSION,    false},
#ifdef FIXIT_MULTI_SAVE
		{BUTTON_SAVE,  	TXT_SAVE_MISSION,    true},
#else
		{BUTTON_SAVE,  	TXT_SAVE_MISSION,    false},
#endif
		{BUTTON_DELETE,	TXT_DELETE_MISSION,  true},
		{BUTTON_GAME,  	TXT_GAME_CONTROLS,   true},
		{BUTTON_QUIT,  	TXT_QUIT_MISSION,    true},
		{BUTTON_DRAW,  	TXT_OK,    true},
		{BUTTON_RESUME,	TXT_RESUME_MISSION,  true},
		{BUTTON_RESTATE,	TXT_RESTATE_MISSION, false},
	};

	/*
	**	Variables.
	*/
	TextButtonClass * buttons = 0;
	int selection;
	bool pressed;
	int curbutton = 7;
	int y;
	TextButtonClass * buttonsel[ARRAY_SIZE(_constants)];
	static int num_buttons = sizeof(_constants)/sizeof(_constants[0]);


	int num_players = 0;
	int i;

	//
	// Compute the number of real players in the game; only allow saves
	// if there are more than 1.
	//
	for (i = 0; i < Session.Players.Count(); i++) {
		if (!(HouseClass::As_Pointer(Session.Players[i]->Player.ID)->IsDefeated)) {
			num_players++;
		}
	}


	Set_Logic_Page(SeenBuff);

	/*
	**	Build the button list for all of the buttons for this dialog.
	*/
	int maxwidth = 0;

	for (int index = 0; index < num_buttons ; index++ ) {
		int text = _constants[index].Text;
		buttonsel[index] = NULL;

		if (!_constants[index].Multiplay) {
			continue;
		}

		if (text == TXT_SAVE_MISSION) {
			continue;
		}

		if (( num_players < 2 ) &&
					text == TXT_SAVE_MISSION) {
			continue;
		}

		if (text == TXT_DELETE_MISSION) {
			continue;
		}

		if (text == TXT_DELETE_MISSION) {
			text = TXT_RESIGN;
		}

		if (index < 6) {
			y = (SeenBuff.Get_Height() - OptionHeight)/2 + ButtonY + ((OButtonHeight+2) * index);
		} else {
			y = OptionY + ButtonResumeY;
		}

		TextButtonClass* g;
		if( _constants[index].ID == BUTTON_DRAW )
		{
			continue;
		}
		else
			g = new TextButtonClass(_constants[index].ID, text, TPF_BUTTON, 0, y);

		if (g->Width > maxwidth) {
			maxwidth = g->Width;
		}
		if (buttons == NULL) {
			buttons = g;
		} else {
			g->Add_Tail(*buttons);
		}

		buttonsel[index] = g;
	}

	/*
	** BG: In skirmish mode, there is no 'restate' button, so we have to
	**     backtrack through the list to find the last valid button.
	*/
	while(!buttonsel[curbutton-1]) curbutton--;

	buttonsel[curbutton-1]->Turn_On();

	/*
	**	Force all button lengths to match the maximum length of the widest button.
	*/
	GadgetClass * g = buttons;
	while (g != NULL) {
		g->Width = max(maxwidth, 90 * RESFACTOR);
		g->X = OptionX+(OptionWidth-g->Width)/2;
		g = g->Get_Next();
	}
//#ifdef FRENCH
//	buttonsel[BUTTON_RESUME-1]->Width = 110 * RESFACTOR;
//	buttonsel[BUTTON_RESUME-1]->X = OptionX + (17 * RESFACTOR) - 5;
//#else
	buttonsel[BUTTON_RESUME-1]->Width = 90 * RESFACTOR;
	buttonsel[BUTTON_RESUME-1]->X = OptionX + (17 * RESFACTOR);
//#endif

	/*
	**	This causes left mouse button clicking within the confines of the dialog to
	**	be ignored if it wasn't recognized by any other button or slider.
	*/
	(new GadgetClass(OptionX, OptionY, OptionWidth, OptionHeight, GadgetClass::LEFTPRESS))->Add_Tail(*buttons);

	/*
	**	This cause a right click anywhere or a left click outside the dialog region
	**	to be equivalent to clicking on the return to game button.
	*/
	(new ControlClass(BUTTON_RESUME, 0, 0, SeenBuff.Get_Width(), SeenBuff.Get_Height(), GadgetClass::LEFTPRESS|GadgetClass::RIGHTPRESS))->Add_Tail(*buttons);

	Keyboard->Clear();

	Fancy_Text_Print(TXT_NONE, 0, 0, GadgetClass::Get_Color_Scheme(), TBLACK, TPF_CENTER|TPF_TEXT);

	/*
	**	Main Processing Loop.
	*/
	bool display = true;
	bool process = true;
	pressed = false;
	while (process) {

		/*
		**	Invoke game callback.
		*/
		Call_Back();

		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored) {
			AllSurfaces.SurfacesRestored = false;
			display = true;
		}

		/*
		**	Refresh display if needed.
		*/
		if (display || RedrawOptionsMenu) {

			/*
			**	Redraw the map.
			*/
			HidPage.Clear();
			Map.Flag_To_Redraw(true);
			Map.Render();

			/*
			**	Reset up the window.  Window x-coords are in bytes not pixels.
			*/
			Set_Window(WINDOW_EDITOR, OptionX, OptionY, OptionWidth, OptionHeight);
			Hide_Mouse();

			/*
			**	Draw the background.
			*/
			Dialog_Box(OptionX, OptionY, OptionWidth, OptionHeight);

			/*
			**	Draw the arrows border if requested.
			*/
 			Draw_Caption(TXT_OPTIONS, OptionX, OptionY, OptionWidth);

			/*
			**	Display the version number at the bottom of the dialog box.
			*/

			buttons->Draw_All();
			TabClass::Hilite_Tab(0);
			Show_Mouse();
			display = false;
			RedrawOptionsMenu = false;
		}

		/*
		**	Get user input.
		*/
		KeyNumType input = buttons->Input();

		/*
		**	Process Input.
		*/
		switch (input) {
			case (BUTTON_RESTATE | KN_BUTTON):
				selection = BUTTON_RESTATE;
				pressed = true;
				break;

			case (BUTTON_LOAD | KN_BUTTON):
				selection = BUTTON_LOAD;
				pressed = true;
				break;

			case (BUTTON_SAVE | KN_BUTTON):
				selection = BUTTON_SAVE;
				pressed = true;
				break;

			case (BUTTON_DELETE | KN_BUTTON):
				selection = BUTTON_DELETE;
				pressed = true;
				break;

			case (BUTTON_QUIT | KN_BUTTON):
				selection = BUTTON_QUIT;
				pressed = true;
				break;

			case (BUTTON_GAME | KN_BUTTON):
				selection = BUTTON_GAME;
				pressed = true;
				break;

			case (BUTTON_DRAW | KN_BUTTON):
				selection = BUTTON_DRAW;
				pressed = true;
				break;

			case (KN_ESC):
			case (BUTTON_RESUME | KN_BUTTON):
				selection = BUTTON_RESUME;
				pressed = true;
				break;

			case (KN_UP):
				buttonsel[curbutton-1]->Turn_Off();
				buttonsel[curbutton-1]->Flag_To_Redraw();
				do {
					curbutton--;
					if (curbutton < 1) curbutton = num_buttons;
				} while (!buttonsel[curbutton-1]);

				buttonsel[curbutton-1]->Turn_On();
				buttonsel[curbutton-1]->Flag_To_Redraw();
				break;

			case (KN_DOWN):
				buttonsel[curbutton-1]->Turn_Off();
				buttonsel[curbutton-1]->Flag_To_Redraw();
				do {
					curbutton++;
					if ( curbutton > num_buttons ) curbutton = 1;
				} while (!buttonsel[curbutton-1]);

				buttonsel[curbutton-1]->Turn_On();
				buttonsel[curbutton-1]->Flag_To_Redraw();
				break;

			case (KN_RETURN):
				buttonsel[curbutton-1]->IsPressed = true;
				buttonsel[curbutton-1]->Draw_Me(true);
				selection = curbutton;
				pressed = true;
				Keyboard->Clear();
				break;

			default:
				break;
		}

		if (pressed) {

			buttonsel[curbutton-1]->Turn_Off();
			buttonsel[curbutton-1]->Flag_To_Redraw();
			curbutton = selection;
			buttonsel[curbutton-1]->Turn_On();
			buttonsel[curbutton-1]->Flag_To_Redraw();

			switch (selection) {
				case BUTTON_RESTATE:
					display = true;
					if (!Restate_Mission(Scen.ScenarioName, TXT_VIDEO, TXT_RESUME_MISSION/*KOTXT_OPTIONS*/)) {
						BreakoutAllowed = true;
						Play_Movie(Scen.BriefMovie);
						BlackPalette.Adjust(0x08, WhitePalette);
						BlackPalette.Set();
						BlackPalette.Adjust(0xFF);
						BlackPalette.Set();
						GamePalette.Set();

						Map.Flag_To_Redraw(true);
						Theme.Queue_Song(THEME_PICK_ANOTHER);
						process = false;
					} else {
						BlackPalette.Adjust(0x08, WhitePalette);
						BlackPalette.Set();
						BlackPalette.Adjust(0xFF);
						BlackPalette.Set();
						GamePalette.Set();
						Map.Flag_To_Redraw(true);
						process = false;
					}
					break;

				case (BUTTON_LOAD):
					display = true;
					if (LoadOptionsClass(LoadOptionsClass::LOAD).Process()) {
						process = false;
					}
					break;

				case (BUTTON_SAVE):
					display = true
					OutList.Add(EventClass(EventClass::SAVEGAME));
					process = false;
					break;

				case (BUTTON_DELETE):
					display = true;
					if (Surrender_Dialog(TXT_SURRENDER)) {
						OutList.Add(EventClass(EventClass::DESTRUCT));
					}
					process = false;
					break;

				case (BUTTON_QUIT):
					if (Surrender_Dialog(TXT_CONFIRM_EXIT)) {
						process = false;
						Queue_Exit();
					} else {
						display = true;
					}
					break;

				case BUTTON_DRAW:
					if( Scen.bLocalProposesDraw )
					{
						//	Retract draw offer.
						OutList.Add(EventClass(EventClass::RETRACT_DRAW));
						process = false;
					}
					else
					{
						if( !Scen.bOtherProposesDraw )
						{
							//	Propose a draw?
							if( Surrender_Dialog( TXT_WOL_PROPOSE_DRAW_CONFIRM ) )
							{
								OutList.Add(EventClass(EventClass::PROPOSE_DRAW));
								process = false;
							}
							else
								display = true;
						}
						else
						{
							//	Accept a draw?
							if( Surrender_Dialog( TXT_WOL_ACCEPT_DRAW_CONFIRM ) )
							{
								OutList.Add(EventClass(EventClass::PROPOSE_DRAW));
								process = false;
							}
							else
								display = true;
						}
					}
					break;

				case (BUTTON_GAME):
					display = true;
					GameControlsClass().Process();
					break;

				case (BUTTON_RESUME):
					Save_Settings();
					process = false;
					display = true;
					break;
			}

			pressed = false;
			buttonsel[curbutton-1]->IsPressed = false;
			buttonsel[curbutton-1]->Turn_Off();
			buttonsel[curbutton-1]->Flag_To_Redraw();
		}
	}

	/*
	**	Clean up and re-enter the game.
	*/
	buttons->Delete_List();

	/*
	**	Redraw the map.
	*/
	Keyboard->Clear();
	HidPage.Clear();
	Map.Flag_To_Redraw(true);
	Map.Render();
}


void GameOptionsClass::Adjust_Variables_For_Resolution(void)
{
	OptionWidth		=	(216+8) * RESFACTOR;
	OptionHeight	=	111 * RESFACTOR;
	OptionX			=	((SeenBuff.Get_Width() - OptionWidth) / 2);
	OptionY			=	((SeenBuff.Get_Height() - OptionHeight) / 2);
	ButtonWidth		=	130 * RESFACTOR;
	OButtonHeight	=	9 * RESFACTOR;
	CaptionYPos		=	5 * RESFACTOR;
	ButtonY			=	21 * RESFACTOR;
	Border1Len		=	72 * RESFACTOR;
	Border2Len		=	16 * RESFACTOR;
	ButtonResumeY	=	(OptionHeight - (19 * RESFACTOR));
}
