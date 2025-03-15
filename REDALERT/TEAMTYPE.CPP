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

/* $Header: /CounterStrike/TEAMTYPE.CPP 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : TEAMTYPE.CPP                                                 *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 06/28/96                                                     *
 *                                                                                             *
 *                  Last Update : July 30, 1996 [JLB]                                          *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   TeamMissionClass::Description -- Compose a text description of team mi                    *
 *   TeamMissionClass::Draw_It -- Draws a team mission list box entry.                         *
 *   TeamMission_Needs -- Determines what extra data is needed by team miss                    *
 *   TeamTypeClass::As_Pointer -- gets ptr for team type with given name                       *
 *   TeamTypeClass::Build_INI_Entry -- Builds the INI entry for this team type.                *
 *   TeamTypeClass::Create_One_Of -- Creates a team of this type.                              *
 *   TeamTypeClass::Description -- Builds a description of the team.                           *
 *   TeamTypeClass::Destroy_All_Of -- Destroy all teams of this type.                          *
 *   TeamTypeClass::Detach -- Detach the specified target from this team type.                 *
 *   TeamTypeClass::Draw_It -- Display the team type in a list box.                            *
 *   TeamTypeClass::Edit -- Edit the team type.                                                *
 *   TeamTypeClass::Fill_In -- fills in trigger from the given INI entry                       *
 *   TeamTypeClass::From_Name -- Converts a name into a team type pointer.                     *
 *   TeamTypeClass::Init -- pre-scenario initialization                                        *
 *   TeamTypeClass::Member_Description -- Builds a member description string                   *
 *   TeamTypeClass::Mission_From_Name -- returns mission for given name                        *
 *   TeamTypeClass::Name_From_Mission -- returns name for given mission                        *
 *   TeamTypeClass::Read_INI -- reads INI data                                                 *
 *   TeamTypeClass::Suggested_New_Team -- Suggests a new team to create.                       *
 *   TeamTypeClass::TeamTypeClass -- class constructor                                         *
 *   TeamTypeClass::Write_INI -- Write out the team types to the INI database.                 *
 *   TeamTypeClass::operator delete -- 'delete' operator                                       *
 *   TeamTypeClass::operator new -- 'new' operator                                             *
 *   TeamTypeClass::~TeamTypeClass -- class destructor                                         *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "function.h"


TeamMissionClass TeamMissions[TMISSION_COUNT] = {
	{TMISSION_ATTACK},
	{TMISSION_ATT_WAYPT},
	{TMISSION_FORMATION},
	{TMISSION_MOVE},
	{TMISSION_MOVECELL},
	{TMISSION_GUARD},
	{TMISSION_LOOP},
	{TMISSION_ATTACKTARCOM},
	{TMISSION_UNLOAD},
	{TMISSION_DEPLOY},
	{TMISSION_HOUND_DOG},
	{TMISSION_DO},
	{TMISSION_SET_GLOBAL},
	{TMISSION_LOAD},
	{TMISSION_SPY},
	{TMISSION_PATROL},
};



int atoh(char * str);


#if defined(CHEAT_KEYS) || defined(SCENARIO_EDITOR)
/***********************************************************************************************
 * TeamTypeClass::Draw_It -- Display the team type in a list box.                              *
 *                                                                                             *
 *    This is a helper routine that is used when this team type is stored into a list box      *
 *    and must be displayed. It will display the team type as a single line of text.           *
 *                                                                                             *
 * INPUT:   see below...                                                                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/30/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void TeamTypeClass::Draw_It(int , int x, int y, int width, int height, bool selected, TextPrintType flags) const
{
	RemapControlType * scheme = GadgetClass::Get_Color_Scheme();
	static int _tabs[] = {35, 60, 80, 100};
	if ((flags & 0x0F) == TPF_6PT_GRAD || (flags & 0x0F) == TPF_EFNT) {

		if (selected) {
			flags = flags | TPF_BRIGHT_COLOR;
			LogicPage->Fill_Rect(x, y, x + width - 1, y + height - 1, scheme->Shadow);
		} else {
			if (!(flags & TPF_USE_GRAD_PAL)) {
				flags = flags | TPF_MEDIUM_COLOR;
			}
		}

		Conquer_Clip_Text_Print(Description(), x, y, scheme, TBLACK, flags, width, _tabs);
	} else {
		Conquer_Clip_Text_Print(Description(), x, y, (selected ? &ColorRemaps[PCOLOR_DIALOG_BLUE] : &ColorRemaps[PCOLOR_GREY]), TBLACK, flags, width, _tabs);
	}
}
#endif


/*
********************************** Globals **********************************
*/
char const * TeamTypeClass::TMissions[TMISSION_COUNT] = {
	"Attack...",
	"Attack Waypoint...",
	"Change Formation to...",
	"Move to waypoint...",
	"Move to Cell...",
	"Guard area (1/10th min)...",
	"Jump to line #...",
	"Attack Tarcom",
	"Unload",
	"Deploy",
	"Follow friendlies",
	"Do this...",
	"Set global...",
	"Invulnerable",
	"Load onto Transport",
	"Spy on bldg @ waypt...",
	"Patrol to waypoint..."
};


