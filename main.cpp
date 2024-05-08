#include <stdio.h>
#include "CSSEngine.h"

int main()
{
	Engine engine;

	char input;

	do
	{
		input = getchar();
		engine.manageInput(input);

	} while (input != EOF);
	

	return 0;
}
