// Sources used:
// https://stackoverflow.com/a/48601511/13640042
// Pasted at position used

#ifndef INDEX_PRIORITY_QUEUE_HPP_
#define INDEX_PRIORITY_QUEUE_HPP_

#include <vector>
#include <algorithm>

template<typename T>
class IndexPriorityQueue {
private:
    // vector to hold priorities.
    // priorities.at(i) is the priority associated to index i
    // the heap will only contain indices, we look up priorities
    // in this vector as needed
    std::vector<T> priorities{};
    // priorityQueue stores indices: priorityQueue.at(i) is an index
    // priorityQueue functions as the heap and is heap ordered:
    // priorities.at(priorityQueue.at(i)) <= priorities.at(priorityQueue.at(2 * i))
    // priorities.at(priorityQueue.at(i)) <= priorities.at(priorityQueue.at(2 * i) + 1)
    std::vector<int> priorityQueue{};
    // indexToPosition.at(i) is the position in priorityQueue of index i
    // priorityQueue.at(indexToPosition.at(i)) = i
    // indexToPosition.at(priorityQueue.at(j)) = j
    std::vector<int> indexToPosition{};
    // Size of priorityQueue as an integer
    int size_ = 0;

public:
    explicit IndexPriorityQueue(int);

    void push(const T &, int);

    void pop();

    void erase(int);

    [[nodiscard]] bool contains(int) const;

    void changeKey(const T &, int);

    std::pair<T, int> top() const;

    [[nodiscard]] bool empty() const;

    [[nodiscard]] int size() const;

private:
    void swim(int i);

    void sink(int i);

    void setIndexToPosAsPq(int i);
};

// -- Useful helper functions --
int leftChild(int i) {
    return 2 * i;
}

int rightChild(int i) {
    return 2 * i + 1;
}

int parent(int i) {
    return i / 2;
}

// -- IndexPriorityQueue member functions --

// Default constructor
template<typename T>
IndexPriorityQueue<T>::IndexPriorityQueue(int N) :
        priorities(N + 1), // Set the priorities vector to size N + 1
        priorityQueue(static_cast<unsigned long>(N) + 1), // Set the priorityQueue vector to size N + 1
        indexToPosition(static_cast<unsigned long>(N) + 1,
                        -1), // Set the indexToPosition vector to size N + 1, with all values set to -1
        size_(0) // Set size_ to 0, since the queue is empty
{
    // We don't need anything extra here since we have already initialised all the values after the :
}

// Determine if the IndexPriorityQueue is empty
template<typename T>
bool IndexPriorityQueue<T>::empty() const {
    return size_ == 0; // Return true when size_ is 0, otherwise false
}

// Return the size of the IndexPriorityQueue
template<typename T>
int IndexPriorityQueue<T>::size() const {
    return size_; // Simply, return size_ which stores the size
}

// Push a new element into IndexPriorityQueue
template<typename T>
void IndexPriorityQueue<T>::push(const T &priority, int index) {
    // First, check if the index is already in the queue
    if (contains(index)) {
        // If the index is already in the queue, we print an error message and don't try to push anything
        std::cerr << "Index is already in queue" << '\n';
    } else {
        // If the index is not in the queue:
        ++size_;                            // Increase the size by 1, as we are adding 1 new element
        priorityQueue[size_] = index;       // Add the index to priorityQueue, at the new size_ index
        indexToPosition[index] = size_;     // Set the position of the index in indexToPosition
        priorities[index] = priority;       // Set the priority to `T& priority` given to this function
        swim(size_);                        // Swim the new element up to where it should be
    }
}

// Pop the top element from IndexPriorityQueue
template<typename T>
void IndexPriorityQueue<T>::pop() {
    // First we check if the queue is empty
    if (size_ == 0) {
        std::cerr << "No elements in the queue" << '\n';    // Print an error if the queue is empty
        return;                                             // Don't execute anything else and exit the function
    }
    // Since the queue is not empty, we now know we can pop
    int minIndex = priorityQueue[1];                        // Int to determine the minimum priority (index at the top)
    std::swap(priorityQueue[1], priorityQueue[size_]);      // Swap the top element with the last element
    setIndexToPosAsPq(1);                                   // Update the position of the element we swapped the indexes of
    indexToPosition[minIndex] = -1;                         // Set the position of the element we are popping to -1, which we use to indicate the element isn't in the queue anymore
    --size_;                                                // Decrease the size by 1, as we have removed an element
    sink(1);                                                // Sink down the swapped element, so it is where it should be
    priorityQueue.pop_back();                               // Remove the last element of priorityQueue, which is what we are popping
}

