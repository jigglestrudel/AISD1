#include "Attribute.h"

void Attribute::print()
{
	printf("(%s|%s)", name, value);
}

bool operator==(Attribute left, Attribute right)
{

	return strcmp(left.name, right.name) == 0;
}
