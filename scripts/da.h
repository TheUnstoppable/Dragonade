/*	Renegade Scripts.dll
    Dragonade
	Copyright 2017 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#ifndef INCLUDE_DA
#define INCLUDE_DA

#include "da_string.h"
#include "da_token.h"
#include "engine_tt.h"

class cPlayer;
class SoldierGameObj;
class ScriptableGameObj;
typedef ScriptableGameObj GameObject;

#define COLORWHITE 255,255,240
#define COLORGRAY 178,178,178
#define COLORLIGHTBLUE 0,255,255
#define COLORGREEN 0,229,51
#define COLORBROWN 210,105,30
#define COLORPURPLE 210,115,247

#define Format_String(Buffer) { Buffer[255] = '\0'; va_list arg_list; va_start(arg_list,Format); vsnprintf(Buffer,255,Format,arg_list); va_end(arg_list); };
#define Format_String_Prefix(Buffer) { Buffer[255] = '\0'; va_list arg_list; va_start(arg_list,Format); strcpy(Buffer,MessagePrefix); vsnprintf(Buffer+MessagePrefix.Get_Length(),255-MessagePrefix.Get_Length(),Format,arg_list); va_end(arg_list); };

class DA_API DA {
public:
	static const char *Get_Version();
	static void Init();
	static void Shutdown();

	static void Host_Message(const char *Format,...); //Sends a host message to all players.
	static void Team_Host_Message(int Team,const char *Format,...); //Sends a host message to a team. Message will be in that team's color.
	static void Private_Host_Message(cPlayer *Player,const char *Format,...); //Sends a host message to a single player.
	static void Private_Host_Message(int Player,const char *Format,...);
	static void Private_Host_Message(GameObject *Player,const char *Format,...);

	static void Admin_Message(const char *Format,...); //Sends an admin message to all players.
	static void Team_Admin_Message(int Team,const char *Format,...); //Sends an admin message to a team.
	static void Private_Admin_Message(cPlayer *Player,const char *Format,...); //Sends an admin message to a single player.
	static void Private_Admin_Message(int Player,const char *Format,...);
	static void Private_Admin_Message(GameObject *Player,const char *Format,...);
	
	static void Player_Message(cPlayer *Player,const char *Format,...); //Sends a message as a player to all players.
	static void Player_Message(int Player,const char *Format,...);
	static void Player_Message(GameObject *Player,const char *Format,...);
	static void Team_Player_Message(cPlayer *Player,const char *Format,...); //Sends a message as a player to all players on that player's team.
	static void Team_Player_Message(int Player,const char *Format,...);
	static void Team_Player_Message(GameObject *Player,const char *Format,...);

	static void Page_Team(int Team,const char *Format,...); //Sends a page to an entire team. Page will be light blue for players with scripts.
	static void Page_Team_Except(int Team,cPlayer *Except,const char *Format,...); //Sends a page to an entire team, except for the given player. Page will be light blue for players with scripts.

	static void Page_Player(cPlayer *Player,const char *Format,...); //Sends a page to a single player. Page will be light blue for players with scripts.
	static void Page_Player(int Player,const char *Format,...);
	static void Page_Player(GameObject *Player,const char *Format,...);

	static void Color_Message(unsigned int Red,int unsigned Green,int unsigned Blue,const char *Format,...); //Sends a colored message to all players. Will be white for players without scripts.
	static void Color_Message_With_Team_Color(int Team,const char *Format,...); //Sends a colored message using the color of the given team to all players. Works for players without scripts.

	static void Team_Color_Message(int Team,unsigned int Red,int unsigned Green,int unsigned Blue,const char *Format,...); //Sends a colored message to a team. Will be white for players without scripts.
	static void Team_Color_Message_With_Team_Color(int Team,const char *Format,...); //Sends a colored message to a team in that team's color. Works for players without scripts.

	static void Private_Color_Message(cPlayer *Player,unsigned int Red,unsigned int Green,unsigned int Blue,const char *Format,...); //Sends a colored message to a single player. Will be white for players without scripts.
	static void Private_Color_Message(int Player,unsigned int Red,unsigned int Green,unsigned int Blue,const char *Format,...);
	static void Private_Color_Message(GameObject *Player,unsigned int Red,unsigned int Green,unsigned int Blue,const char *Format,...);
	static void Private_Color_Message_With_Team_Color(cPlayer *Player,int Team,const char *Format,...); //Sends a colored message to a single player. Works for players without scripts.
	static void Private_Color_Message_With_Team_Color(int Player,int Team,const char *Format,...);
	static void Private_Color_Message_With_Team_Color(GameObject *Player,int Team,const char *Format,...);

	static void Create_2D_Sound(const char *Sound); //Plays a WAV sound file for all players, even ones without scripts. There must be a string associated with the sound file for stock clients to hear it.
	static void Create_2D_Sound_Team(int Team,const char *Sound); //Plays a WAV sound file for all players on the given team, even ones without scripts. There must be a string associated with the sound file for stock clients to hear it.
	static void Create_2D_Sound_Player(cPlayer *Player,const char *Sound);  //Plays a WAV sound file for the given player, even if they aren't running scripts. There must be a string associated with the sound file for stock clients to hear it.
	static void Create_2D_Sound_Player(int Player,const char *Sound);
	static void Create_2D_Sound_Player(GameObject *Player,const char *Sound);
	
	static void HUD_Message(unsigned int Red,int unsigned Green, int unsigned Blue, const char *Format, ...); //Sends a HUD message to all players.
	static void Team_HUD_Message(int Team,unsigned int Red, int unsigned Green, int unsigned Blue, const char *Format, ...); //Sends a HUD message to a team.
	static void Private_HUD_Message(cPlayer *Player,unsigned int Red, int unsigned Green, int unsigned Blue, const char *Format, ...); //Sends a HUD message to a single player.
	static void Private_HUD_Message(int Player,unsigned int Red, int unsigned Green, int unsigned Blue, const char *Format, ...);
	static void Private_HUD_Message(GameObject *Player,unsigned int Red, int unsigned Green, int unsigned Blue, const char *Format, ...);

	static const StringClass &Get_Message_Prefix() {
		return MessagePrefix;
	}

	static inline DynamicVectorClass<ConsoleFunctionClass*> &Get_Console_Function_List() {
		return ConsoleFunctions;
	}

private:
	static StringClass MessagePrefix;
	static DynamicVectorClass<ConsoleFunctionClass*> ConsoleFunctions;
};

template <class T> class DAConsoleFunctionRegistrant {
public:
	DAConsoleFunctionRegistrant() {
		DA::Get_Console_Function_List().Add(new T);
	}
};
#define Register_Console_Function(ClassName) DAConsoleFunctionRegistrant<ClassName> ClassName##Registrant;

DA_API void DebugMsg(const char *Format,...);

DA_API int Get_Random_Int(int Min,int Max);
DA_API float Get_Random_Float(float Min,float Max);
DA_API bool Get_Random_Bool();

inline float Diff(float First, float Second) {
	if (First > Second) {
		return First - Second;
	}
	else {
		return Second - First;
	}
}

inline int Diff(int First, int Second) {
	if (First > Second) {
		return First - Second;
	}
	else {
		return Second - First;
	}
}

inline StringClass Format_Time(unsigned long Time) {
	return StringFormat("%02d.%02d.%02d",Time/3600,(Time/60)%60,Time%60);
}

inline StringClass Long_To_IP(unsigned long IP) {
	return StringFormat("%d.%d.%d.%d",0xFF & IP,(0xFF00 & IP) >> 8,(0xFF0000 & IP) >> 16,((0xFF000000 & IP) >> 24));
}

inline float Round(float Number) {
	return floor(Number+0.5f);
}

#endif