#pragma once

enum ESAFE_LOCK_TYPE
{
	SAFE_LOCK_TYPE_BAG,		// ����
	SAFE_LOCK_TYPE_STORAGE,	// �ֿ�
	SAFE_LOCK_TYPE_SLAVE,	// ����
	SAFE_LOCK_TYPE_POINT,	// Ԫ��
};
#define SAFE_LOCK_TYPE_MAX (SAFE_LOCK_TYPE_POINT + 1)
#define MAX_LOCK_PASSWORD_LENTH 20

#define MAX_ERROR_PASS_TIMES 3
#define FORBID_PASS_TIME 3*60*60

static char * s_szSafeLockTypeName[SAFE_LOCK_TYPE_MAX] = {
	"����", "�ֿ�", "����", "Ԫ��"
};

class CSafeLock
{
public:
	CSafeLock(void);
	~CSafeLock(void);

	bool SetAutoLock(ESAFE_LOCK_TYPE eSafeLockType, bool bAutoLock);
	bool GetAutoLock(ESAFE_LOCK_TYPE eSafeLockType);

	inline void Lock(ESAFE_LOCK_TYPE eSafeLockType) { m_bLock[static_cast<int>(eSafeLockType)] = true; }
	inline void Unlock(ESAFE_LOCK_TYPE eSafeLockType) { m_bLock[static_cast<int>(eSafeLockType)] = false; }
	inline bool IsLocked(ESAFE_LOCK_TYPE eSafeLockType) { return m_bLock[static_cast<int>(eSafeLockType)]; }

protected:
	bool m_bAutoLock[SAFE_LOCK_TYPE_MAX];
	bool m_bLock[SAFE_LOCK_TYPE_MAX];
};