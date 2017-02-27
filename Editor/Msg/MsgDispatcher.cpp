#include <stdafx.h>
#include "MsgDispatcher.h"

///////////////////////////////////////////////////////////////////////////////
MsgDispatcher::MsgDispatcher()
	: m_raiseLock(0)
{
}

///////////////////////////////////////////////////////////////////////////////
void MsgDispatcher::Raise(const Message& msg)
{
	m_raiseLock++;

	auto it = m_specReceivers.find(msg.GetType());
	if (it == m_specReceivers.end())
	{
		// m_receivers only
		for (MsgReceiver* receiver : m_receivers)
		{
			receiver->OnMsgReceived(msg);
		}
	}
	else
	{
		int allIdx = 0;
		int specIdx = 0;

		for (;;)
		{
			MsgReceiver* receiver = nullptr;
			if (allIdx < (int)m_receivers.size() && specIdx < (int)it->second.size())
			{
				// m_receivers && m_specificReceivers has
				if (m_receivers[allIdx]->Priority >= it->second[specIdx]->Priority)
				{
					receiver = m_receivers[allIdx++];
				}
				else
				{
					receiver = it->second[specIdx++];
				}
			}
			else if (allIdx < (int)m_receivers.size())
			{
				receiver = m_receivers[allIdx++];
			}
			else if (specIdx < (int)it->second.size())
			{
				receiver = it->second[specIdx++];
			}
			else
			{
				break;
			}

			AssertContinueUnless(receiver);
			if (!Contains(m_unregistered, receiver))
			{
				receiver->OnMsgReceived(msg);
			}
		}
	}

	m_raiseLock--;
	AssertReturnUnless(m_raiseLock >= 0);
	ReturnIf(m_raiseLock > 0);

	// receivers changes during raise
	{
		for (auto receiver : m_newReceivers)
		{
			Register(receiver);
		}
		m_newReceivers.clear();

		for (auto it : m_newSpecReceivers)
		{
			for (auto receiver : it.second)
			{
				Register(receiver, it.first);
			}
		}
		m_newSpecReceivers.clear();

		for (auto receiver : m_unregistered)
		{
			Unregister(receiver);
		}
		m_unregistered.clear();
	}
}

///////////////////////////////////////////////////////////////////////////////
void MsgDispatcher::Register(MsgReceiver* receiver, Message::Type msgType)
{
	if (m_raiseLock > 0)
	{
		// register during raise
		m_newSpecReceivers[msgType].push_back(receiver);
		return;
	}

	if (!IsRegistered(m_specReceivers[msgType], receiver))
	{
		m_specReceivers[msgType].push_back(receiver);
		std::sort(m_specReceivers[msgType].begin(), m_specReceivers[msgType].end(), ReceiversCompare);
	}
}

///////////////////////////////////////////////////////////////////////////////
void MsgDispatcher::Register(MsgReceiver* receiver)
{
	if (m_raiseLock > 0)
	{
		// register during raise
		m_newReceivers.push_back(receiver);
		return;
	}

	if (!IsRegistered(m_receivers, receiver))
	{
		m_receivers.push_back(receiver);
		std::sort(m_receivers.begin(), m_receivers.end(), ReceiversCompare);
	}
}

///////////////////////////////////////////////////////////////////////////////
void MsgDispatcher::Unregister(MsgReceiver* receiver)
{
	if (m_raiseLock > 0)
	{
		// unregister receiver during Raise
		m_unregistered.push_back(receiver);
		return;
	}

	// delete from m_specificReceivers
	for (auto& it : m_specReceivers)
	{
		Erase(it.second, receiver);
	}

	typedef std::pair<Message::Type, ReceiverContainer> MapValue;
	EraseIf(m_specReceivers, [](const MapValue & element) { return element.second.empty(); });

	// delete from m_receivers
	Erase(m_receivers, receiver);
}

///////////////////////////////////////////////////////////////////////////////
bool MsgDispatcher::IsRegistered(const ReceiverContainer& container, MsgReceiver* receiver) const
{
	return Contains(container, receiver);
}

///////////////////////////////////////////////////////////////////////////////