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
#include "LogWindow.h"
#endif


#include <TextView.h>

LogWindow::LogWindow(BRect frame, const char *title, 
	window_look look, window_feel feel, uint32 flags, uint32 workspaces) :
	BWindow(frame, title, look, feel, flags, workspaces)
{

	BMenu 		*bmuTemp;
	BMessage 	*bmMsg;
	BMenuItem	*bmiTemp;
	
	bSomeThingToSend = false;
	bFindWindow = false;
	bNapster = false;
	iTidDownloadCount = 0;

	BRect rectWinFrame = Bounds();
	BRect brMenuFrame = Bounds();

	myPreferences = new Preferences(this);
	if(myPreferences->InitPreferences())
	{
		bRegistered = true;
	}
	else
	{
		ShowPrefsWindow("Registration");
		bRegistered = false;
	}

	brMenuFrame.bottom = brMenuFrame.top + 19;
	
	myLogMenuBar = new BMenuBar(brMenuFrame, "BeNapster Menu", B_FOLLOW_LEFT_RIGHT|B_FOLLOW_TOP, B_ITEMS_IN_ROW, true);
	//first menu....
	bmuTemp = new BMenu("Server");

	bmMsg = new BMessage(BENAPSTER_REGISTER_WINDOW);
	bmiTemp = new BMenuItem("Register",bmMsg,'R'); 	
	bmuTemp->AddItem(bmiTemp);

	bmMsg = new BMessage(BENAPSTER_CONNECT);
	bmiTemp = new BMenuItem("Connect",bmMsg,'S'); 	
	bmuTemp->AddItem(bmiTemp);
	
	bmMsg = new BMessage(BENAPSTER_DISCONNECT);
	bmiTemp = new BMenuItem("Disconnect",bmMsg,'D'); 	
	bmuTemp->AddItem(bmiTemp);	
	
	bmMsg = new BMessage(BENAPSTER_PREFERENCES);
	bmiTemp = new BMenuItem("Preferences",bmMsg,'P'); 	
	bmuTemp->AddItem(bmiTemp);	

	myLogMenuBar->AddItem(bmuTemp);


	bmuTemp = new BMenu("MP3's");

	bmMsg = new BMessage(BENAPSTER_FIND_WINDOW);
	bmiTemp = new BMenuItem("Find",bmMsg,'R'); 	
	bmuTemp->AddItem(bmiTemp);

//	bmMsg = new BMessage(BENAPSTER_RESUME_WINDOW);
//	bmiTemp = new BMenuItem("Resume",bmMsg,'T'); 	
//	bmuTemp->AddItem(bmiTemp);
	
	myLogMenuBar->AddItem(bmuTemp);

	AddChild(myLogMenuBar);

	rectWinFrame.top += 19;   /// allow for Menu...
	rectWinFrame.right -= B_V_SCROLL_BAR_WIDTH;
	rectWinFrame.bottom -=  B_H_SCROLL_BAR_HEIGHT;
	BRect rectText = rectWinFrame;

	txtLogText = new BTextView(rectText, 
								"Log Window",
								rectText,
								B_FOLLOW_ALL_SIDES,
								B_NAVIGABLE|B_PULSE_NEEDED|B_WILL_DRAW);

	rgb_color color = {255, 255, 255, 0};
	
	txtLogText->SetAlignment(B_ALIGN_LEFT);
	txtLogText->SetViewColor(color);
	txtLogText->SetAutoindent(0);
	txtLogText->MakeEditable(FALSE);
	txtLogText->MakeResizable(TRUE);
	txtLogText->MakeSelectable(TRUE);
	txtLogText->SetStylable(0);
	txtLogText->SetWordWrap(TRUE);
	
	ShowGNU();
	
	scrollTextView = new BScrollView("scrollTextView", txtLogText, B_FOLLOW_ALL, B_FRAME_EVENTS, true, true, B_FANCY_BORDER);

	AddChild(scrollTextView); 
	
}
 
LogWindow::~LogWindow()
{

	//shouldn't need any code;
}	