/***************************************************************************
 * TeamTypeClass::TeamTypeClass -- class constructor                       *
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/07/1994 BR : Created.                                              *
 *   11/22/1995 JLB : Uses initializer constructor method.                 *
 *=========================================================================*/
TeamTypeClass::TeamTypeClass(void) :
	AbstractTypeClass(RTTI_TEAMTYPE, TeamTypes.ID(this), TXT_NONE, ""),
	IsRoundAbout(false),
	IsSuicide(false),
	IsAutocreate(false),
	IsPrebuilt(true),
	IsReinforcable(true),
	IsTransient(false),
	RecruitPriority(7),
	InitNum(0),
	MaxAllowed(0),
	Fear(0),
	House(HOUSE_NONE),
	Trigger(0),
	Origin(-1),
	Number(0),
	MissionCount(0),
	ClassCount(0)
{
	for (int i = 0; i < MAX_TEAM_CLASSCOUNT; i++) {
		Members[i].Class = NULL;
		Members[i].Quantity = 0;
	}
}


/***************************************************************************
 * TeamTypeClass::Init -- pre-scenario initialization                      *
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/07/1994 BR : Created.                                              *
 *=========================================================================*/
void TeamTypeClass::Init(void)
{
	TeamTypes.Free_All();
}


/***************************************************************************
 * TeamTypeClass::As_Pointer -- gets ptr for team type with given name     *
 *                                                                         *
 * INPUT:                                                                  *
 *      name      name of teamtype                                         *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      ptr to TeamType with that name                                     *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/07/1994 BR : Created.                                              *
 *=========================================================================*/
TeamTypeClass * TeamTypeClass::As_Pointer(char const * name)
{
	if (name) {
		for (int index = 0; index < TeamTypes.Count(); index++) {
			if (!stricmp(name, TeamTypes.Ptr(index)->IniName)) {
				return(TeamTypes.Ptr(index));
			}
		}
	}
	return(NULL);
}


/***************************************************************************
 * TeamTypeClass::Mission_From_Name -- returns team mission for given name *
 *                                                                         *
 * INPUT:                                                                  *
 *      name         name to compare                                       *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      mission for that name                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/13/1994 BR : Created.                                              *
 *=========================================================================*/
TeamMissionType TeamTypeClass::Mission_From_Name(char const * name)
{
	if (name) {
		for (TeamMissionType order = TMISSION_FIRST; order < TMISSION_COUNT; order++) {
			if (stricmp(TMissions[order], name) == 0) {
				return(order);
			}
		}
	}

	return(TMISSION_NONE);
}


/***************************************************************************
 * TeamTypeClass::Name_From_Mission -- returns name for given mission      *
 *                                                                         *
 * INPUT:                                                                  *
 *      order      mission to get name for                                 *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      name of mission                                                    *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/13/1994 BR : Created.                                              *
 *=========================================================================*/
char const * TeamTypeClass::Name_From_Mission(TeamMissionType order)
{
	assert((unsigned)order < TMISSION_COUNT);

	return(TMissions[order]);
}


/***************************************************************************
 * TeamTypeClass::operator new -- 'new' operator                           *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      pointer to new TeamType                                            *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   11/28/1994 BR : Created.                                              *
 *=========================================================================*/
void * TeamTypeClass::operator new(size_t )
{
	void * ptr = TeamTypes.Allocate();
	if (ptr) {
		((TeamTypeClass *)ptr)->IsActive = true;
	}
	return(ptr);
}


