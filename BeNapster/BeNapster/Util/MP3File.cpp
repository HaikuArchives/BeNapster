#include "MP3File.h"
#include <stdlib.h>

MP3File::MP3File(entry_ref ref) : BFile(&ref, B_READ_ONLY)
{
	mp3ref = ref;
	
	// This will probably do BadThings(tm) if the initialization fails
	// Seek to the position in the file we want, just to be sure.  
	// The MP3 starts with 12 bits of 1's (0xFFF)
	
	do {
		Read(&mp3header, 4);
	} while(mp3header.sync != 0xFFF);
	
	
}


// Returns Version Header from MP3
// 1 = mpeg1.0	2=mpeg2.0

unsigned int MP3File::GetVersion()
{
	return mp3header.version;
}

//Returns Layer type of the file in question
// Obviously, we only give a damn about layer 3, but what the heck
// 1 = layer 1	2 = layer 2	3 = layer 3

unsigned int MP3File::GetLayer()
{
	return mp3header.lay;
}

// Returns error protection bit from MP3 Header
// 0 = yes 1 = no
unsigned int MP3File::GetErrorProtection()
{
	return mp3header.error;
}


// Returns the 4-bit integer representing the bitrate, format is as follows:
/*
	mpeg1.0
	
			1	2	3	4	5	6	7	8	9	10	11	12	13	14
	layer1	32	64	96	128	160	192	224	256	288	320	352	384	416	448
	layer2	32	48	56	64	80	96	112	128	160	192	224	256	320	384
	layer3	32	40	48	56	64	80	96	112	128	160	192	224	256	320
	
	mpeg2.0
	
			1	2	3	4	5	6	7	8	9	10	11	12	13	14
	layer1	32	48	56	64	80	96	112	128	144	160	176	192	224	256
	layer2	8	16	24	32	40	48	56	64	80	96	112	128	144	160
	layer3	8	16	24	32	40	48	56	54	80	96	112	128	144	160
*/

unsigned int MP3File::GetBitrate()
{
	if(mp3header.version == 1) {
		if(mp3header.lay == 1) {
			switch(mp3header.bitrate) {
				case 1:
					return 32;
				case 2:
					return 64;
				case 3:
					return 96;
				case 4:
					return 128;
				case 5:
					return 160;
				case 6:
					return 192;
				case 7:
					return 224;
				case 8:
					return 256;
				case 9:
					return 288;
				case 10:
					return 320;
				case 11:
					return 352;
				case 12:
					return 384;
				case 13:
					return 416;
				case 14:
					return 448;
			}
		}
		
		if(mp3header.lay == 2) {
			switch(mp3header.bitrate) {
				case 1:
					return 32;
				case 2:
					return 48;
				case 3:
					return 56;
				case 4:;
					return 64;
				case 5:
					return 80;
				case 6:
					return 96;
				case 7:
					return 112;
				case 8:
					return 128;
				case 9:
					return 160;
				case 10:
					return 192;
				case 11:
					return 224;
				case 12:
					return 256;
				case 13:
					return 320;
				case 14:
					return 384;
			}
		}
		
		if(mp3header.lay == 3) {
			switch(mp3header.bitrate) {
				case 1:
					return 32;
				case 2:
					return 40;
				case 3:
					return 48;
				case 4:
					return 56;
				case 5:
					return 64;
				case 6:
					return 80;
				case 7:
					return 96;
				case 8:
					return 112;
				case 9:
					return 128;
				case 10:
					return 160;
				case 11:
					return 192;
				case 12:
					return 224;
				case 13:
					return 256;
				case 14:
					return 320;
			}
		}
	}
	
	if(mp3header.version == 0) {
		if(mp3header.lay == 1) {
			switch(mp3header.bitrate) {
				case 1:
					return 32;
				case 2:
					return 48;
				case 3:
					return 56;
				case 4:
					return 64;
				case 5:
					return 80;
				case 6:
					return 96;
				case 7:
					return 112;
				case 8:
					return 128;
				case 9:
					return 144;
				case 10:
					return 160;
				case 11:
					return 176;
				case 12:
					return 192;
				case 13:
					return 224;
				case 14:
					return 256;
			}
		}
		
		if(mp3header.lay == 2) {
			switch(mp3header.lay) {
				case 1:
					return 8;
				case 2:
					return 16;
				case 3:
					return 24;
				case 4:
					return 32;
				case 5:
					return 40;
				case 6:
					return 48;
				case 7:
					return 56;
				case 8:
					return 64;
				case 9:
					return 80;
				case 10:
					return 96;
				case 11:
					return 112;
				case 12:
					return 128;
				case 13:
					return 144;
				case 14:
					return 160;
			}
		}
		
		if(mp3header.lay == 3) {
			switch(mp3header.bitrate) {
				case 1:
					return 8;
				case 2:
					return 16;
				case 3:
					return 24;
				case 4:
					return 32;
				case 5:
					return 40;
				case 6:
					return 48;
				case 7:
					return 56;
				case 8:
					return 64;
				case 9:
					return 80;
				case 10:
					return 96;
				case 11:
					return 112;
				case 12:
					return 128;
				case 13:
					return 144;
				case 14:
					return 160;
			}
		}
	}	
// Shouldn't get here
return 128;
}

// Returns the sampling frequency of the MP3, format is as follows:
/*

mpeg1.0

0		1		2
44100	48000	32000

mpeg2.0

0		1		2
22050	24000	16000

*/

unsigned int MP3File::GetSamplingFreq() 
{
	
	// mpeg1.0
	if(mp3header.version == 1) {
		switch(mp3header.sampling) {
			case 0:
				return 44100;
			case 1:
				return 48000;
			case 2:
				return 32000;
		}
	}
	
	// mpeg2.0
	if(mp3header.version == 0) {
		switch(mp3header.sampling) {
			case 0:
				return 22050;
			case 1:
				return 24000;
			case 2:
				return 16000;
		}
	}

// Shouldn't get here, but this supresses a warning
	return 44100;

}


// Tells the amount of time the MP3 runs, might not be precise, but it'll be
// close (time is in seconds)

uint32 MP3File::GetTime() 
{
	uint32 fsize;
	
	GetSize((off_t *)&fsize);
	//returnval = ( fsize / ((GetBitrate()*1000) / 1024) );
	return 60;
}

// Returns the MD5 hash for the first 300k of the file (I read somewhere that
// this is what the "official" client does, so I'm copying it)

BString MP3File::GetMD5()
{
	MD5_CTX context;
	unsigned char *digest;	
	unsigned char *p;
	unsigned char buffer[1024];
	BString returnval;
	int bytes_read=0;
	
	digest = (unsigned char *) malloc(16);
	p = (unsigned char *) malloc(16);
	
	int n;
		
	p = (unsigned char *) malloc(16);
	
	
	// Initialize the MD5 Context
	MD5Init(&context);
	
	// Read the buffer
	while(bytes_read < 300000) {
		n=Read(buffer, 1024);
		if(n < 0) break;
		bytes_read+=n;
		MD5Update(&context, buffer, n);
	}
	MD5Final(digest, &context);
	
	for(int i = 0; i < 16; ++i) {
		sprintf(p, "%02x", *digest++);
		returnval += (char *)p;
	}
	
	return returnval;
}