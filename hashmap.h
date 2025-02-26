#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dynamic_arena.h"

#define HASHMAP_DEFAULT_CAPACITY 4
#define HASHMAP_RESIZE_FACTOR 2
#define HASHMAP_LOAD_FACTOR 0.5

size_t hash(char* str) {
    /* djb2 hash */
    size_t hash = 5381;
    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */        
    }
    return hash;
}

typedef struct HashMapCell {
    char* key;
    size_t value_loc; /* location in arena */
} HashMapCell;

typedef struct HashMap {
    HashMapCell* map;
    DynamicArena* arena; 
    size_t len;
    size_t capacity;
} HashMap;

HashMap hm_new_with_capacity(size_t element_size, size_t capacity) {
    DynamicArena* arena = da_new_with_capacity(element_size, capacity);
    HashMapCell* map = calloc(capacity, sizeof(HashMapCell));

    HashMap hm = { .map = map, .arena = arena, .len = 0, .capacity = capacity };

    return hm;
}

HashMap hm_new(size_t element_size) {
    HashMap hm = hm_new_with_capacity(element_size, HASHMAP_DEFAULT_CAPACITY);
    return hm;
}

size_t _linear_probe(HashMapCell* map, size_t capacity, char* key) {
    /* please pretty please pass in a null-terminated string for key */
    size_t index = hash(key) % capacity;
    while (1) {
        if (map[index].key == NULL || !strcmp(key, map[index].key)) {
            return index;
        }
        index = (index + 1) % capacity;
    }
    /* 
    POTENTIAL BUG: if the map is full and the key is not inside, the loop doesn't terminate. Although if we do the load factor thing right, we shouldn't have that problem
    */
}

void _hm_resize(HashMap* hm, size_t new_size) {
    HashMapCell* new_map = calloc(new_size, sizeof(HashMapCell));
    size_t index;

    for (int i = 0; i < hm->capacity; i++) {
        if (hm->map[i].key != NULL) {
            index = _linear_probe(new_map, new_size, hm->map[i].key);
            new_map[index] = hm->map[i];
        }
    }
    free(hm->map);
    hm->map = new_map;
    hm->capacity = new_size;
}

char* hm_get_key(HashMap* hm, size_t index) {
    if (index >= 0 && index < hm->capacity) {
        return hm->map[index].key; 
    } 
    return NULL;
}

void* hm_get(HashMap* hm, char* key) {
    size_t index = _linear_probe(hm->map, hm->capacity, key);

    if (hm->map[index].key == NULL) {
        return NULL;
    } else {
        return da_get(hm->arena, hm->map[index].value_loc);
    }
}

void hm_insert(HashMap* hm, char* key, void* element) { 
    size_t index = _linear_probe(hm->map, hm->capacity, key);

    if (hm->map[index].key == NULL) {
        if (hm->len + 1 > hm->capacity * HASHMAP_LOAD_FACTOR) {
            _hm_resize(hm, hm->capacity * HASHMAP_RESIZE_FACTOR);  
        }
        index = _linear_probe(hm->map, hm->capacity, key);

        /* element is not in HashMap */ 
        hm->map[index].key = malloc(strlen(key) + 1);
        strcpy(hm->map[index].key, key);
        hm->len += 1;
    } 
    hm->map[index].value_loc = da_push(hm->arena, element); 
}

void hm_remove(HashMap* hm, char* key) {
    size_t current_index = _linear_probe(hm->map, hm->capacity, key);
    size_t search_index;
    size_t hash_index;
    
    if (hm->map[current_index].key == NULL) {
        /* trying to remove non-existent key => do nothing */
        return;
    } 


    hm->len -= 1;
    if (hm->len - 1 < hm->capacity * HASHMAP_LOAD_FACTOR * HASHMAP_LOAD_FACTOR) {
        _hm_resize(hm, hm->capacity / HASHMAP_RESIZE_FACTOR); 
        current_index = _linear_probe(hm->map, hm->capacity, key);
    } 

    search_index = current_index;
    free(hm->map[current_index].key);
    hm->map[current_index].key = NULL;

    while (1) {
        search_index = (search_index + 1) % hm->capacity;
        if (hm->map[search_index].key != NULL) {
            hash_index = hash(hm->map[search_index].key) % hm->capacity;
            if (hash_index <= current_index) {
                hm->map[current_index] = hm->map[search_index];
                hm->map[search_index].key = NULL;
                current_index = search_index;
            }
        } else {
            break;
        }
    }
}

void hm_free(HashMap* hm) {
    da_free(hm->arena);
    free(hm->map);
    free(hm);
}
#endif

