//
//  grammar.c
//  FastParser
//
//  Created by Nafisa Ali Amir on 26/06/2019.
//  Copyright © 2019 Fasana. All rights reserved.
//

#include "grammar.h"

Grammar* create_grammar (char* gr_file) {
    Grammar *gr = (Grammar *) malloc(sizeof(Grammar));
    char** syms = (char** ) malloc(MAX_NUM_OF_SYMS*sizeof(char *));
    gr->non_terminals = (char** ) malloc(MAX_NUM_OF_NON_TERMINALS*sizeof(char *));
    gr->terminals = (char** ) malloc(MAX_NUM_OF_TERMINALS*sizeof(char *));
    gr->num_of_non_terminals = 0;
    gr->num_of_terminals = 0;
    gr->num_of_rules = 0;
    
    read_grammar(gr_file, gr, syms);
    
    //print_grammar(gr);
    
    return gr;
}
void read_grammar(char gr_file[], Grammar* gr, char** syms) {
    FILE *fp;
    char buff[100];
    int r;
    
    gr->rule = (Rule **) malloc(MAX_NUM_OF_RULES*sizeof(Rule *));
    
    fp = fopen(gr_file, "r");
    
    if (fp == NULL) {
        perror("ERR: Opening grammar file\n");
        exit(0);
    }
    char ch;
    int i = 0, j = 0;
    int status = 0; //PROB;
    while ((r = fscanf(fp, "%s%c", buff, &ch)) != EOF) {
        switch(status) {
            case 0: //PROB:
                gr->rule[i] = create_gr_rule(-log(atof(buff)));
                status = 1;
                break;
            case 1: //LHS:
                set_lhs_to_rule(gr->rule[i], buff);
                status = 2;
                break;
            case 2: //RHS:
                set_rhs_to_rule(gr->rule[i], buff, j);
                add_syms_to_symbols(gr, i, j, syms);
                j++;
                break;
            default:
                status = 0;
                break;
        }
        if (ch == '\n') {
            gr->num_of_rules++;
            add_lhs_to_non_terminals(gr, i);
            i++;
            j = 0;
            status = 0;
        }
    }
    extract_terminals(gr, syms);
    
    for (int i = 0; syms[i] != NULL; i++) {
        free (syms[i]);
    }
    free (syms);

    fclose(fp);
}

bool add_lhs_to_non_terminals (Grammar* gr, int idx) {
    
    for (int i = 0; i < gr->num_of_non_terminals; i++) {
        if (!strcmp(gr->rule[idx]->lhs, gr->non_terminals[i])) {
            return False;
        }
    }
    gr->non_terminals[gr->num_of_non_terminals] = (char *) malloc (MAX_SIZE_OF_RHS*sizeof(char));

    strcpy(gr->non_terminals[gr->num_of_non_terminals], gr->rule[idx]->lhs);
    gr->non_terminals[gr->num_of_non_terminals][strlen(gr->rule[idx]->lhs)] = '\0';
    gr->num_of_non_terminals++;
    return True;
}

bool add_syms_to_symbols(Grammar* gr, int idx, int rhs_idx, char** syms) {
    int i = 0;
    for (i = 0; syms[i]!=NULL; i++) {
        if (!strcmp(gr->rule[idx]->rhs->key[rhs_idx], syms[i])) {
            return False;
        }
    }
    syms[i] = (char *) malloc (MAX_SIZE_OF_RHS*sizeof(char));
    strcpy(syms[i], gr->rule[idx]->rhs->key[rhs_idx]);
    syms[i][strlen(gr->rule[idx]->rhs->key[rhs_idx])] = '\0';
    syms[i+1] = NULL;
    return True;
}

bool is_non_terminal (Grammar *gr, char* token) {
    for (int i = 0; i < gr->num_of_non_terminals; i++) {
        if (!strcmp(gr->non_terminals[i], token)) {
            return True;
        }
    }
    return False;
}

void extract_terminals (Grammar* gr, char** syms) {
    gr->num_of_terminals = 0;
    for (int i = 0; syms[i] != NULL; i++) {
        if (!is_non_terminal(gr, syms[i])) {
            gr->terminals[gr->num_of_terminals] = (char* ) malloc(MAX_SIZE_OF_RHS*sizeof(char));
            strcpy(gr->terminals[gr->num_of_terminals], syms[i]);
            gr->num_of_terminals++;
        }
    }
}

