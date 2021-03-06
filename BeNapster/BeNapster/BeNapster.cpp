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
//

#include "BeNapster.h"

int main()
{

	BeNapster *benapster;
	benapster = new BeNapster();	
	benapster->Run();
	delete benapster;

}

BeNapster::BeNapster(): BApplication("application/x-vnd.VargolSoft-BeNapster")
{

	
	myLogWindow = new LogWindow(BRect(100,100, 600, 400), "BeNapster", 
			B_DOCUMENT_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL, 0, 
			B_CURRENT_WORKSPACE);
	myLogWindow->Show();

}

void BeNapster::MessageReceived(BMessage* msg)
{
	printf("App Message!\n");
	if (msg->what == BENAPSTER_FIND_WINDOW_GONE)
	{
		myLogWindow->PostMessage(BENAPSTER_FIND_WINDOW_GONE);
		printf("Find_Close sent.\n");
	};
};