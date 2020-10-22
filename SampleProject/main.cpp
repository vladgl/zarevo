#ifdef _DEBUG && !defined(_ZRV_DEBUG)
#define ZRV_DEBUG
#endif
#include <zarevo.h>
#include "mWindow.h"

using namespace std;

int main()
{
	ZRV_LOG << "Proj started\n";
	mWindow wnd(1000, 800, "Sample Project");
	return wnd.exec();

}
