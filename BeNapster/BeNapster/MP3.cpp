#ifndef _MP3_
#include "MP3.h"
#endif

MP3::MP3(const char *sMP3, bool bPattern)
{
	char *pTemp, *pTempEnd;
	BString      bsDetails;
	
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
	
	strncpy(mySize,pTemp,-1+pTempEnd-pTemp);
	*(mySize + (pTempEnd - pTemp)) = '\0';  // strncpy does not null terminate less the src is shorter than the specified length
	//strcat(mySize," bytes");
	
	pTemp = strchr(pTempEnd, ' ') + 1;   // include bitrate
	strncpy(myBitrate,pTempEnd,pTemp-pTempEnd);
	*(myBitrate + (pTempEnd-pTemp)) = '\0';  // strncpy does not null terminate less the src is shorter than the specified length
	//strcat(myBitrate," kbps");
	
	pTempEnd = strchr(pTemp, ' ') + 1;   // include frequency
	strncpy(myFrequency,pTemp,pTempEnd-pTemp);
	*(myFrequency + (pTempEnd-pTemp)) = '\0';  // strncpy does not null terminate less the src is shorter than the specified length
	//strcat(myFrequency," Hz");
	
	pTemp = strchr(pTempEnd, ' ') + 1;   // include length in seconds
	strncpy(myLength,pTempEnd,pTemp-pTempEnd);
	*(myLength + (pTempEnd-pTemp)) = '\0';  // strncpy does not null terminate less the src is shorter than the specified length
	//strcat(myLength," sec");
	
	pTempEnd = strchr(pTemp, ' ') + 1;   // include nick
	strncpy(myNick,pTemp,pTempEnd-pTemp);
	*pTempEnd = '\0';
	strncpy(myNick,pTemp,pTempEnd-pTemp);

	pTemp = strchr(pTempEnd + 1, ' ') + 1;   // skip IP
	pTempEnd = strchr(pTemp +1, ' ') + 1;   // find link type

	myLinkType = atol(pTemp);

	myDetails = (char *)malloc(bsDetails.Length() + 1);
	strcpy(myDetails, bsDetails.String());

	myPattern=bPattern;
	
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
	rgb_color rgbSelectedColor = {255,240,240,0};
	rgb_color rgbMP3Color = {0,0,20,0};
	rgb_color rgbPatternColor={240,240,255};

	rgbColor = bvOwner->ViewColor();

    //if (bDrawEverything || IsSelected()) 
    //{ 
 		if (IsSelected())  
 		{  
        	rgbColor = rgbSelectedColor; 
        }
        else
        {
        	if (myPattern)
        	{
        		rgbColor = rgbPatternColor;
        	};
    	} 
       	bvOwner->SetHighColor(rgbColor); 
       	bvOwner->SetLowColor(rgbColor); 
       	bvOwner->FillRect(brItem); 
    //};

	bvOwner->SetHighColor(rgbMP3Color);

	bvOwner->MovePenTo(brItem.left+4, brItem.bottom-2);  
   	bvOwner->DrawString(myDetails);

	bvOwner->MovePenTo(brItem.left+450, brItem.bottom-2);
	bvOwner->DrawString(mySize);
	
	bvOwner->MovePenTo(brItem.left+500, brItem.bottom-2);
	bvOwner->DrawString(myBitrate);
	
	bvOwner->MovePenTo(brItem.left+550, brItem.bottom-2);
	bvOwner->DrawString(myFrequency);
	
	bvOwner->MovePenTo(brItem.left+600, brItem.bottom-2);
	bvOwner->DrawString(myLength);
	
	bvOwner->MovePenTo(brItem.left+650, brItem.bottom-2);
	bvOwner->DrawString(myNick);
	
	bvOwner->MovePenTo(brItem.left+750, brItem.bottom-2);
	bvOwner->DrawString(aConnectionSpeeds[myLinkType]);
}
