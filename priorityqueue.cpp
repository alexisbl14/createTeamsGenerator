#include <iostream>
#include "json.hpp"
#include <iostream>

#include "priorityqueue.h"

PriorityQueue::PriorityQueue(std::size_t max_size) :   //initializer for PriorityQueue
	nodes_(1, KeyValuePair()),      
	max_size_(max_size),           //set max_size to value input when initialized
	size_(0) {                     //set initial size to 0
}

void PriorityQueue::insert(Key k) {             //defining function insert if only key is given
	insert(std::make_pair(k, std::make_pair(0, 0)));  //inserts pair with first as key and second as (0,0)
}

void PriorityQueue::insert(KeyValuePair kv) {   //defining function insert for keyvaluepair
    nodes_.push_back(kv);      //push back KeyValuePair to end of nodes_
    size_ ++;                  //increment size
    int back = size();         //set back as size(), which is index of last KeyValuePair in nodes_
    heapifyUp(back);           //call to heapifyUp from the index of node we just inserted
}

KeyValuePair PriorityQueue::min() {  //defining function min to retrieve min KeyValuePair
    return nodes_[ROOT];         //min is value at root, or value at index 1
}

KeyValuePair PriorityQueue::removeMin() { //defining function removeMin to remove the min KeyValuePair
    KeyValuePair min_ = min();     //min_ set to min KeyValuePair in queue
    removeNode(ROOT);             //call function to removeNode, specifying root as node to be removed
    return min_;                     //return min_ node just removed
}

bool PriorityQueue::isEmpty() const {   //defining function isEmpty to declare if queue is empty or not
    return size() == 0;      //if size() is 0, then queue is empty
}

size_t PriorityQueue::size() const {   //defining funtion size() to return size of queue
    return size_;  //returns value of variable size_
}

nlohmann::json PriorityQueue::JSON() const {  //function to output queue in a JSON format
	nlohmann::json result;
  for (size_t i = 1; i <= size_; i++) {   //iterate through each node
      nlohmann::json node;
      KeyValuePair kv = nodes_[i];
      node["key"] = kv.first;           //set key and value to key and value of current node_
      node["value"] = kv.second;
      if (i != ROOT) {                //set parent to index of current i/2
          node["parent"] = std::to_string(i / 2);
      }
      if (2 * i <= size_) {                        //set leftchild to index of current i * 2
          node["leftChild"] = std::to_string(2 * i);
      }
      if (2 * i + 1 <= size_) {                      //set rightchild to index of current i * 2 + 1
          node["rightChild"] = std::to_string(2 * i + 1);
      }
      result[std::to_string(i)] = node;       //set node to value of current i
  }
  result["metadata"]["max_size"] = max_size_;  //set max size to max size of queue
  result["metadata"]["size"] = size_;           //set value of size to size of queue
	return result;
}

void PriorityQueue::heapifyUp(size_t i) {  //function to heapify up a value until it fits in the queue
    size_t parent = (i/2);               //parent of given index is index/2
    if (i && nodes_[parent] > nodes_[i]){     //if key of parent is larger than key of given index
        swap(nodes_[i], nodes_[parent]);      //swap the values and 
        heapifyUp(parent);                    //call heapify up on index of parent, which now contains value from nodes_[i]
    }
}

void PriorityQueue::heapifyDown(size_t i) {    //function to heapify down a value until it fits in the queue
    size_t left = (2 * i);                    //left child is at index i * 2
    size_t right = (2 * i + 1);               //right child is at index i * 2 + 1
    size_t smallest = i;                      //set smallest value to index i initially

    if (left <= size() && nodes_[left] < nodes_[i]){   //if left index is in queue and key of left is less than the key of current index
        smallest = left;                               //new smallest is at index of left
    }
    if (right <= size() && nodes_[right] < nodes_[smallest]){   // if right index is in queue and key of right is less than key of current smallest
        smallest = right;                              //new smallest is at index of right
    }
    if (smallest != i){                    //if smallest is not current index, swap the nodes of current index and smallest
        swap(nodes_[i], nodes_[smallest]);
        heapifyDown(smallest);               //then heapify down on the smallest index, which now contains keyValuePair of nodes_[i]
    }
}

void PriorityQueue::removeNode(size_t i) {  //function to remove node at index 
    nodes_[i] = nodes_.back();         //new nodes[i] is now value at end of nodes
    nodes_.pop_back();                //delete value at end of nodes since it is now at index i
    size_ --;                         //decrease size by 1
    heapifyDown(i);                 //call heapify down on index i to find correct place
    
}

Key PriorityQueue::getKey(size_t i) { //function to return key at index i
    return nodes_[i].first;          //return nodes_[i] key, which is at .first
}
