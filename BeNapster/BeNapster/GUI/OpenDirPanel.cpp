#include "OpenDirPanel.h"

OpenDirPanel::OpenDirPanel( BMessenger* target,
                              BRefFilter* filter )
  : BFilePanel( B_OPEN_PANEL, target, NULL,
                B_DIRECTORY_NODE, false, NULL, filter)
{
  BWindow  *w;

  //  1) Get the window from the FilePanel
  //     so we can find the window's children

  w = Window();

  if (w->Lock()) {
    BRect btnrect;
    //  2) Find the first view in the window,
    //     and from that view we find the cancel button
    BView *v = w->ChildAt(0);
    BView *cancelBtn = v->FindView("cancel button");
 
    if (cancelBtn) {
      BView *parentview;
      float charWidth;
      //  3) Construct and add the new 'Select Current
      //     Directory' button
      charWidth    =    cancelBtn->StringWidth("Select Current Directory");
      btnrect = cancelBtn->Frame();
      btnrect.right = btnrect.left - 10;
      btnrect.left = btnrect.right - charWidth - 40;
      fCurrentDirBtn = new BButton(btnrect,
        "current dir button", "Select Current Directory",
        NULL, B_FOLLOW_RIGHT + B_FOLLOW_BOTTOM);
      //  4) Set the target for the new button
      fCurrentDirBtn->SetTarget(*target);
      parentview = cancelBtn->Parent();
      parentview->AddChild(fCurrentDirBtn);

      //  5) Set the message for the new button
      entry_ref ref;
      BMessage *msg = new BMessage('slct');
      GetPanelDirectory(&ref);
      msg->AddRef("refs",&ref);
      fCurrentDirBtn->SetMessage(msg);
    }
    w->Unlock();
  }
}

void OpenDirPanel::SelectionChanged()
{
  BWindow *wind;

  wind = Window();

  if (wind->Lock()) {
    entry_ref currRef;
    GetPanelDirectory(&currRef);
    //  modify the btn's msg
    BMessage *msg = new BMessage('slct');
    msg->AddRef("refs",&currRef);
    fCurrentDirBtn->SetMessage(msg);
    wind->Unlock();
  }
}

bool OpenDirFilter::Filter(const entry_ref* e, BNode* n, struct stat* s,
						   const char* mimetype)
{
	if (strcmp("application/x-vnd.Be-directory",
             mimetype) == 0)
    	return true;
    else if (strcmp("application/x-vnd.Be-volume",
                  mimetype) == 0)
    	return true;
  	else
    	return false;
}