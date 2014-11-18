#include <iostream>
#include <vector>
#include "poorman_priority_queue.h"
#include "sorted_priority_queue.h"

using namespace std;

// Comparison functor for integer pointers
struct intptr_comp {
    bool operator() (const int *a, const int  *b) const    {
        return *a < *b;
    }
};

int main()    {
    poorman_priority_queue<int *, intptr_comp> pmheap;
    sorted_priority_queue<int *, intptr_comp> sheap;
    vector<int> owner = {10, 5, 20, 7, 9, 2, 3};
    
    for(auto &i : owner) {
        pmheap.push(&i);
        sheap.push(&i);
    }
    // reference needed for next line
    // so that this is the address of the value in owner
    
    // Process each number in priority order
    while(!pmheap.empty()){
        
        // Pop one int pointer off the Poor Man's Heap
        // the integer itself is still “alive” in the vector owner
        
        cout << *pmheap.top() << ' ';
        pmheap.pop();
    }
    
    cout << endl;
    
    while(!sheap.empty()) {
        cout << *sheap.top() << ' ';
        sheap.pop();
    }
    
    cout << endl;
    
    // pmheap is empty, but owner still has all 4 integers.
    // program should print 20 10 7 5
    
    return 0;
}