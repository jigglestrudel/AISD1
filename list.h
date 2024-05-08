#pragma once
#include <stdio.h>
#include <stdlib.h>


template <typename T>
struct Node {
	typedef Node<T> Node_t;

	Node<T>* prev;
	T content;
	Node<T>* next;

};

template <typename T>
struct List {
	typedef List<T> List_t;

	Node<T>* head;
	Node<T>* tail;
	int size;

	List();
	~List();

	void print();

	void allocateNewNode(bool prepend);

	void append( T data);
	void prepend( T data);
	T getIndex( int index);

	Node<T>* operator[](int index);

	void addAtIndex(int index, T data);
	void removeAtIndex(int index);
	
	int find(T data);
};
