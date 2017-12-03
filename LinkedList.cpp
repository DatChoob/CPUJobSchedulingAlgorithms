#ifndef LINKEDLIST_CPP
#define LINKEDLIST_CPP

#include "LinkedList.h"

template <class Object>
List<Object>::List()
{
    header = new Node<Object>;
}

template <class Object>
bool List<Object>::isEmpty() const
{
    return header->next == NULL;
}

template <class Object>
ListIterator<Object> List<Object>::zeroth() const
{
    return ListIterator<Object>(header);
}

//returns the first item in the list
template <class Object>
ListIterator<Object> List<Object>::first() const
{
    return ListIterator<Object>(header->next);
}

template <class Object>
void List<Object>::insert(Object &x, ListIterator<Object> &p)
{

    if (p.current != NULL)
        p.current->next = new Node<Object>(x, p.current->next);
}

template <class Object>
void List<Object>::insertToEnd(Object &x)
{
    ListIterator<Object> iterator = zeroth();
    while (iterator.current->next != NULL)
    {
        iterator.advance();
    }
    iterator.current->next = new Node<Object>(x, iterator.current->next);
}

template <class Object>
void List<Object>::insertToBeginning(Object &x)
{
    ListIterator<Object> head = zeroth();
    head.current->next = new Node<Object>(x, head.current->next);
}

template <class Object>
ListIterator<Object> List<Object>::findPrevious(Object &x) const
{
    Node<Object> *itr = header;

    while (itr->next != NULL && itr->next->element != x)
        itr = itr->next;

    return ListIterator<Object>(itr);
}

template <class Object>
Object List<Object>::remove(Object &x)
{
    ListIterator<Object> p = findPrevious(x);

    if (p.current->next != NULL)
    {
        Node<Object> *oldNode = p.current->next;
        p.current->next = p.current->next->next; // Bypass deleted node
        return oldNode->element;
    }
}
template <class Object>
Object List<Object>::removeFirst()
{
    Object currElement = first().current->element;
    return remove(currElement);
}
#endif