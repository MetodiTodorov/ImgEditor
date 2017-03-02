#pragma once
///////////////////////////////////////////////////////////////////////////////
#include "Msg/MsgReceiver.h"

///////////////////////////////////////////////////////////////////////////////
class Commander : public MsgReceiver
{
public:
	Commander();
	~Commander();	

private:
	void OnMsgReceived(const Message& msg) override;
	void OnSave(const Message& msg);
	void OnEndOpenFile(const Message& msg);
	void OnBeginCloseFile(const Message& msg);
	void OnEndSaveAs(const Message& msg);
	void OnBeginBlurEffect(const Message& msg);	
	void OnBeginInverseEffect(const Message& msg);

private:
};

///////////////////////////////////////////////////////////////////////////////
extern Commander* GetCommander();
///////////////////////////////////////////////////////////////////////////////