/***************************************************************************
 * TeamTypeClass::operator delete -- 'delete' operator                     *
 *                                                                         *
 * INPUT:                                                                  *
 *      ptr      pointer to delete                                         *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   11/28/1994 BR : Created.                                              *
 *=========================================================================*/
void TeamTypeClass::operator delete(void * ptr)
{
	if (ptr) {
		((TeamTypeClass *)ptr)->IsActive = false;
	}
	TeamTypes.Free((TeamTypeClass *)ptr);
}


/***********************************************************************************************
 * TeamTypeClass::Create_One_Of -- Creates a team of this type.                                *
 *                                                                                             *
 *    Use this routine to create a team object from this team type.                            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the newly created team object. If one could not be       *
 *          created, then NULL is returned.                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
TeamClass * TeamTypeClass::Create_One_Of(void) const
{
	if (ScenarioInit || Number < MaxAllowed) {
//	if (ScenarioInit || TeamClass::Number[ID] < MaxAllowed) {
		return(new TeamClass(this, HouseClass::As_Pointer(House)));
	}
	return(NULL);
}


/***********************************************************************************************
 * TeamTypeClass::Destroy_All_Of -- Destroy all teams of this type.                            *
 *                                                                                             *
 *    This routine will destroy all teams of this type. Typical use of this is from a trigger  *
 *    event.                                                                                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void TeamTypeClass::Destroy_All_Of(void) const
{
	for (int index = 0; index < Teams.Count(); index++) {
		TeamClass * team = Teams.Ptr(index);

		if (team->Class == this) {
			delete team;
			index--;
		}
	}
}


/***********************************************************************************************
 * TeamTypeClass::Suggested_New_Team -- Suggests a new team to create.                         *
 *                                                                                             *
 *    This routine will scan through the team types available and create teams of the          *
 *    type that can best utilize the existing unit mix.                                        *
 *                                                                                             *
 * INPUT:   house    -- Pointer to the house that this team is to be created for.              *
 *                                                                                             *
 *          atypes   -- A bit mask of the aircraft types available for this house.             *
 *                                                                                             *
 *          utypes   -- A bit mask of the unit types available for this house.                 *
 *                                                                                             *
 *          itypes   -- A bit mask of the infantry types available for this house.             *
 *                                                                                             *
 *          vtypes   -- A bit mask of the vessel types available for this house.               *
 *                                                                                             *
 *          alerted  -- Is this house alerted? If true, then the Autocreate teams will be      *
 *                      considered in the selection process.                                   *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the team type that should be created. If no team should  *
 *          be created, then it returns NULL.                                                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/13/1995 JLB : Created.                                                                 *
 *   07/21/1995 JLB : Will autocreate team even if no members in field.                        *
 *=============================================================================================*/
TeamTypeClass const * TeamTypeClass::Suggested_New_Team(HouseClass * house, long , long , long , long , bool alerted)
//TeamTypeClass const * TeamTypeClass::Suggested_New_Team(HouseClass * house, long atypes, long utypes, long itypes, long vtypes, bool alerted)
{
//	TeamTypeClass const * best = NULL;
//	int bestvalue = 0;

	TeamTypeClass const * choices[20];
	int choicecount = 0;

	for (int index = 0; index < TeamTypes.Count(); index++) {
		TeamTypeClass const * ttype = TeamTypes.Ptr(index);

		assert(ttype != NULL);

		int maxnum = ttype->MaxAllowed;
		if ((alerted && !ttype->IsAutocreate) || (!alerted && ttype->IsAutocreate)) {
			maxnum = 0;
		}

		if (choicecount >= ARRAY_SIZE(choices)) break;

		if (ttype != NULL && ttype->House == house->Class->House && ttype->Number < maxnum) {

			choices[choicecount++] = ttype;

#ifdef OBSOLETE
			/*
			**	Determine what kind of units this team requires.
			*/
			long uneeded = 0;
			long ineeded = 0;
			long vneeded = 0;
			long aneeded = 0;
			for (int ctype = 0; ctype < ttype->ClassCount; ctype++) {
				switch (ttype->Members[ctype].Class->What_Am_I()) {
					case RTTI_INFANTRYTYPE:
						ineeded |= (1 << ((InfantryTypeClass *)ttype->Members[ctype].Class)->Type);
						break;

					case RTTI_UNITTYPE:
						uneeded |= (1 << ((UnitTypeClass *)ttype->Members[ctype].Class)->Type);
						break;

					case RTTI_VESSELTYPE:
						vneeded |= (1 << ((VesselTypeClass *)ttype->Members[ctype].Class)->Type);
						break;

					case RTTI_AIRCRAFTTYPE:
						aneeded |= (1 << ((AircraftTypeClass *)ttype->Members[ctype].Class)->Type);
						break;
				}
			}

			/*
			**	If this team can use the types required, then consider it a possible
			**	team type to create.
			*/
			int value = 0;
			if ((aneeded & atypes) != 0 || (ineeded & itypes) != 0 || (uneeded & utypes) != 0 || (vneeded & vtypes) != 0) {
				value = ttype->RecruitPriority;
			} else {
				value = ttype->RecruitPriority/2;
			}

			if (best == NULL || bestvalue < value) {
				bestvalue = value;
				best = ttype;
			}
#endif
		}
	}

	if (choicecount > 0) {
		return(choices[Random_Pick(0, choicecount-1)]);
	}
	return(NULL);

//	return(best);
}


