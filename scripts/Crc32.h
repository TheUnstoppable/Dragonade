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
#pragma once

class FileClass;
class StringClass;



unsigned long SCRIPTS_API CRC_Memory(const unsigned char* data, unsigned long length, unsigned long crc = 0);
unsigned long SCRIPTS_API CRC_String(const char *data,unsigned long crc);
unsigned long SCRIPTS_API CRC_Stringi(char  const*, unsigned long = 0);
unsigned long SCRIPTS_API CRC_File(FileClass *f, uint32 seed = 0);
