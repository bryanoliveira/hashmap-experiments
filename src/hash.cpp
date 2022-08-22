#include <iostream>
#include <stdexcept>

#include "hash.hpp"
#include "logging.hpp"

#define PROBE_PREP(key, k)                                                     \
    uint hk = hash(key);                                                       \
    k = hk;                                                                    \
    uint i = 1;
#ifdef LINEAR_PROBING
#define PROBE_STEP(k)                                                          \
    k = (hk + i) % max_n;                                                      \
    i++;
#else // quadratic probing
#define PROBE_STEP(k)                                                          \
    k = (hk + i * i) % max_n;                                                  \
    i++;
#endif

#define TKV template <typename TK, typename TV>

TKV HashMap<TK, TV>::HashMap(const uint max_n) {
    log("Instantiating HashMap expecting max of ", max_n, " elements...");
#ifdef LINEAR_PROBING
    log("Using linear probing.");
#else // linear probing
    log("Using quadratic probing.");
#endif
    HashMap::max_n = max_n;
    HashMap::table = new HashNode[max_n];
}

TKV HashMap<TK, TV>::~HashMap() { delete[] HashMap::table; }

TKV uint HashMap<TK, TV>::get_size() { return HashMap::n; }

TKV uint HashMap<TK, TV>::get_comparisons() { return HashMap::comparisons; }

TKV void HashMap<TK, TV>::reset_comparisons() { HashMap::comparisons = 0; }

TKV float HashMap<TK, TV>::get_load_factor() {
    return HashMap::n / (float)HashMap::max_n;
}

TKV uint HashMap<TK, TV>::hash(const TK key) { return key % max_n; }

TKV TV &HashMap<TK, TV>::operator[](const TK key) { return find(key); }

TKV TV &HashMap<TK, TV>::find(const TK key) {
    uint k;
    PROBE_PREP(key, k);
    HashMap::comparisons++;
    while (table[k].status != FREE) {
        if (table[k].status == OCCUPIED && table[k].key == key)
            return table[k].value;

        HashMap::comparisons++;
        PROBE_STEP(k);
    }

    throw std::invalid_argument("Key not found!");
}

TKV void HashMap<TK, TV>::insert(const TK key, const TV value) {
    if (HashMap::n == HashMap::max_n)
        throw std::length_error("Cannot insert! HashMap is full!");

    uint k;
    PROBE_PREP(key, k);
    HashMap::comparisons++;
    while (table[k].status == OCCUPIED) {
        HashMap::comparisons++;
        PROBE_STEP(k);
    }

    table[k].status = OCCUPIED;
    table[k].key = key;
    table[k].value = value;
    n++;
}

TKV void HashMap<TK, TV>::remove(const TK key) {
    uint k;
    PROBE_PREP(key, k);
    HashMap::comparisons++;
    while (table[k].status != FREE) {
        if (table[k].status == OCCUPIED && table[k].key == key) {
            table[k].status = DELETED;
            n--;
            return;
        }

        HashMap::comparisons++;
        PROBE_STEP(k);
    }

    throw std::invalid_argument("Key not found!");
}

// instantiate type combinations that are expected to work
template class HashMap<int, int>;
template class HashMap<int, float>;
template class HashMap<int, std::string>;