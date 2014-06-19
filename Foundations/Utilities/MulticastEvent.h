#ifndef __MULTICASTEVENT_H__
#define __MULTICASTEVENT_H__

#include "Functor.h"
#include "Debug.h"
#include "IwDebug.h"

#define EVENT_MAX_SUBSCRIBERS 5

// functor wrapper with subscriber support
template <class TPublisher, class TArgs> 
class MulticastEvent : FunctorBase<TPublisher, TArgs> {
private:
	FunctorBase<TPublisher, TArgs>* m_apxFunctor[EVENT_MAX_SUBSCRIBERS];

public:
	MulticastEvent() {
		for (int i = 0; i < EVENT_MAX_SUBSCRIBERS; i++) {
			m_apxFunctor[i] = NULL;
		}
	}

	virtual ~MulticastEvent() {
		for (int i = 0; i < EVENT_MAX_SUBSCRIBERS; i++) {
			if (m_apxFunctor[i]) {
				delete m_apxFunctor[i];
			}
		}
	}

	template <class TSubscriber>
	void AddListener(TSubscriber* obj, void(TSubscriber::*func)(const TPublisher&, const TArgs&)) {
		IW_CALLSTACK_SELF;

		IwAssertMsg(MYAPP, obj != NULL, ("Target instance must not be NULL."));
		for (int i = 0; i < EVENT_MAX_SUBSCRIBERS; i++) {
			if (!m_apxFunctor[i]) {
				m_apxFunctor[i] = new Functor<TPublisher, TArgs, TSubscriber>(obj, func);
				break;
		}
			if (i == EVENT_MAX_SUBSCRIBERS - 1) {
				IwAssertMsg(MYAPP, false, ("Functor could not be created. The maximum number of subscribers has been exceeded."));
			}
		}
	};

	template <class TSubscriber>
	void RemoveListener(TSubscriber* obj, void(TSubscriber::*func)(const TPublisher&, const TArgs&)) {
		IW_CALLSTACK_SELF;

		// create instance to be able to calculate hash
		Functor<TPublisher, TArgs, TSubscriber> functor(obj, func);
		uint64 hash = functor.GetHash();

		// find the subscriber, and delete its reference
		for (int i = 0; i < EVENT_MAX_SUBSCRIBERS; i++) {
			if (m_apxFunctor[i] && m_apxFunctor[i]->GetHash() == hash) {
				delete m_apxFunctor[i];
				m_apxFunctor[i] = NULL;
				break;
			}
			if (i == EVENT_MAX_SUBSCRIBERS - 1) {
				IwAssertMsg(MYAPP, false, ("Functor is not set. Failed to remove subscriber."));
			}
		}
	};

	virtual void operator()(TPublisher& sender, const TArgs& arg) {
		IW_CALLSTACK_SELF;
		for (int i = 0; i < EVENT_MAX_SUBSCRIBERS; i++) {
			if (m_apxFunctor[i]) {
				m_apxFunctor[i]->operator()(sender, arg);
			}
		}
	};

	virtual void Invoke(TPublisher& sender, const TArgs& arg) {
		IW_CALLSTACK_SELF;
		for (int i = 0; i < EVENT_MAX_SUBSCRIBERS; i++) {
			if (m_apxFunctor[i]) {
				m_apxFunctor[i]->Invoke(sender, arg);
			}
		}
	};

	virtual uint64 GetHash() {
		return (uint64) this;
	}
};

#endif
