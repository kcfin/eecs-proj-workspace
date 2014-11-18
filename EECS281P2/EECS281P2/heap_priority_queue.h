#ifndef HEAP_PRIORITY_QUEUE_H
#define HEAP_PRIORITY_QUEUE_H

#include "eecs281_priority_queue.h"

//A specialized version of the 'priority_queue' ADT implemented as a binary priority_queue.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class heap_priority_queue : public eecs281_priority_queue<TYPE, COMP_FUNCTOR> {
public:
  typedef unsigned size_type;

  //Description: Construct a priority_queue out of an iterator range with an optional
  //             comparison functor.
  //Runtime: O(n) where n is number of elements in range.
  template<typename InputIterator>
  heap_priority_queue(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR());

  //Description: Construct an empty priority_queue with an optional comparison functor.
  //Runtime: O(1)
  heap_priority_queue(COMP_FUNCTOR comp = COMP_FUNCTOR());

  //Description: Assumes that all elements inside the priority_queue are out of order and
  //             'rebuilds' the priority_queue by fixing the priority_queue invariant.
  //Runtime: O(n)
  virtual void fix();

  //Description: Add a new element to the priority_queue.
  //Runtime: O(log(n))
  virtual void push(const TYPE& val);

  //Description: Remove the most extreme (defined by 'compare') element from
  //             the priority_queue.
  //Note: We will not run tests on your code that would require it to pop an
  //element when the priority_queue is empty. Though you are welcome to if you are
  //familiar with them, you do not need to use exceptions in this project.
  //Runtime: O(log(n))
  virtual void pop();

  //Description: Return the most extreme (defined by 'compare') element of
  //             the priority_queue.
  //Runtime: O(1)
  virtual const TYPE& top() const;

  //Description: Get the number of elements in the priority_queue.
  //Runtime: O(1)
  virtual size_type size() const
    { return data.size(); }

  //Description: Return true if the priority_queue is empty.
  //Runtime: O(1)
  virtual bool empty() const
    { return data.empty(); }
    

private:
  //Note: This vector *must* be used your priority_queue implementation.
  std::vector<TYPE> data;
private:
    
    void fixUp(int valIndex);
    
    void fixDown(int valIndex);
  //***Add any additional member functions or data you require here.
};

template<typename TYPE, typename COMP_FUNCTOR>
template<typename InputIterator>
heap_priority_queue<TYPE, COMP_FUNCTOR>::heap_priority_queue(
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
heap_priority_queue<TYPE, COMP_FUNCTOR>::heap_priority_queue(COMP_FUNCTOR comp) {
    this->compare = comp;
}

template<typename TYPE, typename COMP_FUNCTOR>
void heap_priority_queue<TYPE, COMP_FUNCTOR>::fix() {
    for(int i = size() - 1; i >=0; i--) {
        fixDown(i);
    }
}

template<typename TYPE, typename COMP_FUNCTOR>
void heap_priority_queue<TYPE, COMP_FUNCTOR>::push(const TYPE& val) {
    data.push_back(val);
    if(size() > 1) {
        fixUp(size() - 1);        
    }
}

template<typename TYPE, typename COMP_FUNCTOR>
void heap_priority_queue<TYPE, COMP_FUNCTOR>::pop() {
    std::swap(data[0], data[data.size()-1]);
    data.pop_back();
    if(size() > 1) {
        fixDown(0);
    }
}

template<typename TYPE, typename COMP_FUNCTOR>
const TYPE& heap_priority_queue<TYPE, COMP_FUNCTOR>::top() const {
    return data.front();
}

template<typename TYPE, typename COMP_FUNCTOR>
void heap_priority_queue<TYPE, COMP_FUNCTOR>::fixUp(int valIndex) {
    while(valIndex > 0 && this->compare(data[valIndex/2], data[valIndex])) {
        std::swap(data[valIndex/2], data[valIndex]);
        valIndex /= 2;
    }
}

template<typename TYPE, typename COMP_FUNCTOR>
void heap_priority_queue<TYPE, COMP_FUNCTOR>::fixDown(int valIndex) {
    
    while(valIndex * 2 < (int)size()) {
        int child = valIndex * 2;
        if(child < (int)size()-1 && this->compare(data[child], data[child+1])) {
            child++;
        }
        if(!this->compare(data[valIndex], data[child])) {
            break;
        }
        std::swap(data[valIndex], data[child]);
        valIndex = child;
    }
}

#endif //HEAP_PRIORITY_QUEUE_H

