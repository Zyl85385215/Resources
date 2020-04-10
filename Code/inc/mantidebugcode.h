#pragma once 

#define _WIN32_WINNT 0x0500
#include <Windows.h>
#include <Winbase.h>
#include <winternl.h>
#ifndef _DEBUG
// 加了这个貌似就不能进行adplus挂钩了把
//#define ANTI_DEBUG
#endif // _DEBUG
class CAntiDebugCode
{
public:
	// 一 ======= PEB中 BeingDebugged：调试会重置
	// -- 反调试 
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
	// -- 解决 
	static BOOL	HideDebugger(HANDLE hThread, HANDLE hProcess)
	{// 清除BeginDebuged标记的代码
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

	// 二 ======= 检测 ntGlobalFlag 是否被设置为调试：被调试会被设置成0x70
	// -- 反调试 
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
	// -- 解决 
	// 源码中的LdrQueryImageFileExecutionOptions函数如果成功的话就不会修改NtGlobalFlag，这个函数取注册表的。
	// HKLM_Software\Microsoft\Windows Nt\CurrentVersion\Image File Execution Options
	// 如果在这里建一个名为进程名，值为空的子键，那么NtGlobalFlag(及其引发的)的检测都变得无效了。

	// 三 ==== Heap Magic 
	// Note: BeginDebuged为TRUE时会设置NtGlobalFlag设置了FLG_HEAP_VALIDATE_PARAMETERS,因此RtlCreateHeap选择用RtlDebugCreateHeap。 
	static LPVOID	getHeap(SIZE_T nSize)
	{
		return HeapAlloc(GetProcessHeap(), NULL, nSize);
	}
	// -- 反调试
	static BOOL		isDebugHeap1(VOID)
	{
#ifdef ANTI_DEBUG
		LPVOID HeapPtr;
		PDWORD ScanPtr;
		ULONG nMagic = 0;

		HeapPtr = getHeap(0x100);

		ScanPtr = (PDWORD)HeapPtr;
		// 直到内存无效，SEH会捕捉错误。
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

	// ---- 解决
	// 同二，切断二即可

	// 四====因为RtlCreateHeap中的flag被修改，ForceFlags也会被修改,那么就可以检测ForceFlags
	// -- ForceFlags被修改，那么就可以检测PEB中+18h的内容
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

	// ====注：以上四个要解决的就是彻底消灭源头BeingDebuged，
	// (上面的都是BeingDebuged引起的蝴蝶效应,BeingDebuged-->NtGlobalFlag(FLG_HEAP_VALIDATE_PARAMETERS)-->Flags--->ForceFlags)
	// 编写调试器(或调试器插件)时，创建进程并调用WaitForDebugEvent后，在第一次LOAD_DLL_DEBUG_EVENT发生的位置置BeingDebugged=FALSE就可以了。
	// 做了上面的东西出现没法中断在系统断点，那么第二次LOAD_DLL_DEBUG_EVENT要将BeingDebugged=TRUE，之后停在系统断点，
	// 此时可以安全的清除BegingDebuged。

	// 五====使用函数CheckRemoteDebuggerPresent
	// --- 本质是使用NtQueryInformationProcess()，查询进程的ProcessDebugPort，标识进程与调试器的通信。
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

	// 六====隐藏线程调试事件，这样在调试器中就没有任何信息
	// --- 设置Thread对象的HideFormDebugger成员为TRUE
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

	// 七====使用ZwQuerySystemInformation查看是否有系统调试器在
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


	// ==== 获取其他进程PEB
	// ---- 使用Native API NtQueryInfomationProcess 
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