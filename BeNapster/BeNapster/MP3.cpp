#ifndef _MP3_
#include "MP3.h"
#endif

#include <View.h>
#include <Rect.h>
#include <String.h>

#include <stdlib.h>

#include "Constants.h"

MP3::MP3(const char *sMP3)
{
	char *pTemp, *pTempEnd;
	BString      bsDetails;
	int32        iLinkType;
	
	myMessage = (char *)malloc( strlen(sMP3) + 1);
	strcpy(myMessage, sMP3);
	// now we can mangle the string
	pTempEnd = strchr(sMP3 + 1, '"');
	for(pTemp = pTempEnd; *pTemp != '\\' && *pTemp != '/'; pTemp--);  // '/' in case we ever get BeOS or *nix files
	pTemp++;
	*pTempEnd = '\0';
	
	bsDetails.Append(pTemp);
	bsDetails.Append(" ");
	
	pTemp = strchr(pTempEnd + 2, ' ') + 1;   // size
	pTempEnd = strchr(pTemp, ' ') + 1;
	pTempEnd = strchr(pTempEnd, ' ') + 1;   // include bitrate
	pTempEnd = strchr(pTempEnd, ' ') + 1;   // include frequency
	pTempEnd = strchr(pTempEnd, ' ') + 1;   // include length in seconds
	pTempEnd = strchr(pTempEnd, ' ') + 1;   // include nick
	*pTempEnd = '\0';

	bsDetails.Append(pTemp);
	bsDetails.Append(" ");

	pTemp = strchr(pTempEnd + 1, ' ') + 1;   // skip IP
	pTempEnd = strchr(pTemp +1, ' ') + 1;   // find link type
	
	iLinkType = atol(pTemp);

	bsDetails.Append(aConnectionSpeeds[iLinkType]);
	bsDetails.Append(" ");

	myDetails = (char *)malloc(bsDetails.Length() + 1);
	strcpy(myDetails, bsDetails.String());
	
}


MP3::~MP3()
{
	free (myMessage);
	free (myDetails);
}


char *MP3::GetMessage(void)
{
	return(myMessage);
}

char *MP3::GetDetails(void)
{
	return(myDetails);
}

void MP3::DrawItem(BView *bvOwner, BRect brItem, bool bDrawEverything) 
{ 
   	rgb_color rgbColor;  
	rgb_color rgbSelectedColor = {255,255,0,0};
	rgb_color rgbMP3Color = {0,0,255,0}; 

    if (bDrawEverything || IsSelected()) 
    { 
 		if (IsSelected())  
 		{  
        	rgbColor = rgbSelectedColor; 
        }
        else
        {
        	rgbColor = bvOwner->ViewColor();		 
    	} 
       	bvOwner->SetHighColor(rgbColor); 
       	bvOwner->FillRect(brItem); 
    } 
     

	bvOwner->SetHighColor(rgbMP3Color);

	bvOwner->MovePenTo(brItem.left+4, brItem.bottom-2);  
   	bvOwner->DrawString(myDetails);

}
