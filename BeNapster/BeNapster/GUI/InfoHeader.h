#ifndef INFO_HEADER_H
#define INFO_HEADER_H

#include <View.h>
#include <stdio.h>
//#include "StyleUtils.h"

class InfoHeader : public BView
{
public:
	InfoHeader();
	~InfoHeader();
	InfoHeader(BRect frame);
	virtual void Draw(BRect dummy);
private:
};

#endif