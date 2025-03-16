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

/* $Header: /CounterStrike/TRIGTYPE.CPP 1     3/03/97 10:26a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : TRIGTYPE.CPP                                                 *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 06/05/96                                                     *
 *                                                                                             *
 *                  Last Update : July 9, 1996 [JLB]                                           *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   TriggerTypeClass::As_Target -- Convert this trigger type object into a target value.      *
 *   TriggerTypeClass::Attaches_To -- Determines what trigger can attach to.                   *
 *   TriggerTypeClass::Build_INI_Entry -- Construct the INI entry into the buffer specified.   *
 *   TriggerTypeClass::Description -- Build a text description of the trigger type.            *
 *   TriggerTypeClass::Detach -- Removes attachments to the target object specified.           *
 *   TriggerTypeClass::Draw_It -- Draws this trigger as if it were a line in a list box.       *
 *   TriggerTypeClass::Edit -- Edit the trigger type through the scenario editor.              *
 *   TriggerTypeClass::Fill_In -- fills in trigger from the given INI entry                    *
 *   TriggerTypeClass::From_Name -- Convert an ASCII name into a trigger type pointer.         *
 *   TriggerTypeClass::Init -- Initialize the trigger type object management system.           *
 *   TriggerTypeClass::Read_INI -- reads triggers from the INI file                            *
 *   TriggerTypeClass::TriggerTypeClass -- Constructor for trigger class object.               *
 *   TriggerTypeClass::Write_INI -- Stores all trigger types to the INI database specified.    *
 *   TriggerTypeClass::operator delete -- Returns a trigger type class object back to the pool *
 *   TriggerTypeClass::operator new -- Allocates a trigger type class object.                  *
 *   TriggerTypeClass::~TriggerTypeClass -- Deleting a trigger type deletes associated triggers*
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#include	"function.h"
#include	"trigtype.h"


/***********************************************************************************************
 * TriggerTypeClass::TriggerTypeClass -- Constructor for trigger class object.                 *
 *                                                                                             *
 *    This is the normal constructor for a trigger object. The trigger starts with no team     *
 *    members, no mission, and default values for all settings.                                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/10/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
TriggerTypeClass::TriggerTypeClass(void) :
	AbstractTypeClass(RTTI_TRIGGERTYPE, TriggerTypes.ID(this), TXT_NONE, "x"),
	IsPersistant(VOLATILE),
	EventControl(MULTI_ONLY),
	ActionControl(MULTI_ONLY),
	House(HOUSE_SPAIN)
{
}


/***********************************************************************************************
 * TriggerTypeClass::~TriggerTypeClass -- Deleting a trigger type deletes associated triggers. *
 *                                                                                             *
 *    When a trigger type is deleted, then all triggers that refer to that type must also      *
 *    be deleted as well. There can be no 'orphan' triggers in existence.                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
TriggerTypeClass::~TriggerTypeClass(void)
{
}


/***********************************************************************************************
 * TriggerTypeClass::operator new -- Allocates a trigger type class object.                    *
 *                                                                                             *
 *    This routine will allocate a block of memory from the special trigger type object        *
 *    pool.                                                                                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the allocated trigger type memory block. If there is     *
 *          no more block available in the pool, then NULL is returned.                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void * TriggerTypeClass::operator new(size_t )
{
	void * ptr = TriggerTypes.Allocate();
	if (ptr) {
		((TriggerTypeClass *)ptr)->IsActive = true;
	}

	return(ptr);
}


/***********************************************************************************************
 * TriggerTypeClass::operator delete -- Returns a trigger type class object back to the pool   *
 *                                                                                             *
 *    This routine will return a previously allocated trigger type object to the private       *
 *    memory pool from which it was allocated.                                                 *
 *                                                                                             *
 * INPUT:   ptr   -- Pointer to the trigger type class to return to the pool.                  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void TriggerTypeClass::operator delete(void * ptr)
{
	if (ptr) {
		((TriggerTypeClass *)ptr)->IsActive = false;
	}
	TriggerTypes.Free((TriggerTypeClass *)ptr);
}


/***********************************************************************************************
 * TriggerTypeClass::As_Target -- Convert this trigger type object into a target value.        *
 *                                                                                             *
 *    Use this routine to take this trigger type class object and convert it into a            *
 *    target number.                                                                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the target number that represents this trigger type class object.     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
TARGET TriggerTypeClass::As_Target(void) const
{
	return(Build_Target(RTTI_TRIGGERTYPE, ID));
}


/***********************************************************************************************
 * TriggerTypeClass::Detach -- Removes attachments to the target object specified.             *
 *                                                                                             *
 *    When an object disappears from the game, it must be detached from all other objects that *
 *    may be referring to it. This routine will detach the specified target object from any    *
 *    references to it in this trigger type class.                                             *
 *                                                                                             *
 * INPUT:   target   -- The target object to be detached from this trigger type.               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void TriggerTypeClass::Detach(TARGET target, bool)
{
	Action1.Detach(target);
	Action2.Detach(target);
}


/***********************************************************************************************
 * TriggerTypeClass::Attaches_To -- Determines what trigger can attach to.                     *
 *                                                                                             *
 *    This routine will examine the trigger events and return with a composit bitfield that    *
 *    indicates what this trigger can be attached to. This is used for trigger placement       *
 *    and logic processing.                                                                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with AttachType bitfield representing what this trigger can be attached    *
 *          to.                                                                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/30/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
AttachType TriggerTypeClass::Attaches_To(void) const
{
	AttachType attach = ::Attaches_To(Event1.Event);

	if (EventControl != MULTI_ONLY) {
		attach = attach | ::Attaches_To(Event2.Event);
	}
	return(attach);
}


/***********************************************************************************************
 * TriggerTypeClass::Read_INI -- reads triggers from the INI file                              *
 *                                                                                             *
 *    INI entry format:                                                                        *
 *      Triggername = Eventname, Actionname, Data, Housename, TeamName, IsPersistant           *
 *                                                                                             *
 * This routine reads in the triggers & creates them. Then, other classes can                  *
 * get pointers to the triggers they're linked to.                                             *
 *                                                                                             *
 * The routine relies on the TeamTypeClasses already being loaded so it can resolve            *
 * references to teams in this function.                                                       *
 *                                                                                             *
 * Cell Trigger pointers & IsTrigger flags are set in DisplayClass::Read_INI(),                *
 * and cleared in the Map::Init() routine (which clears all cell objects to 0's).              *
 *                                                                                             *
 * Object's pointers are set in:                                                               *
 *      InfantryClass::Read_INI()                                                              *
 *      BuildingClass::Read_INI()                                                              *
 *      UnitClass::Read_INI()                                                                  *
 *      TerrainClass::Read_INI()                                                               *
 * The object trigger pointers are cleared in the ObjectClass constructor.                     *
 *                                                                                             *
 * The House's EMSListOf triggers is set in this routine, and cleared in the                   *
 * HouseClass::Init() routine.                                                                 *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      buffer      buffer to hold the INI data                                                *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      This function must be called before any other class's Read_INI.                        *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/28/1994 BR : Created.                                                                  *
 *=============================================================================================*/
