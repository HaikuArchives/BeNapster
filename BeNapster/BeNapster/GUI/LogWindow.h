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

#ifndef LOG_WINDOW_H
#define LOG_WINDOW_H

#include <Alert.h>
#include <Application.h>
#include <ScrollView.h>
#include <Screen.h>
#include <TextView.h>
#include <Window.h>

#include "Napster.h"
#include "Constants.h"
#include "Comms/CommsThreads.h"
#include "GUI/FindWindow.h"
#include "GUI/DownloadWindow.h"
#include "GUI/PrefsWindow.h"
#include "Preferences/Preferences.h"

#include <stdlib.h>


class LogWindow : public BWindow 
{
public:
	LogWindow(BRect, const char *, window_look, window_feel, uint32, uint32); 
	~LogWindow();
	
	void	MessageReceived(BMessage *);
	void 	ActOnMessage(uint16, uint16, char *);
	bool	QuitRequested();
	void    Connect(void);

	bool 		bSomeThingToSend, bRegistered;
	char		*mySendThis;
	uint16      myMessageType;
	Napster     *myNapster;
	Preferences *myPreferences;

private:
	BMenuBar			*myLogMenuBar; 
	BScrollView 		*scrollTextView;
	BTextView 			*txtLogText;
	
	FindWindow			*myFindWindow;
	PrefsWindow			*pwPreferences; 

	thread_id 	tidInLoop;
	thread_id 	tidOutLoop;
	thread_id   tidDownload[3];
	thread_id   tidReceive;
	
	void		NewDownload(char *, uint16);
	void 		ShowPrefsWindow(const char *);
	void		LogMessage(char *, int32);
	void		ShowGNU(void);
	bool		bFindWindow;
	bool		bNapster;
	
	int32		iTidDownloadCount;

};

#endif //LOG_WINDOW_H