void destroy_grammar(Grammar* gr) {
    
    for (int i = 0; i < gr->num_of_rules; i++) {
        destroy_rule(gr->rule[i]);
    }
    
    for (int i = 0; i < gr->num_of_terminals; i++) {
        free (gr->terminals[i]);
    }
    free (gr->terminals);
    
    for (int i = 0; i < gr->num_of_non_terminals; i++) {
        free (gr->non_terminals[i]);
    }
    free (gr->non_terminals);
    
    free(gr);
}

void get_rules_from_lhs(Grammar* gr, char lhs[], int rule_indices[]) {
    
    int j = 0;
    for (int i = 0; i < gr->num_of_rules; i++) {
        if (!strcmp(gr->rule[i]->lhs, lhs)) {
            rule_indices[j] = i;
            j++;
        }
    }
    rule_indices[j] = -1;
}

void print_grammar_at_indices (Grammar* gr, int rule_indices[]) {
    printf("Printing Grammar Rules At specific Indices -> %d\n",rule_indices[0]);
    
    for( int j = 0; rule_indices[j] != -1; j++) {
        print_rule(gr->rule[rule_indices[j]]);
        printf("\n");
    }
}

void print_grammar (Grammar* gr) {
    printf("Printing Grammar Rules\n");
    
    for (int i = 0; i < gr->num_of_rules; i++) {
        print_rule(gr->rule[i]);
        printf("\n");
    }
}

void remove_rule_from_grammar_given_rhs_token (Grammar* gr, char* token) {

    Rule** unused_rules = (Rule**) malloc(MAX_NUM_OF_RULES*sizeof(Rule*));
    int num_of_unused_rules = 0;

    for (int i = 0; i < gr->num_of_rules; i++) {
        for (int j = 0; j < gr->rule[i]->rhs->num_of_rhs; j++) {
            
            if (!strcmp(gr->rule[i]->rhs->key[j], token)) {
               
                unused_rules[num_of_unused_rules] = gr->rule[i];
                num_of_unused_rules++;
                break;
            }
        }
    }
    
    for (int i = 0; i < num_of_unused_rules; i++) {
        if (delete_from_rule_array(gr->rule, gr->num_of_rules, unused_rules[i])) {
            gr->num_of_rules--;
        }
    }
    
    free (unused_rules);
}

void remove_terminals (Grammar* gr, char** unused_terminals, int num_of_unused_terminals) {
    int len_of_arr = gr->num_of_terminals;
    for (int i = 0; i < num_of_unused_terminals; i++) {
        if (delete_from_array(gr->terminals, len_of_arr, unused_terminals[i])) {
            len_of_arr--;
            remove_rule_from_grammar_given_rhs_token (gr, unused_terminals[i]);

        }
    }
    
    gr->num_of_terminals = len_of_arr;
    
    
    //printf("Number of unused terminals %d\n",num_of_unused_terminals);
    //printf("Number of terminals %d\n",len_of_arr);

}

bool exists_token_in_terminals(Grammar* gr, char* token) {
    bool found = False;
    for (int i = 0; i < gr->num_of_terminals; i++) {
        if (!strcmp(gr->terminals[i], token)) {
            found = True;
            break;
        }
    }
    return found;
}

bool exists_token_in_non_terminals(Grammar* gr, char* token) {
    bool found = False;
    for (int i = 0; i < gr->num_of_non_terminals; i++) {
        if (!strcmp(gr->non_terminals[i], token)) {
            found = True;
            break;
        }
    }
    return found;
}

bool delete_from_rule_array (Rule** arr, int len_of_arr, Rule* rule2delete) {
    
    bool shift = False;
    for (int i = 0; i < len_of_arr; i++) {
        if(arr[i] == rule2delete) {
            shift = True;
        }
        if (shift && (i < len_of_arr-1)) {
            arr[i] = arr[i+1];
        }
    }
    if (shift) {
        arr[len_of_arr-1] = NULL;
    }
    return shift;
}
