#ifndef EECS281_PQ_H
#define EECS281_PQ_H

#include <functional>
#include <iterator>
#include <vector>

// A simple interface that implements a generic priority queue.
// Runtime specifications assume constant time comparison and copying.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class Eecs281PQ {
public:
    virtual ~Eecs281PQ() {}

    // Description: Add a new element to the priority queue.
    virtual void push(const TYPE &val) = 0;

    // Description: Remove the most extreme (defined by 'compare') element
    //              from the priority queue.
    virtual void pop() = 0;

    // Description: Return the most extreme (defined by 'compare') element of
    //              the priority queue.
    virtual const TYPE &top() const = 0;

    // Description: Get the number of elements in the priority queue.
    virtual std::size_t size() const = 0;

    // Description: Return true if the priority queue is empty.
    virtual bool empty() const = 0;

    // Description: Assumes that all elements in the priority queue are out
    //              of order. Must reorder the data so that the PQ
    //              invariant is restored. Each derived PQ will have to
    //              implement this appropriately.
    virtual void updatePriorities() = 0;

protected:
    Eecs281PQ() {}
    explicit Eecs281PQ(const COMP_FUNCTOR &comp) : compare{ comp } {}

    // Can use this in the derived classes with:
    // this->compare(thing1, thing2)
    // With the default compare function (std::less), this will
    // tell if Thing1 is lower priority than Thing2.
    COMP_FUNCTOR compare;
}; // Eecs281PQ


#endif
