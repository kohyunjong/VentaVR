#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

#include "main.h"
//#include "Layout/Layout.h"
#include <fstream>
#include <windows.h>

#if 1

int timeToString(struct tm *t)
{
	static char s[20];
	int year = t->tm_year + 1900;
	int mon = t->tm_mon + 1;
	int day = t->tm_mday;

	sprintf(s, "%04d-%02d-%02d\n",
		year, mon, day);
	printf("%s\n", s);

	if (year >= EXPIRED_YEAR && mon > EXPIRED_MONTH)
	{
		printf("This trial version was expired.[%d / %d]\n", EXPIRED_MONTH, EXPIRED_YEAR);
		Sleep(5000);
		return FALSE;
	}

	return TRUE;
}
int mainProg() {
#ifdef TRIAL_VERSION
	struct tm *t;
	time_t timer;
	timer = time(NULL);
	t = localtime(&timer);
	int available = timeToString(t);
	if (!available)
		return FALSE;
#endif

	return FALSE;// initMainLayout();
}

#else

void idle(GLWin *glwin) {
	if (glwin) glwin->redraw();
}
int mainProg() {
	Configuration();

	GLWin* glwin = new GLWin(0, 0, 1024, 512);

	glwin->end();
	glwin->resizable(glwin);
	glwin->show();
#ifdef WIN32
	glewInit();
#endif

	return(Fl::run());
}
#endif
int main(int argc, const char* argv[]) {
	return mainProg();
}
/*int CALLBACK WinMain(HINSTANCE inst, HINSTANCE prevInst, LPSTR cmd, int nCmdShow) {
	mainProg();
}*/


