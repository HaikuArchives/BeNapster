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
#include <ctype.h>
#include "Comms/CommsThreads.h"

//main loop is a seperate function so I can spawn it
int32 InCommsLoop(void *pDummy)
{
	
	LogWindow 	*pLogWindow;
	uint16  	iMessageLength, iMessageType;
	char 		*pMessage;
	int32		iBytesReceived;

	pLogWindow = (LogWindow *)pDummy;

	while(!pLogWindow->myNapster->Quit())
	{
		// hows about making Napster inherit from BNetEndpoint
		if(!pLogWindow->bSomeThingToSend)
		{
			if (pLogWindow->myNapster->bneNapsterServer.IsDataPending(100))
			{
				pLogWindow->myNapster->bneNapsterServer.Receive(&iMessageLength, 2);
				pLogWindow->myNapster->bneNapsterServer.Receive(&iMessageType, 2);
				pMessage = (char *)malloc(iMessageLength);
				iBytesReceived = 0;
				while(iBytesReceived != iMessageLength)  
				{
					iBytesReceived += pLogWindow->myNapster->bneNapsterServer.Receive(pMessage + iBytesReceived, iMessageLength - iBytesReceived);
				}	
				pMessage = (char *) realloc((char *)pMessage, (iMessageLength+1));
				*(pMessage + iMessageLength) = '\0';
				pLogWindow->ActOnMessage(iMessageType, iMessageLength, pMessage);
			}
		}
	}
	return(0);
}

int32 OutCommsLoop(void *pDummy)
{
	
	LogWindow *pLogWindow;

	pLogWindow = (LogWindow *)pDummy;

	while(!pLogWindow->myNapster->Quit())
	{
		if(pLogWindow->bSomeThingToSend)
		{
			pLogWindow->myNapster->Send(pLogWindow->mySendThis, pLogWindow->myMessageType);
			pLogWindow->bSomeThingToSend = false;
			free(pLogWindow->mySendThis);
		}
	}
	return(0);
}


int32 DownloadLoop(void *pDummy)
{


	char 			*pDetails, *pFile, *pIP, *pPort, *pUser , *pTemp, *pLength, *pLeaf, *pFilenameEnd;
	
	uint32 			iIPAddress, iFileLength, iDataLength;
	int32  			iPort, iBytesReceived;
	
	status_t 		stReturnCode;
	
	BNetEndpoint 	bneSong;
	BNetAddress 	bnaSong;
	BNetBuffer  	bnbSong;
	BFile       	*bfSong;
	
	Preferences		*myPrefs;
	DownloadView	*dlView;
	DownloadWindow *myDownloadWindow;
	
	myPrefs = (Preferences *)pDummy;
	
	pDetails = myPrefs->pDownloadDetails;

	pUser = pDetails;	
	pTemp = strchr(pDetails, ' ');
	*pTemp = '\0';
	pTemp++;
	
	pIP = pTemp;
	pTemp = strchr(pTemp, ' ');
	*pTemp = '\0';
	iIPAddress = strtoul(pIP, NULL, 10);
	
	pTemp++;
	pPort = pTemp;
	pTemp = strchr(pTemp, ' ');
	*pTemp = '\0';
	iPort = atol(pPort);	
	
	pTemp++;
	pTemp = strchr(pTemp, '"');
	pFile = pTemp;
	pTemp++;
	pTemp = strchr(pTemp, '"') + 1;
	*pTemp = '\0';
	pFilenameEnd = pTemp - 1;

	
	bnbSong.AppendData(myPrefs->GetUser(), strlen(myPrefs->GetUser()));
	bnbSong.AppendData(" ", 1);
	bnbSong.AppendData(pFile, strlen(pFile));
	bnbSong.AppendData(" ", 1);
	bnbSong.AppendData("0", 1);	

	for(pLeaf = pTemp; *pLeaf != '\\' && *pLeaf != '/'; pLeaf-- );
	pLeaf++;
	*pFilenameEnd = '\0';
	BPath Path(myPrefs->GetDownloadPath(), NULL, true);
	if(Path.InitCheck() == B_OK) {	// Making sure we have a valid path.
		Path.Append(pLeaf);
		bfSong = new BFile(Path.Path(), B_WRITE_ONLY|B_CREATE_FILE|B_OPEN_AT_END);
	} else	// No valid path, use default (current directory)
		bfSong = new BFile(pLeaf, B_WRITE_ONLY|B_CREATE_FILE|B_OPEN_AT_END);
		
	if(bfSong->InitCheck() == B_OK)
	{	
		stReturnCode = bnaSong.SetTo(iIPAddress, iPort);
		stReturnCode = bneSong.Connect(bnaSong);
	
	// now we need to recieve the mysterious '1';
	
		char *pReceiveBuffer = (char *)malloc(4096);
		iBytesReceived = bneSong.Receive(pReceiveBuffer, 1);
		if(iBytesReceived == 1)
		{
		
			pLength = (char *)malloc(50);  // more than enough to hold a big number :)
			
			printf("sending get command\n");
			bneSong.Send("GET",3);
			pTemp = (char *)bnbSong.Data();
			bneSong.Send(pTemp, strlen(pTemp));
		
			pTemp = pLength - 1;
			// the other client now sends a length followed by the mp3
			// according to the unoffical protocol you can not tell how long 
			// the length is. but the mp3 starts
			// with 0xFF
			while(isdigit((unsigned char)*pReceiveBuffer) != 0)
			{
				pTemp++;
				iBytesReceived = bneSong.Receive(pReceiveBuffer, 1);
				*pTemp = *pReceiveBuffer;
			}
			*pTemp = '\0';
			iFileLength = strtoul(pLength, NULL, 10);
			
			//create the download window now we know all the details
			myDownloadWindow = new DownloadWindow(BRect(100,100, 400, 200), "BeNapster Download", 
				B_DOCUMENT_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL, B_NOT_RESIZABLE, 
				B_CURRENT_WORKSPACE);
			
			myDownloadWindow->Show();
			
			dlView = myDownloadWindow->AddTransfer(pFile, iFileLength);
			// The 0xFF is part of the mp3 so we need to write it out
			bfSong->Write(pReceiveBuffer, iBytesReceived);	
			iDataLength = iBytesReceived;
			dlView->AddBytesReceived(iBytesReceived);
			
			iBytesReceived = bneSong.Receive(pReceiveBuffer, 4096);
			bfSong->Write(pReceiveBuffer, iBytesReceived);
			iDataLength += iBytesReceived;	
			dlView->AddBytesReceived(iBytesReceived);
			
			while(iDataLength != iFileLength)
			{
					iBytesReceived = bneSong.Receive(pReceiveBuffer, 4096);
					if (iBytesReceived < 0)
					{
						break;
					}
					bfSong->Write(pReceiveBuffer, iBytesReceived);	
					iDataLength += iBytesReceived;
					dlView->AddBytesReceived(iBytesReceived);
			}
			
			bfSong->Unset();
			delete (bfSong);
			myDownloadWindow->PostMessage(B_QUIT_REQUESTED);
		}
	}
	return(0);	
}

