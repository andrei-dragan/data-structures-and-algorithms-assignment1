#include "SMIterator.h"
#include "SortedMap.h"
#include <exception>
using namespace std;

SortedMap::SortedMap(Relation r) {
	this->r = r;
	this->capacity = 1;
	this->length = 0;
	this->elems = new TElem[this->capacity];
} // Theta(1)

TValue SortedMap::add(TKey k, TValue v) {
	// Binary search to check if the key already exists
	int st = 0, fn = this->length - 1;
	int mid = 0;
	while (st <= fn) {
		mid = (st + fn) / 2;
		if (this->elems[mid].first == k) {
			TValue oldValue = this->elems[mid].second;
			this->elems[mid].second = v;
			return oldValue;
		}
		else if (this->r(this->elems[mid].first, k) == true)
			st = mid + 1;
		else if (this->r(this->elems[mid].first, k) == false)
			fn = mid - 1;
	}

	// We first need to check if the array should be increased
	if (this->length == this->capacity) {
		this->capacity *= 2;

		TElem* newArray = new TElem[this->capacity];
		for (int i = 0; i < this->length; i++)
			newArray[i] = this->elems[i];
		delete[] this->elems; 
		this->elems = newArray; 
	}
	
	// We create the element
	TElem elem;
	elem.first = k;
	elem.second = v;

	this->length++;
	this->elems[this->length - 1] = elem; 

	// We place the new element
	int i = this->length - 1; 
	while (this->r(this->elems[i - 1].first, this->elems[i].first) == false && i > 0) {
		TElem elem = this->elems[i];
		this->elems[i] = this->elems[i - 1];
		this->elems[i - 1] = elem; 
		i--;
	}

	return NULL_TVALUE;
} // Best Case: Theta(1), Worst Case: Theta(length) => Total: O(length)

TValue SortedMap::search(TKey k) const {
	// Binary search
	int st = 0, fn = this->length - 1;
	int mid = 0;
	while (st <= fn) {
		mid = (st + fn) / 2;
		if (this->elems[mid].first == k) 
			return this->elems[mid].second;
		else if (this->r(this->elems[mid].first, k) == true)
			st = mid + 1;
		else if (this->r(this->elems[mid].first, k) == false)
			fn = mid - 1;
	}

	return NULL_TVALUE;
} // Best Case: Theta(1), Worst Case: Theta(log(length)) => Total: O(length)

TValue SortedMap::remove(TKey k) {
	
	// Check if we decrease the size
	if (this->length < this->capacity / 4) {
		this->capacity /= 2;

		TElem* newArray = new TElem[this->capacity];
		for (int i = 0; i < this->length; i++)
			newArray[i] = this->elems[i];
		delete[] this->elems;
		this->elems = newArray;
	}
	
	// Remove the element using binary search
	int st = 0, fn = this->length - 1;
	int mid = 0;
	while (st <= fn) {
		mid = (st + fn) / 2;
		if (this->elems[mid].first == k) {
			TValue removedValue = this->elems[mid].second;
			for (int j = mid + 1; j < this->length; j++)
				this->elems[j - 1] = this->elems[j];
			this->length--;
			return removedValue;
		}
		else if (this->r(this->elems[mid].first, k) == true)
			st = mid + 1;
		else if (this->r(this->elems[mid].first, k) == false)
			fn = mid - 1;
	}

	return NULL_TVALUE;
} // Best Case: Theta(log(length)), Worst Case: Theta(length) => Total: O(length)

void SortedMap::replace(TKey k, TValue oldValue, TValue newValue) {
	// Binary search to check if the key exists
	bool found = false;
	int st = 0, fn = this->length - 1;
	int mid = 0;
	while (st <= fn) {
		mid = (st + fn) / 2;
		if (this->elems[mid].first == k) {
			if (this->elems[mid].second != oldValue)
				return;
			found = true;
			break;
		}
		else if (this->r(this->elems[mid].first, k) == true)
			st = mid + 1;
		else if (this->r(this->elems[mid].first, k) == false)
			fn = mid - 1;
	}

	if (found == false)
		return;

	// Put the new value 
	this->elems[mid].second = newValue; 
} // Best Case: Theta(log(length)), Worst Case: Theta(length) => Total: O(length)

int SortedMap::size() const {
	return this->length;
} // Theta(1)

bool SortedMap::isEmpty() const {
	if (this->length == 0)
		return true;
	return false;
} // Theta(1)

SMIterator SortedMap::iterator() const {
	return SMIterator(*this);
} // Theta(1)

SortedMap::~SortedMap() {
	delete[] this->elems;
} // Theta(1)
