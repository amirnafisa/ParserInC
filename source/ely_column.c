//
//  ely_column.c
//  FastParser
//
//  Created by Nafisa Ali Amir on 26/06/2019.
//  Copyright © 2019 Fasana. All rights reserved.
//

#include "ely_column.h"

Column* create_column (int id) {
    Column* new_col = (Column *) malloc (sizeof(Column));
    new_col->_entry = (Entry **) malloc (MAX_NUM_OF_COL_ENTRIES*sizeof(Entry *));
    new_col->_id = id;
    new_col->_num_of_entries = 0;
    return new_col;
}

int entry_exists(Column* column, Entry* entry) {
    for (int i = 0; i < column->_num_of_entries; i++) {
        if (get_rule(column->_entry[i]) == get_rule(entry) &&
            get_dot_index(column->_entry[i]) == get_dot_index(entry) &&
            get_lookup_col_index(column->_entry[i]) == get_lookup_col_index(entry)) {
            return i;
        }
    }
    return -1;
}

int add_entry(Column* column, Entry* entry) {
    int idx = entry_exists(column, entry);
    if (idx == -1) {
        column->_entry[column->_num_of_entries] = entry;
        column->_num_of_entries++;
        return 1;
    } else if (get_score(column->_entry[idx]) < get_score(entry)) {
        return -1;
    } else {
        set_id(entry, get_id(column->_entry[idx]));
        column->_entry[idx] = entry;
        return 0;
    }
}

int get_column_id (Column* column) {
    return column->_id;
}

Entry* get_entry_at_index(Column* column, int idx) {
    if (idx >= column->_num_of_entries) {
        return NULL;
    }
    return column->_entry[idx];
}

void destroy_column (Column* column) {
    free(column);
}
