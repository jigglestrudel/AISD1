#include "CSSEngine.h"

bool isNumber(const char* str)
{
	int i = 0;
	while (str[i] != '\0')
	{
		if (!isdigit(str[i]))
			return false;
		i++;
	}
	return true;
}


char* addCharToString(char* str, char c, int* size)
{
	if (str == nullptr)
	{
		str = (char*)malloc(sizeof(char));
		*size = 1;
		str[0] = '\0';
	}
	(*size)++;
	str = (char*)realloc(str, (*size) * sizeof(char));
	str[(*size) - 2] = c;
	str[(*size) - 1] = '\0';
	return str;
}

char* popWhitesOffEnd(char* str, int* size)
{
	while (*size > 0 && isspace(str[*size - 2]))
	{
		(*size)--;
	}
	str = (char*)realloc(str, (*size) * sizeof(char));
	str[*size - 1] = '\0';
	return str;
}


Engine::Engine()
{
	mainList = List<CSSNodeContent>();
	mainList.allocateNewNode(false);
	lenCounter = 0;
	inputString = nullptr;
	sectionIndex = 0;
	newAttr = Attribute();
	exitCounter = 0;

	readingCSS = true;
	readingName = false;
	readingTag = false;
	readingVal = false;
	readName = false;
	inAttributes = false;
	reading1 = false;
	reading2 = false;
	reading3 = false;
	inOpenedSection = false;
}

Engine::~Engine()
{
}

void Engine::manageInput(char input)
{
	if (readingCSS)
	{
		manageCSSInput(input);
	}
	else
	{
		manageCommandInput(input);
	}
}

void Engine::manageCSSInput(char input)
{
	if (input == '?')
	{
		exitCounter++;
		if (exitCounter == 4)
		{
			exitCounter = 0;
			readingCSS = false;
			reading1 = false;
			reading2 = false;
			reading3 = false;
			inputString = nullptr;
			return;
		}
	}
	else
	{
		exitCounter = 0;
	}

	if (isspace(input))
	{
		/// TODO: ATTR VAL TAKES SPACES BEFORE (FIX)
		if (input != '\n' && (readingName || readingTag || readingVal))
		{
			inputString = addCharToString(inputString, input, &lenCounter);
		}
	}
	else if ((input == ',' || input == '{') && !readingVal) // ending TAG input
	{
		if (readingTag)
		{
			inputString = popWhitesOffEnd(inputString, &lenCounter);
			Tag newTag = { inputString };
			inputString = nullptr;

			appendToLastSection(newTag);

			readingTag = false;

		}
		if (input == '{')
		{
			inAttributes = true;
		}
	}
	else if (input == ':' && readingName)
	{
			inputString = popWhitesOffEnd(inputString, &lenCounter);
			newAttr.name = inputString;
			inputString = nullptr;

			readingName = false;
			readName = true;
	}
	else if ((input == ';' || input == '}'))
	{
		if (readingVal)
		{
			inputString = popWhitesOffEnd(inputString, &lenCounter);
			newAttr.value = inputString;
			inputString = nullptr;

			appendToLastSection(newAttr);

			readingVal = false;
			readName = false;
		}
		
		if (input == '}')
		{
			inAttributes = false;
			inOpenedSection = false;
		}
	}
	else
	{
		if (!inOpenedSection && input != '?')
		{
			openNextSection();
			inOpenedSection = true;
			inputString = nullptr;
		}
		if (!inAttributes && !readingTag)
		{
			readingTag = true;
			inputString = nullptr;
		}
		if (inAttributes && !readingName && !readName)
		{
			readingName = true;
			inputString = nullptr;
		}
		if (inAttributes && !readingVal && readingName)
		{
			readingVal = true;
			inputString = nullptr;
		}

		inputString = addCharToString(inputString, input, &lenCounter);
	}
}

void Engine::manageCommandInput(char input)
{
	if (input == '*')
	{
		exitCounter++;
		if (exitCounter == 4)
		{
			exitCounter = 0;
			readingCSS = true;
			readingName = false;
			readingTag = false;
			readingVal = false;
			readName = false;
			inAttributes = false;
			inputString = nullptr;
			inOpenedSection = false;
			return;
		}
	}
	else
	{
		exitCounter = 0;
	}

	if (isspace(input))
	{
		if ((input != '\n' && input != EOF) && (reading1 || reading2 || reading3))
		{
			inputString = addCharToString(inputString, input, &lenCounter);
		}
		else if ((input == '\n' || input == EOF) && reading3)
		{
			inputString = popWhitesOffEnd(inputString, &lenCounter);
			command.part3 = inputString;
			inputString = nullptr;
			executeCommand();
		}

	}
	else
	{
		if (!(reading1 || reading2 || reading3))
		{
			reading1 = true;
		}

		if (input == '?' && reading1)
		{
			command.part1 = "?";
			executeCommand();
		}
		else if (input == ',')
		{
			if (reading1)
			{
				inputString = popWhitesOffEnd(inputString, &lenCounter);
				command.part1 = inputString;
				inputString = nullptr;

				reading1 = false;
				reading2 = true;
			}
			else if (reading2)
			{
				inputString = popWhitesOffEnd(inputString, &lenCounter);
				command.part2 = inputString;
				inputString = nullptr;
				reading2 = false;
				reading3 = true;
			}
		}
		else
		{
			inputString = addCharToString(inputString, input, &lenCounter);
		}

	}

}

