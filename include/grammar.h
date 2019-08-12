//
//  grammar.h
//  FastParser
//
//  Created by Nafisa Ali Amir on 26/06/2019.
//  Copyright © 2019 Fasana. All rights reserved.
//

#ifndef grammar_h
#define grammar_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "general.h"
#include "grammar_rule.h"

#define MAX_NUM_OF_RULES   12000
#define MAX_NUM_OF_TERMINALS    8000
#define MAX_NUM_OF_NON_TERMINALS    200
#define MAX_NUM_OF_SYMS     8000



typedef struct {
    Rule** rule;
    int num_of_rules;
    char** terminals;
    int num_of_terminals;
    char** non_terminals;
    int num_of_non_terminals;
}Grammar;

Grammar* create_grammar (char* gr_file);
void read_grammar (char gr_file[], Grammar* gr, char** syms);
void print_grammar (Grammar* gr);
void destroy_grammar (Grammar* gr);
void get_rules_from_lhs (Grammar* gr, char lhs[], int* rule_indices);
void print_grammar_at_indices(Grammar* gr, int rule_indices[]);
bool add_lhs_to_non_terminals (Grammar* gr, int idx);
bool add_syms_to_symbols (Grammar* gr, int idx, int rhs_idx, char** syms);
bool is_non_terminal (Grammar *gr, char* token);
void extract_terminals (Grammar* gr, char** syms);
void remove_terminals (Grammar* gr, char** unused_terminals, int num_of_unused_terminals);
bool exists_token_in_terminals(Grammar* gr, char* token);
bool exists_token_in_non_terminals(Grammar* gr, char* token);
bool delete_from_rule_array (Rule** arr, int len_of_arr, Rule* rule2delete);
#endif /* grammar_h */
