//BeNapster - Napster Client for the Be Operating system
//Copyright(C) 2000 David Burnett
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

#ifndef NAPSTER_H
#define NAPSTER_H



#include <NetAddress.h>
#include <NetEndpoint.h>
#include <NetBuffer.h>
#include <String.h>
#include <File.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "Constants.h"

class Napster
{

public:
	Napster();

	BNetAddress  bnaNapsterServer;
	BNetEndpoint bneNapsterServer;
	bool 		 bConnected;

	char   		*GetServer(char *);

	void		Connect(const char *, int32);
	void 		Register(const char *);
	void 		FirstLogin(const char *, const char *, const char *, const char *, const char *, const char *);
	void 	 	Login(const char *, const char *, const char *, const char *, const char *);
	void	 	Send(const char *, uint16);
	bool        Quit(void); 

private:
	bool         myQuit;
	

};


#endif