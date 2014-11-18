#ifndef EECS281_PRIORITY_QUEUE_H
#define EECS281_PRIORITY_QUEUE_H

#include <functional>
#include <vector>

//A simple interface that implements a generic priority_queue.
//Runtime specifications assume constant time comparison and copying.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class eecs281_priority_queue {
public:
  typedef unsigned size_type;

  virtual ~eecs281_priority_queue() {}

  //Description: Assumes that all elements inside the priority_queue are out of order and
  //             "fixes" the priority queue
  virtual void fix() = 0;

  //Description: Add a new element to the priority_queue.
  virtual void push(const TYPE& val) = 0;

  //Description: Remove the most extreme (defined by 'compare') element from
  //             the priority_queue.
  //Note: We will not run tests on your code that would require it to pop an
  //element when the priority_queue is empty. Though you are welcome to if you are
  //familiar with them, you do not need to use exceptions in this project.
  virtual void pop() = 0;

  //Description: Return the most extreme (defined by 'compare') element of
  //             the priority_queue.
  virtual const TYPE& top() const = 0;

  //Description: Get the number of elements in the priority_queue.
  virtual size_type size() const = 0;

  //Description: Return true if the priority_queue is empty.
  virtual bool empty() const = 0;

protected:
  //Note: These data members *must* be used in all of your priority_queue
  //      implementations.
  
  //You can use this in the derived classes with:
  //this->compare(Thing1, Thing2)
  //With the default compare function (std::less), this will
  //tell you if Thing1 is less than Thing2.
  COMP_FUNCTOR compare;
};


#endif //EECS281_PRIORITY_QUEUE_H
