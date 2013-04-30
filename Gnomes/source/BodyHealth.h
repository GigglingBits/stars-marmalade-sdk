#ifndef __BODYHEALTH_H__
#define __BODYHEALTH_H__

class BodyHealth {
private:
	float m_fResilience;
	int m_iHealth;

public:
	BodyHealth();

	void SetResilience(float resi);
	void Injure(float impulse);

	void RenewLife();
	void Kill();
	bool IsDead();

	int GetHealthValue();
};

#endif
