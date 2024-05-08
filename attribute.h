#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Attribute
{
	const char* name;
	const char* value;

	void print();

}Attribute_t;

bool operator==(Attribute left, Attribute right);

