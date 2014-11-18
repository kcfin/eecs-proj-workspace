#ifndef PAIRING_PRIORITY_QUEUE_H
#define PAIRING_PRIORITY_QUEUE_H

#include "eecs281_priority_queue.h"
using namespace std;

//A specialized version of the 'priority_queue' ADT implemented as a pairing priority_queue.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class pairing_priority_queue : public eecs281_priority_queue<TYPE, COMP_FUNCTOR> {
public:
  typedef unsigned size_type;

  //Description: Construct a priority_queue out of an iterator range with an optional
  //             comparison functor.
  //Runtime: O(n) where n is number of elements in range.
  template<typename InputIterator>
  pairing_priority_queue(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR());

  //Description: Construct an empty priority_queue with an optional comparison functor.
  //Runtime: O(1)
  pairing_priority_queue(COMP_FUNCTOR comp = COMP_FUNCTOR());
  
  //Description: Copy constructor.
  //Runtime: O(n)
  pairing_priority_queue(const pairing_priority_queue& other);
  
  //Description: Copy assignment operator.
  //Runtime: O(n)
  pairing_priority_queue& operator=(const pairing_priority_queue& rhs);
  
  //Description: Destructor
  //Runtime: O(n)
  ~pairing_priority_queue();

  //Description: Assumes that all elements inside the priority_queue are out of order and
  //             'rebuilds' the priority_queue by fixing the priority_queue invariant.
  //Runtime: O(n)
  virtual void fix();

  //Description: Add a new element to the priority_queue. This has been provided for you,
  //             in that you should implement push functionality in the
  //             add_node function.
  //Runtime: Amortized O(1)
  virtual void push(const TYPE& val) { add_node(val); }

  //Description: Remove the most extreme (defined by 'compare') element from
  //             the priority_queue.
  //Note: We will not run tests on your code that would require it to pop an
  //element when the priority_queue is empty. Though you are welcome to if you are
  //familiar with them, you do not need to use exceptions in this project.
  //Runtime: Amortized O(log(n))
  virtual void pop();

  //Description: Return the most extreme (defined by 'compare') element of
  //             the priority_queue.
  //Runtime: O(1)
  virtual const TYPE& top() const;

  //Description: Get the number of elements in the priority_queue.
  //Runtime: O(1)
    virtual size_type size() const { return heapSize; }

  //Description: Return true if the priority_queue is empty.
  //Runtime: O(1)
  virtual bool empty() const {
      return (root == nullptr);
  }

  class Node {
    //This node class will allow external users to update the priority of
    //elements that are already inside the priority_queue.
  public:
      Node(TYPE val) {
          elt = val;
          next = prev = child = nullptr;
      }
  public:
    //Description: Allows access to the element at that Node's position.
    //Runtime: O(1) - this has been provided for you.
    const TYPE& operator*() const { return elt; }

    //The following line allows you to access any private data members of this
    //Node class from within the pairing_priority_queue class. (ie: myNode.elt is a legal
    //statement in pairing_priority_queue's add_node() function).
    friend pairing_priority_queue;
  private:
    TYPE elt;
  private:
      Node *next;
      Node *prev;
      Node *child;
    //***Add any additional member functions or data you require here.
  };

  //Description: Updates the priority of an element already in the priority_queue by
  //             replacing the element refered to by the Node with new_value.
  //             Must maintain priority_queue invariants.
  //
  //PRECONDITION: The new priority, given by 'new_value' must be more extreme
  //              (as defined by comp) than the old priority.
  //
  //Runtime: As discussed in reading material.
  void updateElt(Node* node, const TYPE &new_value);

  //Description: Add a new element to the priority_queue. Returns a Node* corresponding
  //             to the newly added element.
  //Runtime: Amortized O(1)
  Node* add_node(const TYPE& val);

private:
    Node *root;
    size_type heapSize;
    
private:
    void meld(Node *node);
};

template<typename TYPE, typename COMP_FUNCTOR>
template<typename InputIterator>
pairing_priority_queue<TYPE, COMP_FUNCTOR>::pairing_priority_queue(
  InputIterator start,
  InputIterator end,
  COMP_FUNCTOR comp
) {
    this->compare = comp;
    while(start != end) {
        push(*start++);
    }
}

template<typename TYPE, typename COMP_FUNCTOR>
pairing_priority_queue<TYPE, COMP_FUNCTOR>::pairing_priority_queue(COMP_FUNCTOR comp) {
    this->compare = comp;
}

template<typename TYPE, typename COMP_FUNCTOR>
pairing_priority_queue<TYPE, COMP_FUNCTOR>::pairing_priority_queue(const pairing_priority_queue& other) {
    // copy constructor
}

template<typename TYPE, typename COMP_FUNCTOR>
pairing_priority_queue<TYPE, COMP_FUNCTOR>&
pairing_priority_queue<TYPE, COMP_FUNCTOR>::operator=(const pairing_priority_queue& rhs) {
  //Your code.
}

template<typename TYPE, typename COMP_FUNCTOR>
pairing_priority_queue<TYPE, COMP_FUNCTOR>::~pairing_priority_queue() {
  //Your code.
}

template<typename TYPE, typename COMP_FUNCTOR>
void pairing_priority_queue<TYPE, COMP_FUNCTOR>::fix() {
    vector<Node*> temp = vector<Node*>();
    
    if(root->child != nullptr) {
        Node *child = root->child;
        temp.push_back(child);
        while(child->next != nullptr) {
            temp.push_back(child->next);
            while(child->child != nullptr) {
                temp.push_back(child->child);
                child = child->child;
            }
            child = child->next;
        }
    }
    
    for(int i = 0; i < temp.size(); i++) {
        meld(temp[i]);
    }
}

template<typename TYPE, typename COMP_FUNCTOR>
void pairing_priority_queue<TYPE, COMP_FUNCTOR>::pop() {
    Node *child = root->child;
    child->prev = nullptr;
    child->child = root->child->next;
    child->next = nullptr;
    root = child;
    
    fix();
}

template<typename TYPE, typename COMP_FUNCTOR>
const TYPE& pairing_priority_queue<TYPE, COMP_FUNCTOR>::top() const {
    return root->elt;
}

template<typename TYPE, typename COMP_FUNCTOR>
typename pairing_priority_queue<TYPE, COMP_FUNCTOR>::Node*
pairing_priority_queue<TYPE, COMP_FUNCTOR>::add_node(const TYPE& val) {

    Node *newNode;
    newNode->elt = val;
    meld(newNode);
    return newNode;
}

template<typename TYPE, typename COMP_FUNCTOR>
void pairing_priority_queue<TYPE, COMP_FUNCTOR>::updateElt(Node* node, const TYPE &new_value) {
    node->elt = new_value;
    fix();
}

template<typename TYPE, typename COMP_FUNCTOR>
void pairing_priority_queue<TYPE, COMP_FUNCTOR >::meld(Node *node) {
    if(this->compare(root->elt, node->elt)) {
        if(node->child != nullptr) {
            root->next = node->child;
            node->child->prev = root;
        }
        root->prev = node;
        node->child = root;
        root = node;
    } else {
        if(root->child != nullptr) {
            node->next = root->child;
            root->child->prev = node;
        }
        node->prev = root;
        root->child = node;
    }
}

#endif //PAIRING_PRIORITY_QUEUE_H

