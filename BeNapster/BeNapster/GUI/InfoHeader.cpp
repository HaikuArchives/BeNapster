#include "GUI/InfoHeader.h"


InfoHeader::InfoHeader(BRect frame) : BView(frame,"Header",B_FOLLOW_LEFT_RIGHT, B_WILL_DRAW| B_FRAME_EVENTS)
{
};

InfoHeader::~InfoHeader()
{
};

//void InfoHeader::Draw(BRect dummy=BRect(0,0,0,0))
void InfoHeader::Draw(BRect dummy)
{
	
//	BView *bvParent;
	
//	bvParent = Parent();
	
	
	rgb_color rgbInfoBG={180,180,180,0};
	rgb_color rgbInfoHigh={200,200,200,0};
	rgb_color rgbInfoBlack={0,0,0,0};
	
	BRect rectBounds=dummy;
//	BRect rectBounds=Bounds();
//	BRect rectParentBounds = bvParent->Bounds();
//	rectBounds.right = rectParentBounds.right;
	float bottom=rectBounds.bottom;
	
	SetHighColor(rgbInfoBG);
	SetLowColor(rgbInfoBG);
	
	FillRect(rectBounds);
	SetHighColor(rgbInfoBlack);
	FillRect(BRect (0,0,rectBounds.right,0));
	SetHighColor(rgbInfoHigh);
	FillRect(BRect (0,1,rectBounds.right,1));
	
	SetHighColor(rgbInfoBlack);
	
	MovePenTo(4, bottom-4);  
   	DrawString("Name");

	MovePenTo(450, bottom-4);
	DrawString("Size");
	
	MovePenTo(500, bottom-4);
	DrawString("Bitrate");
	
	MovePenTo(550, bottom-4);
	DrawString("Freq.");
	
	MovePenTo(600, bottom-4);
	DrawString("Length");
	
	MovePenTo(650, bottom-4);
	DrawString("Nick");
	
	MovePenTo(750, bottom-4);
	DrawString("Connection");
};