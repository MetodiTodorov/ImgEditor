#pragma once
///////////////////////////////////////////////////////////////////////////////
#include <thread>
#include "Core/Timer.h"
#include "Task/ImgTask.h"

///////////////////////////////////////////////////////////////////////////////
class WorkFile;

///////////////////////////////////////////////////////////////////////////////
class History
{
public:
	History(WorkFile& file);
	History(History&& rhs);
	History&		operator= (History&& rhs);

	const ImgTask*	GetPrevTask() const;
	ImgTask*		GetCurrentTask();
	void			PushNewTask(ImgTask* newTask);	
	void			CancelCurrentTask();

private:
	void			CheckTask();

private:
	typedef std::unique_ptr<ImgTask> UniqueTask;
	typedef std::deque<UniqueTask> TaskQueue;

	WorkFile&		file_;
	UniqueTask		task_;
	TaskQueue		prevTasks_;
	TaskQueue		futureTasks_;
	std::thread		thread_;
	Timer			timer_;
};

///////////////////////////////////////////////////////////////////////////////