/***********************************************************************************************
 * TeamTypeClass::From_Name -- Converts a name into a team type pointer.                       *
 *                                                                                             *
 *    This routine is used to convert an ASCII name of a team type into the corresponding      *
 *    team type pointer.                                                                       *
 *                                                                                             *
 * INPUT:   name  -- Pointer to the ASCII name of the team type.                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the team type that this ASCII name represents. If there  *
 *          is no match, the NULL is returned.                                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/26/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
TeamTypeClass * TeamTypeClass::From_Name(char const * name)
{
	if (name) {
		for (int index = 0; index < TeamTypes.Count(); index++) {
			if (stricmp(name, TeamTypes.Ptr(index)->IniName) == 0) {
				return(TeamTypes.Ptr(index));
			}
		}
	}
	return(0);
}


/***********************************************************************************************
 * TeamMission_Needs -- Determines what extra data is needed by team mission.                  *
 *                                                                                             *
 *    This routine will return the required extra data that the specified team mission will    *
 *    need.                                                                                    *
 *                                                                                             *
 * INPUT:   tmtype   -- The team mission type to check.                                        *
 *                                                                                             *
 * OUTPUT:  Returns with the data type needed for this team mission.                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/26/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
NeedType TeamMission_Needs(TeamMissionType tmtype)
{
	switch (tmtype) {
		/*
		**	Requires a formation type.
		*/
		case TMISSION_FORMATION:
			return(NEED_FORMATION);

		/*
		**	Team mission requires a target quarry value.
		*/
		case TMISSION_ATTACK:
			return(NEED_QUARRY);

		/*
		**	Team mission requires a data value.
		*/
		case TMISSION_MOVECELL:
			return(NEED_HEX_NUMBER);

		case TMISSION_SET_GLOBAL:
		case TMISSION_GUARD:
		case TMISSION_LOOP:
			return(NEED_NUMBER);

		/*
		**	Team mission requires a waypoint.
		*/
		case TMISSION_PATROL:
		case TMISSION_MOVE:
		case TMISSION_ATT_WAYPT:
		case TMISSION_SPY:
			return(NEED_WAYPOINT);

		/*
		**	Team mission requires a general mission type.
		*/
		case TMISSION_DO:
			return(NEED_MISSION);

		default:
			break;
	}
	return(NEED_NONE);
}


#if defined(CHEAT_KEYS) || defined(SCENARIO_EDITOR)
/***********************************************************************************************
 * TeamTypeClass::Member_Description -- Builds a member description string.                    *
 *                                                                                             *
 *    This routine will build a team member description string. The string will be composed    *
 *    of the team member type and quantity. As many team member types will be listed that      *
 *    can fit within a reasonable size.                                                        *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the text string that contains a description of the team  *
 *          type members.                                                                      *
 *                                                                                             *
 * WARNINGS:   The return string may be truncated if necessary to fit within reasonable size   *
 *             limits.                                                                         *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/05/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
char const * TeamTypeClass::Member_Description(void) const
{
	static char buffer[128];

	buffer[0] = '\0';

	/*
	**	Fill in class & count for all classes
	*/
	for (int index = 0; index < ClassCount; index++) {
		char txt[10];

		strcat(buffer, Members[index].Class->IniName);
		strcat(buffer, ":");

		sprintf(txt, "%d", Members[index].Quantity);
		strcat(buffer, txt);

		if (index < ClassCount-1) {
			strcat(buffer, ",");
		}
	}

	if (strlen(buffer) > 25) {
		strcpy(&buffer[25-3], "...");
	}

	return(buffer);
}


