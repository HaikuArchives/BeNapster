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

#ifndef PREFS_WINDOW_H
#include "PrefsWindow.h"
#endif

#include <Box.h>

PrefsWindow::PrefsWindow(const char *title, Preferences *myPreferences) :
	BWindow(BRect(20,30,350,380), title, B_TITLED_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL, B_NOT_RESIZABLE)
{

	BMessage *bmDummy; 

	sConnectionType = (char *)calloc(3,1);

	bvMainView = new BBox(Bounds(), "CoveringView", 
					   B_FOLLOW_ALL, 
					   B_NAVIGABLE|B_WILL_DRAW,
					   B_NO_BORDER);
	AddChild(bvMainView);


	BMessage *bmUsername = new BMessage((uint32)0);
	btcUsername = new BTextControl(BRect(10,10,300,25), "txtUsername", "Username: ",myPreferences->GetUser(), bmUsername, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	btcUsername->SetDivider(be_plain_font->StringWidth("Username: "));
	bvMainView->AddChild(btcUsername);

	
	BMessage *bmPassword = new BMessage((uint32)0);
	btcPassword = new BTextControl(BRect(10,50,300,65), "txtPassword", "Password: ",myPreferences->GetPassword(), bmPassword, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	btcPassword->SetDivider(be_plain_font->StringWidth("Username: "));
	bvMainView->AddChild(btcPassword);


	BMessage *bmPort = new BMessage((uint32)0);
	btcPort = new BTextControl(BRect(10,90,300,115), "txtUsername", "Port: ",myPreferences->GetPort(), bmPort, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	btcPort->SetDivider(be_plain_font->StringWidth("Username: "));
	bvMainView->AddChild(btcPort);

	BMessage *bmEmail = new BMessage((uint32)0);
	btcEmail = new BTextControl(BRect(10,130,300,155), "txtEmail", "E-mail: ",myPreferences->GetEmail(), bmEmail, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	btcEmail->SetDivider(be_plain_font->StringWidth("Username: "));
	bvMainView->AddChild(btcEmail);

	BMessage *bmPath = new BMessage((uint32)0);
	btcDownloadPath = new BTextControl(BRect(10,170,300,195), "txtDownloadPath", "Path: ",myPreferences->GetDownloadPath(), bmPath, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	btcDownloadPath->SetDivider(be_plain_font->StringWidth("Username: "));
	bvMainView->AddChild(btcDownloadPath);

	bpmConnection = new BPopUpMenu(myPreferences->GetConnectionText());
	bmfConnection = new BMenuField(BRect(10,210,300,235), "TOVTranslation", "Connction Speed:  ",bpmConnection);
	bmfConnection->SetDivider(be_plain_font->StringWidth("Connction Speed:  "))  ;

	bmDummy = new BMessage(PREFS_CONNECTYION_TYPE);
	bmDummy->AddString("CONNECTION", "0");
	bpmConnection->AddItem(new BMenuItem("Unknown", bmDummy));

	bmDummy = new BMessage(PREFS_CONNECTYION_TYPE);
	bmDummy->AddString("CONNECTION", "1");
	bpmConnection->AddItem(new BMenuItem("14.4 kbps", bmDummy));

	bmDummy = new BMessage(PREFS_CONNECTYION_TYPE);
	bmDummy->AddString("CONNECTION", "2");
	bpmConnection->AddItem(new BMenuItem("28.8 kpbs", bmDummy));

	bmDummy = new BMessage(PREFS_CONNECTYION_TYPE);
	bmDummy->AddString("CONNECTION", "3");
	bpmConnection->AddItem(new BMenuItem("33.6 kbps", bmDummy));

	bmDummy = new BMessage(PREFS_CONNECTYION_TYPE);
	bmDummy->AddString("CONNECTION", "4");
	bpmConnection->AddItem(new BMenuItem("56.7 kbps", bmDummy));

	bmDummy = new BMessage(PREFS_CONNECTYION_TYPE);
	bmDummy->AddString("CONNECTION", "5");
	bpmConnection->AddItem(new BMenuItem("64K ISDN", bmDummy));

	bmDummy = new BMessage(PREFS_CONNECTYION_TYPE);
	bmDummy->AddString("CONNECTION", "6");
	bpmConnection->AddItem(new BMenuItem("128K ISDN", bmDummy));

	bmDummy = new BMessage(PREFS_CONNECTYION_TYPE);
	bmDummy->AddString("CONNECTION", "7");
	bpmConnection->AddItem(new BMenuItem("Cable", bmDummy));

	bmDummy = new BMessage(PREFS_CONNECTYION_TYPE);
	bmDummy->AddString("CONNECTION", "8");
	bpmConnection->AddItem(new BMenuItem("DSL", bmDummy));

	bmDummy = new BMessage(PREFS_CONNECTYION_TYPE);
	bmDummy->AddString("CONNECTION", "9");
	bpmConnection->AddItem(new BMenuItem("T1", bmDummy));

	bmDummy = new BMessage(PREFS_CONNECTYION_TYPE);
	bmDummy->AddString("CONNECTION", "10");
	bpmConnection->AddItem(new BMenuItem("T3 or greater", bmDummy));

	bvMainView->AddChild(bmfConnection);

	BMessage *bmSave = new BMessage(PREFS_SAVE);
	bbSave = new BButton(BRect(10, 250, 100, 285), "cmdSave", "Save", bmSave, B_FOLLOW_NONE, B_NAVIGABLE|B_WILL_DRAW);
	bvMainView->AddChild(bbSave);

	BMessage *bmCancel = new BMessage(B_QUIT_REQUESTED);
	bbCancel = new BButton(BRect(210, 250, 300, 275), "cmdCancel", "Cancel", bmCancel, B_FOLLOW_NONE, B_NAVIGABLE|B_WILL_DRAW);
	bvMainView->AddChild(bbCancel);

	btcShareDir = new BTextControl(BRect(10,280,200,235), "txtShared",
								   "Shared MP3's Directory: ",
								   myPreferences->GetShareDir(),
								   NULL,
								   B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	btcShareDir->SetDivider(be_plain_font->StringWidth("Shared MP3's Directory: "));
	
	// The "Browse" Button
	
	bvMainView->AddChild(btcShareDir);	
	
	browseButton = new BButton(BRect(210, 280, 300, 305), "Browse", "Browse",
	(new BMessage(BENAPSTER_BROWSE)));
	bvMainView->AddChild(browseButton);
	
	myTempPrefs = 	myPreferences; 
}


void	PrefsWindow::MessageReceived(BMessage *bmMessage)
{
	// Used for the browse window
	BPath *myPath;
	
	BEntry    *beDropped;
	BEntry    *beTraversed;
	BPath     bpDropped;
	entry_ref erDropped;

	switch ( bmMessage->what )
	{
		case B_SIMPLE_DATA:
			if(B_OK == bmMessage->FindRef("refs", &erDropped))
			{
				beDropped = new BEntry(&erDropped);
				if(beDropped->IsDirectory())
				{
					beDropped->GetPath(&bpDropped);
					btcDownloadPath->SetText(bpDropped.Path());
				}
				else
				{
					if(beDropped->IsSymLink())
					{
						beTraversed = new BEntry(&erDropped, true);
						if(beTraversed->IsDirectory())
						{
							beDropped->GetPath(&bpDropped);
							btcDownloadPath->SetText(bpDropped.Path());
						}
					}
				}
			}
			break;					
		case PREFS_CONNECTYION_TYPE:
			memcpy(sConnectionType, bmMessage->FindString("CONNECTION"),2);
			break;
		case BENAPSTER_BROWSE:
			openPanel = new OpenDirPanel(new BMessenger(NULL,this), openFilter);
			openPanel->Show();	
			break;
		case B_REFS_RECEIVED:
			bmMessage->FindRef("refs", 0, &shareRef);
			myPath = new BPath((new BEntry(&shareRef)));
			btcShareDir->SetText(myPath->Path());			
			break;
		case PREFS_SAVE:
			myTempPrefs->SetUser(btcUsername->Text());
			myTempPrefs->SetPassword(btcPassword->Text());
			myTempPrefs->SetPort(btcPort->Text());
			myTempPrefs->SetConnection(sConnectionType);
			myTempPrefs->SetEmail(btcEmail->Text());			
			myTempPrefs->SetShareDir(btcShareDir->Text());
			myTempPrefs->SetDownloadPath(btcDownloadPath->Text());			
			myTempPrefs->SaveFile();	
			myTempPrefs->InitPreferences();
			PostMessage(B_QUIT_REQUESTED);
			break;		
		default:
// call inherited if you did not handle message
			BWindow::MessageReceived(bmMessage);
			break;			
	}
}
