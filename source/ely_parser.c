//
//  ely_parser.c
//  FastParser
//
//  Created by Nafisa Ali Amir on 27/06/2019.
//  Copyright © 2019 Fasana. All rights reserved.
//

#include "ely_parser.h"

bool parse_sen (Grammar* gr, Sentence* sen2parse, bool speedup, RTable* rtable, LAncestorTable* latable, char* output_str) {

    Chart* myELYChart;
    myELYChart = create_chart(sen2parse, gr, speedup, rtable, latable);
    
    
    for (int i = 0; i < get_num_of_elychart_tokens(myELYChart)+1; i++) {
        //printf("Working on column %d and token %s ",i, get_current_token(myELYChart));
        if (!exists_token_in_terminals(gr, get_current_token(myELYChart))) {
            printf("ERR: Token %s not found in grammar\n",get_current_token(myELYChart));
            return False;
        }
        
        set_cur_col_idx(myELYChart, i);
        
        char** prev_predicted_NT = (char** ) malloc(gr->num_of_non_terminals*sizeof(char*));
        int num_of_prev_predicted_NT = 0;
        
        Entry* ely_entry;
        for (int j = 0; (ely_entry = get_entry_at_index(myELYChart->_cur_col, j)); j++) {
            if (is_completed(ely_entry)) {
                complete(myELYChart, ely_entry, speedup);
            } else if (i < sen2parse->num_of_tokens) {
                char next_token[MAX_SIZE_OF_RHS];
                strcpy(next_token, get_next_rhs(ely_entry));
                next_token[strlen(next_token)] = '\0';
                if (exists_token_in_non_terminals(gr, next_token)) {
                    if (exists_element_in_arr(prev_predicted_NT, next_token, num_of_prev_predicted_NT) == -1) {
                        predict(myELYChart, next_token);
                        add_to_arr(prev_predicted_NT, next_token, num_of_prev_predicted_NT);
                        num_of_prev_predicted_NT++;
                    }
                } else {
                    scan(myELYChart, ely_entry);
                }
            }
        }
        free (prev_predicted_NT);
        //printf(" with %d entries\n",myELYChart->_cur_col->_num_of_entries);
    }
    
    print_parse(myELYChart, output_str);
    //print_chart(myELYChart);
    destroy_chart(myELYChart);
    return True;
}
