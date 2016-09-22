#ifndef FIXED_HASH_MAP_HPP
#define FIXED_HASH_MAP_HPP

#include <iostream>
#include <string>
#include <functional>
#include <sstream>
/*

Using only primitive types, implement a fixed-size hash map that associates string keys with arbitrary data object
references (you don't need to copy the object). Your data structure should be optimized for algorithmic runtime and
memory usage. You should not import any external libraries, and may not use primitive hash map or dictionary types
in languages like Python or Ruby.

The solution should be delivered in one class (or your language's equivalent) that provides the following functions:

constructor(size): return an instance of the class with pre-allocated space for the given number of objects.

boolean set(key, value): stores the given key/value pair in the hash map.
                         Returns a boolean value indicating success / failure of the operation.
get(key): return the value associated with the given key, or null if no value is set.

delete(key): delete the value associated with the given key, returning the value on success or null if
             the key has no value.

float load(): return a float value representing the load factor (`(items in hash map)/(size of hash map)`) of the
              data structure. Since the size of the dat structure is fixed, this should never be greater than 1.
*/

template<typename T>
class Node {
public:
    std::string key;
    T* value;
    Node<T>* next;

    Node<T>()                         : next{nullptr}{}
    Node<T>(const Node& node)         : key{node.key}, value{node.value}, next{node.next}{}
    Node<T>(std::string k, T* v, Node<T>* n = nullptr) : key{k}, value{v}, next{n}{}
};


template<typename T> // template for value types
class FixedHashMap {
public:
    // Constructor
    FixedHashMap(int size);

    // "Big Three", since we're using a dynamically allocated array
    // we don't care about no move constructors
    FixedHashMap(const FixedHashMap<T>& hm);            // copy constructor
    ~FixedHashMap();                                    // destructor
    FixedHashMap& operator=(const FixedHashMap<T>& hm); // copy assignment

    // Public methods
    bool set(const std::string& key, T* value);
    T* get(const std::string& key);
    T* remove(const std::string& key); // delete is a keyword in C++; declare as remove(key) instead.
    float load();

    // not required
    unsigned int maxSize() { return mapSize; }
    unsigned int size() { return numObjects; }
    bool contains(const std::string& key);
    void printMap();

private:
    // We're using a dynamically allocated array as the data structure for our hashmap,
    // resolving collisions with linear probing
    int mapSize;
    int numObjects;
    Node<T>** map;

    unsigned int hash(const std::string& key);
    Node<T>* findKey(int bin, const std::string& key);
    Node<T>** makeBins(int num);
    Node<T>** copyBins(Node<T>** ht, int bins);
    void deleteBins(Node<T>**& ht, int bins);
};

// Implementation

// Constructor
template<typename T>
FixedHashMap<T>::FixedHashMap(int size)
            : mapSize{size}, numObjects{0}
{
    map = makeBins(mapSize);
}

// Big Three
template<typename T>
FixedHashMap<T>::FixedHashMap(const FixedHashMap<T>& hm)
            : mapSize{hm.mapSize}, numObjects{hm.numObjects}
{
    map = copyBins(hm.map, mapSize);
}

template<typename T>
FixedHashMap<T>::~FixedHashMap()
{
    deleteBins(map, mapSize);
}

template<typename T>
FixedHashMap<T>& FixedHashMap<T>::operator=(const FixedHashMap<T>& hm)
{
    if(this != &hm) {
        deleteBins(map, mapSize);
        mapSize = hm.mapSize;
        numObjects = hm.numObjects;
        map = copyBins(hm.map, mapSize);
    }
    return *this;
}

// Public methods
template<typename T>
bool FixedHashMap<T>::set(const std::string& key, T* value)
{
    if (mapSize == numObjects) {
        return false;
    }
    int h = hash(key);
    Node<T>* n = findKey(h, key);
    if (n == nullptr) {
        ++numObjects;
        map[h] = new Node<T>(key, value, map[h]);
    }
    else {
        n->value = value;
    }
    return true;
}

template<typename T>
T* FixedHashMap<T>::get(const std::string& key)
{
    Node<T>* n = findKey(hash(key), key);
    if (n == nullptr) {
        return nullptr;
    }
    return n->value;
}

template<typename T>
T* FixedHashMap<T>::remove(const std::string& key)
{
    Node<T>* n = findKey(hash(key), key);
    if (n == nullptr) {
        return nullptr;
    }
    --numObjects;
    T* val = n->value;
	Node<T>* next = n->next;
	*n = *next;
	delete next;
    return val;
}

template<typename T>
float FixedHashMap<T>::load()
{
    if (numObjects == 0) {
        return 0;   // handle div by 0 case
    }
    // to get a float from int/int division, at least one of the ints
    // needs to be casted to a float/double
    return static_cast<float>(numObjects)/mapSize;
}

template<typename T>
bool FixedHashMap<T>::contains(const std::string& key)
{
    return findKey(hash(key), key) != nullptr;
}

template<typename T>
void FixedHashMap<T>::printMap() {
    std::ostringstream ss;
    Node<T>* n;
    if (map == nullptr)
        ss << "nullptr" << std::endl;
    else {
        for (int i = 0; i < mapSize; ++i) {
            ss << "bin[" << i << "]:";
            n = map[i];
            while(n->next != nullptr) {
                ss << n << "[" << n->key << "->" << n->value << "]->";
                n = n->next;
            }
            ss << n << "[TRAILER]" << std::endl;
        }
    }
    std::cout << ss.str();
}

// Private methods
template<typename T>
unsigned int FixedHashMap<T>::hash(const std::string& key) {
    // FNV-1a hash algorithm from http://isthe.com/chongo/tech/comp/fnv/
    unsigned int h = 2166136261;
    for(char c : key) {
        h = h^c; // XOR
        h = h * 16777619;
    }
    return h % mapSize;
    // const std::hash<std::string> hash_fn;
    // return hash_fn(key) % mapSize;
}

template<typename T>
Node<T>** FixedHashMap<T>::makeBins(int num) {
    Node<T>** ret = new Node<T>*[num];
    for (int i = 0; i < num; ++i) {
        ret[i] = new Node<T>();
    }
    return ret;
}

template<typename T>
Node<T>** FixedHashMap<T>::copyBins(Node<T>** ht, int bins) {
    if (ht == nullptr) {
        return nullptr;
    }
    Node<T>** hm = new Node<T>*[bins];
    for(int i = 0; i < bins; ++i) {
        Node<T>* n = nullptr;
        Node<T>* l = ht[i];
        if (l != nullptr) {
            Node<T>* l = ht[i];
            n = new Node<T>();
            while (l->next != nullptr) {
            	n = new Node<T>(l->key, l->value, n);
            	l = l->next;
            }
        }
        hm[i] = n;
    }
    return hm;
}

template<typename T>
void FixedHashMap<T>::deleteBins(Node<T>**& ht, int bins) {
	if (ht == nullptr)
		return;
	for (int i = 0; i < bins; ++i) {
		Node<T>* n = ht[i];
		Node<T>* tmp;
		while (n != nullptr) {
			tmp = n->next;
			delete n;
			n = tmp;
		}
		ht[i] = nullptr;
	}
	delete[] ht;
}

template<typename T>
Node<T>* FixedHashMap<T>::findKey(int bin, const std::string& key) {
    if(map == nullptr || bin >= mapSize)
        return nullptr;
    Node<T>* n = map[bin];
    while (n->next != nullptr) {
        if (n->key == key)
            return n;
        n = n->next;
    }
    return nullptr;
}



#endif // ifndef FIXED_HASH_MAP_HPP
