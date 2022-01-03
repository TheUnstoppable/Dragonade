/*	Renegade Scripts.dll
    Dragonade Token Class
	Copyright 2017 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#ifndef INCLUDE_DATOKEN
#define INCLUDE_DATOKEN

#include "engine_string.h"
#include "engine_vector.h"

class DATokenParserClass {
public:
	inline DATokenParserClass(const char *Str,char Delim) {
		int Len = strlen(Str)+1;
		String = new char[Len];
		memcpy(String,Str,Len);
		Position = String;
		Delimiter = Delim;
	}
	inline DATokenParserClass(const wchar_t *Str,char Delim) {
		int Len = wcslen(Str)+1;
		String = new char[Len];
		wcstombs(String,Str,Len);
		Position = String;
		Delimiter = Delim;
	}
	inline void Set(const char *Str,char Delim) {
		delete[] String;
		int Len = strlen(Str)+1;
		String = new char[Len];
		memcpy(String,Str,Len);
		Position = String;
		Delimiter = Delim;
	}
	inline void Set(const wchar_t *Str,char Delim) {
		delete[] String;
		int Len = wcslen(Str)+1;
		String = new char[Len];
		wcstombs(String,Str,Len);
		Position = String;
		Delimiter = Delim;
	}
	inline ~DATokenParserClass() {
		delete[] String;
	}
	char *Get_String() {
		char *LastPosition = Position;
		do {
			if (*Position == Delimiter) {
				*Position = '\0';
				if (Position == LastPosition) {
					LastPosition++;
					continue;
				}
				else {
					Position++;
					return LastPosition;
				}
			}
			else if (*Position == '\0') {
				if (Position == LastPosition) {
					return 0;
				}
				else {
					return LastPosition;
				}
			}
		} while (Position++);
		return 0;
	}
	inline const char *Get_Remaining_String() {
		if (*Position == '\0') {
			return 0;
		}
		return Position;
	}
	inline bool Get_Int(int &Buffer) {
		const char *Token = Get_String();
		if (!Token) {
			return false;
		}
		Buffer = atoi(Token);
		return true;
	}
	inline bool Get_UInt(unsigned int &Buffer) {
		const char *Token = Get_String();
		if (!Token) {
			return false;
		}
		Buffer = (unsigned int)atoi(Token);
		return true;
	}
	inline bool Get_Float(float &Buffer) {
		const char *Token = Get_String();
		if (!Token) {
			return false;
		}
		Buffer = (float)atof(Token);
		return true;
	}
	
private:
	char Delimiter;
	char *String;
	char *Position;
};


class DATokenClass {
public:
	inline DATokenClass() {
		Delimiter = ' ';
	}
	
	inline DATokenClass(const DATokenClass &Copy) {
		Tokens = Copy.Tokens;
		Delimiter = Copy.Delimiter;
	}

	inline DATokenClass(const char *Str,int StartPos = 1,char Delim = ' ') {
		Build(Str,StartPos,Delim);
	}

	inline DATokenClass(const wchar_t *Str,int StartPos = 1,char Delim = ' ') {
		Build(Str,StartPos,Delim);
	}

	inline DATokenClass(DATokenParserClass &Parser) {
		Build(Parser);
	}

	inline DATokenClass &operator=(const DATokenClass &That) {
		Tokens = That.Tokens;
		Delimiter = That.Delimiter;
		return *this;
	}
	
	inline DATokenClass &operator=(const char *Str) {
		Build(Str,0,' ');
		return *this;
	}

	inline DATokenClass &operator=(DATokenParserClass &Parser) {
		Build(Parser);
		return *this;
	}

	inline bool operator==(const DATokenClass &That) {
		return (Tokens == That.Tokens);
	}

	inline bool operator!=(const DATokenClass &That) {
		return (Tokens != That.Tokens);
	}
	
	void Build(DATokenParserClass  &Parser) {
		Tokens.Delete_All();
		Tokens.Resize(5);
		Tokens.Add(Parser.Get_Remaining_String());
		while (const char *Token = Parser.Get_String()) {
			Tokens.Add(Token);
		}
	}
	void Build(const char *Str,int StartPos,char Delim) {
		DATokenParserClass Parser(Str,Delim);
		for (int i = 1;i < StartPos;i++) {
			Parser.Get_String();
		}
		Build(Parser);
	}

	void Build(const wchar_t *Str,int StartPos,char Delim) {
		DATokenParserClass Parser(Str,Delim);
		for (int i = 1;i < StartPos;i++) {
			Parser.Get_String();
		}
		Build(Parser);
	}

	void To_Lower() {
		for (int i = 0;i < Tokens.Count();i++) {
			Tokens[i].ToLower();
		}
	}
	
	inline const StringClass &Get_Delimiter() const {
		return Delimiter;
	}

	inline const StringClass &operator[](int Pos) const {
		if (Pos < Tokens.Count()) {
			return Tokens[Pos];
		}
		return Delimiter;
	}

	StringClass operator()(int Start,int End = 0) const {
		if (Tokens.Count() <= Start || Tokens.Count() <= End) {
			return Delimiter;
		}
		if (!Start) {
			Start = 1;
		}
		StringClass Ret;
		if (!End) {
			End = Size();
		}
		for (;Start <= End && Start < Tokens.Count();Start++) {
			Ret += Tokens[Start];
			if (Start != End) Ret += Delimiter;
		}
		return Ret;
	}
	
	inline int Size() const {
		if (Tokens.Count()) {
			return Tokens.Count()-1;
		}
		return 0;
	}
	
	inline void Erase(int Pos) {
		if (Pos < Tokens.Count() && Pos) {
			Tokens.Delete(Pos);
			Tokens[0] = operator()(1);
		}
	}

	inline void Replace(int Pos,const StringClass &New) {
		if (Pos < Tokens.Count() && Pos) {
			Tokens[Pos] = New;
			Tokens[0] = operator()(1);
		}
	}
	
	inline void Add(const StringClass &Text,int Pos = 0) {
		if (!Pos) {
			Tokens.Add(Text);
			Tokens[0] = operator()(1);
		}
		else if (Pos < Tokens.Count()) {
			Tokens.Insert(Pos,Text);
			Tokens[0] = operator()(1);
		}
	}
		
	inline bool Is_Int(int Pos) const {
		if (Pos >= Tokens.Count()) {
			return false;
		}
		for (int i = 0;i < Tokens[Pos].Get_Length();i++) {
			if ((Tokens[Pos][i] < 48 || Tokens[Pos][i] > 57) && Tokens[Pos][i] != '-') {
				return false;
			}
		}
		return true;
	}

	inline bool Is_UInt(int Pos) const {
		if (Pos >= Tokens.Count()) {
			return false;
		}
		for (int i = 0;i < Tokens[Pos].Get_Length();i++) {
			if (Tokens[Pos][i] < 48 || Tokens[Pos][i] > 57) {
				return false;
			}
		}
		return true;
	}

	inline bool Is_Float(int Pos) const {
		if (Pos >= Tokens.Count()) {
			return false;
		}
		for (int i = 0;i < Tokens[Pos].Get_Length();i++) {
			if ((Tokens[Pos][i] < 48 || Tokens[Pos][i] > 57) && Tokens[Pos][i] != '.' && Tokens[Pos][i] != '-') {
				return false;
			}
		}
		return true;
	}

	inline int Find(const StringClass &Text) {
		for (int i = 0;i < Tokens.Count();i++) {
			if (!Tokens[i].Compare_No_Case(Text)) {
				return i;
			}
		}
		return -1;
	}

	inline int Find(uint Hash) {
		for (int i = 0;i < Tokens.Count();i++) {
			if (Tokens[i].GetHash() == Hash) {
				return i;
			}
		}
		return -1;
	}

	inline bool As_Int(int Pos,int &Buffer) const {
		if (Is_Int(Pos)) {
			Buffer = atoi(Tokens[Pos]);
			return true;
		}
		return false;
	}

	inline bool As_UInt(int Pos,unsigned int &Buffer) const {
		if (Is_UInt(Pos)) {
			Buffer = atoi(Tokens[Pos]);
			return true;
		}
		return false;
	}

	inline bool As_Float(int Pos,float &Buffer) const {
		if (Is_Float(Pos)) {
			Buffer = (float)atof(Tokens[Pos]);
			return true;
		}
		return false;
	}

	inline WideStringClass As_Wide(int Pos) const {
		if (Pos < Tokens.Count()) {
			return Tokens[Pos].Peek_Buffer();
		}
		return Delimiter.Peek_Buffer();
	}

private:
	DynamicVectorClass<StringClass> Tokens;
	StringClass Delimiter;
};

inline StringClass Get_Token(const char *Str,int Pos,char Delim = ' ') {
	DATokenParserClass Parser(Str,Delim);
	for (int i = 1;i < Pos;i++) {
		Parser.Get_String();
	}
	return Parser.Get_String();
}

inline StringClass Get_Token(const wchar_t *Str,int Pos,char Delim = ' ') {
	DATokenParserClass Parser(Str,Delim);
	for (int i = 1;i < Pos;i++) {
		Parser.Get_String();
	}
	return Parser.Get_String();
}

#endif