void Engine::executeCommand()
{
	
	int result = 0; // -1 > string result, 0 > no result, 1 > int result
	int numResult = -1;
	const char* strResult = nullptr;

	if (command.part1[0] == '?')
	{
		result = 2;
		numResult = countSections();
	}
	else
	{
		switch (command.part2[0])
		{
		case 'S':
			if (command.part3[0] == '?')
			{
				result = 1;
				if (isNumber(command.part1))
				{
					numResult = countTagsInSection(atoi(command.part1)-1);
				}
				else
				{
					numResult = countTagInAll({ command.part1 });
				}
			}
			else
			{
				result = -1;
				strResult = getTagInSection(atoi(command.part1)-1, atoi(command.part3)-1);
			}
			break;

		case 'A':
			if (command.part3[0] == '?')
			{
				result = 1;
				if (isNumber(command.part1))
				{
					numResult = countAttrInSection(atoi(command.part1)-1);
				}
				else
				{
					numResult = countAttrInAll({ command.part1, "" });
				}
			}
			else
			{
				result = -1;
				strResult = getAttrValInSection(atoi(command.part1)-1, { command.part3, "" });
			}
			break;

		case 'E':
			result = -1;
			strResult = getAttrValForTag({ command.part1 }, { command.part3, "" });
			break;

		case 'D':
			
			if (command.part3[0] == '*')
			{
				if (removeSection(atoi(command.part1) - 1))
				{
					result = -1;
					strResult = "deleted";
				}
				else
				{
					result = 0;
				}
			}
			else
			{
				if (removeAttrfromSection(atoi(command.part1)-1, { command.part3, "" }))
				{
					result = -1;
					strResult = "deleted";
				}
				else
				{
					result = 0;
				}
			}
			break;

		default:
			break;
		}
	}

	if (result == 1)
	{
		if (numResult != -1)
			printf("%s,%s,%s == %d\n", command.part1, command.part2, command.part3, numResult);
	}
	else if (result == -1)
	{
		if (strResult != nullptr)
			printf("%s,%s,%s == %s\n", command.part1, command.part2, command.part3, strResult);
	}
	else if (result == 2)
	{
		printf("? == %d\n", numResult);

	}
	reading1 = false;
	reading2 = false;
	reading3 = false;
	command = Command();
}

void Engine::print()
{
	printf("\nEngine has allocated %d spaces but filled %d\n", mainList.size * TABLE_SIZE, countSections());
	for (int i = 0; i < countSections(); i++)
	{
		printSection(i);
	}
}

void Engine::printSection(int index)
{
	printf("SECTION %d :\n", index);
	if (findSection(index) != nullptr)
		findSection(index)->print();
}

CSSstruct* Engine::findSection(int index)
{
	int sectionsCount = countSections();

	if (index >= sectionsCount)
	{
		return nullptr;
	}

	int sizeSum = 0, allSpaces = 0;
	Node<CSSNodeContent>* tmp = mainList.head;
	do
	{
		sizeSum += tmp->content.size;
		allSpaces += TABLE_SIZE;
		if (index < sizeSum) 
			return &(tmp->content.array[index - (sizeSum - tmp->content.size)]);

		tmp = tmp->next;
	} while (tmp != nullptr);

	return nullptr;
}

Node<CSSNodeContent>* Engine::findSelectionNode(int index)
{
	int sectionsCount = countSections();

	if (index >= sectionsCount) return nullptr;

	int sizeSum = 0;
	Node<CSSNodeContent>* tmp = mainList.head;
	do
	{
		sizeSum += tmp->content.size;
		if (index < sizeSum)
			return tmp;

		tmp = tmp->next;
	} while (tmp != nullptr);

	return nullptr;
}

void Engine::appendToLastSection(Tag tag)
{
	int dupCheck = mainList.tail->content.array[mainList.tail->content.size - 1].tagList.find(tag);
	if (dupCheck != -1)
		mainList.tail->content.array[mainList.tail->content.size - 1].tagList.removeAtIndex(dupCheck);
	mainList.tail->content.array[mainList.tail->content.size - 1].tagList.append(tag);
}

void Engine::appendToLastSection(Attribute attr)
{
	int dupCheck = mainList.tail->content.array[mainList.tail->content.size - 1].attrList.find(attr);
	if (dupCheck != -1)
		mainList.tail->content.array[mainList.tail->content.size - 1].attrList.removeAtIndex(dupCheck);
	mainList.tail->content.array[mainList.tail->content.size - 1].attrList.append(attr);
}

