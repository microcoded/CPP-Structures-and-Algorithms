#include <string>
#include "myList.hpp"
#include "myInteger.hpp"

// default constructor
template <typename T>
MyList<T>::MyList() {
    head = nullptr;
    tail = nullptr;
}

// copy constructor
template <typename T>
MyList<T>::MyList(const MyList& other) : head(nullptr), tail(nullptr) {
    size_ = 0;
    // Create a new node based on the start of the original list.
    Node* current = other.head;
    // Transverse through the list until we're pointing to a nullptr.
    while (current != nullptr) {
        // Add the data to the current node by using push_back.
        push_back(current->data);
        // Go to the next node.
        current = current->next;
    }
}

// assignment operator
template <typename T>
MyList<T>& MyList<T>::operator=(MyList other) {
    // Copy the data, swap it, and then the library clears it from memory.
    // Simple way of doing assignment!
    std::swap(head, other.head);
    std::swap(tail, other.tail);
    std::swap(size_, other.size_);
    return *this;
}

// destructor
template <typename T>
MyList<T>::~MyList() {
    Node* del = head;
    // Delete each and every node to prevent memory leaks.
    while (del) {
        // We use current and not head so that we are not pointing to any nullptrs,
        // and while (current) is the same as while (current != nullptr). This makes sure
        // that we don't access any memory we shouldn't.
        Node* temp = del;
        del = del->next;
        delete temp;
    }
    size_ = 0; // The size is now empty!
    head = nullptr; // All nodes are gone, so the head should be moved to a nullptr.
}

// constructor from an initializer list
template <typename T>
MyList<T>::MyList(std::initializer_list<T> vals) {
    // For each element in our list of vals (of any type)
    // we push it to back of the list.
    for (const T& val : vals) {
        push_back(val);
    }
}

// push back
template <typename T>
void MyList<T>::push_back(T val) {
    // Create a new node containing the value
    Node* node = new Node(val);
    // Increase the size of the list by one, as we're about to insert one item into it.
    ++size_;
    if (tail) {
        // Add the node to the back of the list and fix up where the next and previous are pointing to.
        tail->next = node;
        node->prev = tail;
    }
    // We make the new node be the head if the list is empty.
    if (!head) head = node;
    // Now since we are pushing to the back, we know the tail must now be this node.
    // So, set the tail as the new node.
    tail = node;
}

// pop back
template <typename T>
void MyList<T>::pop_back() {
    // If the list is empty, we should escape this function.
    // This is our guard.
    if (!tail) return;

    // We are removing one node (and can as of above!), so decrease the size by 1.
    --size_;

    // We are going to delete this node, so store it as another variable.
    Node* del = tail;
    // Move tail back one
    tail = tail->prev;
    // Delete what comes after the tail, so it is the end
    if (tail) {
        tail->next = nullptr;
    } else {
        head = nullptr;
    }
    // Delete the old tail, goodbye memory leak.
    delete del;
}

// push front
template <typename T>
void MyList<T>::push_front(T val) {
    // Create a new node containing the value
    Node* node = new Node(val);
    // Increase the size of the list by one, as we're about to insert one item into it.
    ++size_;
    if (head) {
        // Add the node to the front of the list and fix up where the next and previous are pointing to.
        head->prev = node;
        node->next = head;
    }
    // We make the new node be the tail if the list is empty. (We also make it be the head but in every case,
    // so we don't need to do it in this if statement and repeat ourselves!).
    if (!tail) tail = node;
    // Now since we are pushing to the front, we know the head must now be this node.
    // So, set the head as the new node.
    head = node;
}

// pop front
template <typename T>
void MyList<T>::pop_front() {
    // If the list is empty, we should escape this function.
    // This is our guard.
    if (!head) return;

    // We are removing one node (and can as of above!), so decrease the size by 1.
    --size_;
    // We are going to delete this node, so store it as another variable.
    Node* del = head;
    // Move head forward one
    head = head->next;
    // Delete what comes before the head, so it is the start
    if (head) {
        head->prev = nullptr;
    } else {
        tail = nullptr;
    }
    // Delete the old head, goodbye memory leak.
    delete del;
}


// return the first element by reference
template <typename T>
T& MyList<T>::front() {
    // Return the data in the head!
    return head->data;
}

// return the first element by const reference
template <typename T>
const T& MyList<T>::front() const {
    // Return the data in the head! BUT THIS TIME IT IS A CONST 😱😱😱😱😱
    return head->data;
}

// return the last element by reference
template <typename T>
T& MyList<T>::back() {
    // Return the data in the tail, what a surprise!
    return tail->data;
}

// return the last element by const reference
template <typename T>
const T& MyList<T>::back() const {
    // Return the data in the tail. But get this, we do it as a const. 🤯🤯🤯
    return tail->data;
}

// is the list empty?
template <typename T>
bool MyList<T>::empty() const {
    // We use our Super Useful (TM) size() function.
    // If the size is 0, we know it's empty! If it is anything else, it is not empty!
    // So, we return if the size is 0 or not, woohoo.
    return size() == 0;
}

// return the number of elements in the list
template <typename T>
int MyList<T>::size() const {
    // We return the size_ int which we have been automatically updating with push/pop etc.
    return size_;
}

// These lines let the compiler know with which types we will be
// instantiating MyList
template class MyList<int>;
template class MyList<std::string>;
template class MyList<MyInteger>;

