void main()
{
HANDLE hHandle = GetModuleHandle("gpkcsp.dll");
 
if(hHandle == 0)
{
 
  LoadLibraryA("gpkcsp.dll");
 
  hHandle = GetModuleHandle("gpkcsp.dll");
 
}

LogDebug("gpkcsp.dll base adres: 0x%x", hHandle);

DWORD baseadres = (DWORD)hHandle;

DWORD offset = 0x0000E280;

DWORD patchadres = (DWORD)hHandle + (DWORD)offset;	

LogDebug("patchlenecek adres: 0x%x", patchadres);

unsigned long OldProtection;
VirtualProtect((LPVOID)(patchadres), 1, PAGE_EXECUTE_READWRITE, &OldProtection);
int* pcontent = (int*)patchadres;
int content = *pcontent;
LogDebug("Patch Oncesi %p: content %08x\n", pcontent, content);

DWORD patch1 = (DWORD)patchadres;
DWORD patch2 = (DWORD)patchadres+1;
DWORD patch3 = (DWORD)patchadres+2;
DWORD patch4 = (DWORD)patchadres+3;
DWORD patch5 = (DWORD)patchadres+4;
DWORD patch6 = (DWORD)patchadres+5;

*(char *)patch1 = 0x90; /* one byte NOP */
*(char *)patch2 = 0xB9; /* MOV ECX, 0x80 */
*(char *)patch3 = 0x80;
*(char *)patch4 = 0x00;
*(char *)patch5 = 0x00;
*(char *)patch6 = 0x00;

}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	int processId = GetCurrentProcessId();

	if(fdwReason == DLL_PROCESS_ATTACH)
	{
		DWORD id;
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)main, NULL, 0, &id);
	}

	return TRUE;
}