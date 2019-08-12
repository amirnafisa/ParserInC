//
//  ely_chart.h
//  FastParser
//
//  Created by Nafisa Ali Amir on 27/06/2019.
//  Copyright © 2019 Fasana. All rights reserved.
//

#ifndef ely_chart_h
#define ely_chart_h

#include <stdio.h>
#include "ely_column.h"
#include "ely_entry.h"
#include "sentence.h"
#include "speedup.h"

#define MAX_NUM_OF_ENTRIES  1000000
#define MAX_SIZE_OF_PARSE   1000
typedef struct {
    int _cur_col_idx;
    int _cur_entry_idx;
    Column* _cur_col;
    Column** _ELY;
    Entry** _nELY;
    int _num_of_cols;
    char* _root;
    char** _token;
    int _num_of_tokens;
    Grammar* _pcfg;
    bool speedup;
    RTable* rtable;
    LAncestorTable* latable;
} Chart;

Chart* create_chart (Sentence* sentence, Grammar* gr, bool speedup, RTable* rtable, LAncestorTable* latable);
char* get_current_token (Chart* chart);
void add_entry_to_chart (Chart* chart, Column* column, Entry* entry);
void predict (Chart* chart, char* non_terminal);
void scan (Chart* chart, Entry* entry);
void complete (Chart* chart, Entry* completed_entry, bool speedup);
Column** get_chart(Chart* chart);
Column* get_column (Chart* chart, int col_idx);
void set_cur_col_idx (Chart* chart, int col_idx);
int get_cur_col_idx (Chart* chart);
void print_chart (Chart* chart);
bool recurse_print (Chart* chart, int id, char* output_str);
void print_parse (Chart* chart, char* output_str);
int get_num_of_elychart_tokens(Chart* chart);
void destroy_chart (Chart* chart);

#endif /* ely_chart_h */