void TriggerTypeClass::Read_INI(CCINIClass & ini)
{
	TriggerTypeClass *trigger;				// Working trigger pointer.
	char buf[128];

	int len = ini.Entry_Count(INI_Name());
	for (int index = 0; index < len; index++) {
		char const * entry = ini.Get_Entry(INI_Name(), index);

		/*
		**	Create a new trigger.
		*/
		trigger = new TriggerTypeClass();

		/*
		**	Get the trigger entry.
		*/
		ini.Get_String(INI_Name(), entry, NULL, buf, sizeof(buf));

		/*
		**	Fill in the trigger.
		*/
		trigger->Fill_In((char *)entry, buf);
	}

	if (NewINIFormat < 2) {
		/*
		**	Fix up the self-referential trigger pointers.
		*/
		for (int trig_index = 0; trig_index < TriggerTypes.Count(); trig_index++) {
			TriggerTypeClass * trigger = TriggerTypes.Ptr(trig_index);

			char * ptr = (char *)trigger->Action1.Trigger.Raw();
			if (ptr /*&& trigger->Action1.Trigger.Raw() != -1*/) {
				trigger->Action1.Trigger = TriggerTypeClass::From_Name(ptr);
				free(ptr);
			}

			ptr = (char *)trigger->Action2.Trigger.Raw();
			if (ptr /*&& trigger->Action2.Trigger.Raw() != -1*/) {
				trigger->Action2.Trigger = TriggerTypeClass::From_Name(ptr);
				free(ptr);
			}
		}
	}
}


