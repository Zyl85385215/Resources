#include "safelock.h"
#include <windows.h>

CSafeLock::CSafeLock( void )
{
	for(int i=0; i<SAFE_LOCK_TYPE_MAX; i++)
	{
		m_bLock[i] = false;
		m_bAutoLock[i] = false;
	}
}

CSafeLock::~CSafeLock( void )
{

}

bool CSafeLock::SetAutoLock( ESAFE_LOCK_TYPE eSafeLockType, bool bAutoLock )
{
	m_bAutoLock[static_cast<int>(eSafeLockType)] = bAutoLock;
	return true;
}

bool CSafeLock::GetAutoLock( ESAFE_LOCK_TYPE eSafeLockType )
{
	return m_bAutoLock[static_cast<int>(eSafeLockType)];
}