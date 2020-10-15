#ifdef _DEBUG
#define ZRV_DEBUG
#endif
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <zarevo.h>
#include "mWindow.h"

using namespace std;

int main()
{
	ZRV_LOG << "Proj started";
	mWindow wnd(500, 400, "Sample Project");
	return wnd.exec();

}
