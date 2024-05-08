#pragma once
#include <ctype.h>
#include "List.h"
#include "List.cpp"
#include "Attribute.h"
#include "Tag.h"

#define TABLE_SIZE 17

typedef struct Command {
	const char* part1;
	const char* part2;
	const char* part3;

	Command() : part1(nullptr), part2(nullptr), part3(nullptr) {};
};


typedef struct CSSstruct {
	List<Tag> tagList;
	List<Attribute> attrList;

	CSSstruct();

	void print();
};

typedef struct CSSNodeContent {
	CSSstruct* array;
	int size;

	CSSNodeContent();
};

typedef struct Engine {
	List<CSSNodeContent> mainList;

	bool readingCSS, readingTag, inAttributes, readingName, readName, readingVal, inOpenedSection;
	int lenCounter, sectionIndex, exitCounter;
	char* inputString;
	Attribute newAttr;
	Command command;
	bool reading1, reading2, reading3;

	Engine();
	~Engine();


	void manageInput(char input);
	void manageCSSInput(char input);
	void manageCommandInput(char input);

	void executeCommand();

	void print();
	void printSection(int index);
	
	CSSstruct* findSection(int index);

	Node<CSSNodeContent>* findSelectionNode(int index);
		
	void appendToSection(int index, Tag tag);
	void appendToSection(int index, Attribute attr);

	void appendToLastSection(Tag tag);
	void appendToLastSection(Attribute attr);

	void openNextSection();

	int countSections();

	// i,S,? –	wypisz liczbę selektorów dla sekcji nr i (numery zaczynają się od 1), jeśli nie ma takiego bloku pomiń;
	int countTagsInSection(int index);								
	
	// z, S, ? – wypisz łączną(dla wszystkich bloków) liczbę wystąpień selektora z.Możliwe jest 0;
	int countTagInAll(Tag tag);										
	
	// i, S, j – wypisz j - ty selector dla i - tego bloku(numery sekcji oraz atrybutów zaczynają się od 1) jeśli nie ma sekcji lub selektora pomiń;
	const char* getTagInSection(int SectionIndex, int TagIndex);			
	
	// i, A, ? - wypisz liczbę atrybutów dla sekcji nr i, jeśli nie ma takiego bloku lub sekcji pomiń;
	int countAttrInSection(int index);								
	
	// n, A, ? – wypisz łączną(dla wszystkich bloków) liczbę wystąpień atrybutu nazwie n. (W ramach pojedynczego bloku duplikaty powinny zostać usunięte na etapie wczytywania).Możliwe jest 0;
	int countAttrInAll(Attribute attr);								
	
	// i, A, n – wypisz dla i - tej sekcji wartość atrybutu o nazwie n, jeśli nie ma takiego pomiń;
	const char* getAttrValInSection(int SectionIndex, Attribute attr);	
	
	// z, E, n – wypisz wartość atrybutu o nazwie n dla selektora z, w przypadku wielu wystąpień selektora z bierzemy ostatnie.W przypadku braku pomiń;
	const char* getAttrValForTag(Tag tag, Attribute attrname);
	
	// i, D, * - usuń całą sekcję nr i(tj.separatory + atrybuty), po poprawnym wykonaniu wypisz deleted;
	bool removeSection(int index);							
	
	// i, D, n – usuń z i - tej sekcji atrybut o nazwie n, jeśli w wyniku operacji pozostaje pusta sekcja powinna zostać również usunięta(wraz z ew.selektorami), po poprawnym wykonaniu wypisz deleted.
	bool removeAttrfromSection(int index, Attribute attrname);	

};