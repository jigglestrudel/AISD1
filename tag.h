#pragma once
#include <stdio.h>
#include <string.h>


typedef struct Tag {
	const char* name;

	void print();

};

bool operator==(Tag left, Tag right);
