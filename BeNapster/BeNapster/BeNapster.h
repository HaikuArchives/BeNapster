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

#include <Application.h>
#include <NetAddress.h>
#include <NetEndpoint.h>
#include <NetBuffer.h>
#include <Rect.h>

#include "GUI/LogWindow.h"

class BeNapster :public BApplication
{
public:
// insert public methods and data here
// constructor
	BeNapster();
private:
	LogWindow *myLogWindow;
};
// END (class declaration)
