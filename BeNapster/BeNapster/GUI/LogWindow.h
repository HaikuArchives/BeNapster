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
	void	BeginUpload(char *);
	void		LogMessage(const char *, uint16);
	
	bool 		bSomeThingToSend, bRegistered;
	char		*mySendThis;
	
	uint16      myMessageType;
	Napster     *myNapster;
	Preferences *myPreferences;
	BNetEndpoint	*netEndpoint;
	
private:
	BMenuBar			*myLogMenuBar; 
	BScrollView 		*scrollTextView;
	BTextView 			*txtLogText;
	
	BDirectory			*shareDir;
	
	FindWindow			*myFindWindow;
	PrefsWindow			*pwPreferences; 

	thread_id 	tidInLoop;
	thread_id 	tidOutLoop;
	thread_id   tidDownload[3];
	thread_id   tidReceive;
	thread_id	shareThread;
		
	void		NewDownload(char *, uint16);
	void 		ShowPrefsWindow(const char *);
	void		ShowGNU(void);
	void		SetStatusColor(rgb_color);
	void		SetErrorColor(rgb_color);
	void		SetMessageColor(rgb_color mc);
	void		ShareMP3s(void);
	int32		FindAllMP3s(void*);
	
	// Where we will store color codes for different messages in the
	// log window
	rgb_color statusColor, errorColor, messageColor;
	
	int32		iTidDownloadCount;
	bool		inSubdir;

};

#endif //LOG_WINDOW_H
