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

#include "Preferences.h"

Preferences::Preferences(BLooper *myLogWindow)
{
	blLogWindow = myLogWindow;
}


bool Preferences::InitPreferences(void)
{

	BPath bpPrefs;
	char *pPrefs;
	bool bReturn = false;
	
	off_t stPrefsLength;
	
	
	find_directory(B_USER_SETTINGS_DIRECTORY, &bpPrefs);
	bpPrefs.Append("BeNapster.prefs");

	sUser 		= (char *)malloc(100);
	sPassword 	= (char *)malloc(100);
	sPort 		= (char *)malloc(100);
	sConnection = (char *)malloc(100);	
	sEmail		 = (char *)malloc(100);	

	BFile *bfPrefs = new BFile(bpPrefs.Path(), B_READ_ONLY);
	
	if(bfPrefs->InitCheck() == B_OK)
	{	

		bfPrefs->GetSize(&stPrefsLength);
		
		pPrefs = (char *)malloc(stPrefsLength);
		
		bfPrefs->Read(pPrefs, stPrefsLength);
		
		sUser 		= GetPref(pPrefs, "USER", sUser);
		sPassword 	= GetPref(pPrefs, "PASSWORD", sPassword);
		sPort 		= GetPref(pPrefs, "PORT", sPort);
		sConnection = GetPref(pPrefs, "CONNECTION", sConnection);
		sEmail 		= GetPref(pPrefs, "EMAIL", sEmail);
		bReturn = true;
	
	}
	else
	{
		*sUser 		 = '\0';
		*sPassword 	 = '\0';
		*sPort 		 = '\0';
		*sConnection = '\0';
		*sEmail 	 = '\0';

	}
	delete (bfPrefs);
	
	return bReturn;	
}

const char *Preferences::GetUser(void)
{

	return(sUser);

}

const char *Preferences::GetPassword(void)
{

	return(sPassword);

}

const char *Preferences::GetPort(void)
{

	return(sPort);

}

const char *Preferences::GetConnection(void)
{

	return(sConnection);

}

const char *Preferences::GetConnectionText(void)
{
	return(aConnectionSpeeds[atol(sConnection)]);
}

const char *Preferences::GetEmail(void)
{

	return(sEmail);

}

void Preferences::SetUser(const char *sNewValue)
{

	memcpy(sUser, sNewValue, strlen(sNewValue));
	*(sUser + strlen(sNewValue)) = '\0';
}

void Preferences::SetPassword(const char *sNewValue)
{

	memcpy(sPassword, sNewValue, strlen(sNewValue));
	*(sPassword + strlen(sNewValue)) = '\0';
}

void Preferences::SetPort(const char *sNewValue)
{

	memcpy(sPort, sNewValue, strlen(sNewValue));
	*(sPort + strlen(sNewValue)) = '\0';
}

void Preferences::SetConnection(const char *sNewValue)
{

	if(*sNewValue != '\0')
	{
		memcpy(sConnection, sNewValue, strlen(sNewValue));
		*(sConnection + strlen(sNewValue)) = '\0';
	}
}

void Preferences::SetEmail(const char *sNewValue)
{

	memcpy(sEmail, sNewValue, strlen(sNewValue));
	*(sEmail + strlen(sNewValue)) = '\0';
}

void Preferences::SaveFile(void)
{

	BPath bpPrefs;
		
	find_directory(B_USER_SETTINGS_DIRECTORY, &bpPrefs);
	bpPrefs.Append("BeNapster.prefs");


	BFile *bfPrefs = new BFile(bpPrefs.Path(), B_WRITE_ONLY|B_CREATE_FILE);
	
	if(bfPrefs->InitCheck() == B_OK)
	{			
		bfPrefs->Write("USER=",5);
		bfPrefs->Write(sUser,strlen(sUser));
		bfPrefs->Write("\nPASSWORD=",10);
		bfPrefs->Write(sPassword,strlen(sPassword));
		bfPrefs->Write("\nPORT=",6);
		bfPrefs->Write(sPort,strlen(sPort));
		bfPrefs->Write("\nCONNECTION=",12);
		bfPrefs->Write(sConnection,strlen(sConnection));
		bfPrefs->Write("\nEMAIL=",7);
		bfPrefs->Write(sEmail,strlen(sEmail));		
		bfPrefs->Write("\n.",2);
		bfPrefs->Unset();
	}
	blLogWindow->PostMessage(BENAPSTER_REGISTER);
	delete bfPrefs;
}

char *Preferences::GetPref(char *pPrefs, char *sPref, char *sValue)
{
		char *pStart, *pEnd;
		
		//blank sValue 
		memset(sValue,'\0', 100);
		//find preference in buffer
		pStart = strstr(pPrefs, sPref);
		if (pStart != NULL)
		{
			pStart += (strlen(sPref) + 1);
			pEnd =  strchr(pStart, '\n');
			if (pEnd != NULL)	
			{
				// techically this means the '.' at the end of the prefs file is no longer needed
 				*pEnd = '\0';
				strcpy (sValue, pStart);
				*pEnd = '\n';
			}
			else
			{
				strcpy (sValue, pStart);
			}
		}
		return sValue;
}

