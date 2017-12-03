#ifndef LinkedList_H
#define LinkedList_H

#include <iostream>     //For NULL

using namespace std;
        //List class
        
       /** CONSTRUCTION: with no initializer
        Access is via ListItr class
        
        ******************PUBLIC OPERATIONS*********************
        boolean isEmpty( )     --> Return true if empty; else false
        ListItr zeroth( )      --> Return position to prior to first
        ListItr first( )       --> Return first position
        void insert( x, p )    --> Insert x after current iterator position p
        void remove( x )       --> Remove x
        ListItr find( x )      --> Return position that views x
        ListItr findPrevious( x )
                               --> Return position prior to x
        ******************ERRORS********************************
        No special errors
        */

template <class Object>
class List;
template <class Object>
class ListIterator;

template <class Object>
class Node{
  public:
     Node( const Object & theElement = Object( ), Node * n = NULL )
              : element( theElement ), next( n ) { }

      Object element;
      Node *next;
};

template <class Object>
class List{
    public:
        List();
        bool isEmpty() const;
        void insert( Object &object,  ListIterator<Object> &p);
        void insertToEnd( Object &x);
        void insertToBeginning( Object &x);
        Object remove( Object &object);
        Object removeFirst();
        ListIterator<Object> zeroth( ) const;
        ListIterator<Object> first( ) const;
        ListIterator<Object> findPrevious(  Object & x ) const;

    private:
        Node<Object> *header;
};


template <class Object>
class ListIterator{
          public:
            ListIterator() : current( NULL ) { }
            bool isPastEnd( ) const
              { return current == NULL; }
            void advance( )
              { if( !isPastEnd( ) ) current = current->next; }
            Object &retrieve( ) const
              { return current->element; }

          private:
            Node<Object> *current;    // Current position

            ListIterator( Node<Object> *theNode )
              : current( theNode ) { }

            friend class List<Object>; // Grant access to constructor
};


  #include "LinkedList.cpp"
        #endif