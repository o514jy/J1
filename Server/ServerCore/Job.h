#pragma once
#include <functional>

/*---------
	Job
----------*/

using CallbackType = std::function<void()>;

class Job
{
public:
	Job(CallbackType&& callback) : _callback(std::move(callback))
	{
		_cancled = false;
	}

	template<typename T, typename Ret, typename... Args>
	Job(shared_ptr<T> owner, Ret(T::* memFunc)(Args...), Args&&... args)
	{
		_callback = [owner, memFunc, args...]()
		{
			(owner.get()->*memFunc)(args...);
		};

		_cancled = false;
	}

	void Execute()
	{
		if (_cancled == false)
			_callback();
	}

	void SetCancled(bool flag) { _cancled = flag; }
	bool GetCancled() { return _cancled; }

private:
	CallbackType	_callback;
	bool			_cancled;
};

