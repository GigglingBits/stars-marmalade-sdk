#ifndef __DUSTCOUNTER_H__
#define __DUSTCOUNTER_H__

class DustCounter {
private:
	int m_iQueuedCount;
	float m_fQueuedAmount;
	
	float m_fMaxDust;
	float m_fCollectedDust;
	
public:
	DustCounter(float maxdust);

	// queue
	void EnqueueDust(float dustamount);
	void CommitDustQueue();
	void ClearDustQueue();
	
	int GetQueuedDustCount();
	float GetQueuedDustAmount();

	// container
	float GetMaxDustAmount();
	float GetCollectedDustAmount();
	float GetDustFillPercent();
};

#endif
