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

#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <File.h>
#include <FindDirectory.h>
#include <Looper.h>
#include <Path.h>


#include <stdlib.h>
#include <string.h>

#include "Constants.h"

class Preferences
{

public:
	Preferences(BLooper *);
	
	bool			InitPreferences(void);
	void			SaveFile(void);
	char            *pDownloadDetails;

	void 			SetUser(const char *);
	void    		SetPassword(const char *);
	void			SetPort(const char *);
	void			SetConnection(const char *);			
	void			SetEmail(const char *);			
	void			SetDownloadPath(const char *);			

	const char 		*GetUser(void);
	const char 		*GetPassword(void);
	const char 		*GetPort(void);
	const char 		*GetConnection(void);
	const char		*GetConnectionText(void);			
	const char 		*GetEmail(void);
	const char		*GetDownloadPath(void);

private:

	char 			*sUser;
	char			*sPassword;
	char			*sPort;
	char			*sConnection;
	char			*sEmail;
	char			*sDownloadPath;
	BLooper			*blLogWindow;
	
	char            *GetPref(char *, char *, char *);
};

#endif //PREFS_WINDOW_H
