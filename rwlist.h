#pragma once
#include <pthread.h>

/// TODO: complete this implementation of a thread-safe (concurrent) sorted
/// linked list of integers, which should use readers/writer locking.
pthread_rwlock_t lock;

class rwlist
{
	/// a node consists of a value and a pointer to another node
	struct Node
	{
		int value;
		Node* next;
	};

	/// The head of the list is referenced by this pointer
	Node* head;
	Node* curr;
	Node *temp;

public:
	rwlist(int): head(NULL)
	{
		head = NULL;
	}

	/// insert *key* into the linked list if it doesn't already exist; return
	/// true if the key was added successfully.
	bool insert(int key)
	{
		pthread_rwlock_wrlock(&lock);
		if(head == NULL) {
			head = new Node;
			head->value = key;
			head->next = NULL;
		} else {
			curr = head;
			temp = new Node;
			temp->value = key;

			while(curr->next != NULL && curr->next->value < key) 
			{
				curr = curr->next;
			}
			curr->next = temp;
		}
		pthread_rwlock_unlock(&lock);
		return true;
	}
	/// remove *key* from the list if it was present; return true if the key
	/// was removed successfully.
	bool remove(int key)
	{
		curr = head;
		if(head != NULL) {
			temp = curr->next;
		}
		pthread_rwlock_rdlock(&lock);
		while(curr != NULL) {
			if(curr->value == key) {
				temp = curr;
				curr = curr->next;
				pthread_rwlock_unlock(&lock);
				return true;
			}
			curr = curr->next;
		}
		pthread_rwlock_unlock(&lock);
		return false;
	}
	/// return true if *key* is present in the list, false otherwise
	bool lookup(int key)
	{
		curr = head;
		pthread_rwlock_rdlock(&lock);
		while(curr != NULL) {
			if(key == curr->value) {
				pthread_rwlock_unlock(&lock);
				return true;
			}
			curr = curr->next;
		}
		pthread_rwlock_unlock(&lock);
		return false;
	}

	//The following are not tested by the given tester but are required for grading
	//No locks are required for these.
	size_t getSize() const
	{
		return 0;
	}
	int getElement(size_t idx) const
	{
		return 0;
	}

	//These functions just need to exist, they do not need to do anything
	size_t getBucketSize(size_t bucket) const
	{
		return 0;
	}
	int getElement(size_t bucket, size_t idx) const
	{
		return 0;
	}
};
