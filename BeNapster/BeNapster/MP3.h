#ifndef _MP3_
#define _MP3_

#include <ListItem.h>
#include <View.h>
#include <Rect.h>
#include <String.h>
#include <stdlib.h>
#include <string.h>
#include "Constants.h"

class MP3 : public BListItem
{
public:
//  Ctor
	MP3(const char *,bool bPattern);
	 
//	Dtor
	~MP3();	
	
	virtual void DrawItem(BView *, BRect, bool);

	char *GetDetails(void);
	char *GetMessage(void);
private:
	char *myMessage;   	//original message
	char *myDetails;	//string to show in list
	
	bool myPattern;
	
	char mySize[20];
	char myBitrate[20];
	char myFrequency[20];
	char myLength[20];
	char myNick[20];
	int32 myLinkType;
};

#endif
