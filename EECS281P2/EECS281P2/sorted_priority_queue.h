#ifndef SORTED_PRIORITY_QUEUE_H
#define SORTED_PRIORITY_QUEUE_H

#include "eecs281_priority_queue.h"
#include <algorithm>

//A specialized version of the 'priority_queue' ADT that is implemented with an
//underlying sorted array-based container.
//Note: The most extreme element should be found at the end of the
//'data' container, such that traversing the iterators yields the elements in
//sorted order.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class sorted_priority_queue : public eecs281_priority_queue<TYPE, COMP_FUNCTOR> {
public:
  typedef unsigned size_type;

  //Description: Construct a priority_queue out of an iterator range with an optional
  //             comparison functor.
  //Runtime: O(n log n) where n is number of elements in range.
  template<typename InputIterator>
  sorted_priority_queue(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR());

  //Description: Construct an empty priority_queue with an optional comparison functor.
  //Runtime: O(1)
  sorted_priority_queue(COMP_FUNCTOR comp = COMP_FUNCTOR());

  //Description: Assumes that all elements inside the priority_queue are out of order and
  //             'rebuilds' the priority_queue by fixing the priority_queue invariant.
  //Runtime: O(n log n)
  virtual void fix();

  //Description: Add a new element to the priority_queue.
  //Runtime: O(n)
  virtual void push(const TYPE& val);

  //Description: Remove the most extreme (defined by 'compare') element from
  //             the priority_queue.
  //Note: We will not run tests on your code that would require it to pop an
  //element when the priority_queue is empty. Though you are welcome to if you are
  //familiar with them, you do not need to use exceptions in this project.
  //Runtime: Amortized O(1)
  virtual void pop();

  //Description: Return the most extreme (defined by 'compare') element of
  //             the priority_queue.
  //Runtime: O(1)
  virtual const TYPE& top() const;

  //Description: Get the number of elements in the priority_queue. This has been
  //             implemented for you.
  //Runtime: O(1)
  virtual size_type size() const { return data.size(); }

  //Description: Return true if the priority_queue is empty. This has been implemented
  //             for you.
  //Runtime: O(1)
  virtual bool empty() const { return data.empty(); }
private:
  //Note: This vector *must* be used your priority_queue implementation.
  std::vector<TYPE> data;
private:
  //***Add any additional member functions or data you require here.
};

template<typename TYPE, typename COMP_FUNCTOR>
template<typename InputIterator>
sorted_priority_queue<TYPE, COMP_FUNCTOR>::sorted_priority_queue(
  InputIterator start,
  InputIterator end,
  COMP_FUNCTOR comp
) {
    this->compare = comp;
    while(start != end) {
        data.push_back(*start++);
    }
    fix();
}

template<typename TYPE, typename COMP_FUNCTOR>
sorted_priority_queue<TYPE, COMP_FUNCTOR>::sorted_priority_queue(COMP_FUNCTOR comp) {
    this->compare = comp;
}

template<typename TYPE, typename COMP_FUNCTOR>
void sorted_priority_queue<TYPE, COMP_FUNCTOR>::fix() {
    sort(data.begin(), data.end(), this->compare);
}

template<typename TYPE, typename COMP_FUNCTOR>
void sorted_priority_queue<TYPE, COMP_FUNCTOR>::push(const TYPE& val) {
    auto iter = data.begin();
    
    while(iter != data.end() && this->compare(*iter, val)) {
        iter++;
    }
    
    data.insert(iter,val);
}

template<typename TYPE, typename COMP_FUNCTOR>
void sorted_priority_queue<TYPE, COMP_FUNCTOR>::pop() {
    data.pop_back();
}

template<typename TYPE, typename COMP_FUNCTOR>
const TYPE& sorted_priority_queue<TYPE, COMP_FUNCTOR>::top() const {
    return data.back();
}

#endif //SORTED_PRIORITY_QUEUE_H
