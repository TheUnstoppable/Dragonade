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
#include "General.h"
#include "CommandLineParser.h"


CommandLineParser::CommandLineParser(const char* _command)
{
	// Copy the command
	command = new char[strlen(_command) + 1];
	strcpy(command, _command);

	// Set the current command iterator to the start of the buffer
	commandIter = command;
}

CommandLineParser::~CommandLineParser()
{
	// Free the buffer
	delete[] command;
}

bool CommandLineParser::skipWhitespace()
{
	// Loop while the character is non-printing or whitespace
	for (; *commandIter <= ' '; commandIter++)
	{
		// Return false if the end of the string was reached
		if (*commandIter == '\0')
			return false;
	}
	return true;
}

void CommandLineParser::skipToken()
{
	// Loop while the character is printing and not whitespace
	for (; *commandIter > ' '; commandIter++)
		;
}

void CommandLineParser::skip()
{
	skipWhitespace();
	skipToken();
}

const char* CommandLineParser::getString()
{
	// Skip whitespace and return if the command ends here
	if (!skipWhitespace())
		return NULL;

	// Get the token starting at this position
	const char* string = commandIter;

	// Skip nonprintable characters
	skipToken();

	// If the current iterator does not point to the end of the string, end the string and increase the iterator pointer
	if (*commandIter != '\0')
	{
		*commandIter++ = '\0';
	}

	// Return the string
	return string;
}

const char* CommandLineParser::getRemainingString()
{
	// Skip whitespace and return if the command ends here
	if (!skipWhitespace())
		return NULL;

	// Return the string
	return commandIter;
}

int CommandLineParser::getInt()
{
	// Get the string if possible
	const char* string = getString();
	if (!string)
		return -1;

	// Check whether the string is a valid number
	for (const char* s = string; *s != '\0'; ++s)
		if (*s < '0' || *s > '9')
			return -1;

	// Return the converted string
	return atoi(string);
}

double CommandLineParser::getDouble()
{
	// Get the string if possible
	const char* string = getString();
	if (!string)
		return 0;

	// Return the converted string
	return atof(string);
}
