#ifndef SNODE_H
#define SNODE_H

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#define MAX_ATOM_SIZE 128

enum SNodeType {
    ATOM, 
    LIST,
};

typedef struct SNode SNode;
typedef struct SNode {
    enum SNodeType type;
    SNode* next;
    union {
        void* atom;
        SNode* list;
    };
} SNode;

SNode* sn_new_empty() {
    SNode* snode = malloc(sizeof(SNode));
    *snode = (SNode) { .type = LIST, .list = NULL, .next = NULL };
    return snode;
}

SNode* sn_new_atom(void* atom) {
    SNode* snode = malloc(sizeof(SNode));
    *snode = (SNode) { .type = ATOM, .atom = atom, .next = NULL };
    return snode;
}

SNode* sn_traverse_to_end(SNode* start) {
    SNode* last = start;
    while (last->next != NULL) {
        last = last->next;
    }
    return last;
}

void sn_push_into(SNode* into, SNode* snode) {
    assert(into->type == LIST);
    if (into->list == NULL) {
        into->list = snode;    
    } else {
        SNode* last = sn_traverse_to_end(into->list);
        last->next = snode;
    }
}

SNode* _sn_deep_copy_recursive(SNode* snode) {
    SNode* snode_copy = malloc(sizeof(SNode));
    if (snode->type == ATOM) {
        *snode_copy = (SNode) { .type = ATOM, .atom = snode->atom };
        if (snode->next == NULL) {
            snode_copy->next = NULL; 
        } else {
            snode_copy->next = _sn_deep_copy_recursive(snode->next);
        }
    } else {
        *snode_copy = (SNode) { .type = LIST };
        if (snode->next == NULL) {
            snode_copy->next = NULL;
        } else {
            snode_copy->next = _sn_deep_copy_recursive(snode->next);
        }
        if (snode->list == NULL) {
            snode_copy->list = NULL;
        } else {
            snode_copy->list = _sn_deep_copy_recursive(snode->list);
        }
    }
    return snode_copy;
}

SNode* sn_deep_copy(SNode* snode) {
/* We need to traverse and for each create a deepcopy */
    SNode* snode_copy = malloc(sizeof(SNode));
    if (snode->type == ATOM) {
        *snode_copy = (SNode) { .type = ATOM, .next = NULL, .atom = snode->atom };
    } else {
        *snode_copy = (SNode) { .type = LIST, .next = NULL, .list = NULL };
        if (snode->list != NULL) {
            snode_copy->list = _sn_deep_copy_recursive(snode->list);
        } 
    }
    return snode_copy;
}

void sn_free_recursive(SNode* snode) {
    if (snode->type == ATOM) {
        free(snode->atom);    
    } else {
        if (snode->list != NULL) {
            sn_free_recursive(snode->list);
        }
    } 
    if (snode->next != NULL) {
        sn_free_recursive(snode->next);
    }
    free(snode);
}

// void sn_free(SNode snode) {
//     if (snode.type == ATOM) {
//         free(snode.atom);    
//     } else {
//         if (snode.list != NULL) {
//             sn_free_recursive(snode.list);
//         }
//     } 
//     if (snode.next != NULL) {
//         sn_free_recursive(snode.next);
//     }
// }

void debug_char_atom(void* atom) {
    printf("%s", (char*) atom); 
}

void _sn_debug_recursive(SNode* snode, void (*debug_fptr)(void*)) {
    if (snode->type == ATOM) {
        debug_fptr(snode->atom);
    } else {
        printf("(");
        if (snode->list != NULL) {
            _sn_debug_recursive(snode->list, debug_fptr);
        }
        printf(")");
    }
    if (snode->next != NULL) {
        printf(" ");
        _sn_debug_recursive(snode->next, debug_fptr);
    }
}

void sn_debug(SNode* snode, void (*debug_fptr)(void*)) { 
    if (snode->type == ATOM) {
        debug_fptr(snode->atom);
    } else {
        printf("(");
        if (snode->list != NULL) {
            _sn_debug_recursive(snode->list, debug_fptr);
        }
        printf(")");
    }
}

int sn_list_len(SNode* snode) {
    assert(snode->type == LIST);

    int len = 0;
    SNode* current = snode->list;
    while (current != NULL) {
        current = current->next;
        len += 1;
    }
    return len;
}

int sn_next_count(SNode* snode) {
    int count = 0;
    SNode* after = snode->next;
    while (after != NULL) {
        after = after->next;
        count += 1;
    }
    return count;
}

int sn_all_atomic(SNode* snode) {
    assert(snode->type == LIST); 
    int all_atomic = 1;
    SNode* current = snode->list;
    while (current != NULL) {
        all_atomic = all_atomic && (current->type == ATOM);
        current = current->next;
    }
    return all_atomic;
}

#endif
