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

#include "DownloadWindow.h"


DownloadWindow::DownloadWindow(BRect frame, const char *title, 
	window_look look, window_feel feel, uint32 flags, uint32 workspaces) :
	BWindow(frame, title, look, feel, flags, workspaces)
{

	BRect brView     = Bounds();
	
	brView.right -= B_V_SCROLL_BAR_WIDTH;
	brView.bottom -= B_H_SCROLL_BAR_HEIGHT;
	rgb_color rgbBGColor={220,220,220};

	bvCoverView = new BView(brView,
							"Download View", 
					   		B_FOLLOW_ALL, 
					   		B_NAVIGABLE|B_WILL_DRAW);


	bvCoverView->SetViewColor(rgbBGColor);

	bsvCoverScroll = new BScrollView("scrollDownloadViews", bvCoverView, B_FOLLOW_ALL, B_FRAME_EVENTS, true, true, B_FANCY_BORDER);
//	bsvCoverScroll->ScrollBar(B_HORIZONTAL)->SetRange(0.0, 0.0); 
//	bsvCoverScroll->ScrollBar(B_VERTICAL)->SetRange(0.0, 0.0); 
	AddChild(bsvCoverScroll);
		

}

DownloadView *DownloadWindow::AddTransfer(char *sFileName, float fFileSize)
{
	BRect brView;
	
	Lock();
	brView = bvCoverView->Frame();
	brView.top = bvCoverView->CountChildren() * 100;
	brView.bottom = brView.top + 99;
		
	DownloadView *dvNew = new DownloadView(brView, "TransferView", sFileName, fFileSize);
	bvCoverView->AddChild(dvNew);
	Unlock();
	return(dvNew);
}

void DownloadWindow::Refresh(void)
{

	Lock();
	int32 iNumberOfTransfers, iIndex, iNewIndex;
	DownloadView *dvView;
	float fNewTop = 0;	
	
	iNumberOfTransfers = bvCoverView->CountChildren();
	iNewIndex = 0;
	
	for(iIndex = 0; iIndex < iNumberOfTransfers; iIndex++)
	{
		dvView = (DownloadView *)bvCoverView->ChildAt(iIndex);
		if(dvView->IsFinished())
		{
			bvCoverView->RemoveChild(dvView);  // remove it from the hierarchy
			delete(dvView);					   // then delete it
		}
		else
		{
			fNewTop = 100 * iNewIndex;	
			dvView->MoveTo(0.0, fNewTop);
			iNewIndex++;
		}
	}
	bsvCoverScroll->ScrollBar(B_HORIZONTAL)->SetProportion(Frame().Width() / bvCoverView->Frame().Width()); 
	bsvCoverScroll->ScrollBar(B_VERTICAL)->SetProportion(Frame().Height()/(iNewIndex * 100)); 
	
	Unlock();
	return;	
}
