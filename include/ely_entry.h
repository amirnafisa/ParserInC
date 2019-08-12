//
//  ely_entry.h
//  FastParser
//
//  Created by Nafisa Ali Amir on 26/06/2019.
//  Copyright © 2019 Fasana. All rights reserved.
//

#ifndef ely_entry_h
#define ely_entry_h

#include <stdio.h>
#include <stdbool.h>
#include "grammar.h"
#include "general.h"

typedef struct {
    int _entry_index;
    int _column_index;
    int _lookup_col_index;
    int _dot_pos;
    int _score;
    int _bktrack_indices[MAX_NUM_OF_RHS];
    bool _is_completed;
    Rule* _gr_rule;
} Entry;

Entry* create_entry (int id, Rule* rule, int column_index, int lookup_col_index);
void set_dot_index (Entry* entry, int dot_index);
int get_dot_index (Entry* entry);
int* get_bktrack(Entry* entry);
void add_bktrack(Entry* dest_entry, Entry* src_entry, int bktrack_id);
int get_id (Entry* entry);
void set_id (Entry* entry, int id);
Rule* get_rule (Entry* entry);
char* get_next_rhs (Entry* entry);
int get_col_index (Entry* entry);
bool is_completed (Entry* entry);
int get_lookup_col_index (Entry* entry);
int get_score (Entry* entry);
int add_score_by (Entry* entry, int score_update);
void destroy_entry (Entry* entry);

#endif /* ely_entry_h */
