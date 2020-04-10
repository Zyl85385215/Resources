#pragma once 

#define _WIN32_WINNT 0x0500
#include <Windows.h>
#include <Winbase.h>
#include <winternl.h>
#ifndef _DEBUG
// �������ò�ƾͲ��ܽ���adplus�ҹ��˰�
//#define ANTI_DEBUG
#endif // _DEBUG
class CAntiDebugCode
{
public:
	// һ ======= PEB�� BeingDebugged�����Ի�����
	// -- ������ 
	static bool isDebuggerPresent1()
	{
#ifdef ANTI_DEBUG
		_asm{
			mov eax,fs:[0x30]
			movzx eax, byte ptr[eax+2]
		}
#else
		return false;
#endif // ANTI_DEBUG
	}
	// -- ��� 
	static BOOL	HideDebugger(HANDLE hThread, HANDLE hProcess)
	{// ���BeginDebuged��ǵĴ���
		CONTEXT ctx;
		LDT_ENTRY sel;
		DWORD fs;
		DWORD peb;
		SIZE_T bytesrw;
		WORD flag;
		ctx.ContextFlags = CONTEXT_SEGMENTS;
		if (!GetThreadContext(hThread, &ctx))
			return FALSE;
		if (!GetThreadSelectorEntry(hThread, ctx.SegFs, &sel))
			return FALSE;
		fs = (sel.HighWord.Bytes.BaseHi << 8 | sel.HighWord.Bytes.BaseMid) << 16 |
			sel.BaseLow;    
		if (!ReadProcessMemory(hProcess, (LPCVOID)(fs + 0x30), &peb, 4, &bytesrw) ||
			bytesrw != 4)
			return FALSE;
		if (!ReadProcessMemory(hProcess, (LPCVOID)(peb + 0x2), &flag, 2, &bytesrw) ||
			bytesrw != 2)
			return FALSE;
		flag = 0;
		if (!WriteProcessMemory(hProcess, (LPVOID)(peb + 0x2), &flag, 2, &bytesrw) || bytesrw != 2)
			return FALSE;
		return TRUE;
	}

	// �� ======= ��� ntGlobalFlag �Ƿ�����Ϊ���ԣ������Իᱻ���ó�0x70
	// -- ������ 
	static bool isDebuggerPresent2()
	{
#ifdef ANTI_DEBUG
_asm{
			mov eax,fs:[0x30]
			mov eax,[eax+0x68]
			cmp eax,0x70
			jne END
			mov eax,1
END:
		}
#else
		return false;
#endif // ANTI_DEBUG
	}
	// -- ��� 
	// Դ���е�LdrQueryImageFileExecutionOptions��������ɹ��Ļ��Ͳ����޸�NtGlobalFlag���������ȡע���ġ�
	// HKLM_Software\Microsoft\Windows Nt\CurrentVersion\Image File Execution Options
	// ��������ｨһ����Ϊ��������ֵΪ�յ��Ӽ�����ôNtGlobalFlag(����������)�ļ�ⶼ�����Ч�ˡ�

	// �� ==== Heap Magic 
	// Note: BeginDebugedΪTRUEʱ������NtGlobalFlag������FLG_HEAP_VALIDATE_PARAMETERS,���RtlCreateHeapѡ����RtlDebugCreateHeap�� 
	static LPVOID	getHeap(SIZE_T nSize)
	{
		return HeapAlloc(GetProcessHeap(), NULL, nSize);
	}
	// -- ������
	static BOOL		isDebugHeap1(VOID)
	{
#ifdef ANTI_DEBUG
		LPVOID HeapPtr;
		PDWORD ScanPtr;
		ULONG nMagic = 0;

		HeapPtr = getHeap(0x100);

		ScanPtr = (PDWORD)HeapPtr;
		// ֱ���ڴ���Ч��SEH�Ჶ׽����
		try {
			for(;;) {
				switch (*ScanPtr++) 
				{
				case 0xABABABAB:
				case 0xBAADF00D:
				case 0xFEEEFEEE:
					nMagic++;
					break;
				}
			}
		}
		catch(...) {        
			return (nMagic > 10) ? TRUE : FALSE;
		}
#else
		return false;
#endif // ANTI_DEBUG
	}

	// ---- ���
	// ͬ�����ж϶�����

	// ��====��ΪRtlCreateHeap�е�flag���޸ģ�ForceFlagsҲ�ᱻ�޸�,��ô�Ϳ��Լ��ForceFlags
	// -- ForceFlags���޸ģ���ô�Ϳ��Լ��PEB��+18h������
	static BOOL	isDebugHeap2(VOID)
	{
#ifdef ANTI_DEBUG
		__asm {
			mov eax, fs:[0x30]
			mov eax, [eax+0x18]
			cmp dword ptr [eax+0x0C], 2
			jne __debugger_detected
			cmp dword ptr [eax+0x10], 0
			jne __debugger_detected
			xor eax, eax
__debugger_detected:
		}
#else
		return false;
#endif // ANTI_DEBUG
	}

