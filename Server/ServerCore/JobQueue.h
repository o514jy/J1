#pragma once
#include "Job.h"
#include "LockQueue.h"
#include "JobTimer.h"

/*--------------
	JobQueue
---------------*/

class JobQueue : public enable_shared_from_this<JobQueue>
{
public:
	void DoAsync(CallbackType&& callback)
	{
		Push(make_shared<Job>(std::move(callback)));
	}

	template<typename T, typename Ret, typename... Args>
	void DoAsync(Ret(T::*memFunc)(Args...), Args... args)
	{
		shared_ptr<T> owner = static_pointer_cast<T>(shared_from_this());
		Push(make_shared<Job>(owner, memFunc, std::forward<Args>(args)...));
	}

	JobRef DoTimer(uint64 tickAfter, CallbackType&& callback)
	{
		JobRef job = make_shared<Job>(std::move(callback));
		PushAfter(job, tickAfter, shared_from_this());
		//GJobTimer->Reserve(tickAfter, shared_from_this(), job);
		return job;
	}

	template<typename T, typename Ret, typename... Args>
	JobRef DoTimer(uint64 tickAfter, Ret(T::* memFunc)(Args...), Args... args)
	{
		shared_ptr<T> owner = static_pointer_cast<T>(shared_from_this());
		JobRef job = make_shared<Job>(owner, memFunc, std::forward<Args>(args)...);
		PushAfter(job, tickAfter, shared_from_this());
		//GJobTimer->Reserve(tickAfter, shared_from_this(), job);
		return job;
	}

	template<typename T, typename Ret, typename... Args>
	JobRef DoTimer(uint64 tickAfter, shared_ptr<T> InOwner, Ret(T::* memFunc)(Args...), Args... args)
	{
		shared_ptr<T> owner = static_pointer_cast<T>(InOwner);
		JobRef job = make_shared<Job>(owner, memFunc, std::forward<Args>(args)...);
		PushAfter(job, tickAfter, shared_from_this());
		//GJobTimer->Reserve(tickAfter, shared_from_this(), job);
		return job;
	}

	void					ClearJobs() { _jobs.Clear(); }

public:
	virtual void			Push(JobRef job, bool pushOnly = false);
	virtual void			PushAfter(JobRef job, uint64 tickAfter, weak_ptr<JobQueue> owner);
	void					Execute();

protected:
	LockQueue<JobRef>		_jobs;
	atomic<int32>			_jobCount = 0;
};

