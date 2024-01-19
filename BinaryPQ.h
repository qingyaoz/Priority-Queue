#ifndef BINARYPQ_H
#define BINARYPQ_H


#include <algorithm>
#include "Eecs281PQ.h"

// A specialized version of the priority queue ADT implemented as a binary
// heap.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class BinaryPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR> {
    // This is a way to refer to the base class object.
    using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

public:
    // Description: Construct an empty PQ with an optional comparison functor.
    // Runtime: O(1)
    explicit BinaryPQ(COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp } {
        TYPE zero;
        data.push_back(zero); 
    } // BinaryPQ


    // Description: Construct a PQ out of an iterator range with an optional
    //              comparison functor.
    // Runtime: O(n) where n is number of elements in range.
    template<typename InputIterator>
    BinaryPQ(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp } {
        TYPE zero;
        data.push_back(zero); // need to push zero first, then add others
        // can't use begin(), or the zero will be at the last instead of the first. 
        data.insert(data.end(), start, end); 
        updatePriorities();
    } // BinaryPQ


    // Description: Destructor doesn't need any code, the data vector will
    //              be destroyed automatically.
    virtual ~BinaryPQ() {
    } // ~BinaryPQ()


    // Description: Assumes that all elements inside the heap are out of
    //              order and 'rebuilds' the heap by fixing the heap
    //              invariant.
    // Runtime: O(n)
    virtual void updatePriorities() {
        size_t index = size() / 2; // start from the one before last leaf
        while (index != 0){
            fixDown(index--);
        }
    } // updatePriorities()


    // Description: Add a new element to the PQ.
    // Runtime: O(log(n))
    virtual void push(const TYPE &val) {
        data.push_back(val);
        fixUp(size());
    } // push()


    // Description: Remove the most extreme (defined by 'compare') element
    //              from the PQ.
    // Note: We will not run tests on your code that would require it to pop
    //       an element when the PQ is empty. Though you are welcome to if
    //       you are familiar with them, you do not need to use exceptions in
    //       this project.
    // Runtime: O(log(n))
    virtual void pop() {
        data[1] = data.back();
        data.pop_back();
        fixDown(1);
    } // pop()


    // Description: Return the most extreme (defined by 'compare') element of
    //              the PQ. This should be a reference for speed. It MUST
    //              be const because we cannot allow it to be modified, as
    //              that might make it no longer be the most extreme element.
    // Runtime: O(1)
    virtual const TYPE &top() const {
        return data[1];
    } // top()


    // Description: Get the number of elements in the PQ.
    // Runtime: O(1)
    virtual std::size_t size() const {
        return data.size()-1;
    } // size()


    // Description: Return true if the PQ is empty.
    // Runtime: O(1)
    virtual bool empty() const {
        return (data.size()-1) == 0;
    } // empty()


private:
    std::vector<TYPE> data;

    void fixUp(size_t k) {
        while (k > 1 && this->compare(data[k/2], data[k])) {
            std::swap(data[k], data[k/2]);
            k /= 2;
        }// while loop
    } // fixUp

    void fixDown(size_t k) {
        while (2 * k <= size()){ // use size instead of data.size()-1 to save time
            size_t j = 2 * k; // start with left child
            if (j < size() && this->compare(data[j], data[j+1]))
                ++j; // swap larger one first
            if (!this->compare(data[k], data[j])) break; // aleady ordered
                // compare(data[j], data[k]) won't be true when data[j]=data[k]
            std::swap(data[k], data[j]);
            k = j; //move down
        } // while loop
    } // fixDown
}; // BinaryPQ


#endif // BINARYPQ_H