/***********************************************************************************************
 * TeamTypeClass::Description -- Builds a description of the team.                             *
 *                                                                                             *
 *    This routine will build a brief description of the team type. This description is used   *
 *    in the team type list.                                                                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the composed text string that represents the team type.               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/05/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
char const * TeamTypeClass::Description(void) const
{
	static char _buffer[128];
	char extra = ' ';
	char loc[3];

	loc[0] = loc[1] = loc[2] = 0;
	if (IsAutocreate) extra = '*';
	if (Origin > -1) {
//	if (Origin != -1) {
		if (Origin < 26) {
			loc[0] = 'A' + Origin;
		} else {
			loc[0] = Origin / 26 + 'A'-1;
			loc[1] = Origin % 26 + 'A';
		}
	}

	sprintf(_buffer, "%s\t%s\t%c%s\t%d\t%s", IniName, HouseTypeClass::As_Reference(House).Suffix, extra, loc, MissionCount, Member_Description());
	return(_buffer);
}


/***********************************************************************************************
 * TeamMissionClass::Description -- Compose a text description of team mission.                *
 *                                                                                             *
 *    This routine will create a text representation of a team mission. This description will  *
 *    be used in the list of team missions for display purposes.                               *
 *                                                                                             *
 * INPUT:   index -- The index to assign to this team. The index is used since some team       *
 *                   missions refer to the another team mission by index number.               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the team mission text.                                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/05/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
char const * TeamMissionClass::Description(int index) const
{
	static char buffer[64];

	sprintf(buffer, "%d\t%s", index, TeamTypeClass::Name_From_Mission(Mission));

	switch (TeamMission_Needs(Mission)) {
		case NEED_MISSION:
			strcat(buffer, MissionClass::Mission_Name(Data.Mission));
			break;

		case NEED_FORMATION:
			strcat(buffer, FormationName[Data.Quarry]);
			break;

		case NEED_NUMBER:
			sprintf(&buffer[strlen(buffer)], "%d", Data.Value);
			break;

		case NEED_HEX_NUMBER:
			sprintf(&buffer[strlen(buffer)], "%x", Data.Value);
			break;

		case NEED_QUARRY:
			strcat(buffer, QuarryName[Data.Quarry]);
			break;

		case NEED_WAYPOINT:
			if (Data.Value < 26) {
				sprintf(&buffer[strlen(buffer)], "%c", Data.Value + 'A');
			} else {
				sprintf(&buffer[strlen(buffer)], "%c%c", (Data.Value/26) + 'A'-1, (Data.Value % 26) + 'A');
			}
			break;

	}

	return(buffer);
}
#endif


/***********************************************************************************************
 * TeamTypeClass::Detach -- Detach the specified target from this team type.                   *
 *                                                                                             *
 *    This routine is called when some object is about to be removed from the game system and  *
 *    all references to it must be severed. This will check to see if the specified object     *
 *    is a trigger that this team refers to. If so, then the reference will be cleared.        *
 *                                                                                             *
 * INPUT:   target   -- The target object to remove references to.                             *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/30/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void TeamTypeClass::Detach(TARGET target, bool)
{
	if (Is_Target_TriggerType(target) && Trigger.Is_Valid() && Trigger == As_TriggerType(target)) {
		Trigger = NULL;
	}
}


/***************************************************************************
 * TeamTypeClass::Read_INI -- reads INI data                               *
 *                                                                         *
 * INI entry format:                                                       *
 *      TeamName = Housename,Roundabout,Learning,Suicide,Spy,Mercenary,    *
 *       RecruitPriority,MaxAllowed,InitNum,Fear,                          *
 *       ClassCount,Class:Num,Class:Num,...,                               *
 *       MissionCount,Mission:Arg,Mission:Arg,Mission:Arg,...              *
 *                                                                         *
 * INPUT:                                                                  *
 *      buffer      buffer to hold the INI data                            *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/07/1994 BR : Created.                                              *
 *   02/01/1995 BR : No del team if no classes (editor needs empty teams!) *
 *=========================================================================*/
