//
//  ely_column.h
//  FastParser
//
//  Created by Nafisa Ali Amir on 26/06/2019.
//  Copyright © 2019 Fasana. All rights reserved.
//

#ifndef ely_column_h
#define ely_column_h

#include <stdio.h>
#include "grammar.h"
#include "ely_entry.h"

#define MAX_NUM_OF_COL_ENTRIES  50000

typedef struct {
    int _id;
    Entry** _entry;
    int _num_of_entries;
} Column;

Column* create_column (int id);
int entry_exists(Column* column, Entry* entry);
int add_entry(Column* column, Entry* entry);
int get_column_id (Column* column);
Entry* get_entry_at_index(Column* column, int idx);
void destroy_column (Column* column);

#endif /* ely_column_h */
