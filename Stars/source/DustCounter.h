#ifndef __DUSTCOUNTER_H__
#define __DUSTCOUNTER_H__

class DustCounter {
private:
	bool m_bIsQueueing;
	
	int m_iQueuedCount;
	float m_fQueuedAmount;
	
	float m_fMaxDust;
	float m_fCollectedDust;
	
public:
	DustCounter(float maxdust);

	// insert
	void AddDust(float dustamount);

	// queue
	bool IsQueueing();
	void BeginDustQueue();
	void CommitDustQueue();
	void RollbackDustQueue();
	
	int GetQueuedDustCount();
	float GetQueuedDustAmount();
	float GetQueuedDustPercent();

	// container
	float GetMaxDustAmount();
	float GetCollectedDustAmount();
	float GetDustFillPercent();
};

#endif
