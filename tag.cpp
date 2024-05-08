#include "Tag.h"

void Tag::print()
{
	printf("(%s)", name);
}

bool operator==(Tag left, Tag right)
{
	return strcmp(left.name, right.name) == 0;
}
