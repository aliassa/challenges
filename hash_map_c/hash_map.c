#include "hash_map.h"

HashMap* hm_create(size_t capacity) {
    if(capacity == 0) return NULL;

    HashMap* hm = malloc(sizeof *hm);
    if(!hm) return NULL;

    hm->buckets = calloc(capacity, sizeof(entry_t*));
    if(!hm->buckets) {
        free(hm);
        return NULL;
    }
    hm->size = capacity;
    hm->count = 0;
    return hm;
}

static void list_free(entry_t* head) {
    entry_t* curr = head;
    while (curr != NULL)
    {
        entry_t* tmp = curr;
        curr = curr->next;
        free((char*)tmp->key);
        free(tmp);
    }
}

// djb2 hash
static uint32_t hm_hash(size_t size, const char* key) {
    uint32_t hash = 5381;
    while (*key) {
        hash = ((hash << 5) + hash) + (uint8_t)(*key);
        key++;
    }
    return hash % size;
}

static entry_t* list_add(entry_t* head, const char* key, void* value) {
    entry_t* curr = head;
    // update if key already exists
    while(curr != NULL) {
        if(strcmp(curr->key, key) == 0) {
            curr->value = value;
            return head;
        }
        curr = curr->next;
    }
    // key not found
    entry_t* new_entry = malloc(sizeof *new_entry);
    if(!new_entry) return head;
    // prepend (simpler and faster)
    new_entry->key = strdup(key);
    if(!new_entry->key) { free(new_entry); return head; }
    new_entry->value = value;
    new_entry->next = head;

    return new_entry;
}

void hm_destroy(HashMap* hm) {
    if(!hm) return;
    for(size_t i = 0; i < hm->size; i++) {
        list_free(hm->buckets[i]);
    }
    free(hm->buckets);
    free(hm);
}


void hm_set(HashMap* hm, const char* key, void* value) {
    if(!hm || !key) return;

    // grow if load factor exceeded
    if((float)hm->count / (float)hm->size > HM_LOAD_FACTOR)
        hm_resize(hm);

    uint32_t idx      = hm_hash(hm->size, key);
    entry_t* new_head = list_add(hm->buckets[idx], key, value);
    if(new_head != hm->buckets[idx]) {
        hm->buckets[idx] = new_head;
        hm->count++;
    }
}

static entry_t* list_get_by_key(entry_t* head, const char* key) {
    entry_t* curr = head;
    while(curr != NULL) {
        if(strcmp(curr->key, key) == 0) return curr;
        curr = curr->next;
    }
    return NULL;
}

static entry_t* list_delete_by_key(entry_t* head, const char* key, int* deleted) {
    *deleted = 0;
    if(!head || !key) return NULL;

    entry_t* curr = head;
    entry_t* prev = NULL;

    while (curr != NULL)
    {
        if(strcmp(curr->key, key) ==0) {
            if(prev == NULL) head = curr->next; // delete head
            else prev->next = curr->next;

            free((char*)curr->key);
            // Caller owns the value, we do not free it
            free(curr);
            *deleted = 1;
            return head;
        }
        prev = curr;
        curr = curr->next;
    }
    return head; // key not found, head unchanged
}

void* hm_get(HashMap* hm, const char* key) {
    if(!hm || !key) return NULL;

    uint32_t idx = hm_hash(hm->size, key);
    entry_t* entry = list_get_by_key(hm->buckets[idx], key);
    if(!entry) return NULL;
    return entry->value;
}

// Removes a key-value pair
int hm_delete(HashMap* hm, const char* key) {
    if(!hm || !key || hm->count == 0) return -1;
    
    uint32_t idx = hm_hash(hm->size, key);
    int deleted = 0;
    hm->buckets[idx] = list_delete_by_key(hm->buckets[idx], key, &deleted);
    if(deleted) hm->count--;
    return deleted ? 0 : -1;
}

int hm_has(HashMap* hm, const char* key) {
    return hm_get(hm, key) != NULL;
}

char** hm_keys(HashMap* hm) {
    if(!hm) return NULL;

    char** keys = malloc(hm->count * sizeof(char*));
    if(!keys) return NULL;
    size_t idx = 0;

    for(size_t i = 0; i < hm->size; i++) {
        entry_t* curr = hm->buckets[i];
        while(curr != NULL) {
            keys[idx++] = (char*)curr->key;
            curr = curr->next;
        }
    }
    return keys;
}

void hm_resize(HashMap* hm) {
    if(!hm) return;

    size_t new_size      = hm->size * 2;
    entry_t** new_buckets = calloc(new_size, sizeof(entry_t*));
    if(!new_buckets) return;

    for(size_t i = 0; i < hm->size; i++) {
        entry_t* curr = hm->buckets[i];
        while(curr != NULL) {
            entry_t* next = curr->next;
            uint32_t new_idx = hm_hash(new_size, curr->key);
            curr->next = new_buckets[new_idx];
            new_buckets[new_idx] = curr;
            curr = next;
        }
    }

    free(hm->buckets);
    hm->buckets = new_buckets;
    hm->size    = new_size;
}

void hm_print(HashMap* hm, print_func p) {
    if(!hm) return;

    for(size_t i = 0; i < hm->size; i++) {
        printf("[%zu] -> ", i);
        entry_t* curr = hm->buckets[i];
        while (curr != NULL)
        {
            if(p) {
                printf("(%s: ", curr->key);
                p(curr->value);
                printf(") -> ");
            }
            else printf("(%s) -> ", curr->key);
            curr = curr->next;
        }
        printf("NULL\n");
    }
}
