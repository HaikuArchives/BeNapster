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

#ifndef COMMS_TREADS_H
#define COMMS_TREADS_H

#include <File.h>
#include <NetworkKit.h>
#include <String.h>
#include "GUI/LogWindow.h"
#include "GUI/DownloadWindow.h"
#include "GUI/DownloadView.h"
#include "Preferences/Preferences.h"
#include "Napster.h"

#include <stdlib.h>

int32 InCommsLoop(void *);
int32 OutCommsLoop(void *);
int32 DownloadLoop(void *);
int32 ReceiveLoop(void *);
int32 DoAccept(void *);
bool SendAFile(class LogWindow*, const char*);
bool UploadFromUser(class LogWindow*, const char*, uint32);


#endif //COMMS_TREADS_H
