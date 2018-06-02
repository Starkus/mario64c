#pragma once

template <typename T>
struct _node {
	T value;
	_node<T> *next;
};

template <typename T>
class LinkedList {
	_node<T>* root;

	_node<T>* getLastNode() {

		_node<T>* last = root;
		if (!root)
			return nullptr;

		while (true) {
			if (!last->next)
				return last;
			last = last->next;
		}
	}

	_node<T>* getNodeByIndex(int i) {
		_node<T> *current = root;
		for (int j = 0; j < i; ++j) {
			current = current->next;
			if (!current)
				break;
		}
		return current;
	}

public:
	LinkedList() {
		root = nullptr;
	}
	
	int size() {
		int s = 0;
		_node<T> *current = root;
		while (true) {
			if (current == nullptr)
				return s;
			current = current->next;
			s++;
		}
		return s;
	}

	void add(T value) {
		_node<T> *newnode = new _node<T>;
		newnode->value = value;
		newnode->next = nullptr;

		if (!root)
			root = newnode;
		else
			getLastNode()->next = newnode;
	}

	T get(int index) {
		if (index < 0) {
			index = size() + index;
		}
		_node<T> *node = getNodeByIndex(index);
		return node->value;
	}
	void remove(int index) {
		if (index == 0) {
			_node<T> *second = root->next;
			delete root;
			root = second;
		}
		else {
			_node<T> *prev = getNodeByIndex(index - 1);
			_node<T> *next = prev->next->next;
			delete prev->next;
			prev->next = next;
		}
	}

};
