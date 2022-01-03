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

class WideStringClass;
struct ConnectionRequest;


class ConnectionAcceptanceFilter
{
public:
	enum STATUS
	{
		// This filter has not yet decided about the acceptance of this client;
		// should be returned from getStatus it is waiting for more data.
		STATUS_INDETERMINATE,

		// This filter has rejected the client. If any filter returns this value
		// from getStatus, this client will be refused.
		STATUS_REFUSING,

		// The filter has accepted the client; if all filters eventually
		// return this from getStatus, the client will be accepted.
		STATUS_ACCEPTING,
	};
	
	// Called once for each request, before any other functions are called.
	virtual void handleInitiation(const ConnectionRequest& connectionRequest) = 0;

	// Called at most once for each request, when this filter accepted or refused the request, as well as after the call to handleCancellation.
	virtual void handleTermination(const ConnectionRequest& connectionRequest) = 0;

	// Called at most once for each request, when the request is cancelled due to one of the filters rejecting the request, or due to a broken connection.
	virtual void handleCancellation(const ConnectionRequest& connectionRequest) = 0;

	// Called every time the connection is checked for acceptance. connectionRequest may be updated in subsequent calls to include additional information that was fetched.
	// Iff the returned status is STATUS_REFUSED, refusalMessage will be shown to the user.
	virtual STATUS getStatus(const ConnectionRequest& connectionRequest, WideStringClass& refusalMessage) = 0;
};
