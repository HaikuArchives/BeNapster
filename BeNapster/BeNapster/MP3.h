#ifndef _MP3_
#define _MP3_

#include <ListItem.h>

class MP3 : public BListItem
{
public:
//  Ctor
	MP3(const char *);
	
//	Dtor
	~MP3();	
	
	void DrawItem(BView *, BRect, bool);

	char *GetDetails(void);
	char *GetMessage(void);
private:
	char *myMessage;   	//original message
	char *myDetails;	//string to show in list
};

#endif
