#ifndef __MP3FILE_H
	#define __MP3FILE_H

#include <Be.h>
#include <File.h>
#include "MD5.h"

class MP3File : public BFile
{
	public:
		MP3File(entry_ref);
		
		unsigned int GetVersion();
		unsigned int GetLayer();
		unsigned int GetErrorProtection();
		unsigned int GetBitrate();
		unsigned int GetSamplingFreq();
		uint32 GetTime();
		BString GetMD5();
		
	private:
		struct {
			unsigned int sync:12, version:1, lay:2, error:1, bitrate:4,
					 sampling:2, padding:1, extension:1, mode:2, mode_ext:2,
					 copyright:1, original:1, emphasis:2;
			} mp3header;

		entry_ref mp3ref;
};


#endif	/* __MP3FILE_H */