/*	Renegade Scripts.dll
	Copyright 2017 Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/
#ifndef TT_INCLUDE_NETWORKOBJECTCLASS_H
#define TT_INCLUDE_NETWORKOBJECTCLASS_H

class BitStreamClass;
class Vector3;
class StringClass;

enum PACKET_TIER_ENUM
{
	PACKET_TIER_CREATION,
	PACKET_TIER_RARE,
	PACKET_TIER_OCCASIONAL,
	PACKET_TIER_FREQUENT,

	PACKET_TIER_COUNT
};
class NetworkObjectClass
{
public:
	typedef enum
	{
		BIT_MASK_FREQUENT   = 0x01,
		BIT_MASK_OCCASIONAL = 0x02,
		BIT_MASK_RARE       = 0x04,
		BIT_MASK_CREATION   = 0x08,

		BIT_FREQUENT        = 0x01,
		BIT_OCCASIONAL      = 0x03,
		BIT_RARE            = 0x07,
		BIT_CREATION        = 0x0f,
		
		BIT_SUPPRESS_UPDATES = 0x10, // Set when the local end should not send updates to the remote end. This happens if the remote end created the object (by receiving a BIT_CREATION message), when the remote end has been informed that the object has been deleted, or when an object has not yet been created on the remote end.
	} DIRTY_BIT;
	enum
	{
		MAX_CLIENT_COUNT	= 128
	};
	NetworkObjectClass (void);
	virtual ~NetworkObjectClass (void);
	int					Get_Network_ID (void) const								{ return NetworkID; }
	void					Set_Network_ID (int id);
#ifdef TTLE_EXPORTS
	int					Get_Created_By_Packet_ID (void) const					{ return CreatedByPacketID; }
	void					Set_Created_By_Packet_ID (int id);
#endif
	virtual uint32		Get_Network_Class_ID (void) const						{ return 0; }
	virtual void		Import_Creation (BitStreamClass &packet)				{}
	virtual void		Import_Rare (BitStreamClass &packet)					{}
	virtual void		Import_Occasional (BitStreamClass &packet)			{}
	virtual void		Import_Frequent (BitStreamClass &packet)				{}
	virtual void		Export_Creation (BitStreamClass &packet)				{}
	virtual void		Export_Rare (BitStreamClass &packet)					{}
	virtual void		Export_Occasional (BitStreamClass &packet)			{}
	virtual void		Export_Frequent (BitStreamClass &packet)				{}
	virtual void		Network_Think (void) {}
	bool					Is_Delete_Pending (void)	{ return IsDeletePending; }
	virtual void		Set_Delete_Pending (void);
	void                Set_Is_Delete_Pending (bool pending) { IsDeletePending = pending; }
	virtual void		Delete (void) = 0;
	void					Set_App_Packet_Type (BYTE type)	{ AppPacketType = type; }
	BYTE					Get_App_Packet_Type (void)			{ return AppPacketType; }
	virtual void		Set_Object_Dirty_Bit (DIRTY_BIT dirty_bit, bool onoff);
	virtual void		Set_Object_Dirty_Bit (int client_id, DIRTY_BIT dirty_bit, bool onoff);
	void		Set_Object_Dirty_Bitx(int client_id, DIRTY_BIT dirty_bit, bool onoff);
	virtual void		Clear_Object_Dirty_Bits (void);
	virtual bool		Get_Object_Dirty_Bit (int client_id, DIRTY_BIT dirty_bit);
	virtual BYTE		Get_Object_Dirty_Bits (int client_id);
	virtual void		Set_Object_Dirty_Bits (int client_id, BYTE bits);
	virtual bool		Is_Client_Dirty (int client_id);
	void Set_Dirty_Bits(uint8 dirtyBits);
	inline bool			Get_Object_Dirty_Bit_2 (int client_id, DIRTY_BIT dirty_bit)
	{
		return ((ClientStatus[client_id] & dirty_bit) == dirty_bit);
	}
	inline BYTE			Get_Object_Dirty_Bits_2 (int client_id)
	{
		return ClientStatus[client_id];
	}
	virtual int			Get_Vis_ID (void)																		{ return -1; }
	virtual bool		Get_World_Position (Vector3 &pos) const										{ return false; }
	virtual float		Get_Filter_Distance(void) const													{ return 10000.0f; }
	void					Reset_Client_Hint_Count(int client_id);
	void					Increment_Client_Hint_Count(int client_id);
	void					Hint_To_All_Clients(void);
	BYTE					Get_Client_Hint_Count(int client_id);
	inline BYTE			Get_Client_Hint_Count_2(int client_id)
	{
		return UpdateInfo[client_id].ClientHintCount;
	}
	void					Reset_Import_State_Count (void)					{ ImportStateCount = 0; }
	void					Increment_Import_State_Count (void)				{ ImportStateCount ++; }
	int					Get_Import_State_Count (void)						{ return ImportStateCount; }
	void					Reset_Last_Clientside_Update_Time (void);
	void					Set_Last_Clientside_Update_Time (ULONG time);
	ULONG					Get_Last_Clientside_Update_Time (void)			{ return LastClientsideUpdateTime; }
	TT_DEPRECATED("This function is no longer necessary and the member variable it uses has been hijacked to host a different variable.") int					Get_Clientside_Update_Frequency(void);
	bool					Belongs_To_Client(int client_id);
	unsigned char		Get_Frequent_Update_Export_Size(void)						{return(FrequentExportPacketSize);}
	void					Set_Frequent_Update_Export_Size(unsigned char size)	{FrequentExportPacketSize = size;}
	unsigned long		Get_Last_Update_Time(int client_id);
	unsigned short		Get_Update_Rate(int client_id);
	void					Set_Last_Update_Time(int client_id, unsigned long time);
	void					Set_Update_Rate(int client_id, unsigned short rate);
	virtual bool		Is_Tagged(void)											{ return false; }
	virtual void		Get_Description(StringClass & description)		{}
	void					Set_Unreliable_Override(bool flag)					{UnreliableOverride = flag;}
	bool					Get_Unreliable_Override(void)							{return UnreliableOverride;}
#if !defined(DDBEDIT) && !defined(W3DVIEWER)
	static void			Set_Is_Server(bool flag)								{ IsServer = flag; }
#endif
	void					Set_Cached_Priority (float priority);
	virtual float		Get_Cached_Priority (void) const						{ return CachedPriority; }
	inline void			Set_Cached_Priority_2 (int client_id, float priority)
	{
		CachedPriority_2[client_id] = priority;
	}
	inline float		Get_Cached_Priority_2 (int client_id) const
	{
		return(CachedPriority_2[client_id]);
	}
	void					Set_Last_Object_Id_I_Damaged(int id)				{LastObjectIdIDamaged = id;}
	int					Get_Last_Object_Id_I_Damaged(void) const			{return LastObjectIdIDamaged;}
	void					Set_Last_Object_Id_I_Got_Damaged_By(int id)		{LastObjectIdIGotDamagedBy = id;}
	int					Get_Last_Object_Id_I_Got_Damaged_By(void) const	{return LastObjectIdIGotDamagedBy;}
	
	ULONG getLastHintRequestTime() const { return LastHintRequestTime; }
	void setLastHintRequestTime(ULONG _lastHintRequestTime) { LastHintRequestTime = _lastHintRequestTime; }
private:
	int					NetworkID; // 0004
#ifdef TTLE_EXPORTS
	int					CreatedByPacketID;
#endif
	struct PerClientUpdateInfoStruct {
		unsigned long	LastUpdateTime;
		unsigned short	UpdateRate;
		BYTE				ClientHintCount;
	} UpdateInfo [MAX_CLIENT_COUNT]; // 0008
	BYTE					ClientStatus[MAX_CLIENT_COUNT]; // 0408
	int					ImportStateCount; // 0488
	ULONG					LastClientsideUpdateTime; // 048C
	ULONG					ClientsideUpdateFrequencySampleStartTime; // 0490
	int					ClientsideUpdateFrequencySampleCount; // 0494
	ULONG					LastHintRequestTime; // 0498; originally ClientsideUpdateRate, but this offset has been hijacked to host the current variable instead
	bool					IsDeletePending; // 049C
	BYTE					AppPacketType; // 049D
	int					LastObjectIdIDamaged; // 04A0
	int					LastObjectIdIGotDamagedBy; // 04A4
	unsigned char		FrequentExportPacketSize; // 04A8
	float					CachedPriority; // 04AC
	float					CachedPriority_2[MAX_CLIENT_COUNT]; // 04B0
	bool					UnreliableOverride; // 06B0
#if !defined(DDBEDIT) && !defined(W3DVIEWER)
	static REF_DECL(bool, IsServer);
#endif
}; // 06B1

#endif