int32 ReceiveLoop(void *pDummy)
{
	LogWindow		*logWindow;
	
	BNetEndpoint 	bneNapsterPort;
	BNetEndpoint 	*bneReceived;
	thread_id		acceptedThread;
		
	logWindow = (LogWindow *)pDummy;
	
	bneNapsterPort.Bind(atol(logWindow->myPreferences->GetPort()));
	
	bneNapsterPort.Listen();
	for ( ; ; ) {
		bneReceived = bneNapsterPort.Accept();
		logWindow->netEndpoint = bneReceived;
		acceptedThread = spawn_thread(DoAccept, "Post-Connection Accept", B_LOW_PRIORITY, logWindow);
		resume_thread(acceptedThread);
	}
	return(0);
}

// Run after a client connects
int32 DoAccept(void *pointer)
{
	LogWindow*	logWindow;
	BString received,status,path;
	int32 offset;
	uint32 fileLength, bytesRead;
	BString nick;
	BString filename;
	BString lengthStr;
			
	unsigned char* buffer;	// For holding whatever it is we receive
	
	logWindow = (LogWindow *)pointer;
	
	buffer = (unsigned char *)malloc(1024);		// We'll take the data in 1K
												// Chunks
	logWindow->netEndpoint->Send("\n", 1);		
	
	// Get a line from the connection
	while(logWindow->netEndpoint->IsDataPending()) {
		bytesRead = logWindow->netEndpoint->Receive((unsigned char *)buffer, 1024);
		*(buffer + bytesRead) = '\0';
		received += (char *)buffer;
		if(isdigit(*(buffer + (bytesRead - 1))))
			break;
	}
	if(received.Compare("GET", 3) == 0) {		// Looks like somebody wants a file
		offset = received.FindFirst("\"", 3);	// Figure out where the filename starts
		received.CopyInto(nick, 3, (offset - 3));
		received.CopyInto(filename, (offset + 1), ((received.FindLast("\"") - 1) - offset) );
		status << "Received request from " << nick << " To download " << filename;	
		logWindow->LogMessage(status.String(), BN_MESSAGE); 			
		if((SendAFile(logWindow, filename.String()) == false)) {
			status << "\nAn Error Occured While Sending " << filename << " to " << nick;
			logWindow->LogMessage(status.String(), BN_ERROR);
		} else {
			status << "\nTransfer of " << filename << " to " << nick << " successful";
			logWindow->LogMessage(status.String(), BN_STATUS);
		}
	}
	if(received.Compare("SEND", 4) == 0) {
		offset = received.FindFirst("\"", 4);
		received.CopyInto(nick, 4, (offset -4));
		received.CopyInto(filename, (offset + 1), ((received.FindLast("\"") - 1) - offset) );
		received.CopyInto(lengthStr, (received.FindLast("\"")+2), (received.Length() - (received.FindLast("\"")+2)));
		fileLength = strtoul(lengthStr.String(), NULL, 10);
		status << "Received Request from " << nick << " To Upload " << filename << " To Us.";
		logWindow->LogMessage(status.String(), BN_MESSAGE);
		if((UploadFromUser(logWindow, filename.String(), fileLength) == false)) {
			logWindow->LogMessage(status.String(), BN_ERROR);
		} else {
			status << "\nSuccesfully Received " << filename << " from " << nick;
			logWindow->LogMessage(status.String(), BN_STATUS);
		}
	}	
	// Close the connection
	logWindow->netEndpoint->Close();
	return 0;
}

