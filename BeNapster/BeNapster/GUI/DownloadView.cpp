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

#include "DownloadView.h"


DownloadView::DownloadView(BRect frame, const char *title, const char *sFileName, float fFileSize) :
	BView(frame, title, B_FOLLOW_LEFT, B_WILL_DRAW)
{

	rgb_color rgbBGColor={220,220,220};
	myFileSize = fFileSize;
	bFinished = false;
	SetViewColor(rgbBGColor);
	
	BRect brWinFrame = Bounds();

	brWinFrame.top += 10;
	brWinFrame.left += 10;
	brWinFrame.bottom -= 50;
	brWinFrame.right -= 10;

	bsbDownload = new BStatusBar(brWinFrame, "Download Completion", sFileName);
	AddChild(bsbDownload);
	bsbDownload->SetMaxValue(fFileSize);
	
	brWinFrame.top = brWinFrame.bottom + 10;
	brWinFrame.bottom += 30;
	brWinFrame.left = brWinFrame.right - 50;
	
	
	BMessage *bmTemp = new BMessage(BNAPSTER_DOWNLOAD_CANCEL);
	BButton  *bbCancel = new BButton(brWinFrame, "cmdCancel", "Cancel", bmTemp);
	AddChild(bbCancel);
	
}

void DownloadView::AddBytesReceived(float fBytes)
{
	bsbDownload->Update(fBytes);
	return;
}

bool DownloadView::IsFinished(void)
{
	return (bFinished);
}

void DownloadView::SetFinished(bool bMyFinished)
{
	bFinished = bMyFinished;
	return;
}
