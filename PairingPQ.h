#ifndef PAIRINGPQ_H
#define PAIRINGPQ_H

#include "Eecs281PQ.h"
#include <deque>
#include <utility>

// A specialized version of the priority queue ADT implemented as a pairing
// heap.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class PairingPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR> {
    // This is a way to refer to the base class object.
    using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

public:
    // Each node within the pairing heap
    class Node {
        public:
            explicit Node(const TYPE &val)
                : elt{ val }, child{ nullptr }, sibling{ nullptr }, parent{ nullptr }
            {}

            // Description: Allows access to the element at that Node's position.
            // Runtime: O(1) - this has been provided for you.
            const TYPE &getElt() const { return elt; }
            const TYPE &operator*() const { return elt; }

            // Access any private data members of this Node class from within the PairingPQ class.
            friend PairingPQ;

        private:
            TYPE elt;
            Node *child;
            Node *sibling;
            Node *parent;
    }; // Node


    // Description: Construct an empty pairing heap with an optional
    //              comparison functor.
    // Runtime: O(1)
    explicit PairingPQ(COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp } {
        root = nullptr;
        count = 0;
    } // PairingPQ()


    // Description: Construct a pairing heap out of an iterator range with an
    //              optional comparison functor.
    // Runtime: O(n) where n is number of elements in range.
    template<typename InputIterator>
    PairingPQ(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp } {
        root = nullptr;
        count = 0;
        while (start != end) {
            push(*start);
            start ++;
        }
        
    } // PairingPQ()


    // Description: Copy constructor.
    // Runtime: O(n)
    PairingPQ(const PairingPQ &other) :
        BaseClass{ other.compare } {
        root = nullptr;
        count = 0;

        if (other.root == nullptr){
            return;
        }

        std::deque<Node*> temp;
        temp.push_back(other.root);
        while (!temp.empty()) {
            if (temp.front()->child != nullptr){
                temp.push_back(temp.front()->child);
            }
            if (temp.front()->sibling != nullptr){
                temp.push_back(temp.front()->sibling);
            }
            push(temp.front()->elt);
            temp.pop_front();
        }
    } // PairingPQ()


    // Description: Copy assignment operator.
    // Runtime: O(n)
    PairingPQ &operator=(const PairingPQ &rhs) {
        // use pointer, since the whole PQ maybe large;
        PairingPQ* temp = new PairingPQ(rhs);
        std::swap(root, temp->root);
        count = temp->count;
        delete temp;
        return *this;
    } // operator=()


    // Description: Destructor
    // Runtime: O(n)
    ~PairingPQ() {
        std::deque<Node*> temp;
        if (root != nullptr) temp.push_back(root);
        while(!temp.empty()){
            Node* poping = temp.front();
            if (poping->child != nullptr){
                temp.push_back(poping->child);
            }
            if (poping->sibling != nullptr){
                temp.push_back(poping->sibling);
            }

            temp.pop_front();
            delete poping;
            // delete temp.front(); 
            // temp.pop_front();
            count --;
        }
    } // ~PairingPQ()


    // Description: Assumes that all elements inside the pairing heap are out
    //              of order and 'rebuilds' the pairing heap by fixing the
    //              pairing heap invariant. CANNOT delete 'old' nodes
    //              and create new ones!
    // Runtime: O(n)
    virtual void updatePriorities() {
        std::deque<Node*> temp;
        if (root == nullptr || root->child == nullptr) return;

        temp.push_back(root->child);
        root->child = nullptr;

        while (!temp.empty()){
            Node* curr = temp.front();

            if (curr->child != nullptr){
                temp.push_back(curr->child);
                curr->child = nullptr;
            }
            if (curr->sibling != nullptr){
                temp.push_back(curr->sibling);
                curr->sibling = nullptr;
            }
            curr->parent = nullptr;

            root = meld(root, curr);
            temp.pop_front();
        }
    } // updatePriorities()


    // Description: Add a new element to the pairing heap. 
    // Runtime: O(1)
    virtual void push(const TYPE &val) {
        addNode(val);
    } // push()


    // Description: Remove the most extreme (defined by 'compare') element
    //              from the pairing heap.
    // Runtime: Amortized O(log(n))
    virtual void pop() {
        if (root == nullptr) return;

        if (root->child == nullptr){
            delete root;
            root = nullptr;
            count = 0;
            return;
        }

        std::deque<Node*> temp;
        Node* to_next = root->child;
        while (to_next != nullptr) {
            temp.push_back(to_next);
            to_next = to_next->sibling;
        }

        while (temp.size() > 1) {
            Node* meld_1 = temp[0];
            meld_1->parent = nullptr;
            meld_1->sibling = nullptr;

            Node* meld_2 = temp[1];
            meld_2->parent = nullptr;
            meld_2->sibling = nullptr;

            temp.pop_front();
            temp.pop_front();

            temp.push_back(meld(meld_1, meld_2));
        }

        delete root;
        // std::cout << temp.size() <<std::endl;
        root = temp.front();
        count --;
    } // pop()


    // Description: Return the most extreme (defined by 'compare') element of
    //              the pairing heap. This should be a reference for speed.
    //              It MUST be const because we cannot allow it to be
    //              modified, as that might make it no longer be the most
    //              extreme element.
    // Runtime: O(1)
    virtual const TYPE &top() const {
        return root->elt;
    } // top()


    // Description: Get the number of elements in the pairing heap.
    // Runtime: O(1)
    virtual std::size_t size() const {
        return count;
    } // size()

    // Description: Return true if the pairing heap is empty.
    // Runtime: O(1)
    virtual bool empty() const {
        return count == 0;
    } // empty()


    // Description: Updates the priority of an element already in the pairing
    //              heap by replacing the element refered to by the Node with
    //              new_value. Must maintain pairing heap invariants.
    //
    // PRECONDITION: The new priority, given by 'new_value' must be more
    //               extreme (as defined by comp) than the old priority.
    //
    // Runtime: As discussed in reading material.
    void updateElt(Node* node, const TYPE &new_value) {
        node->elt = new_value;
        if (node == root) return;

        // if new value still less extreme than parent, nothing need to do
        if (this->compare(new_value, node->parent->elt)) return;

        if (node->parent->child == node){ // leftest one
            node->parent->child = node->sibling;
        }   
        else {
            Node* prv = node->parent->child;
            while (prv->sibling != node){
                prv = prv->sibling;
            }
            prv->sibling = node->sibling;
            // if node is the rightest one, prv->sibling will be nullptr too
        }
        node->sibling = nullptr;
        node->parent = nullptr;
        root = meld(node, root);
    } // updateElt()


    // Description: Add a new element to the pairing heap. Returns a Node*
    //              corresponding to the newly added element.
    // Runtime: O(1)
    Node* addNode(const TYPE &val) {
        // TODO: Implement this function
        Node* add = new Node(val);

        if (root == nullptr) root = add;
        else root = meld(add, root);
        count ++;
        return add;
    } // addNode()


private:

    Node* root;
    size_t count;

    Node* meld(Node* tree1, Node* tree2){

        if (this->compare(tree1->elt, tree2->elt)){
            if (tree2->child != nullptr){
                tree1->sibling = tree2->child;
            }
            tree2->child = tree1;
            tree1->parent = tree2;
            return tree2;
        } else{
            if (tree1->child != nullptr){
                tree2->sibling = tree1->child;
            }
            tree1->child = tree2;
            tree2->parent = tree1;
            return tree1;
        }
    }
};


#endif // PAIRINGPQ_H