bool SendAFile(LogWindow* logWindow, const char* filename)
{
	BString path,status;
	int32 bytesSent, bytesRead, totbytes = 0;
	DownloadView *dlView;
	DownloadWindow *myDownloadWindow;
	unsigned char* buffer;
	
	// Allocate some memory for the receive buffer
	buffer = (unsigned char*) malloc(1024);
	
	// Try and find the file
	path << logWindow->myPreferences->GetShareDir() << "/" << filename;
	BFile sendFile(path.String(), B_READ_ONLY);
	if(sendFile.InitCheck() != B_OK) {
		status << "Could not open " << path << "for reading";
		logWindow->LogMessage(status.String(), BN_ERROR);
		logWindow->netEndpoint->Close();
		return false;
	}

	
	int32 fsize;
	
	sendFile.GetSize((off_t *)&fsize);
	status = "";
	status << fsize;
	
	logWindow->myNapster->Send("", NAPSTER_UPLOAD_START);
	bytesSent = logWindow->netEndpoint->Send((char *) status.String(), status.Length());
	myDownloadWindow = new DownloadWindow(BRect(100,100, 400, 200), "BeNapster Upload", 
			B_DOCUMENT_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL, B_NOT_RESIZABLE, 
			B_CURRENT_WORKSPACE);
	myDownloadWindow->Show();
	dlView = myDownloadWindow->AddTransfer(filename, (float)fsize);
	BNetBuffer netBuffer;
	while((bytesRead = sendFile.Read((unsigned char *)buffer, 1024)) > 0) {
		bytesSent = logWindow->netEndpoint->Send((unsigned char*)buffer, bytesRead);
		totbytes += bytesSent;
		myDownloadWindow->Lock();
		dlView->AddBytesReceived((float)bytesSent);
		myDownloadWindow->Unlock();
		if((bytesSent == -1) || (totbytes >= fsize)) break;
	}
	
	// Tell the napster server we're through sending the file
	logWindow->myNapster->Send("", NAPSTER_UPLOAD_DONE);
	dlView->SetFinished(true);
	myDownloadWindow->Refresh();
	return true;
}


bool UploadFromUser(LogWindow* logWindow, const char* filename, uint32 iFileLength)
{
	uint32 iBytesReceived;
	BString fileString;
	BFile* bfMp3;
	DownloadWindow* myDownloadWindow;
	unsigned char* pBuffer;
	BString status;
	BString tmp;
	DownloadView* dlView;
	
	pBuffer = (unsigned char *) malloc(4096);
	
	fileString = filename;
	fileString.ReplaceAll("\\", "/");
	
	tmp << logWindow->myPreferences->GetDownloadPath() << "/";
	
	fileString.Prepend(tmp);
	
	
	bfMp3 = new BFile(fileString.String(), B_WRITE_ONLY|B_CREATE_FILE|B_OPEN_AT_END);	
	
	if (bfMp3->InitCheck() == B_OK)
	{
		logWindow->netEndpoint->Send("0",1);   // I think this is the start position, other values may be resumes
	
		myDownloadWindow = new DownloadWindow(BRect(100,100, 400, 200), "Upload From User", 
				B_DOCUMENT_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL, B_NOT_RESIZABLE, 
				B_CURRENT_WORKSPACE);
		myDownloadWindow->Show();
		dlView = myDownloadWindow->AddTransfer(filename, (float)iFileLength);
		uint32 iDataLength = 0;
		
		while(iDataLength < iFileLength)
		{
			iBytesReceived = logWindow->netEndpoint->Receive((unsigned char*)pBuffer, 4096);
			if (iBytesReceived < 0)
			{
				break;
			}
			bfMp3->Write(pBuffer, iBytesReceived);	
			iDataLength += iBytesReceived;
			myDownloadWindow->Lock();
			dlView->AddBytesReceived(iBytesReceived);
			myDownloadWindow->Unlock();
		}
		dlView->SetFinished(true);
		bfMp3->Unset();
		myDownloadWindow->Refresh();
	} else {
		logWindow->LogMessage("Could Not Open The File", BN_ERROR);
	}
	return true;
}