void TeamTypeClass::Read_INI(CCINIClass & ini)
{
	TeamTypeClass * team;					// Working team pointer.
	char buf[500];								// INI entry buffer

	int len = ini.Entry_Count(INI_Name());

	/*
	**	Loop for all team entries, create and fill in.
	*/
	for (int index = 0; index < len; index++) {
		team = new TeamTypeClass();
		if (team != NULL) {
			char const * entry = ini.Get_Entry(INI_Name(), index);
			ini.Get_String(INI_Name(), entry, NULL, buf, sizeof(buf));
			team->Fill_In((char *)entry, buf);
		}
	}
}


/***********************************************************************************************
 * TeamTypeClass::Fill_In -- fills in trigger from the given INI entry                         *
 *                                                                                             *
 * This routine fills in the given teamtype with the given name, and values from               *
 * the given INI entry.                                                                        *
 *                                                                                             *
 * (This routine is used by the scenario editor, to import teams from the MASTER.INI file.)    *
 *                                                                                             *
 *    INI entry format:                                                                        *
 *      TeamName = Housename,Roundabout,Learning,Suicide,Spy,Mercenary,                        *
 *       RecruitPriority,MaxAllowed,InitNum,Fear,                                              *
 *       ClassCount,Class:Num,Class:Num,...,                                                   *
 *       MissionCount,Mission:Arg,Mission:Arg,Mission:Arg,...                                  *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      name      mnemonic for the desired trigger                                             *
 *      entry      INI entry to parse                                                          *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/28/1994 BR : Created.                                                                  *
 *   11/29/1995 JLB : Revamped to use new team class.                                          *
 *=============================================================================================*/
void TeamTypeClass::Fill_In(char * name, char * entry)
{
	assert(TeamTypes.ID(this) == ID);

	/*
	**	Set its name
	*/
	Set_Name(name);

	House = HousesType(atoi(strtok(entry, ",")));

	int code;
	switch (NewINIFormat) {
		default:
			code = atoi(strtok(NULL, ","));
			IsRoundAbout = ((code & 0x0001) != 0);
			IsSuicide = ((code & 0x0002) != 0);
			IsAutocreate = ((code & 0x0004) != 0);
			IsPrebuilt = ((code & 0x0008) != 0);
			IsReinforcable = ((code & 0x0010) != 0);
			break;

		case 0:
		case 1:
			IsRoundAbout = atoi(strtok(NULL, ","));
			IsSuicide = atoi(strtok(NULL, ","));
			IsAutocreate = atoi(strtok(NULL, ","));
			IsPrebuilt = atoi(strtok(NULL, ","));
			IsReinforcable = atoi(strtok(NULL, ","));
			break;
	}

	RecruitPriority = atoi(strtok(NULL, ","));
	InitNum = atoi(strtok(NULL, ","));
	MaxAllowed = atoi(strtok(NULL, ","));
	Origin = atoi(strtok(NULL, ","));

	switch (NewINIFormat) {
		default:
			Trigger.Set_Raw(atoi(strtok(NULL, ",")));
			break;

		case 0:
		case 1:
			// Throw this token away -- it isn't used.
			strtok(NULL, ",");
			break;
	}

	/*
	**	Fetch the team member types and quantity values.
	*/
	ClassCount = atoi(strtok(NULL, ","));
	for (int index = 0; index < ClassCount; index++) {
		char * p1 = strtok(NULL, ",:");
		char * p2 = strtok(NULL, ",:");
		TechnoTypeClass const * otype = NULL;

		/*
		**	See if this is an infantry name
		*/
		InfantryType i_id = InfantryTypeClass::From_Name(p1);
		if (i_id != INFANTRY_NONE) {
			otype = &InfantryTypeClass::As_Reference(i_id);
		} else {

			/*
			**	See if this is a unit name
			*/
			UnitType u_id = UnitTypeClass::From_Name(p1);
			if (u_id != UNIT_NONE) {
				otype = &UnitTypeClass::As_Reference(u_id);
			} else {

				/*
				**	See if this is an aircraft name
				*/
				AircraftType a_id = AircraftTypeClass::From_Name(p1);
				if (a_id != AIRCRAFT_NONE) {
					otype = &AircraftTypeClass::As_Reference(a_id);
				} else {

					/*
					**	See if this is a vessel name.
					*/
					VesselType v_id = VesselTypeClass::From_Name(p1);
					if (v_id != VESSEL_NONE) {
						otype = &VesselTypeClass::As_Reference(v_id);
					}
				}
			}
		}

		/*
		**	If the name was resolved, add this class
		*/
		if (otype) {
			if (index < MAX_TEAM_CLASSCOUNT) {
				Members[index].Class = otype;
				Members[index].Quantity = atoi(p2);
			}
		} else {
			ClassCount--;
			if (index == 0) break;
			index--;
		}
	}
	ClassCount = min(MAX_TEAM_CLASSCOUNT, ClassCount);

	/*
	**	Fetch the missions assigned to this team type.
	*/
	MissionCount = atoi(strtok(NULL, ","));
	for (int index = 0; index < MissionCount; index++) {
		MissionList[index].Mission = TeamMissionType(atoi(strtok(NULL, ",:")));
		MissionList[index].Data.Value = atoi(strtok(NULL, ",:"));
	}

	if (NewINIFormat < 2) {
		/*
		**	Fetch the trigger ID.
		*/
		Trigger.Set_Raw(atoi(strtok(NULL, ",")));
	}
}


