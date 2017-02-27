#include <stdafx.h>
#include "History.h"
#include "WorkFile.h"
#include "Task/ImgTask.h"
#include "Task/BlurTask.h"

///////////////////////////////////////////////////////////////////////////////
History::History(WorkFile& file)
	: file_(file)
	, timer_(std::bind(&History::CheckTask, this))
{
}

///////////////////////////////////////////////////////////////////////////////
History::History(History&& rhs)
	: file_(rhs.file_)
	, task_(std::move(rhs.task_))
	, futureTasks_(std::move(rhs.futureTasks_))
	, thread_(std::move(rhs.thread_))
	, timer_(std::bind(&History::CheckTask, this))
{
}

///////////////////////////////////////////////////////////////////////////////
History& History::operator= (History&& rhs)
{
	file_ = std::move(rhs.file_);
	task_ = std::move(rhs.task_);
	futureTasks_ = std::move(rhs.futureTasks_);
	thread_ = std::move(rhs.thread_);
	timer_ = Timer(std::bind(&History::CheckTask, this));

	return *this;
}

///////////////////////////////////////////////////////////////////////////////
const ImgTask* History::GetPrevTask() const
{
	return prevTasks_.empty() ? nullptr : prevTasks_.back().get();
}

///////////////////////////////////////////////////////////////////////////////
ImgTask* History::GetCurrentTask()
{
	return task_.get();
}

///////////////////////////////////////////////////////////////////////////////
void History::PushNewTask(ImgTask* newTask)
{
	AssertReturnUnless(newTask);

	if (task_)
	{
		futureTasks_.emplace_back(UniqueTask(newTask));
	}
	else
	{
		// start working
		task_.reset(newTask);
		thread_ = std::thread(&Task::Run, task_.get());
		timer_.Start(20);
	}
}

///////////////////////////////////////////////////////////////////////////////
void History::CancelCurrentTask()
{
	if (task_)
	{
		task_->Cancel();
	}
}

///////////////////////////////////////////////////////////////////////////////
void History::CheckTask()
{
	AssertReturnUnless(task_);

	auto dispatcher = GetMsgDispatcher();
	auto workingMsg = WorkingMsg(task_->GetName(), task_->GetPercent());
	dispatcher->Raise(workingMsg);

	if (task_->IsRunning())
	{
		// working
		task_->Sync(file_.GetImage());	
	}
	else
	{
		if (task_->IsCanceled())
		{
			file_.SetImage(task_->GetSrcImage());
			task_.reset();
		}
		else
		{
			file_.SetImage(task_->GetDstImage());
			prevTasks_.emplace_back(std::move(task_));
			task_.reset();
		}		
		
		if (futureTasks_.empty())
		{
			// no more tasks
			timer_.Stop();
			thread_.join();
		}
		else
		{
			// start next task
			thread_.join();
			task_ = std::move(futureTasks_.front());
			futureTasks_.pop_front();
			thread_ = std::thread(&Task::Run, task_.get());
		}
	}
}


///////////////////////////////////////////////////////////////////////////////