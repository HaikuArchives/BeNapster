#ifndef FIND_WINDOW_H
#define FIND_WINDOW_H

#include <Window.h>
#include <View.h>
#include <ScrollView.h>
#include <ListView.h>
#include <Button.h>
#include <TextControl.h>

#include "Napster.h"
#include "Constants.h"
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

#include <stdlib.h>


class FindWindow : public BWindow 
{
public:
	FindWindow(BRect frame, const char *, window_look, window_feel, uint32, uint32, BLooper *); 
//	~FindWindow();
	
	void	MessageReceived(BMessage *);
	void    UnlockFind(void);
	void    LockFind(void);   
	void 	AddToList(char *, uint16);

//	void	LogMessage(char *, int32);
//	bool	QuitRequested();

private:
	BView *bvMainView;
	BListView *blvMp3s;
	BScrollView *bsvListView;
	BTextControl *myArtist;
	BTextControl *mySong;
	BTextControl *myMax;
	BButton *myFind;
	BButton *myGet;
	BLooper	*myLooper;
	
	char *CreateFindString(char *);
	char *CreateGetString(const char *, char *);

	bool bCanDoFind;
};

#endif
