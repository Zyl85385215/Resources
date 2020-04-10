#include "main.h"
#include "AppDelegate.h"
#include "CCEGLView.h"
#include "GlobalDefine.h"

USING_NS_CC;

// uncomment below line, open debug console
// #define USE_WIN32_CONSOLE

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

#ifdef USE_WIN32_CONSOLE
    AllocConsole();
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
#endif

    // create the application instance
    AppDelegate app;
    CCEGLView* eglView = CCEGLView::sharedOpenGLView();
	//eglView->setFrameSize(480, 320);
	eglView->setFrameSize(1280, 720);
	//eglView->setFrameSize(1136, 640);
	//eglView->setFrameSize(800, 480);
	//eglView->setFrameSize(1920, 1080);
	
	//eglView->setContentScaleFactor (1.1f);

    int ret = CCApplication::sharedApplication()->run();

#ifdef USE_WIN32_CONSOLE
    FreeConsole();
#endif

    return ret;
}
