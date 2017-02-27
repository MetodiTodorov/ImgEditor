#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "MsgReceiver.h"
#include "Message.h"
#include <map>
#include <vector>

////////////////////////////////////////////////////////////////////////////////////////////////////
class MsgDispatcher
{
public:
	MsgDispatcher();

	void Raise(const Message& msg);

	void Register(MsgReceiver* receiver, Message::Type msgType);
	void Register(MsgReceiver* receiver);

	void Unregister(MsgReceiver* receiver);

protected:
	typedef std::vector<MsgReceiver*>					ReceiverContainer;
	typedef std::map<Message::Type, ReceiverContainer>	ReceiverMap;
	bool IsRegistered(const ReceiverContainer& container, MsgReceiver* receiver) const;

	ReceiverMap			m_specReceivers;	// receivers for specific messages
	ReceiverContainer	m_receivers;		// receivers for all messages

	int					m_raiseLock;
	ReceiverContainer	m_unregistered;		// unregistered during raise
	ReceiverMap			m_newSpecReceivers;	// registered during raise
	ReceiverContainer	m_newReceivers;		// registered during raise
};

////////////////////////////////////////////////////////////////////////////////////////////////////
extern MsgDispatcher* GetMsgDispatcher();

////////////////////////////////////////////////////////////////////////////////////////////////////



