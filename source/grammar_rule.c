//
//  grammar_rule.c
//  FastParser
//
//  Created by Nafisa Ali Amir on 27/06/2019.
//  Copyright © 2019 Fasana. All rights reserved.
//

#include "grammar_rule.h"

Rule* create_gr_rule(float score) {
    
    Rule* new_rule = (Rule *) malloc(sizeof(Rule));
    new_rule->rhs = (Rhs *) malloc(sizeof(Rhs));
    new_rule->rhs->key = (char **) malloc(MAX_NUM_OF_RHS*sizeof(char *));
    new_rule->score = score;
    new_rule->rhs->num_of_rhs = 0;
    
    return new_rule;
}

void set_lhs_to_rule (Rule* rule, char lhs[]) {
    rule->lhs = (char *) malloc(MAX_SIZE_OF_RHS*sizeof(char));
    strcpy(rule->lhs, lhs);
    rule->lhs[strlen(lhs)] = '\0';
}

void set_rhs_to_rule(Rule* rule, char rhs[], int j) {
    rule->rhs->key[j] = (char *) malloc(MAX_SIZE_OF_RHS*sizeof(char));
    strcpy(rule->rhs->key[j], rhs);
    rule->rhs->key[j][strlen(rhs)] = '\0';
    rule->rhs->num_of_rhs++;
}

void print_rule (Rule* rule) {
    printf("%s => ",rule->lhs);
    for (int k = 0; k < rule->rhs->num_of_rhs; k++) {
        printf("%s, ", rule->rhs->key[k]);
    }
    printf("[%f]",rule->score);
}

void destroy_rule (Rule* rule) {
    free (rule->lhs);
    
    for (int j = 0; j < rule->rhs->num_of_rhs; j++) {
        free (rule->rhs->key[j]);
    }
    free (rule->rhs->key);
    free (rule->rhs);
    
    free (rule);
}
