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

#ifndef FIND_WINDOW_H
#include "FindWindow.h"
#endif

FindWindow::FindWindow(BRect frame, const char *title, BLooper *blMainWindow):
	BWindow(frame, title, B_DOCUMENT_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL, 0)
{

	BRect rectWinFrame = Bounds();
	myLooper = blMainWindow;
	bvMainView = new BBox(rectWinFrame, "CoveringView", 
					   B_FOLLOW_ALL, 
					   B_NAVIGABLE|B_WILL_DRAW,
					   B_NO_BORDER);

	AddChild(bvMainView);

	BMessage *bmArtist = new BMessage((uint32)0);
	myArtist = new BTextControl(BRect(10,10,300,18), "txtArtist", "Artist: ","", bmArtist, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	myArtist->SetDivider(be_plain_font->StringWidth("Artist: "));
	bvMainView->AddChild(myArtist);

	
	BMessage *bmSong = new BMessage((uint32)0);
	mySong = new BTextControl(BRect(10,40,300,58), "txtSong", "Song: ","", bmSong, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	mySong->SetDivider(be_plain_font->StringWidth("Artist: "));
	bvMainView->AddChild(mySong);

	BMessage *bmMax = new BMessage((uint32)0);
	myMax = new BTextControl(BRect(10,70,300,88), "txtArtist", "Max: ","", bmMax, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	myMax->SetDivider(be_plain_font->StringWidth("Artist: "));
	myMax->SetText("100");
	bvMainView->AddChild(myMax);

	BMessage *bmFind = new BMessage(BENAPSTER_FIND);
	myFind = new BButton(BRect(10, 100, 100, 118), "cmdFind", "Find", bmFind, B_FOLLOW_NONE, B_NAVIGABLE|B_WILL_DRAW);
	bvMainView->AddChild(myFind);

	BMessage *bmGet = new BMessage(BENAPSTER_GET);
	myGet = new BButton(BRect(210, 100, 300, 118), "cmdGet", "Get", bmGet, B_FOLLOW_NONE, B_NAVIGABLE|B_WILL_DRAW);
	bvMainView->AddChild(myGet);

	myHeader=new InfoHeader(BRect(0,130,rectWinFrame.right,149));
	bvMainView->AddChild(myHeader);
	
	rectWinFrame.top = 150;	
	rectWinFrame.right -= B_V_SCROLL_BAR_WIDTH;
	rectWinFrame.bottom -=  B_H_SCROLL_BAR_HEIGHT;
	BRect rectText = rectWinFrame;

	/* give us some border room */
	/* this also seems to fix our "listview overlaps its scrollbars" problem */
//	rectText.InsetBy(10,10);

	blvMp3s = new BListView(rectText, "Found", B_SINGLE_SELECTION_LIST, B_FOLLOW_ALL);

	BScrollView *bsvListView = new BScrollView("scrollTextView", blvMp3s, B_FOLLOW_ALL, B_FRAME_EVENTS, true, true, B_FANCY_BORDER);
	bvMainView->AddChild(bsvListView); 

}

void FindWindow::Quit()
{
	be_app->PostMessage(BENAPSTER_FIND_WINDOW_GONE);
	BWindow::Quit();
};

void	FindWindow::MessageReceived(BMessage *bmMessage)
{

//	myConnection *objNewsConnection;
	char 		*sFindString = NULL; 
	const char	*sMp3Details;
	int32 		iSelectedMp3;
	MP3 		*mp3Item;
	
	switch ( bmMessage->what )
	{
		case B_SIMPLE_DATA:
			break;
		case BENAPSTER_FIND:
			// Empty previous result list first
			blvMp3s->RemoveItems(0, blvMp3s->CountItems());
			// Fill in new result list
			sFindString = CreateFindString(sFindString);
			bmMessage->AddString("FindString", sFindString);
			myLooper->PostMessage(bmMessage);
			LockFind();
			break;
		case BENAPSTER_FIND_END:
			bCanDoFind = true;
			break;
		case BENAPSTER_GET:
			iSelectedMp3 = blvMp3s->CurrentSelection();
			if(iSelectedMp3 >= 0)
			{
				mp3Item = (MP3 *)blvMp3s->ItemAt(iSelectedMp3);
				sMp3Details = mp3Item->GetMessage();
				sFindString = CreateGetString(sMp3Details, sFindString);
				bmMessage->AddString("GetString", sFindString);
				myLooper->PostMessage(bmMessage);
			}
			break;
		default:
// call inherited if you did not handle message
			BWindow::MessageReceived(bmMessage);
			break;			
	}
}

char *FindWindow::CreateFindString(char *pBuffer)
{

	BString bsFind;
	
	bsFind.SetTo("FILENAME CONTAINS \"");
	bsFind.Append(myArtist->Text());
	if (strcmp(mySong->Text(), "") != 0)
	{
		bsFind.Append(" ");
		bsFind.Append(mySong->Text());
	}
	bsFind.Append("\" MAX RESULTS ");
	bsFind.Append(myMax->Text());
	
	pBuffer = (char *)malloc(bsFind.Length()+1);
	strcpy(pBuffer, bsFind.String());
	return (pBuffer);
}

char *FindWindow::CreateGetString(const char *sMp3Details,  char *pBuffer)
{

	BString bsGet;
	char *sDetailsCopy;  // need to copy the details so we can fiddle with them
	char *pSong, *pNick, *pTemp;
	int32 iCounter;
	
	sDetailsCopy = (char *)malloc(strlen(sMp3Details) + 1);
	strcpy(sDetailsCopy, sMp3Details);
		
	pSong = strchr(sDetailsCopy, '"');
	pTemp = strchr(pSong + 1, '"') + 1;
	*pTemp = '\0';
	for(iCounter = 1; iCounter < 6; iCounter++)
	{
		pTemp = strchr(pTemp + 1, ' ');
	}
	pTemp++;
	pNick = pTemp;
	pTemp = strchr(pTemp, ' ');
	*pTemp = '\0';
	
	bsGet.SetTo(pNick);
	bsGet.Append(" ");
	bsGet.Append(pSong);
	
	pBuffer = (char *)malloc(bsGet.Length()+1);
	strcpy(pBuffer, bsGet.String());
	return (pBuffer);
}

void FindWindow::LockFind(void)
{
	myFind->SetEnabled(false);
}

void FindWindow::UnlockFind(void)
{
	Lock();
	myFind->SetEnabled(true);
	Unlock();
}

void FindWindow::AddToList(char *pMp3, uint16 iBufferLength)
{
	MP3 *mp3Item;
	//needs to be expanded, to interpret the results
	char *pBuffer = (char *)malloc(iBufferLength + 1);
	
	memcpy(pBuffer, pMp3, iBufferLength);
	*(pBuffer + (int32)iBufferLength) = '\0';
	mp3Item = new MP3(pBuffer, bPatternToggler);
	Lock();
	blvMp3s->AddItem(mp3Item);
	Unlock();
}
