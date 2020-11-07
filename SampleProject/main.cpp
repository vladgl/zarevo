#ifdef _DEBUG && !defined(_ZRV_DEBUG)
#define ZRV_DEBUG
#endif
#include <zarevo.h>
#include "mWindow.h"

#ifdef ZRV_DEBUG
    #define __CRTDBG_MAP_ALLOC
    #include <crtdbg.h>
    #define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
    #define new DEBUG_NEW
#endif

using namespace std;

int dostuff()
{
	ZRV_LOG << "Proj started\n";
	mWindow wnd(1000, 800, "Sample Project");
    return wnd.exec();    
}

int main()
{
    int return_val = dostuff();

    #ifdef __CRTDBG_MAP_ALLOC
        _CrtDumpMemoryLeaks();
    #endif

	return return_val;
}
