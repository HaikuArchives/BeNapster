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
			while((unsigned char)*pReceiveBuffer != 0xFF)
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
				B_CURRENT_WORKSPACE, (float )iFileLength, pLeaf);
			
			myDownloadWindow->Show();
			
			
			// The 0xFF is part of the mp3 so we need to write it out
			bfSong->Write(pReceiveBuffer, iBytesReceived);	
			iDataLength = iBytesReceived;
			myDownloadWindow->AddBytesReceived(iBytesReceived);
			
			iBytesReceived = bneSong.Receive(pReceiveBuffer, 4096);
			bfSong->Write(pReceiveBuffer, iBytesReceived);
			iDataLength += iBytesReceived;	
			myDownloadWindow->AddBytesReceived(iBytesReceived);
			
			while(iDataLength != iFileLength)
			{
					iBytesReceived = bneSong.Receive(pReceiveBuffer, 4096);
					if (iBytesReceived < 0)
					{
						break;
					}
					bfSong->Write(pReceiveBuffer, iBytesReceived);	
					iDataLength += iBytesReceived;
					myDownloadWindow->AddBytesReceived(iBytesReceived);
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
	
	DownloadWindow 	*myDownloadWindow;
	Preferences 	*myPreferences;

	BNetEndpoint 	bneNapsterPort;
	BNetEndpoint 	*bneReceived;
	BFile           *bfMp3;
	
	uint16			*pShort;
	uint16 			iMessageType, iLength;
	size_t          iBytesReceived;
	uint32          iFileLength, iDataLength = 0;

	char *sFileName, *pTemp, *pNameEnd;	
	char *pBuffer;
	
	myPreferences = (Preferences *)pDummy;
	
	pBuffer = (char *)malloc(4096);
	pShort = (uint16 *)pBuffer;
	
	
	bneNapsterPort.Bind(atol(myPreferences->GetPort()));
	bneNapsterPort.Listen();
	bneReceived = bneNapsterPort.Accept();
	bneReceived->Send("\n",1);

	// should get a standard napster message

	iBytesReceived = bneReceived->Receive(pBuffer, 2);	
	iLength = *pShort;
	iBytesReceived = bneReceived->Receive(pBuffer, 2);
	iMessageType = *pShort;
	iBytesReceived = bneReceived->Receive(pBuffer, (int32)iLength);

	// message should contain username and file name	
	
	pTemp = strchr(pBuffer,'\"');
	pTemp++;
	
	pNameEnd = strchr(pTemp,'\"');
	*pNameEnd = '\0';
	
	for(pTemp = pNameEnd; *pTemp != '\\' && *pTemp != '/'; pTemp--);  // '/' in case we ever get BeOS or *nix files
	pTemp++;
	
	sFileName = (char *)malloc(pNameEnd - pTemp + 2);
	*sFileName = '\0';   // terminate the string
	
	strcpy(sFileName, pTemp);
	bfMp3 = new BFile(sFileName, B_WRITE_ONLY|B_CREATE_FILE|B_OPEN_AT_END);	
	
	if (bfMp3->InitCheck() == B_OK)
	{
		pTemp = pNameEnd + 2;  // should give us the file length
		iFileLength = strtoul(pTemp, NULL, 10);
	
		bneReceived->Send("0",1);   // I think this is the start position, other values may be resumes
	
		myDownloadWindow = new DownloadWindow(BRect(100,100, 400, 200), "Upload From User", 
				B_DOCUMENT_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL, B_NOT_RESIZABLE, 
				B_CURRENT_WORKSPACE, (float )iFileLength, sFileName);
		
		myDownloadWindow->Show();
	
		iDataLength = 0;
		
		while(iDataLength < iFileLength)
		{
			iBytesReceived = bneReceived->Receive(pBuffer, 4096);
			if (iBytesReceived < 0)
			{
				break;
			}
			bfMp3->Write(pBuffer, iBytesReceived);	
			iDataLength += iBytesReceived;
			myDownloadWindow->AddBytesReceived(iBytesReceived);
		}
		bfMp3->Unset();
		myDownloadWindow->PostMessage(B_QUIT_REQUESTED);
	}
		
	bneReceived->Close();
	return(0);
}
