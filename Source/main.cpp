#include <windows.h>
#include <winternl.h>
typedef NTSTATUS(NTAPI* RtlAdjustPrivilege)(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN PreviousValue);
typedef NTSTATUS(NTAPI* NtShutdownSystem)(IN SHUTDOWN_ACTION Action);

INT main() {
	HMODULE hNtdll = LoadLibraryW(L"ntdll.dll");
	if (!hNtdll) {
		return 0x01;
	}

	RtlAdjustPrivilege pRtlAdjustPrivilege = (RtlAdjustPrivilege)GetProcAddress(hNtdll, "RtlAdjustPrivilege");
	NtShutdownSystem pNtShutdownSystem = (NtShutdownSystem)GetProcAddress(hNtdll, "NtShutdownSystem");

	if (!pRtlAdjustPrivilege || !pNtShutdownSystem) {
		FreeLibrary(hNtdll);
		return 0x01;
	}

	BOOLEAN previousValue;
	NTSTATUS status = pRtlAdjustPrivilege(19 /* SeShutdownPrivilege */, TRUE, FALSE, &previousValue);

	if (status == 0) {
		NtShutdownSystem(2);
	}

	FreeLibrary(hNtdll);
	return 0x00;
}
