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
#include "Util/Util.h"

const char* MD5_file(char *filename);


LogWindow::LogWindow(BRect frame, const char *title, 
	window_look look, window_feel feel, uint32 flags, uint32 workspaces) :
	BWindow(frame, title, look, feel, flags, workspaces),
	myNapster(NULL),
	myFindWindow(NULL)
{

	BMenu 		*bmuTemp;
	BMessage 	*bmMsg;
	BMenuItem	*bmiTemp;
	
	bSomeThingToSend = false;
	iTidDownloadCount = 0;
	tidInLoop = 0;
	tidOutLoop = 0;
	tidReceive = 0;


	myPreferences = new Preferences(this);
	if(myPreferences->InitPreferences()){
		bRegistered = true;
	} else 	{
		ShowPrefsWindow("Registration");
		bRegistered = false;
	}

	BRect brMenuFrame(Bounds());
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

	BRect rectWinFrame(Bounds());
	rectWinFrame.top = myLogMenuBar->Bounds().bottom + 1;   /// allow for Menu...
	rectWinFrame.right -= B_V_SCROLL_BAR_WIDTH;
	rectWinFrame.bottom -=  B_H_SCROLL_BAR_HEIGHT;
	BRect rectText = rectWinFrame;

	txtLogText = new BTextView(rectText, 
								"Log Window",
								rectText,
								B_FOLLOW_ALL_SIDES,
								B_NAVIGABLE|B_PULSE_NEEDED|B_WILL_DRAW);

	// Status messages
	rgb_color sc = {0, 0, 255, 0}; // Blue
	// Error Messages
	rgb_color ec = {255, 0, 0, 0};	// Red
	// 'Chat' messages
	rgb_color mc = {0, 255, 0, 0};	// Green
	
	SetStatusColor(sc);
	SetErrorColor(ec);
	SetMessageColor(mc);
	
	// Used for FindAllMP3s
	inSubdir = false;
	
	// Make the background black
	rgb_color color = {0, 0, 0, 0};
	
	txtLogText->SetAlignment(B_ALIGN_LEFT);
	txtLogText->SetViewColor(color);
	txtLogText->SetAutoindent(0);
	txtLogText->MakeEditable(FALSE);
	txtLogText->MakeResizable(TRUE);
	txtLogText->MakeSelectable(TRUE);
	txtLogText->SetWordWrap(TRUE);
	txtLogText->SetStylable(TRUE);
	
	txtLogText->SetFontAndColor(be_bold_font, B_FONT_ALL, &statusColor);
	
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
	BString dbug;
	
	switch ( bmMessage->what )
	{
		case B_SIMPLE_DATA:
			break;
		case B_QUIT_REQUESTED:
			be_app->PostMessage(B_QUIT_REQUESTED);
			break;
		case BENAPSTER_FIND_WINDOW:
			if(!myFindWindow)
			{
				bsScreen = new BScreen(this);
				brFindWindow = bsScreen->Frame();
				delete(bsScreen);
				brFindWindow.top = 40;
				brFindWindow.bottom = brFindWindow.bottom - 40;
				brFindWindow.left = 30;
				brFindWindow.right -= 30;			 

				/* put at least a little limit on the max size of */
				/* the window, for those of us running at 1600x1200 */
				if (brFindWindow.Width() > 800) {
					brFindWindow.right = brFindWindow.left + 800;
				}
				
				if (brFindWindow.Height() > 600) {
					brFindWindow.bottom = brFindWindow.top + 600;
				}

				myFindWindow = new FindWindow(brFindWindow, 
											  "BeNapster Find",
											   Looper());
			}
			myFindWindow->Show();
			break;
		case BENAPSTER_FIND_WINDOW_GONE:
			myFindWindow=NULL;
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
			if(myNapster) {
				if(myNapster->bConnected && !bRegistered) {
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
	

void LogWindow::LogMessage(const char *sTextToLog, uint16 type = BN_STATUS)
{
	// Just to make sure
	txtLogText->SetStylable(true);
	
	switch(type) {
		case BN_STATUS:
			txtLogText->SetFontAndColor(NULL, 0, &statusColor);
			break;
		case BN_ERROR:
			txtLogText->SetFontAndColor(NULL, 0, &errorColor);
			break;
		case BN_MESSAGE:
			txtLogText->SetFontAndColor(NULL, 0, &messageColor);
			break;
	}
	
	Lock();
	/*if (iLength > 0)
	{
		sBuffer = (char *)malloc(iLength + 1);
		memcpy(sBuffer,sTextToLog,iLength);
		*(sBuffer + iLength) = '\0';
		txtLogText->Insert(sBuffer);
	}
	else
	{*/
		txtLogText->Insert(sTextToLog);
	//}
	txtLogText->Insert("\n");
	txtLogText->ScrollToSelection();
	Unlock();

}


void LogWindow::ActOnMessage(uint16 iMessageType, uint16 iMessageLength, char *pMessage)
{

	BAlert     *baGeneralAlert;
	BString status;
		
	switch(iMessageType)
	{
		case NAPSTER_LOGIN_OKAY:
			LogMessage("Login completed", BN_STATUS);
			ShareMP3s();
			if(myFindWindow)
			{
				myFindWindow->UnlockFind();
			}
			break;			
		case NAPSTER_FIND:			
			break;
		case NAPSTER_FIND_END:
			if(myFindWindow)
			{
				myFindWindow->UnlockFind();
			}
			break;
		case NAPSTER_FOUND:
			if(myFindWindow) {
				myFindWindow->bPatternToggler=1-myFindWindow->bPatternToggler;
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
	   							   BENAPSTER_VERSION,
			 					   myPreferences->GetConnection(),
			 					   myPreferences->GetEmail());
			break;
		case NAPSTER_NICK_TAKEN:
			baGeneralAlert = new BAlert("Nick Taken","The Username you provided has been taken\nPlease choose another one.","OK");
			baGeneralAlert->Go();
			ShowPrefsWindow("Registration");							
			break;
		case NAPSTER_NICK_INVALID:
			LogMessage("The nick requested was already taken", BN_ERROR);
			baGeneralAlert = new BAlert("Nick Taken","The Username you provided is invalid\nPlease choose another one.","OK");
			baGeneralAlert->Go();
			ShowPrefsWindow("Registration");							
			break;
		case NAPSTER_MOTD:
			LogMessage(pMessage, BN_STATUS);
			break;
		case NAPSTER_UPLOAD_REQUEST:
			BeginUpload(pMessage);
			break;		
		default:
			if(iMessageLength > 0)
			{
				status << "[" << (int32) iMessageType << "]" << pMessage;
				LogMessage(status.String(), BN_ERROR);
			}
			break;
	}



}

bool
LogWindow::QuitRequested()
{
	if(myFindWindow) {
		myFindWindow->PostMessage(B_QUIT_REQUESTED);
	}
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
	
	
	if(myNapster->bConnected) {
		LogMessage("Connected...", BN_STATUS);

		tidInLoop = spawn_thread(InCommsLoop, "Communications In Thread", B_LOW_PRIORITY, this);
		stTheadStatus = resume_thread(tidInLoop);

		tidOutLoop = spawn_thread(OutCommsLoop, "Communications Out Thread", B_LOW_PRIORITY, this);
		stTheadStatus = resume_thread(tidOutLoop);

		if(memcmp(myPreferences->GetPort(),"0\0",2 != 0 && tidReceive == 0 )) // port isn't o and Remote Thread isn't running
		{
			tidReceive = spawn_thread(ReceiveLoop, "Remote Thread", B_LOW_PRIORITY, this);
			stTheadStatus = resume_thread(tidReceive);
		}
		

		LogMessage("Logging in...", BN_STATUS);		
		if(bRegistered) {
			myNapster->Login(myPreferences->GetUser(),
							  myPreferences->GetPassword(),
			 				  myPreferences->GetPort(),
			 				  "BeNapster 0.4",
						   	  myPreferences->GetConnection());
		
		}
	} else {
		LogMessage("Connection Failed...\n", BN_ERROR);		
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

	if(iPort > 0) {
		if (iTidDownloadCount < 2) {	
			myPreferences->pDownloadDetails = pBuffer;		
			tidDownload[iTidDownloadCount] = spawn_thread(DownloadLoop, "Downloading Thread", B_LOW_PRIORITY, myPreferences);
			stTheadStatus = resume_thread(tidDownload[iTidDownloadCount]);
			iTidDownloadCount++;
		} else {
			LogMessage("Maximum Downloads Exceeded", BN_ERROR);
		}
	} else {
		LogMessage("Asking Remote client for file", BN_STATUS);
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
/*<<<<<<< LogWindow.cpp

	pwPreferences = new PrefsWindow(BRect (20, 20, 330, 310),
							  			  sName,
                                          myPreferences);
    
=======*/
	pwPreferences = new PrefsWindow(sName, myPreferences);
	pwPreferences->Show();
}

void LogWindow::ShowGNU(void)
{
    LogMessage("BeNapster 0.4, Copyright (C) 2000  David Burnett", BN_STATUS);
    LogMessage("BeNapster comes with ABSOLUTELY NO WARRANTY", BN_STATUS);
    LogMessage("This is free software, and you are welcome to redistribute it", BN_STATUS);
    LogMessage("under certain conditions as described in the GNU Public Licence.", BN_STATUS);
	LogMessage("Please refer to the file named COPYING that came with this archive\n", BN_STATUS);
}

void LogWindow::SetStatusColor(rgb_color sc) {
	statusColor = sc;
}

void LogWindow::SetErrorColor(rgb_color ec) {
	errorColor = ec;
}

void LogWindow::SetMessageColor(rgb_color mc) {
	messageColor = mc;
}

void	LogWindow::ShareMP3s(void)
{
	BEntry bentry;
	BPath bpath;
	entry_ref ref;
	BString statusMsg, filename;
	
	LogMessage("Sharing MP3's...", BN_STATUS);
	// Setup the BDirectory with the directory given in the preferences
	shareDir = new BDirectory(myPreferences->GetShareDir());
	
	// Make sure it was all peachy-keen and stuff
	if(shareDir->InitCheck() != B_OK) {
		 	statusMsg << "Error While Sharing MP3's: could not open share directory"
		 			  << myPreferences->GetShareDir();
		 	LogMessage(statusMsg.String(), BN_ERROR);
			return;
	}
	FindAllMP3s(shareDir);
	//shareThread = spawn_thread(FindAllMP3s, "Sharing MP3 Thread", B_LOW_PRIORITY, shareDir);
	//if(resume_thread(shareThread) != B_OK)
	//	LogMessage("An Error Occured While Launching The Thread to Share MP3s", BN_ERROR);
/*	
	while(shareDir->GetNextEntry(&bentry, true) == B_OK) {
		bentry.GetPath(&bpath);
		filename = bpath.Leaf();
		if(filename.IFindLast("mp3", filename.Length()) != B_ERROR) {
			if(get_ref_for_path(bpath.Path(), &ref) != B_OK)
				LogMessage("Error Sharing MP3", BN_ERROR);
			else
				myNapster->ShareFile(ref);
		}
	}	*/
}

void	LogWindow::BeginUpload(char *message) 
{
	BString realFilename, nick, status;
	
	while(*message != '\"') {
		nick += *message;
		message++;
	}

	// Advance past the quote
	message++;

	// Get the "real" filename
	while(*message != '\"') {
		realFilename += *message;
		message++;
	}

	status << "Upload Request Received For: " << realFilename << " "
	       << "From: " << " " << nick;
	
	LogMessage(status.String(), BN_STATUS);
	
	status="";
	
	if(myNapster->UploadFile(realFilename.String())) {
		status << "Upload Of " << realFilename << " Acknowledged";
		LogMessage(status.String(), BN_STATUS);
		status = "";
		status << nick << " " << "\"" << realFilename << "\"";
		myNapster->Send(status.String(), NAPSTER_UPLOAD_ACK);
	}
	else {
		status << "Error While Attempting To Upload " << realFilename;
		LogMessage(status.String(), BN_ERROR);
	}
}

int32	LogWindow::FindAllMP3s(void* dummy)
{
	BDirectory *startDir, *next;
	BEntry entry;
	dirent dir;
	entry_ref ref;
	node_ref dirRef;
	BPath path;
	BString filename;
	BString shareName;
		
	startDir = (BDirectory*) dummy;
	
	while(startDir->GetNextEntry(&entry, true) == B_OK)
	{
		entry.GetPath(&path);	
		filename = path.Leaf();
		if(filename.IFindLast("mp3", filename.Length()) != B_ERROR)
		{
			printf("%s\n", filename.String());
			shareName.Truncate(0);
			if(inSubdir)
				shareName << (new BPath(startDir, NULL, false))->Leaf() << "/" << filename;
			else
				shareName << filename;			
			if(get_ref_for_path(path.Path(), &ref) != B_OK)
				LogMessage("Error Sharing MP3", BN_ERROR);
			else
				myNapster->ShareFile(ref, shareName.String());
			printf("Done with that one...\n");
		}
	}
	
	while(startDir->GetNextDirents(&dir, 1))
	{
		printf("got dir...\n");
		dirRef.device = dir.d_pdev;
		dirRef.node = dir.d_ino;
		LogMessage(dir.d_name, BN_ERROR);
		if((strcmp(dir.d_name, ".") == 0) || (strcmp(dir.d_name, "..") == 0)) continue;
		next = new BDirectory(&dirRef);
		inSubdir = true;
		FindAllMP3s(next);
	}
	printf("Exiting...\n");
	return 0;
}