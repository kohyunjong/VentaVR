#include "Layout/Layout.h"

int main(int argc, const char* argv[]) {
	printf("%d,%d / %d,%d\n", screenw, screenh, window_w, window_h);
	return initMainLayout();
}