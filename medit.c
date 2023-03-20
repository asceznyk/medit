#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "defs.h"
#include "mode.h"
#include "editor.h"

int main() {
	enableRawMode();
	initEditor();

	while(1) {
		editorRefreshScreen();
		editorProcessKeypress();
	}
	
	return 0;
}