void Engine::openNextSection()
{
	if (mainList.tail->content.size == TABLE_SIZE)
	{
		mainList.allocateNewNode(false);
		mainList.tail->content.array[0] = CSSstruct();
		mainList.tail->content.size += 1;
	}
	else
	{
		mainList.tail->content.array[mainList.tail->content.size] = CSSstruct();
		mainList.tail->content.size += 1;
	}
}

int Engine::countSections()
{
	int sizeSum = mainList.head->content.size;
	Node<CSSNodeContent>* tmp = mainList.head;
	while (tmp != mainList.tail)
	{
		if (tmp->next != nullptr) tmp = tmp->next;
		sizeSum += tmp->content.size;
	} 
	return sizeSum;
}

const char* Engine::getAttrValInSection(int SectionIndex, Attribute attr)
{
	CSSstruct* tmp = findSection(SectionIndex);
	if (tmp == nullptr) return nullptr;

	int AttrIndex = tmp->attrList.find(attr);
	if (AttrIndex == -1)return nullptr;
	
	return tmp->attrList[AttrIndex]->content.value;
}

int Engine::countAttrInAll(Attribute attr)
{
	int counter = 0;
	Node<CSSNodeContent>* sectionnode = mainList.head;
	Node<Attribute>* attrnode;
	while (sectionnode != nullptr)
	{
		for (int i = 0; i < sectionnode->content.size; i++)
		{
			attrnode = sectionnode->content.array[i].attrList.head;
			while (attrnode != nullptr)
			{
				if (attrnode->content == attr)
					counter++;

				attrnode = attrnode->next;
			}
		}

		sectionnode = sectionnode->next;
	}
	return counter;
}

int Engine::countTagInAll(Tag tag)
{
	int counter = 0;
	Node<CSSNodeContent>* sectionnode = mainList.head;
	Node<Tag>* tagnode;
	while (sectionnode != nullptr)
	{
		for (int i = 0; i < sectionnode->content.size; i++)
		{
			tagnode = sectionnode->content.array[i].tagList.head;
			while (tagnode != nullptr)
			{
				if (tagnode->content == tag)
					counter++;

				tagnode = tagnode->next;
			}
		}

		sectionnode = sectionnode->next;
	}
	return counter;
}

const char* Engine::getAttrValForTag(Tag tag, Attribute attrname)
{
	//TODO
	int tagIndex, attrIndex;
	CSSstruct* tmp;
	for (int i = countSections()-1; i >= 0; i--)
	{
		tmp = findSection(i);

		tagIndex = tmp->tagList.find(tag);

		if (tagIndex != -1 || tmp->tagList.size == 0)
		{
			attrIndex = tmp->attrList.find(attrname);
			if (attrIndex != -1)
			{
				return tmp->attrList[attrIndex]->content.value;
			}
			else
			{
				continue;
			}
		}
	}

	return nullptr;
}

bool Engine::removeSection(int index)
{
	Node<CSSNodeContent>* node = findSelectionNode(index);
	CSSstruct* sectionToDelete = findSection(index);
	if (sectionToDelete == nullptr) return false;
	*sectionToDelete = CSSstruct();

	int i = 0;
	while (&node->content.array[i] != sectionToDelete) i++;

	while (i < node->content.size-1)
	{
		node->content.array[i] = node->content.array[i + 1];
		i++;
	}
	node->content.size--;
	node->content.array[node->content.size] = CSSstruct();
	return true;
}

bool Engine::removeAttrfromSection(int index, Attribute attr)
{
	CSSstruct* tmp = findSection(index);
	if (tmp == nullptr) return false;
	if (tmp->attrList.find(attr) != -1)
	{
		tmp->attrList.removeAtIndex(tmp->attrList.find(attr));
		if (tmp->attrList.size == 0)
		{
			removeSection(index);
		}
		return true;
	}
	else return false;
}

int Engine::countTagsInSection(int index)
{
	CSSstruct* tmp = findSection(index);
	if (tmp != nullptr)
		return tmp->tagList.size;
	return -1;
}

int Engine::countAttrInSection(int index)
{
	CSSstruct* tmp = findSection(index);
	if (tmp != nullptr)
		return tmp->attrList.size;
	return -1;
}

const char* Engine::getTagInSection(int SectionIndex, int TagIndex)
{
	CSSstruct* tmp = findSection(SectionIndex);
	if (tmp == nullptr || tmp->tagList.size <= TagIndex)
		return nullptr;
	return findSection(SectionIndex)->tagList[TagIndex]->content.name;
}



CSSNodeContent::CSSNodeContent()
{
	array = new CSSstruct[TABLE_SIZE];
	size = 0;
}

CSSstruct::CSSstruct()
{
	tagList = List<Tag>();
	attrList = List<Attribute>();
}

void CSSstruct::print()
{
	printf("Tag List :\n");
	tagList.print();
	printf("Attr List :\n");
	attrList.print();
}
