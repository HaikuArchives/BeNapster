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

#include "Napster.h"

// Main Napster class, GUI, takes care of the connections with the server
//

Napster::Napster()
{
	myQuit = false;
}


char   *Napster::GetServer(char *pBuffer)
{
// connects to the Server Server and gets the 'best' server details

	int32 iBytesReceived;
	char *pTemp;
	BNetAddress  bnaServerServer;
	BNetEndpoint bnpServerServer;
	
	bnaServerServer.SetTo("server.napster.com", 8875);
	bnpServerServer.Connect(bnaServerServer);
	
	// server sends best server details
	// the format returned is nnn.nnn.nnn.nnn:xxxx
	// i.e. 127.0.0.1:8888
	
	iBytesReceived = bnpServerServer.Receive(pBuffer, 30);
	pTemp = (char *)memchr(pBuffer, '\n', 30);
	*pTemp = '\0';
	bnpServerServer.Close();	
	return (pBuffer);

}

void Napster::Connect(const char *sServer, int32 iPort)
{

	status_t stReturnCode;
	bool bConnectedOkay = true;
	
	stReturnCode = bnaNapsterServer.SetTo(sServer, iPort);
	try
	{
		stReturnCode = bneNapsterServer.Connect(bnaNapsterServer);
		if (stReturnCode != B_OK )
		{
			bConnectedOkay = false;		
		}
	}
	catch(...)
	{
		bConnectedOkay = false;		
	}
	bConnected = bConnectedOkay;

}

void Napster::Register(const char *sUsername)
{
	Send(sUsername, NAPSTER_FIRST_LOGIN);	
}


void Napster::FirstLogin(const char *sUsername, const char *sPassword, const char *sPort, const char *sClient, const char *sLink, const char *sEmail)
{
	BNetBuffer bnbLogin;
	BString    bsLogin;
	
	bsLogin.SetTo(sUsername);
	bsLogin.Append(" ");
	bsLogin.Append(sPassword);
	bsLogin.Append(" ");
	bsLogin.Append(sPort);
	bsLogin.Append(" ");
	bsLogin.Append(sClient);
	bsLogin.Append(" ");
	bsLogin.Append(sLink);
	bsLogin.Append(" ");
	bsLogin.Append(sEmail);

	Send(bsLogin.String(), NAPSTER_FIRST_LOGIN);

}


void Napster::Login(const char *sUsername, const char *sPassword, const char *sPort, const char *sClient, const char *sLink)
{
	BNetBuffer bnbLogin;
	BString    bsLogin;
//	uint16     iLoginLength, iMessageType;
//	uint16     iReplyLength, iReplyType;
	
	bsLogin.SetTo(sUsername);
	bsLogin.Append(" ");
	bsLogin.Append(sPassword);
	bsLogin.Append(" ");
	bsLogin.Append(sPort);
	bsLogin.Append(" ");
	bsLogin.Append(sClient);
	bsLogin.Append(" ");
	bsLogin.Append(sLink);

	Send(bsLogin.String(), NAPSTER_LOGIN);
}

void Napster::Send(const char *pBuffer, uint16 iType)
{
	BNetBuffer bnbSend;
	uint16     iFindLength;
	
//  note to PPC owners the whole Napster system was written with
//  no thought to non intel users, so they didn't use network
//  integers. You'll need to add a conversion function in here for the lengths
//  and byte reverse the uint16 constants in Constants.h

	iFindLength = (uint16)strlen(pBuffer);

	bnbSend.AppendData(&iFindLength, 2);	
	bnbSend.AppendData(&iType, 2);
	bnbSend.AppendData(pBuffer, (size_t)iFindLength);
	
	bneNapsterServer.Send(bnbSend);

	
}


bool Napster::Quit(void)
{
	return(myQuit);	
}