void	LogWindow::MessageReceived(BMessage *bmMessage)
{

	const char *sFindString;

	BScreen *bsScreen;
	BRect brFindWindow;
	
	switch ( bmMessage->what )
	{
		case B_SIMPLE_DATA:
			break;
		case B_QUIT_REQUESTED:
			be_app->PostMessage(B_QUIT_REQUESTED);
			break;
		case BENAPSTER_FIND_WINDOW:
			if(!bFindWindow)
			{
				bsScreen = new BScreen(this);
				brFindWindow = bsScreen->Frame();
				delete(bsScreen);
				brFindWindow.top = 40;
				brFindWindow.bottom = brFindWindow.bottom - 40;
				brFindWindow.left = 30;
				brFindWindow.right -= 30;			 

				myFindWindow = new FindWindow(brFindWindow, 
											  "BeNapster Find",
											  B_MODAL_WINDOW_LOOK,
											  B_NORMAL_WINDOW_FEEL,
											  0,
											  B_CURRENT_WORKSPACE,
											  Looper());
				bFindWindow = true;
			}
			myFindWindow->Show();
			break;
		case BENAPSTER_FIND:
			bmMessage->FindString("FindString",&sFindString);
			mySendThis = (char *)malloc(strlen(sFindString) + 1);
			strcpy(mySendThis, sFindString);
			myMessageType = NAPSTER_FIND;
			bSomeThingToSend = true;
			break;
		case BENAPSTER_GET:
			bmMessage->FindString("GetString",&sFindString);
			mySendThis = (char *)malloc(strlen(sFindString) + 1);
			strcpy(mySendThis, sFindString);
			myMessageType = NAPSTER_GET;
			bSomeThingToSend = true;
			break;
		case BENAPSTER_PREFERENCES:
			ShowPrefsWindow("Preferences");
			break;
		case BENAPSTER_REGISTER_WINDOW:
			ShowPrefsWindow("Registration");
			break;			
		case BENAPSTER_CONNECT:
			Connect();
			break;
		case BENAPSTER_DISCONNECT:
			myNapster->Quit();
			break;
		case BENAPSTER_REGISTER:
			if(bNapster)
			{
				if(myNapster->bConnected && !bRegistered)
				{
					myNapster->Register(myPreferences->GetUser());
				}
			}
			break;
		default:
// call inherited if you did not handle message
			BWindow::MessageReceived(bmMessage);
			break;			
	}
}
	

void LogWindow::LogMessage(char *sTextToLog, int32 iLength)
{

	char *sBuffer;
	Lock();
	if (iLength > 0)
	{
		sBuffer = (char *)malloc(iLength + 1);
		memcpy(sBuffer,sTextToLog,iLength);
		*(sBuffer + iLength) = '\0';
		txtLogText->Insert(sBuffer);
	}
	else
	{
		txtLogText->Insert(sTextToLog);
	}
	txtLogText->Insert("\n");
	txtLogText->ScrollToSelection();
	Unlock();

}


void LogWindow::ActOnMessage(uint16 iMessageType, uint16 iMessageLength, char *pMessage)
{

	BAlert     *baGeneralAlert;

	switch(iMessageType)
	{
		
		case NAPSTER_LOGIN_OKAY:
			LogMessage("Login completed", 0);
			if(bFindWindow)
			{
				myFindWindow->UnlockFind();
			}
			break;			
		case NAPSTER_FIND:			
			break;
		case NAPSTER_FIND_END:
			if(bFindWindow)
			{
				myFindWindow->UnlockFind();
			}
			break;
		case NAPSTER_FOUND:
			if(bFindWindow)
			{
				myFindWindow->AddToList(pMessage, iMessageLength);
			}
			break;
		case NAPSTER_DOWNLOAD_ACK:
			NewDownload(pMessage, iMessageLength);
			break;
		case NAPSTER_NICK_OKAY:
			myNapster->FirstLogin(myPreferences->GetUser(),
				 				   myPreferences->GetPassword(),
				 				   myPreferences->GetPort(),
	   							   "BeNapster 0.3",
			 					   myPreferences->GetConnection(),
			 					   myPreferences->GetEmail());
			break;
		case NAPSTER_NICK_TAKEN:
			baGeneralAlert = new BAlert("Nick Taken","The Username you provided has been taken\nPlease choose another one.","OK");
			baGeneralAlert->Go();
			ShowPrefsWindow("Registration");							
			break;
		case NAPSTER_NICK_INVALID:
			baGeneralAlert = new BAlert("Nick Taken","The Username you provided is invalid\nPlease choose another one.","OK");
			baGeneralAlert->Go();
			ShowPrefsWindow("Registration");							
			break;
		default:
			if(iMessageLength > 0)
			{
				LogMessage(pMessage, iMessageLength);
			}
			break;
	}



}

bool	LogWindow::QuitRequested()
{
	if(bFindWindow)
	{
		myFindWindow->PostMessage(B_QUIT_REQUESTED);
	}
//	pwPreferences->PostMessage(B_QUIT_REQUESTED);
	kill_thread(tidInLoop);
	kill_thread(tidOutLoop);
	be_app->PostMessage(B_QUIT_REQUESTED);
	return(true);
}


