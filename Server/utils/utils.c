#include <stdio.h>
#include <stdlib.h>

void clearBuffer(void) {
	int c;
	while ((c = getchar()) != '\n' && c != EOF) { }
}