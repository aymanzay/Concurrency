#pragma once
#include <mutex>
/// TODO: complete this implementation of a thread-safe (concurrent) hash
///       table of integers, implemented as an array of linked lists.  In
///       this implementation, each list should have a "sentinel" node that
///       contains the lock, so we can't just reuse the clist implementation.
///       In addition, the API now allows for multiple keys on each
///       operation.
class shash2
{
private:
	struct Node
	{
		int value;
		Node* next;
	};

	/* Sentinel Node */
	struct sentinel
	{
		Node* list;
		std::mutex mtx;
	};

	Node* head;
	Node* curr;
	Node* temp;
	sentinel* s_node;
public:
	int size;
	sentinel** buckets;
	shash2(unsigned _buckets)
	{
		size = _buckets;
		buckets = new sentinel*[size];
		for(int i = 0; i < size; i++) {
			s_node = new sentinel;
			head = NULL;
			s_node->list = head;
			buckets[i] = s_node;
		}
	}

	/// insert /num/ values from /keys/ array into the hash, and return the
	/// success/failure of each insert in /results/ array.
	void insert(int* keys, bool* results, int num)
	{
		if(head == NULL) { /* List is empty */
			for(int i = 0; i < num; i++) {
				head = new Node;
				head->next = NULL;
				head->value = keys[i];
				results[i] = true;
				s_node->mtx.lock();

				if(head == NULL) {
					results[i] = false;
				}
				s_node->mtx.unlock();
			}
		} else { /* Place element in sorted list position */
			curr = head;
			Node* temp2;
			for(int j = 0; j < num; j++) {
				temp2 = new Node;
				s_node->mtx.lock();
				printf("HELLO Insert\n");
				temp2->value = keys[j];
				results[j] = true;
				
				if(temp2 == NULL) {
					results[j] = false;
				}
				s_node->mtx.unlock();
			}
			for(int i = 0; i < num; i++) {
				while(curr) {
					curr = curr->next;
				}
			}
		}
	}
	/// remove *key* from the list if it was present; return true if the key
	/// was removed successfully.
	void remove(int* keys, bool* results, int num)
	{
		curr = head;
		if(head != NULL) {
			temp = curr->next;
		}
		if(curr->next != NULL) {
			while(curr != NULL) {
				for(int i = 0; i < num; i++) {
					if(curr->value == keys[i]) {
						s_node->mtx.lock();
						temp = curr;
						curr = curr->next;
						results[i] = true;
						printf("REMOVE %d\n",temp->value);
						s_node->mtx.unlock();
					} else {
						results[i] = false;
						s_node->mtx.unlock();
					}
				}
				curr = curr->next;
			}
		} else {
			for(int i = 0; i < num; i++) {
				results[i] = false;
			}
		}
	}

	/// return true if *key* is present in the list, false otherwise
	void lookup(int* keys, bool* results, int num)
	{
		Node* curr_node = new Node;
		curr_node = curr;
		curr = head;

		while(curr != NULL) 
		{
			printf("HERE Lookup %d and key is %d\n", curr->value, keys[num]);
			for(int i = 0; i < num; i++) {
				if(curr->value == keys[i]) {
					s_node->mtx.lock();
					temp = new Node;
					temp->value = keys[i];
					if(temp->value == keys[i]) {
						results[i] = true;
					} else {
						results[i] = false;
					}
					s_node->mtx.unlock();
				}
			}
			
			curr = curr->next;
		}
	}

	//The following are not tested by the given tester but are required for grading
	//No locks are required for these.

	//This refers to the number of buckets not the total number of elements.
	size_t getSize() const
	{
		return 0;
	}

	//This refers to the number of elements in a bucket, not the sentinel node.
	size_t getBucketSize() const
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