void LogWindow::Connect(void)
{
	char 	*pServer, *pPort;
	status_t  stTheadStatus;
	myNapster = new Napster();			

	pServer = (char *)malloc(30);  //should be more than enough

	myNapster->GetServer(pServer);
	pPort = strchr(pServer, ':');
	*pPort = '\0';
	pPort++;

	txtLogText->Insert("Connecting...\n");


	myNapster->Connect(pServer, atol(pPort));
	
	
	if(myNapster->bConnected)
	{
		
		bNapster = true;
		txtLogText->Insert("Connected...\n");

		tidInLoop = spawn_thread(InCommsLoop, "Communications In Thread", B_LOW_PRIORITY, this);
		stTheadStatus = resume_thread(tidInLoop);

		tidOutLoop = spawn_thread(OutCommsLoop, "Communications Out Thread", B_LOW_PRIORITY, this);
		stTheadStatus = resume_thread(tidOutLoop);

		tidReceive = spawn_thread(ReceiveLoop, "Remote Thread", B_LOW_PRIORITY, myPreferences);
		stTheadStatus = resume_thread(tidReceive);
		

		txtLogText->Insert("Loging in...\n");		
		if(bRegistered)
		{
			myNapster->Login(myPreferences->GetUser(),
							  myPreferences->GetPassword(),
			 				  myPreferences->GetPort(),
			 				  "BeNapster 0.3",
						   	  myPreferences->GetConnection());
		
		}
	}
	else
	{
		txtLogText->Insert("Connection Failed...\n");		
	}


}

void LogWindow::NewDownload(char *pBuffer, uint16 iBufferLength)
{
	
	char *pTemp, *pPort, *pUser, *pFile;
	BString *bsRemoteSend;
	int32 iPort;
	status_t stTheadStatus;
	
	
	//create a new thread for the download, need an array to store the threads
	
	pTemp = strchr(pBuffer, ' ');
	pTemp++;
	pTemp = strchr(pTemp, ' ');	
	pTemp++;
	pPort = pTemp;
	pTemp = strchr(pTemp, ' ');
	*pTemp = '\0';
	iPort = atol(pPort);	
	*pTemp = ' ';  // repair the damage we've done

	if(iPort > 0)
	{
		if (iTidDownloadCount < 2)
		{	
			myPreferences->pDownloadDetails = pBuffer;		
			tidDownload[iTidDownloadCount] = spawn_thread(DownloadLoop, "Downloading Thread", B_LOW_PRIORITY, myPreferences);
			stTheadStatus = resume_thread(tidDownload[iTidDownloadCount]);
			iTidDownloadCount++;
		}
		else
		{
			LogMessage("Maximum Downloads Exceeded", 0);
		}
	}
	else
	{
		LogMessage("Asking Remote client for file", 0);
		// Send a 500, asking the other client to send me the file.
		pUser = pBuffer;	
		pTemp = strchr(pBuffer, ' ');
		*pTemp = '\0';
		pTemp++;
		pTemp = strchr(pTemp, ' ');
		pTemp++;
		pTemp = strchr(pTemp, ' ');
		pTemp++;
		pTemp = strchr(pTemp, '"');
		pFile = pTemp;
		pTemp++;
		pTemp = strchr(pTemp, '"') + 1;
		*pTemp = '\0';		
		
		bsRemoteSend = new BString(pUser);
		bsRemoteSend->Append(" ");
		bsRemoteSend->Append(pFile);
		mySendThis = (char *)malloc(bsRemoteSend->Length()+1);
		strcpy(mySendThis, bsRemoteSend->String());
		myMessageType = NAPSTER_REMOTE_SEND;
		bSomeThingToSend = true;
	}
}

void LogWindow::ShowPrefsWindow(const char *sName)
{

	pwPreferences = new PrefsWindow(BRect (20, 20, 330, 265),
							  			  sName,
						  				  B_MODAL_WINDOW_LOOK,
						  				  B_NORMAL_WINDOW_FEEL,
						  				  0,
						                  B_CURRENT_WORKSPACE,
                                          myPreferences);
    
	pwPreferences->Show();
}

void LogWindow::ShowGNU(void)
{

    txtLogText->Insert("BeNapster 0.3, Copyright (C) 2000  David Burnett\n");
    txtLogText->Insert("BeNapster comes with ABSOLUTELY NO WARRANTY\n");
    txtLogText->Insert("This is free software, and you are welcome to redistribute it\n");
    txtLogText->Insert("under certain conditions as described in the GNU Public Licence.\n");
	txtLogText->Insert("Please refer to the file named COPYING that came with this archive\n\n\n");
}