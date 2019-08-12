//
//  speedup.h
//  FastParser
//
//  Created by Nafisa Ali Amir on 27/06/2019.
//  Copyright © 2019 Fasana. All rights reserved.
//

#ifndef speedup_h
#define speedup_h

#include <stdio.h>
#include <stdbool.h>
#include "grammar.h"
#include "sentence.h"
#include "general.h"

typedef struct {
    char* key0;
    char* key1;
    Rule** rule;
    int num_of_rules;
}RColumn;


typedef struct {
    RColumn** rcolumn;
    int num_of_columns;
}RTable;

typedef struct {
    char* key_rhs;
    char** entry_lhs;
    int num_of_entries;
}LParentColumn;

typedef struct {
    LParentColumn** lparent_col;
    int num_of_columns;
}LParentTable;

typedef struct{
    char* key_lhs;
    char** entry_rhs;
    int num_of_entries;
} LAncestorColumn;

typedef struct {
    LAncestorColumn** lancestor_col;
    int num_of_columns;
}LAncestorTable;

typedef struct {
    char** token;
    int num_of_tokens;
}LookedAt;

bool do_discard(int cur_col_idx, int n_tokens, int score);
void filter_grammar (Grammar* gr, Sentence* sentence);
RTable* extract_RTable(Grammar* gr);
int get_index_rtable(RTable* rtable, char* non_terminal, char* rhs);
void destroy_RColumn (RColumn* rcolumn);
void destroy_RTable (RTable* rtable);
LParentTable* extract_LParentTable (Grammar* gr);
void destroy_LParentColumn (LParentColumn* lparent_col);
void destroy_LParentTable (LParentTable* lptable);
LParentColumn* get_lpcolumn_at (LParentTable* lptable, char* token);
void recurse (char* non_term, LParentColumn* lpcolumn, LParentTable* lptable, LAncestorTable* latable, LookedAt* prev_looked_at);
LAncestorTable* extract_LAncestors (Grammar* gr, LParentTable* lptable);
void destroy_LAncestorColumn (LAncestorColumn* lancestor_col);
void destroy_LAncestorTable (LAncestorTable* latable);

#endif /* speedup_h */
