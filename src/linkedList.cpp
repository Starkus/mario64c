#include "linkedList.hpp"
/*
template <typename T>
LinkedList::LinkedList() {
	root = NULL;
}


template <typename T>
_node<T>* LinkedList::getLastNode() {

	if (last == NULL)
		return NULL;

	_node<T>* last = root;
	while (true) {
		if (last->next == NULL)
			return last;
		last = last->next;
	}
}

template <typename T>
_node<T>* LinkedList::getNodeByIndex(int i) {
	_node *current = root;
	for (int j = 1; j < i; ++j) {
		current = current->next;
		if (!current)
			break;
	}
	return current;
}


template <typename T>
int LinkedList::size() {
	int s = 0;
	_node *current = root;
	while (true) {
		if (current == NULL)
			return s;
		current = current->next;
		s++;
	}
	return s;
}

template <typename T>
void LinkedList::add(T value) {
	_node *newnode = new _node{ value, NULL };

	getLastNode()->next = newnode;
}

template <typename T>
T LinkedList::get(int i) {
	_node *node = getNodeByIndex(i);
	return node->value;
}

template <typename T>
void LinkedList::remove(int i) {
	if (i == 0) {
		_node *second = root->next;
		delete root;
		root = second;
	}
	else {
		_node *prev = getNodeByIndex(i - 1);
		_node *next = prev->next->next;
		delete prev->next;
		prev->next = next;
	}
}
*/