//
//  grammar_rule.h
//  FastParser
//
//  Created by Nafisa Ali Amir on 27/06/2019.
//  Copyright © 2019 Fasana. All rights reserved.
//

#ifndef grammar_rule_h
#define grammar_rule_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE_OF_RHS 20
#define MAX_NUM_OF_RHS 20

typedef struct {
    char** key;
    int num_of_rhs;
}Rhs;

typedef struct {
    char* lhs;
    Rhs* rhs;
    float score;
}Rule;

Rule* create_gr_rule(float score);
void set_lhs_to_rule (Rule* rule, char lhs[]);
void set_rhs_to_rule(Rule* rule, char rhs[], int j);
void print_rule (Rule* rule);
void destroy_rule (Rule* rule);

#endif /* grammar_rule_h */
