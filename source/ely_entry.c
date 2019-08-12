//
//  ely_entry.c
//  FastParser
//
//  Created by Nafisa Ali Amir on 26/06/2019.
//  Copyright © 2019 Fasana. All rights reserved.
//

#include "ely_entry.h"

Entry* create_entry (int id, Rule* rule, int column_index, int lookup_col_index) {
    Entry* new_entry = (Entry *) malloc (sizeof(Entry));
    
    new_entry->_entry_index = id;
    new_entry->_column_index = column_index;
    new_entry->_dot_pos = 0;
    new_entry->_lookup_col_index = lookup_col_index;
    new_entry->_gr_rule = rule;
    new_entry->_score = 0;
    new_entry->_is_completed = False;
    
    for (int i = 0; i < rule->rhs->num_of_rhs; i++) {
        new_entry->_bktrack_indices[i] = -1;
    }
    
    return (new_entry);
}

void set_dot_index (Entry* entry, int dot_index) {
    entry->_dot_pos = dot_index;
    if (dot_index == entry->_gr_rule->rhs->num_of_rhs) {
        entry->_is_completed = True;
    }
}

int get_dot_index (Entry* entry) {
    return entry->_dot_pos;
}

int* get_bktrack(Entry* entry) {
    return entry->_bktrack_indices;
}

void add_bktrack(Entry* dest_entry, Entry* src_entry, int bktrack_id) {
    int dot_pos = get_dot_index(src_entry);
    for (int i = 0; i < dot_pos; i++) {
        dest_entry->_bktrack_indices[i] = src_entry->_bktrack_indices[i];
    }
    if (bktrack_id >= 0) {
        dest_entry->_bktrack_indices[dot_pos] = bktrack_id;
    }
}

int get_id (Entry* entry) {
    return entry->_entry_index;
}

void set_id (Entry* entry, int id) {
    entry->_entry_index = id;
}

Rule* get_rule (Entry* entry) {
    return entry->_gr_rule;
}

char* get_next_rhs (Entry* entry) {
    int dot_pos = get_dot_index(entry);
    if (dot_pos >= entry->_gr_rule->rhs->num_of_rhs) {
        return NULL;
    }
    return entry->_gr_rule->rhs->key[dot_pos];
}

int get_col_index (Entry* entry) {
    return entry->_column_index;
}

bool is_completed (Entry* entry) {
    return entry->_is_completed;
}

int get_lookup_col_index (Entry* entry) {
    return entry->_lookup_col_index;
}

int get_score (Entry* entry) {
    return entry->_score;
}

int add_score_by (Entry* entry, int score_update) {
    entry->_score += score_update;
    return entry->_score;
}

void destroy_entry (Entry* entry) {
    free (entry);
}
