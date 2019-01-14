#ifndef _HASH_H_
#define _HASH_H_

#include <array>
#include <cstdio>

template <typename hashType, typename valType, std::size_t N> 
class hash {
public:
	hash() {}
	template <class hashIter, class valIter>
	void create(hashIter hashStart, hashIter hashEnd, valIter valStart, valIter valEnd) {
		if(hashEnd - hashStart != N || valEnd - valStart != N) {
			printf("MUST PROVIDE EXACTLY %d KEYS AND VALUES!!!\n", N);
			exit(1);
		}
		binarytreeInd = size;
		while(hashStart != hashEnd && valStart != valEnd) {
			addPair(*hashStart, *valStart);
			++valStart;
			++hashStart;
		}
	}

	void addPair(hashType key, valType value) {
		int ind =  hashFunc(key) % size;
		if(!table[ind].occupied) {
			table[ind].key = key;
			table[ind].value = value;
			table[ind].occupied = true;
		} else {
			binarytreeAdd(key, value, ind);
		}
	}

	valType access(hashType key) {
		int ind = hashFunc(key) % size;
		valType value = table[ind].value;
		if(key != table[ind].key) {
			value = binarytreeSearch(key, ind);
		}
		return value;
	}
private:
	static constexpr std::size_t size = N * 4;
	static constexpr std::size_t total_size = size + N;
	std::hash<hashType> hashFunc;
	int binarytreeInd;
	bool error;
	struct node {
		hashType key;
		valType value;
		int rightInd = 0, leftInd = 0;
		bool occupied = false;
	};
	std::array<node, total_size> table;

	void binarytreeAdd(hashType key, valType value, int root) {
		node *current = &table[root];
		bool traverse = true;
		while(traverse) {
			if(key == current->key) {
				return;
			} else if(key < current->key) {
				if(current->leftInd) {
					current = &table[current->leftInd];
				} else {
					current->leftInd = binarytreeInd;
					traverse = false;
				}
			} else {
				if(current->rightInd) {
					current = &table[current->leftInd];
				} else {
					current->rightInd = binarytreeInd;
					traverse = false;
				}
			}
		}
		node *leaf = &table[binarytreeInd];
		leaf->occupied = true;
		leaf->key = key;
		leaf->value = value;
		++binarytreeInd;
	}

	valType binarytreeSearch(hashType key, int root) {
		error = false;
		node current = table[root];
		while(key != current.key) {
			if(key < current.key) {
				if(!current.leftInd) {
					error = true;
					break;
				} else {
					current = table[current.leftInd];
				}
			} else {
				if(!current.rightInd) {
					error = true;
					break;
				} else {
					current = table[current.rightInd];
				}
			}
		}
		return current.value;
	}
};

#endif