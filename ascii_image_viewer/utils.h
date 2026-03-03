// Source - https://stackoverflow.com/a
// Posted by MD XF, modified by community. See post 'Timeline' for change history
// Retrieved 2025-12-21, License - CC BY-SA 3.0

#ifdef _WIN32
#include <conio.h>
#else
#include <stdio.h>
#define clrscr() printf("\e[1;1H\e[2J")
#endif