// Erase a specific element from IndexPriorityQueue
template<typename T>
void IndexPriorityQueue<T>::erase(int index) {
    // First we check that the index is not in the queue
    if (not contains(index)) {
        // If the index is not in the queue, we error and stop executing.
        std::cerr << "Element is not in queue" << '\n';
        return;
    }
    // Now we know that the index to erase is in the queue, so we can erase it!
    int pos = indexToPosition[index];                       // Get the position of the index to erase
    std::swap(priorityQueue[pos], priorityQueue[size_]);    // Swap the element to erase with the top element
    setIndexToPosAsPq(pos);                                 // Update the position of the element we swapped
    indexToPosition[index] = -1;                            // Set the position of the index we are erasing to -1 (not in queue, can now sink/swim correctly)
    --size_;                                                // Decrease size by 1, as we have 1 less element now
    priorityQueue.pop_back();                               // Pop the back element, as this is the one we are erasing
    swim(pos);                                              // Swim the swapped element
    sink(pos);                                              // Sink the swapped element, now we know it is where it should be 😎
}

// Return the top element of the queue
template<typename T>
std::pair<T, int> IndexPriorityQueue<T>::top() const {
    // Return a pair in the format of {top element, index of top element}
    return std::make_pair(priorities[priorityQueue[1]], priorityQueue[1]);
    // Used https://stackoverflow.com/a/48601511/13640042 for return statement
}

// Change the priority of some given element to some given key
template<typename T>
void IndexPriorityQueue<T>::changeKey(const T &key, int index) {
    // If the index is in the queue, we can change it:
    if (contains(index)) {
        T currentKey = priorities[index];       // Get the current priority of the index. We use this to determine if we should sink or swim with the new priority.
        priorities[index] = key;                // Set the priority of the element at the given index to the new key.
        if (key < currentKey) {                 // If the new priority of the element is lower...
            swim(indexToPosition[index]);       // we must swim the element up the queue.
        } else {                                // Otherwise if it is higher (or equal),
            sink(indexToPosition[index]);       // then we sink the index down. If it is equal this will still be functionally okay.
        }
    } else {
        // Otherwise, push the index into the queue with its priority
        push(key, index);
    }
}

// Return whether the IndexPriorityQueue contains some given index
template<typename T>
bool IndexPriorityQueue<T>::contains(int index) const {
    // We know that the index is in the queue IF all these conditions are satisfied:
    //      - index is 0 or greater
    //      - index is less than the size of the indexToPosition vector
    //      - the position of the index is not -1, as this indicates an element no longer in the queue
    return index >= 0 && index < static_cast<int>(indexToPosition.size()) && indexToPosition[index] != -1;
}

// Swim helper function for min heap property.
template<typename T>
void IndexPriorityQueue<T>::swim(int i) {
    // While the current element is not the root (i != 1) AND its priority is less than its parent's priority
    while (i > 1 and priorities[priorityQueue[i]] < priorities[priorityQueue[parent(i)]]) {
        std::swap(priorityQueue[i], priorityQueue[parent(i)]);  // Swap the current element with its parent
        setIndexToPosAsPq(i);                                   // Set the new position of the element,
        setIndexToPosAsPq(parent(i));                           // and its parent
        i = parent(i);                                          // go up to the index of the parent and continue.
    }
}

// Sink helper function for min heap property
template<typename T>
void IndexPriorityQueue<T>::sink(int i) {
    while (leftChild(i) <= size_) { // While there is a left child for the current element
        int child = leftChild(
                i); // Get the index of the left child. We do this here so that we can increment it (leftChild(i) is not assignable)
        // If the left child's priority is greater than the right child's priority, and it is not the root
        if (child < size_ and priorities[priorityQueue[child]] > priorities[priorityQueue[rightChild(i)]]) {
            ++child; // Increase by 1
        }
        // If the current element's priority is <= the priority of the child we don't need to do any further processing, so we exit.
        if (priorities[priorityQueue[i]] <= priorities[priorityQueue[child]]) {
            return;
        }
        std::swap(priorityQueue[i], priorityQueue[child]);  // Swap the current element with the child element
        setIndexToPosAsPq(i);                               // Update the position of the elements we swapped
        setIndexToPosAsPq(child);                           
        i = child;                                          // Now we redo the loop, moving down to the child index (hence sinking)
    }
}

// Helper function to make code easier to read.
// We need to maintain the indexToPosition vector as it is a priorityQueue.
// As part of this, we sometimes need to create this position in the vector for an element based on where it is in the priorityQueue.
// Thus, we often call indexToPosition[priorityQueue[element]] = element. For less writing and easier debugging I have added this as a function.
template<typename T>
void IndexPriorityQueue<T>::setIndexToPosAsPq(int i) {
    indexToPosition[priorityQueue[i]] = i;
}

#endif  // INDEX_PRIORITY_QUEUE_HPP_

