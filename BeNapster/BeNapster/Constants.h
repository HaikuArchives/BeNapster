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

#ifndef _CONSTANTS_H
#define _CONSTANTS_H

const uint32 BENAPSTER_FIND         	= 'BNmf';
const uint32 BENAPSTER_GET         		= 'BNmg';
const uint32 BENAPSTER_FIND_END     	= 'BNfe';
const uint32 BENAPSTER_REGISTER			= 'BNrg';
const uint32 BENAPSTER_REGISTER_WINDOW	= 'BNrd';
const uint32 BENAPSTER_CONNECT			= 'BNct';
const uint32 BENAPSTER_DISCONNECT		= 'BNdc';
const uint32 BENAPSTER_PREFERENCES		= 'BNpf';
const uint32 BENAPSTER_FIND_WINDOW     	= 'BNfw';
const uint32 BENAPSTER_RESUME_WINDOW   	= 'BNrw';



// PPC owners will need to 'byte reverse' these to little endian


const uint16 NAPSTER_LOGIN			= 0x0002;  
const uint16 NAPSTER_LOGIN_OKAY		= 0x0003;  

const uint16 NAPSTER_FIRST_LOGIN	= 0x0006;  
const uint16 NAPSTER_REGISTER		= 0x0007;  
const uint16 NAPSTER_NICK_OKAY		= 0x0008;  
const uint16 NAPSTER_NICK_TAKEN		= 0x0009; 
const uint16 NAPSTER_NICK_INVALID	= 0x000A; 

const uint16 NAPSTER_FIND			= 0x00C8;  
const uint16 NAPSTER_FOUND			= 0x00C9;  
const uint16 NAPSTER_FIND_END		= 0x00CA;  
const uint16 NAPSTER_GET        	= 0x00CB;

const uint16 NAPSTER_DOWNLOAD_ACK   = 0x00CC;
const uint16 NAPSTER_REMOTE_SEND    = 0x01F4;

const uint32 PREFS_SAVE				= 'PWsv';
const uint32 PREFS_CONNECTYION_TYPE	= 'PWct';

#endif