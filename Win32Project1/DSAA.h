#ifndef DSAA_H
#define DSAA_H
#endif
#include<iostream>
#include<assert.h>
#include<stdlib.h>
#include<algorithm>
using namespace std;

template <typename T> class List
{
private:
	void operator = (const List&) {}			//??
	List(const List&) {}
public:
	List() {}
	virtual ~List() {}
	virtual void clear() = 0;
	virtual void insert(T&) = 0;
	virtual void append(T&) = 0;
	virtual void pop(int) = 0;
	virtual void moveToStart() = 0;
	virtual void moveToEnd()= 0;
	virtual void prev() = 0;
	virtual void next() = 0;
	virtual int len() const = 0;
	virtual int currPos() const = 0;
	virtual void moveToPos(int) = 0;
	virtual T& getValue(int) =0;	//??
};

template <typename T> class ArrayList : public List<T>
{
private:
	int MaxSize;
	int ListSize;
	int curr;
	T* ListArray;
public:
	ArrayList(int size)
	{
		MaxSize = size;
		ListSize = curr = 0;
		ListArray = new T[MaxSize];
	}
	~ArrayList()
	{
		delete[] ListArray;
	}
	void clear()
	{
		delete[] ListArray;
		ListSize = curr = 0;
		ListArray = new T[MaxSize];
	}
	void insert(const T& item)
	{
		//assert(ListSize < MaxSize, "List capacity exceeded!");
		for (int i = ListSize; i > curr; i--)
			ListArray[i] = ListArray[i - 1];
		ListArray[curr] = item;
		ListSize++;
	}
	void append(const T& item)
	{
		//assert(ListSize < MaxSize, "List capacity exceeded!");
		ListArray[ListSize++] = item;
		//ListSize++;
	}
	T pop()
	{
		T item = ListArray[curr];
		for (int i = curr; i < ListSize - 1; i++)
			ListArray[i] = ListArray[i + 1];
		ListSize--;
		return item;
	}
	void moveToStart()
	{
		curr = 0;
	}
	void moveToEnd()
	{
		curr = ListSize;		//??
	}
	void prev()
	{
		if (curr != 0) curr--;
	}
	void next()
	{
		if (curr < ListSize) curr++;	//??
	}
	int len() const
	{
		return  ListSize;
	}
	int currPos() const
	{
		return curr;
	}
	void moveToPos(int pos)
	{
		//assert(pos >= 0 && pos < ListSize, "Pos out of range");
		curr = pos;
	}
	const T& getValue() const
	{
		//assert(curr >= 0 && curr < ListSize - 1, "Pos out of range");
		return ListArray[curr];
	}
	void show()
	{
		for (int pos = 0; pos < ListSize; pos++)
			cout << ListArray[pos] << endl;
	}
};

template <typename T> class SingleLinkNode
{
public:
	T element;
	SingleLinkNode* next;
	SingleLinkNode(T& tempele, SingleLinkNode* tempnext = NULL)
	{
		element = tempele;
		next = tempnext;
	}
	SingleLinkNode(SingleLinkNode* tempnext = NULL)
	{
		next = tempnext;
	}
};

template <typename T> class SingleLinkList :public List<T>
{
private:
	SingleLinkNode<T>* head;
	SingleLinkNode<T>* curr;
	SingleLinkNode<T>* tail;
	int size;
	void init()
	{
		curr = head = tail = new SingleLinkNode<T>;
		size = 0;
	}
	void removeAll()
	{
		while (head != NULL)
		{
			curr = head;
			head = head->next;
			delete curr;
		}
	}
public:
	SingleLinkList()
	{
		init();
	}
	~SingleLinkList()
	{
		removeAll();
	}
	//void print() const;//??
	void clear()
	{
		removeAll();
		init();
	}
	void insert(T& item)
	{
		curr->next = new SingleLinkNode<T>(item, curr->next);
		if (tail == curr)
			tail = curr->next;
		size++;
	}
	void append(T& item)
	{
		tail->next = new SingleLinkNode<T>(item, NULL);
		tail = tail->next;	//Also tail = tail->next = new Link<T>(it, NULL);
		size++;
	}
	void pop(int pos)
	{	
		curr = head;
		for (int i = 0; i < pos; i++)
		{
			curr = curr->next;
		}
		SingleLinkNode<T>* temp = curr->next;
		if (curr->next == tail)
			tail = curr;
		curr->next = curr->next->next;
		delete temp;
		size--;
	}
	void moveToStart()
	{
		curr = head;
	}
	void moveToEnd()
	{
		curr = tail;
	}
	void prev()
	{

		if (curr == head) return;
		SingleLinkNode<T>* temp = head;
		while (temp != curr)
			temp = temp->next;
		curr = temp;
	}
	void next()
	{
		if (curr != tail) curr = curr->next;
	}
	int len() const
	{
		return size;
	}
	int currPos() const
	{
		int i;
		SingleLinkNode<T>* temp = head;
		for (i = 0; curr != temp; i++)
		{
			temp = temp->next;
		}
		return i;
	}
	void moveToPos(int pos)
	{
		curr = head;
		for (int i = 0; i < pos; i++)
		{
			curr = curr->next;
		}
	}
	T& getValue(int pos)
	{
		curr = head;
		for (int i = 0; i < pos; i++)
		{
			curr = curr->next;
		}
		return curr->next->element;//!!!
	}
	void show()
	{
		if (size == 0) return;
		
		for (moveToPos(1);; next())
		{
			cout << curr->element << endl;
			if (curr->next == NULL) break;
		}
	}
};


