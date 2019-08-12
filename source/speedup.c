//
//  speedup.c
//  FastParser
//
//  Created by Nafisa Ali Amir on 27/06/2019.
//  Copyright © 2019 Fasana. All rights reserved.
//

#include "speedup.h"

bool do_discard(int cur_col_idx, int n_tokens, int score) {
    if ((score > n_tokens*13) && (cur_col_idx < 5)) {
        return True;
    } else if (score > n_tokens*11) {
        return True;
    }
    return False;
}


void filter_grammar (Grammar* gr, Sentence* sentence) {
    char** unused_terminals = (char**) malloc (gr->num_of_terminals*sizeof(char*));
    int num_of_unused_terminals = 0;
    for (int i = 0; i < gr->num_of_terminals; i++) {
        bool needed = False;
        for (int j = 0; j < sentence->num_of_tokens; j++) {
            if (!strcmp(gr->terminals[i], sentence->token[j])) {
                needed = True;
                break;
            }
        }
        if (!needed) {
           // unused_terminals[num_of_unused_terminals] = (char* ) malloc(strlen(gr->terminals[i])*sizeof(char));
            unused_terminals[num_of_unused_terminals] = gr->terminals[i];
            num_of_unused_terminals++;
        }
    }
    remove_terminals(gr, unused_terminals, num_of_unused_terminals);
    for (int i = 0; i < num_of_unused_terminals; i++) {
        free (unused_terminals[i]);
    }
    free (unused_terminals);
}

RTable* extract_RTable(Grammar* gr) {
    RTable* rtable = (RTable* ) malloc(sizeof(RTable));
    rtable->rcolumn = (RColumn** ) malloc(MAX_NUM_OF_RULES*sizeof(RColumn*));
    
    rtable->num_of_columns = 0;
    
    for (int i = 0; i < gr->num_of_non_terminals; i++) {
        int rule_indices[MAX_NUM_OF_RULES];
        get_rules_from_lhs(gr, gr->non_terminals[i], rule_indices);
        for (int j = 0; rule_indices[j] != -1; j++) {
            int index = get_index_rtable(rtable, gr->rule[rule_indices[j]]->lhs, gr->rule[rule_indices[j]]->rhs->key[0]);
            if (index == -1) {
                index = rtable->num_of_columns;
                rtable->rcolumn[index] = (RColumn* ) malloc(sizeof(RColumn));
                rtable->rcolumn[index]->rule = (Rule** ) malloc(MAX_NUM_OF_RULES*sizeof(Rule*));
                rtable->rcolumn[index]->num_of_rules = 0;
                rtable->num_of_columns++;
            }
            
            rtable->rcolumn[index]->rule[rtable->rcolumn[index]->num_of_rules] = gr->rule[rule_indices[j]];
            rtable->rcolumn[index]->key0 = gr->rule[rule_indices[j]]->lhs;
            rtable->rcolumn[index]->key1 = gr->rule[rule_indices[j]]->rhs->key[0];
            rtable->rcolumn[index]->num_of_rules++;
            
            
        }
        
    }
    return rtable;
}

int get_index_rtable(RTable* rtable, char* non_terminal, char* rhs) {
    int index = -1;
    for (int k = 0; k < rtable->num_of_columns; k++) {
        if ((!strcmp(rtable->rcolumn[k]->key0, non_terminal)) &&
            (!strcmp(rtable->rcolumn[k]->key1, rhs))) {
            index = k;
            break;
        }
    }
    return index;
}

void destroy_RColumn (RColumn* rcolumn) {
    free (rcolumn);
}


void destroy_RTable (RTable* rtable) {
    for (int i = 0; i < rtable->num_of_columns; i++) {
        destroy_RColumn (rtable->rcolumn[i]);
    }
    free (rtable);
}

LParentTable* extract_LParentTable (Grammar* gr) {
    LParentTable* lptable = (LParentTable* ) malloc(sizeof(LParentTable));
    lptable->lparent_col = (LParentColumn** ) malloc(MAX_NUM_OF_RULES*sizeof(LParentColumn*));
    
    lptable->num_of_columns = 0;
    
    for (int i = 0; i < gr->num_of_non_terminals; i++) {
        int rule_indices[MAX_NUM_OF_RULES];
        get_rules_from_lhs(gr, gr->non_terminals[i], rule_indices);
        for (int j = 0; rule_indices[j] != -1; j++) {
            int index = -1;
            for (int k = 0; k < lptable->num_of_columns; k++) {
                if (!strcmp(lptable->lparent_col[k]->key_rhs, gr->rule[rule_indices[j]]->rhs->key[0])) {
                    index = k;
                    break;
                }
            }
            if (index == -1) {
                index = lptable->num_of_columns;
                lptable->lparent_col[index] = (LParentColumn* ) malloc(sizeof(LParentColumn));
                lptable->lparent_col[index]->entry_lhs = (char** ) malloc(MAX_NUM_OF_RULES*sizeof(char*));
                lptable->lparent_col[index]->num_of_entries = 0;
                lptable->num_of_columns++;
            }
            
            lptable->lparent_col[index]->entry_lhs[lptable->lparent_col[index]->num_of_entries] = gr->rule[rule_indices[j]]->lhs;
            lptable->lparent_col[index]->key_rhs = gr->rule[rule_indices[j]]->rhs->key[0];
            lptable->lparent_col[index]->num_of_entries++;
        }
    }
    return lptable;
            
}