	// ====ע�������ĸ�Ҫ����ľ��ǳ�������ԴͷBeingDebuged��
	// (����Ķ���BeingDebuged����ĺ���ЧӦ,BeingDebuged-->NtGlobalFlag(FLG_HEAP_VALIDATE_PARAMETERS)-->Flags--->ForceFlags)
	// ��д������(����������)ʱ���������̲�����WaitForDebugEvent���ڵ�һ��LOAD_DLL_DEBUG_EVENT������λ����BeingDebugged=FALSE�Ϳ����ˡ�
	// ��������Ķ�������û���ж���ϵͳ�ϵ㣬��ô�ڶ���LOAD_DLL_DEBUG_EVENTҪ��BeingDebugged=TRUE��֮��ͣ��ϵͳ�ϵ㣬
	// ��ʱ���԰�ȫ�����BegingDebuged��

	// ��====ʹ�ú���CheckRemoteDebuggerPresent
	// --- ������ʹ��NtQueryInformationProcess()����ѯ���̵�ProcessDebugPort����ʶ�������������ͨ�š�
	typedef BOOL (WINAPI *CHECK_REMOTE_DEBUGGER_PRESENT)(HANDLE, PBOOL);
	static BOOL isRemoteDebuggerPresent(VOID)
	{
#ifdef ANTI_DEBUG
		HANDLE      hProcess;
		HINSTANCE   hModule;    
		BOOL        bDebuggerPresent = FALSE;
		CHECK_REMOTE_DEBUGGER_PRESENT CheckRemoteDebuggerPresent;
		hModule = GetModuleHandleA("Kernel32");
		CheckRemoteDebuggerPresent = (CHECK_REMOTE_DEBUGGER_PRESENT)GetProcAddress(hModule, "CheckRemoteDebuggerPresent");
		hProcess = GetCurrentProcess();
		return CheckRemoteDebuggerPresent(hProcess, &bDebuggerPresent) ? bDebuggerPresent : FALSE;
#else
		return false;
#endif // ANTI_DEBUG
	}

	// ��====�����̵߳����¼��������ڵ������о�û���κ���Ϣ
	// --- ����Thread�����HideFormDebugger��ԱΪTRUE
	typedef DWORD (WINAPI *ZW_SET_INFORMATION_THREAD)(HANDLE, DWORD, PVOID, ULONG);
	#define ThreadHideFromDebugger 17
	static VOID disableThreadDebugEvent(VOID)
	{
#ifdef ANTI_DEBUG
		HINSTANCE hModule;
		ZW_SET_INFORMATION_THREAD ZwSetInformationThread;
		hModule = GetModuleHandleA("Ntdll");
		ZwSetInformationThread = 
			(ZW_SET_INFORMATION_THREAD)GetProcAddress(hModule, "ZwSetInformationThread");
		ZwSetInformationThread(GetCurrentThread(), ThreadHideFromDebugger, NULL, NULL);
#endif // ANTI_DEBUG
	}

	// ��====ʹ��ZwQuerySystemInformation�鿴�Ƿ���ϵͳ��������
	#define SystemKernelDebuggerInformation 35
	#pragma pack(4)
	typedef struct _SYSTEM_KERNEL_DEBUGGER_INFORMATION 
	{ 
		BOOLEAN DebuggerEnabled; 
		BOOLEAN DebuggerNotPresent; 
	} SYSTEM_KERNEL_DEBUGGER_INFORMATION, *PSYSTEM_KERNEL_DEBUGGER_INFORMATION;
	typedef DWORD (WINAPI *ZW_QUERY_SYSTEM_INFORMATION)(DWORD, PVOID, ULONG, PULONG);
	static BOOL	isKernelDbgr(VOID)
	{
#ifdef ANTI_DEBUG
		HINSTANCE hModule = GetModuleHandleA("Ntdll");
		ZW_QUERY_SYSTEM_INFORMATION ZwQuerySystemInformation = 
			(ZW_QUERY_SYSTEM_INFORMATION)GetProcAddress(hModule, "ZwQuerySystemInformation");
		SYSTEM_KERNEL_DEBUGGER_INFORMATION Info = {0};

		ZwQuerySystemInformation(
			SystemKernelDebuggerInformation,
			&Info, 
			sizeof(Info),
			NULL);

		return (Info.DebuggerEnabled && !Info.DebuggerNotPresent);
#else
		return false;
#endif // ANTI_DEBUG
	}


	// ==== ��ȡ��������PEB
	// ---- ʹ��Native API NtQueryInfomationProcess 
	static ULONG getPebBase(ULONG ProcessId)
	{
		HANDLE hProcess = NULL;
		PROCESS_BASIC_INFORMATION pbi = {0};
		ULONG peb = 0;        
		ULONG cnt = 0;
		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, ProcessId);
		if (hProcess != NULL) {
			if (NtQueryInformationProcess(hProcess,ProcessBasicInformation,&pbi,sizeof(PROCESS_BASIC_INFORMATION),&cnt) == 0) {
				return (ULONG)pbi.PebBaseAddress;
			}            
			CloseHandle(hProcess);
		}
		return -1;
	}
};