template <typename T> class Stack
{
private:
	void operator=(const Stack&) {}
	Stack(const Stack&) {}
public:
	Stack() {}
	~Stack() {}
	virtual void clear() = 0;
	virtual void push(const T&) = 0;
	virtual T pop() = 0;
	virtual const T& topValue() const = 0;
	virtual int len() const = 0;
};

template <typename T> class ArrayStack: public Stack<T>
{
private:
	int MaxSize;
	int top;
	T* Array;
public:
	ArrayStack(int size)
	{
		MaxSize = size;
		top = 0;
		Array = new T[MaxSize];
	}
	~ArrayStack()
	{
		delete[] Array;
	}
	void clear()
	{
		top = 0;
	}
	void push(const T& item)
	{
		if (top != MaxSize) Array[top++] = item;
	}
	T pop()
	{
		return Array[--top];
	}
	const T& topValue() const
	{
		return Array[top - 1];
	}
	int len() const
	{
		return top;
	}
	void show() const
	{
		for (int i = 0; i < top; i++)
		{
			cout << Array[i] << endl;
		}
	}
};

template <typename T> class LinkStack : public Stack<T>
{
private:
	int size;
	SingleLinkNode<T>* top;
public:
	LinkStack()
	{
		top = NULL;
		size = 0;
	}
	~LinkStack()
	{
		clear();
	}
	void clear()
	{
		while (top != NULL)
		{
			SingleLinkNode<T> *temp = top;
			top = top->next;
			delete temp;
		}
		size = 0;
	}
	void push(const T& item)
	{
		top = new SingleLinkNode<T>(item, top);
		size++;
	}
	T pop()
	{
		T item = top->element;
		SingleLinkNode<T> *temp = top;
		top = top->next;
		delete temp;
		size--;
		return item;
	}
	const T& topValue() const
	{
		return top->element;
	}
	int len() const
	{
		return size;
	}
	void show() const
	{
		SingleLinkNode<T> *temp = top;
		while (temp != NULL)
		{
			cout << temp->element << endl;
			temp = temp->next;
		}
	}
};

template <typename T> class BinNode
{
public:
	virtual T& element() = 0;
	virtual void setElement(const T&) = 0;
	virtual BinNode* left() const = 0;
	virtual BinNode* right() const = 0;
	virtual void setLeft(BinNode*) = 0;
	virtual void setRight(BinNode*) = 0;
	virtual bool isLeaf() = 0;
};

template <typename T> class BSTNode
{
private:
	T item;
	BSTNode* leftchild;
	BSTNode* rightchild;
public:
	BSTNode() 
	{
		leftchild = rightchild = NULL;
	}
	BSTNode(T it, BSTNode* l = NULL, BSTNode* r = NULL)
	{
		item = it;
		leftchild = l;
		rightchild = r;
	}
	~BSTNode() {}
	T& element()
	{
		return item;
	}
	void setElement(const T& it)
	{
		item = it;
	}
	BSTNode* left() const
	{
		return leftchild;
	}
	BSTNode* right() const
	{
		return rightchild;
	}
	void setLeft(BSTNode<T>* b)
	{
		leftchild = b;
	}
	void setRight(BSTNode<T>* b)
	{
		rightchild = b;
	}
	bool isLeaf()
	{
		return (leftchild == NULL) && (rightchild == NULL);
	}
	bool hasGrandSon()
	{
		if (isLeaf()) return false;
		if ((leftchild != NULL && !leftchild->isLeaf()) || (rightchild != NULL && !rightchild->isLeaf())) return true;
		return false;
	}
	/*void preorder(BSTNode<T> *root)
	{
		if (!root->hasGrandSon()) return;
		if (root->left()->left() != NULL && root->left()->left()->item*5== root->item) 
			cout << root->left()->left()->item << endl;
		if (root->left()->right() != NULL && root->left()->right()->item*5 == root->item) 
			cout << root->left()->right()->item << endl;
		if (root->right()->right() != NULL && root->right()->right()->item*5 == root->item)
			cout << root->right()->right()->item << endl;
		if (root->right()->left() != NULL && root->right()->left()->item * 5 == root->item)
			cout << root->right()->left()->item << endl;
		preorder(root->left());
		preorder(root->right());
	}*/
	T temparray[10];
	int i = 0;
	void preorder(BSTNode<T> *root)
	{
		if (root == NULL) return;
		temparray[i++] = root->item;
		preorder(root->left());
		preorder(root->right());
	}
	void Bsort()
	{
		preorder(this);
		sort(temparray,temparray+i);
		for (int j = 0; j < i; j++)
			cout << temparray[j] << endl;
	}
};