void destroy_LParentColumn (LParentColumn* lparent_col) {
    free (lparent_col);
}


void destroy_LParentTable (LParentTable* lptable) {
    for (int i = 0; i < lptable->num_of_columns; i++) {
        destroy_LParentColumn (lptable->lparent_col[i]);
    }
    free (lptable);
}

LParentColumn* get_lpcolumn_at (LParentTable* lptable, char* token) {
    for (int i = 0; i < lptable->num_of_columns; i++) {
        if (!strcmp(lptable->lparent_col[i]->key_rhs, token)) {
            return lptable->lparent_col[i];
        }
    }
    return NULL;
}

LAncestorTable* extract_LAncestors (Grammar* gr, LParentTable* lptable) {
    LAncestorTable* latable = (LAncestorTable* ) malloc(sizeof(LAncestorTable));
    latable->lancestor_col = (LAncestorColumn** ) malloc(MAX_NUM_OF_RULES*sizeof(LAncestorColumn));
    latable->num_of_columns = 0;
    
    LookedAt* prev_looked_at = (LookedAt* ) malloc(sizeof(LookedAt));
    prev_looked_at->token = (char** ) malloc(MAX_NUM_OF_TERMINALS*sizeof(char*));
    prev_looked_at->num_of_tokens = 0;
    
    for (int i = 0; i < gr->num_of_terminals; i++) {
        LParentColumn* lpcolumn;
        lpcolumn = get_lpcolumn_at(lptable, gr->terminals[i]);
        for (int j = 0; j < lpcolumn->num_of_entries; j++) {
            
            int index = -1;
            for (int k = 0; k < latable->num_of_columns; k++) {
                if (!strcmp(latable->lancestor_col[k]->key_lhs, lpcolumn->entry_lhs[j])) {
                    index = k;
                    break;
                }
            }
            if (index == -1) {
                index = latable->num_of_columns;
                latable->lancestor_col[index] = (LAncestorColumn* ) malloc(sizeof(LAncestorColumn));
                latable->lancestor_col[index]->entry_rhs = (char** ) malloc(MAX_NUM_OF_RULES*sizeof(char*));
                latable->lancestor_col[index]->num_of_entries = 0;
                latable->num_of_columns++;
            }
            
            latable->lancestor_col[index]->entry_rhs[latable->lancestor_col[index]->num_of_entries] = gr->terminals[i];
            latable->lancestor_col[index]->key_lhs = lpcolumn->entry_lhs[j];
            latable->lancestor_col[index]->num_of_entries++;
            prev_looked_at->token[prev_looked_at->num_of_tokens] = gr->terminals[i];
            prev_looked_at->num_of_tokens++;
            if (exists_element_in_arr(prev_looked_at->token, lpcolumn->entry_lhs[j], prev_looked_at->num_of_tokens) == -1) {
                LParentColumn* new_col;
                new_col = get_lpcolumn_at(lptable, lpcolumn->entry_lhs[j]);
                
                if (new_col) {
                    recurse (lpcolumn->entry_lhs[j], new_col, lptable, latable, prev_looked_at);
                }
            }
            
        }
    }
    return latable;
}

void recurse (char* non_term, LParentColumn* lpcolumn, LParentTable* lptable, LAncestorTable* latable, LookedAt* prev_looked_at) {
    
    for (int j = 0; j < lpcolumn->num_of_entries; j++) {
        
        int index = -1;
        for (int k = 0; k < latable->num_of_columns; k++) {
            if (!strcmp(latable->lancestor_col[k]->key_lhs, lpcolumn->entry_lhs[j])) {
                index = k;
                break;
            }
        }
        if (index == -1) {
            index = latable->num_of_columns;
            latable->lancestor_col[index] = (LAncestorColumn* ) malloc(sizeof(LAncestorColumn));
            latable->lancestor_col[index]->entry_rhs = (char** ) malloc(MAX_NUM_OF_RULES*sizeof(char*));
            latable->lancestor_col[index]->num_of_entries = 0;
            latable->num_of_columns++;
        }
        
        latable->lancestor_col[index]->entry_rhs[latable->lancestor_col[index]->num_of_entries] = non_term;
        latable->lancestor_col[index]->key_lhs = lpcolumn->entry_lhs[j];
        latable->lancestor_col[index]->num_of_entries++;
        prev_looked_at->token[prev_looked_at->num_of_tokens] = non_term;
        prev_looked_at->num_of_tokens++;
        if (exists_element_in_arr(prev_looked_at->token, lpcolumn->entry_lhs[j], prev_looked_at->num_of_tokens) == -1) {
            LParentColumn* new_col;
            new_col = get_lpcolumn_at(lptable, lpcolumn->entry_lhs[j]);
            if (new_col) {
                recurse (lpcolumn->entry_lhs[j], new_col, lptable, latable, prev_looked_at);
            }
        }
        
    }
}

void destroy_LAncestorColumn (LAncestorColumn* lancestor_col) {
    free (lancestor_col);
}


void destroy_LAncestorTable (LAncestorTable* latable) {
    for (int i = 0; i < latable->num_of_columns; i++) {
        destroy_LAncestorColumn (latable->lancestor_col[i]);
    }
    free (latable);
}