/***********************************************************************************************
 * TeamTypeClass::Write_INI -- Write out the team types to the INI database.                   *
 *                                                                                             *
 *    This routine will take all team types and write them out to the INI database specified.  *
 *                                                                                             *
 * INPUT:   ini   -- Reference to the INI database that will hold al the teams.                *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   All preexisting team data in the database will be erased by this routine.       *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/30/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void TeamTypeClass::Write_INI(CCINIClass & ini)
{
	ini.Clear("TeamTypes");
	ini.Clear(INI_Name());

	/*
	**	Now write all the team data out
	*/
	for (int index = 0; index < TeamTypes.Count(); index++) {
//	for (int index = TeamTypes.Count()-1; index >= 0; index--) {
		TeamTypeClass * team = TeamTypes.Ptr(index);
		char buf[256];

		buf[0] = 0;
		team->Build_INI_Entry(buf);
		ini.Put_String(INI_Name(), team->IniName, buf);
	}
}


/***********************************************************************************************
 * TeamTypeClass::Build_INI_Entry -- Builds the INI entry for this team type.                  *
 *                                                                                             *
 *    This routine is used to build the text string that will go into the INI database for     *
 *    a team of this type. This text string will be parsed back into a team object when the    *
 *    scenario INI is read in.                                                                 *
 *                                                                                             *
 * INPUT:   buf   -- Pointer to a buffer that will hold the team text entry.                   *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Be sure that the buffer can hold the string build. A size of 80 or so is        *
 *             usually sufficient.                                                             *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/30/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void TeamTypeClass::Build_INI_Entry(char * buf)
{
	int code = 0;
	code |= IsRoundAbout ? 0x0001 : 0;
	code |= IsSuicide ? 0x0002 : 0;
	code |= IsAutocreate ? 0x0004 : 0;
	code |= IsPrebuilt ? 0x0008 : 0;
	code |= IsReinforcable ? 0x0010 : 0;

	/*
	**	Output the general data for this team type.
	*/
	sprintf(buf, "%d,%d,%d,%d,%d,%d,%d",
		House,
		code,
		RecruitPriority,
		InitNum,
		MaxAllowed,
		Origin,
		TriggerTypes.Logical_ID(Trigger)
		);
	buf += strlen(buf);

	/*
	**	For every class in the team, record the class's name & desired count
	*/
	sprintf (buf, ",%d", ClassCount);
	buf += strlen(buf);
	for (int i = 0; i < ClassCount; i++) {
		sprintf (buf, ",%s:%d", Members[i].Class->IniName, Members[i].Quantity);
		buf += strlen(buf);
	}

	/*
	**	Record the # of missions, and each mission name & argument value.
	*/
	sprintf(buf, ",%d", MissionCount);
	buf += strlen(buf);
	for (int i = 0; i < MissionCount; i++) {
		sprintf (buf, ",%d:%d", MissionList[i].Mission, MissionList[i].Data.Value);
		buf += strlen(buf);
	}
}