/***********************************************************************************************
 * TriggerTypeClass::Fill_In -- fills in trigger from the given INI entry                      *
 *                                                                                             *
 * This routine fills in the given trigger with the given name, and values from                *
 * the given INI entry.                                                                        *
 *                                                                                             *
 * (This routine is used by the scenario editor, to import teams from the MASTER.INI file.)    *
 *                                                                                             *
 *    INI entry format:                                                                        *
 *      Triggername = Eventname, Actionname, Data, Housename, TeamName, IsPersistant           *
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
 *=============================================================================================*/
void TriggerTypeClass::Fill_In(char * name, char * entry)
{
	assert(TriggerTypes.ID(this) == ID);

	/*
	**	Set its name.
	*/
	Set_Name(name);

	IsPersistant = PersistantType(atoi(strtok(entry, ",")));
	House = HousesType(atoi(strtok(NULL, ",")));
	EventControl = MultiStyleType(atoi(strtok(NULL, ",")));
	ActionControl = MultiStyleType(atoi(strtok(NULL, ",")));

	Event1.Read_INI();
	Event2.Read_INI();
	Action1.Read_INI();
	Action2.Read_INI();
}


/***********************************************************************************************
 * TriggerTypeClass::Write_INI -- Stores all trigger types to the INI database specified.      *
 *                                                                                             *
 *    This routine will write out all trigger type objects to the INI database. Any existing   *
 *    trigger types in the database will be cleared out.                                       *
 *                                                                                             *
 * INPUT:   ini   -- Reference to the INI database to have the trigger types added.            *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void TriggerTypeClass::Write_INI(CCINIClass & ini)
{
	ini.Clear("Triggers");
	ini.Clear(INI_Name());

	/*
	**	Now write all the trigger data out
	*/
	for (int index = 0; index < TriggerTypes.Count(); index++) {
//	for (int index = TriggerTypes.Count()-1; index >= 0; index--) {
		char buf[256];
		TriggerTypeClass * trigger = TriggerTypes.Ptr(index);

		trigger->Build_INI_Entry(buf);
		ini.Put_String(INI_Name(), trigger->IniName, buf);
	}
}


/***********************************************************************************************
 * TriggerTypeClass::Build_INI_Entry -- Construct the INI entry into the buffer specified.     *
 *                                                                                             *
 *    This low level routine will take the information in this trigger type and store it       *
 *    into a buffer such that the resultant string can be stored into an INI database for      *
 *    later retrieval.                                                                         *
 *                                                                                             *
 * INPUT:   buffer   -- Pointer to the buffer to store the INI entry string.                   *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Be sure the buffer is big enough. Usually 128 bytes is more than sufficient.    *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void TriggerTypeClass::Build_INI_Entry(char * buffer) const
{
	/*
	**	Build the root portion of the trigger event.
	*/
	sprintf(buffer, "%d,%d,%d,%d,", IsPersistant, House, EventControl, ActionControl);

	/*
	**	Append the event and action values.
	*/
	buffer += strlen(buffer);
	Event1.Build_INI_Entry(buffer);

	strcat(buffer, ",");
	buffer += strlen(buffer);
	Event2.Build_INI_Entry(buffer);

	strcat(buffer, ",");
	buffer += strlen(buffer);
	Action1.Build_INI_Entry(buffer);

	strcat(buffer, ",");
	buffer += strlen(buffer);
	Action2.Build_INI_Entry(buffer);
}


/***********************************************************************************************
 * TriggerTypeClass::Init -- Initialize the trigger type object management system.             *
 *                                                                                             *
 *    This routine should be called to initialize the trigger type object system. It should    *
 *    be called when clearing out a scenario.                                                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   All trigger types will be destroyed by this routine.                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void TriggerTypeClass::Init(void)
{
	TriggerTypes.Free_All();
}


/***********************************************************************************************
 * TriggerTypeClass::From_Name -- Convert an ASCII name into a trigger type pointer.           *
 *                                                                                             *
 *    Given just an ASCII representation of the trigger type, this routine will return with    *
 *    a pointer to the trigger type it refers to. Typical use of this is when parsing          *
 *    scenario INI files.                                                                      *
 *                                                                                             *
 * INPUT:   name  -- Pointer to the name to use to identify the trigger type class object to   *
 *                   be looked up.                                                             *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the trigger type class object that matches the name      *
 *          specified. If no match could be found, then NULL is returned.                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
TriggerTypeClass * TriggerTypeClass::From_Name(char const * name)
{
	if (name != NULL) {
		for (int index = 0; index < TriggerTypes.Count(); index++) {
			if (stricmp(TriggerTypes.Ptr(index)->Name(), name) == 0) {
				return(TriggerTypes.Ptr(index));
			}
		}
	}
	return(NULL);
}
