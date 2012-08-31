//Are we on the windows platform?
#ifdef WIN32
//if so include the header for windows applications
#include <Windows.h>
#endif

#include "Win32Window.h"

//Check to see if we are on windows
#ifdef WIN32
int WINAPI WinMain(HINSTANCE hInstance, 
					HINSTANCE hPrevInstance, 
					LPSTR lpCmdLine,
					int nCmdShow)
#else
//we are on some other platform
int main(int argc, char **argv)
#endif
{
	CWin32Window * pWin=new CWin32Window();
	pWin->init(TEXT("My First Window"),860,640,false);

	while(pWin->running()){
		pWin->checkForWindowMessages();
	}

	delete pWin;

	return 0;
}

