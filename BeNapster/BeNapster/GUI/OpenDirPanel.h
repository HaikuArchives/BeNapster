#ifndef __OPENDIRPANEL_H
	#define __OPENDIRPANEL_H
	
#include <Window.h>
#include <Button.h>
#include <FilePanel.h>

class OpenDirPanel : public BFilePanel
{
	public:
		OpenDirPanel( BMessenger* target,
					  BRefFilter* filter=NULL );
		void SelectionChanged();
		
	private:
		BButton *fCurrentDirBtn;
};

class OpenDirFilter : public BRefFilter
{
	public:
  	  bool Filter(const entry_ref* e, BNode* n, struct stat* s,
    	          const char* mimetype);
};

#endif	/* __OPENDIRPANEL_H */