#pragma once
#include <mutex>

/// TODO: complete this implementation of a thread-safe (concurrent) hash
///       table of integers, implemented as an array of linked lists.  In
///       this implementation, each list should have a "sentinel" node that
///       contains the lock, so we can't just reuse the clist implementation
class shash
{
private:
	struct Node
	{
		int value;
		Node* next;
	};

	struct sentinel
	{
		Node* list_elem;
		std::mutex mtx;
	};

	Node* head;
	Node* curr;
	Node* temp;
	sentinel* s_node;

public:
	int size;
	sentinel** buckets;

	shash(unsigned _buckets)
	{
		size = _buckets;
		buckets = new sentinel*[size];
		for(int i = 0; i < size; i++) {
			s_node = new sentinel;
			head = new Node;
			s_node->list_elem = head;
			buckets[i] = s_node;
		}
	}


	/// insert *key* into the appropriate linked list if it doesn't already
	/// exist; return true if the key was added successfully.
	bool insert(int key)
	{
		s_node->mtx.lock();
		if(head == NULL) {
			head = new Node;
			head->value = key;
			head->next = NULL;
		} else {
			curr = head;
			temp = new Node;
			temp->value = key;

			while(curr->next != NULL && curr->next->value < key) {
				curr = curr->next;
			}
			curr->next = temp;
		}
		s_node->mtx.unlock();
		return true;
	}
	/// remove *key* from the appropriate list if it was present; return true
	/// if the key was removed successfully.
	bool remove(int key)
	{
		curr = head;
		if(head != NULL) {
			temp = curr->next;
		}
		s_node->mtx.lock();

		while(curr != NULL)
		{
			if(curr->value == key) {
				temp = curr;
				curr = curr->next;
				s_node->mtx.unlock();
				return true;
			}
			curr = curr->next;
		}
		s_node->mtx.unlock();
		return false;
	}
	/// return true if *key* is present in the appropriate list, false
	/// otherwise
	bool lookup(int key)
	{
		curr = head;
		s_node->mtx.lock();
		while(curr != NULL) 
		{
			if(key == curr->value) {
				s_node->mtx.unlock();
				return true;
			}
			curr = curr->next;
		}
		s_node->mtx.unlock();
		return false;
	}

	//The following are not tested by the given tester but are required for grading
	//No locks are required for these.

	//This refers to the number of buckets not the total number of elements.
	size_t getSize() const
	{
		return 0;
	}

	//This refers to the number of elements in a bucket, not the sentinel node.
	size_t getBucketSize(size_t bucket) const
	{
		return 0;
	}
	int getElement(size_t bucket, size_t idx) const
	{
		return 0;
	}


	//These functions just need to exist, they do not need to do anything
	int getElement(size_t idx) const
	{
		return 0;
	}
};
