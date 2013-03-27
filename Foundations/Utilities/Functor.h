#ifndef __FUNCTOR_H__ 
#define __FUNCTOR_H__

#include "Debug.h"

// inspired by: http://www.newty.de/fpt/functor.html

// template for virtual base functor as common interface for all callbacks
template <class TSender, class TArgs>
class FunctorBase {
public:
    virtual ~FunctorBase() {};
    
public:
    // two possible functions to call member function. virtual cause derived
    // classes will use a pointer to an object and a pointer to a member function
    // to make the function call
    virtual void operator()(TSender&, const TArgs&) = 0;	// call using operator
    virtual void Invoke(TSender&, const TArgs&) = 0;		// call using function
	virtual uint64 GetHash() = 0;							// identifies the function to be called
};

// template for specific functor; stores pointers to the object and its callback funtion
template <class TSender, class TArgs, class TTarget> 
class Functor : public FunctorBase<TSender, TArgs> {
private:
	void (TTarget::*m_pxFunc)(const TSender&, const TArgs&);	// pointer to member function
	TTarget* m_pxObj;											// pointer to object
	uint64 m_llHash;

public:
	Functor(TTarget* obj, void(TTarget::*func)(const TSender&, const TArgs&)) { 
		m_pxObj = obj;  
		m_pxFunc = func; 

		m_llHash = GetHash(obj, func);
	};

	virtual void operator()(TSender& sender, const TArgs& arg) { 
		IW_CALLSTACK_SELF;
		(*m_pxObj.*m_pxFunc)(sender, arg);				// execute member function
	};              

	virtual void Invoke(TSender& sender, const TArgs& arg) { 
		IW_CALLSTACK_SELF;
		(*m_pxObj.*m_pxFunc)(sender, arg);				// execute member function
	};             

	virtual uint64 GetHash() {
		return m_llHash;
	}

protected:
	uint64 GetHash(TTarget* obj, void(TTarget::*func)(const TSender&, const TArgs&)) {
		uint64 hash = ((uint64)obj) << 32;
		// herer is some pointer magic because I didn't manage to cast 
		// the function pointer to void* without tricking the compiler
		return hash + ((uint64)*((long*)&func)); 
	}
};

#endif
