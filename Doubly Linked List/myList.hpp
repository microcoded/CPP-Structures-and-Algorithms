#ifndef MY_LIST_HPP_
#define MY_LIST_HPP_

#include <initializer_list>

template <typename T>
class MyList  {
 public:
  struct Node {
    T data {};
    Node* prev {nullptr};
    Node* next {nullptr};
    Node(T input_data = T {}, Node* prevNode = nullptr,
      Node* nextNode = nullptr) : data {input_data}, prev {prevNode},
                                  next {nextNode} {}
  };

private:
  // Define a head and tail to track the ends of the list,
  // and a size int which tracks the size of the list.
  Node* head = nullptr;
  Node* tail = nullptr;
  int size_ = 0;

 public:
  // Default Constructor
  // Do some setup of your list architecture?
  MyList();

  // Construct a list from an initializer list
  // This lets us create a list with the ints 1,2,3,4 by saying
  // MyList<int> li {1,2,3,4};
  MyList(std::initializer_list<T> vals);

  // Copy Constructor
  MyList(const MyList&); 

  // Operator=
  MyList& operator=(MyList); 

  // Destructor
  // free all memory allocated with new
  ~MyList();
 
  // return the first element by reference
  T& front();
  // return the first element by const reference
  // this one can be used on a const MyList
  const T& front() const;

  // return the last element by reference
  T& back();
  // return the last element by const reference
  // this one can be used on a const MyList
  const T& back() const;

  // add an element to the front of the list
  void push_front(T);
  // remove the first element
  void pop_front();

  // add an element to the back of the list
  void push_back(T);
  // remove the last element
  void pop_back();

  // does the list have any elements?
  bool empty() const;
  // return the number of elements in the list
  int size() const;

  // Define any additional member functions?
};

#endif    // MY_LIST_HPP_
