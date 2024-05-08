#include "List.h"

template <typename T>
List<T>::List()
{
	head = nullptr;
	tail = nullptr;
	size = 0;
}

template <typename T>
List<T>::~List()
{
}

template <typename T>
void List<T>::append(T data)
{
	allocateNewNode(false);
	tail->content = data;

}

template<typename T>
void List<T>::print()
{
	Node<T>* tmp = head;

	while (tmp != nullptr)
	{
		printf("%p-> prev: %p | next: %p | ", tmp, tmp->prev, tmp->next);
		tmp->content.print();
		printf("\n");
		tmp = tmp->next;
	}
}

template <typename T>
void List<T>::prepend(T data)
{
	allocateNewNode(true);
	head->content = data;
}

template <typename T>
T List<T>::getIndex(int index)
{
	if (index > size - 1 || index < 0)
	{
		printf("Index %d out of bounds at %p\n", index, this);
		return T();
	}
	Node<T>* tmp = head;

	for (int i = 0; i < index; i++)
	{
		tmp = tmp->next;
	}

	return tmp->content;
}

template<typename T>
void List<T>::addAtIndex(int index, T data)
{
	if (index == size)
	{
		this->append(data);
		return;
	}
	if (index == 0)
	{
		this->prepend(data);
		return;
	}

	Node<T>* tmp = head;
	for (int i = 0; i < index; i++)
	{
		tmp = tmp->next;
	}

	Node<T>* newNode = new Node<T>;
	newNode->next = tmp->next;
	newNode->prev = tmp;
	newNode->content = data;

	tmp->next = newNode;
	newNode->next->prev = newNode;

}

template<typename T>
void List<T>::allocateNewNode(bool prepend)
{
	Node<T>* tmp = new Node<T>;
	*tmp = Node<T>();
	if (tail == nullptr)
	{
		tmp->next = nullptr;
		tmp->prev = nullptr;
		tail = tmp;
		head = tmp;
	}
	else if (!prepend)
	{
		tmp->prev = tail;
		tmp->next = nullptr;
		tail->next = tmp;
		tail = tmp;
	}
	else
	{
		tmp->next = this->head;
		tmp->prev = nullptr;
		head->prev = tmp;
		head = tmp;
	}
	size++;
}

template<typename T>
Node<T>* List<T>::operator[](int index)
{
	if (index >= size || index < -size)
	{
		printf("Index %d out of bounds at %p\n", index, this);
		return nullptr;
	}

	if (index == 0 || index == -size)
		return head;
	if (index == size-1 || index == -1)
		return tail;

	Node<T>* tmp = head;

	if (index > 0)
	{
		for (int i = 0; i < index; i++)
		{
			tmp = tmp->next;
		}
	}
	else
	{
		tmp = tail;
		for (int i = 0; i < -index-1; i++)
		{
			tmp = tmp->prev;
		}
	}

	return tmp;
}

template<typename T>
void List<T>::removeAtIndex(int index)
{
	Node<T>* tmp = (*this)[index];

	if (tmp == head && tmp == tail)
	{
		head = nullptr;
		tail = nullptr;
		size--;
		return;
	}
	else if (tmp == head)
	{
		head = tmp->next;
		head->prev = nullptr;
	}
	else if (tmp == tail)
	{
		tail = tmp->prev;
		tail->next = nullptr;
	}
	else
	{
		tmp->next->prev = tmp->prev;
		tmp->prev->next = tmp->next;
	}
	
	size--;

}

template<typename T>
int List<T>::find(T data)
{
	Node<T>* tmp = head;
	int index = 0;

	while (tmp != nullptr)
	{
		if (tmp->content == data)
		{
			return index;
		}
		index++;
		tmp = tmp->next;
	}

	return -1;
}
