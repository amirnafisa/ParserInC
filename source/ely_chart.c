//
//  ely_chart.c
//  FastParser
//
//  Created by Nafisa Ali Amir on 27/06/2019.
//  Copyright © 2019 Fasana. All rights reserved.
//

#include "ely_chart.h"

Chart* create_chart (Sentence* sentence, Grammar* gr, bool speedup, RTable* rtable, LAncestorTable* latable) {
    Chart* chart = (Chart* ) malloc(sizeof(Chart));
    chart->_cur_col_idx = 0;
    chart->_cur_entry_idx = 0;
    chart->_cur_col = create_column(chart->_cur_col_idx);
    chart->_ELY = (Column** ) malloc((sentence->num_of_tokens + 1)*sizeof(Column*));
    chart->_ELY[0] = chart->_cur_col;
    chart->_nELY = (Entry** ) malloc(MAX_NUM_OF_ENTRIES*sizeof(Entry*));
    chart->_num_of_cols = 1;
    chart->_root = (char* ) malloc((strlen("ROOT")+1)*sizeof(char));
    strcpy(chart->_root, "ROOT");
    chart->_root[strlen("ROOT")] = '\0';
    chart->_token = sentence->token;
    chart->_num_of_tokens = sentence->num_of_tokens;
    chart->_pcfg = gr;
    chart->speedup = speedup;
    chart->rtable = rtable;
    chart->latable = latable;

    predict(chart, chart->_root);
    
    return chart;
}

char* get_current_token (Chart* chart){
    if (chart->_cur_col_idx < chart->_num_of_tokens) {
        return chart->_token[chart->_cur_col_idx];
    }
    return NULL;
}

void add_entry_to_chart (Chart* chart, Column* column, Entry* entry) {
    if (add_entry(column, entry) == 1) {
        chart->_nELY[chart->_cur_entry_idx] = entry;
        chart->_cur_entry_idx++;
    } else {
        chart->_nELY[get_id(entry)] = entry;
    }
}

void predict (Chart* chart, char* non_terminal) {
    int found = -1;
    if (chart->speedup) {
        for (int i = 0; i < chart->latable->num_of_columns; i++) {
            if (!strcmp(non_terminal, chart->latable->lancestor_col[i]->key_lhs)) {
                found = i;
                break;
            }
        }
    }
    if (chart->speedup && (found != -1)) {
        for (int i = 0; i < chart->latable->lancestor_col[found]->num_of_entries; i++) {
            int index = get_index_rtable(chart->rtable, non_terminal, chart->latable->lancestor_col[found]->entry_rhs[i]);
            for (int j = 0; j < chart->rtable->rcolumn[index]->num_of_rules; j++) {
                Entry* new_entry;
                new_entry = create_entry(chart->_cur_entry_idx, chart->rtable->rcolumn[index]->rule[j], chart->_cur_col_idx, chart->_cur_col_idx);
                add_score_by(new_entry, chart->rtable->rcolumn[index]->rule[j]->score);
                add_entry_to_chart(chart, chart->_cur_col, new_entry);
            }
        }
    } else {
        int rule_indices[MAX_NUM_OF_RULES];
        get_rules_from_lhs(chart->_pcfg, non_terminal, rule_indices);
        
        for (int i = 0; rule_indices[i]!=-1; i++) {
            Entry* new_entry;
            new_entry = create_entry(chart->_cur_entry_idx, chart->_pcfg->rule[rule_indices[i]], chart->_cur_col_idx, chart->_cur_col_idx);
            add_score_by(new_entry, chart->_pcfg->rule[rule_indices[i]]->score);
            add_entry_to_chart(chart, chart->_cur_col, new_entry);
        }
    }
}

void scan (Chart* chart, Entry* entry) {
    char* this_token;
    this_token = get_next_rhs(entry);
    if (!(strcmp(this_token, get_current_token(chart)))) {
        if (chart->_cur_col_idx + 1 == chart->_num_of_cols) {
            chart->_ELY[chart->_num_of_cols] = create_column(chart->_num_of_cols);
            chart->_num_of_cols++;
        }
        Column* new_col;
        new_col = chart->_ELY[chart->_cur_col_idx + 1];
        
        Entry *new_entry;
        new_entry = create_entry(chart->_cur_entry_idx, get_rule(entry), chart->_cur_col_idx + 1, get_lookup_col_index(entry));
        add_score_by(new_entry, get_score(entry));
        set_dot_index(new_entry, get_dot_index(entry) + 1);
        add_bktrack(new_entry, entry, -1);
        add_entry_to_chart(chart, new_col, new_entry);
    }
}

void complete (Chart* chart, Entry* completed_entry, bool speedup) {
    char* lookup_token;
    lookup_token = get_rule(completed_entry)->lhs;
    int lookup_col_idx = get_lookup_col_index(completed_entry);
    Entry* entry;
    for (int i = 0;(entry = get_entry_at_index(chart->_ELY[lookup_col_idx], i)); i++) {
        if ((!is_completed(entry)) && (!strcmp(get_next_rhs(entry), lookup_token))) {
            int entry_lookup_col = get_lookup_col_index(entry);
            Entry *new_entry;
            new_entry = create_entry(chart->_cur_entry_idx, get_rule(entry), chart->_cur_col_idx, entry_lookup_col);
            int new_score = add_score_by(new_entry, get_score(entry) + get_score(completed_entry));
            set_dot_index(new_entry, get_dot_index(entry) + 1);
            add_bktrack(new_entry, entry, get_id(completed_entry));
            bool discard = False;
            if (speedup) {
                discard = do_discard(chart->_cur_col_idx, chart->_cur_col_idx - entry_lookup_col, new_score);
            }
            if (!discard) {
                add_entry_to_chart(chart, chart->_cur_col, new_entry);
            }
        }
    }
}

Column** get_chart(Chart* chart) {
    return chart->_ELY;
}

Column* get_column (Chart* chart, int col_idx) {
    return chart->_ELY[col_idx];
}

void set_cur_col_idx (Chart* chart, int col_idx) {
    chart->_cur_col_idx = col_idx;
    chart->_cur_col = chart->_ELY[col_idx];
}

int get_cur_col_idx (Chart* chart) {
    return chart->_cur_col_idx;
}

void print_chart (Chart* chart) {
    printf("Printing Chart:\n");
    
    for (int i = 0; i < chart->_num_of_cols; i++) {
        Entry* entry;
        for (int j = 0; (entry = get_entry_at_index(chart->_ELY[i], j)); j++) {
            Rule *rule;
            rule = get_rule(entry);
            int *bktrack;
            bktrack = get_bktrack(entry);
            
            printf("Entry ID %d, Lookup ID %d: %s => ",get_id(entry), get_lookup_col_index(entry), rule->lhs);
            for (int k = 0; k < rule->rhs->num_of_rhs; k++) {
                if (k == get_dot_index(entry)) {
                    printf(".");
                }
                printf("%s - %d ", rule->rhs->key[k], bktrack[k]);
            }
            printf("[%d]\n",get_score(entry));
        }
    }
}

bool recurse_print (Chart* chart, int id, char* output_str) {
    if (id >= chart->_cur_entry_idx || id < 0) {
        return False;
    }
    Entry* entry;
    entry = chart->_nELY[id];

    Rhs* rhs;
    rhs = get_rule(entry)->rhs;
    int* bktrack;
    bktrack = get_bktrack(entry);
    
    for (int i = 0; i < rhs->num_of_rhs; i++) {
        if (bktrack[i] != -1) {
            strcat(output_str, "(");
        }
        strcat(output_str, rhs->key[i]);
        strcat(output_str, " ");
        recurse_print(chart, bktrack[i], output_str);
        if (bktrack[i] != -1) {
            strcat(output_str, ")");
        }
    }
    return True;
}

void print_parse (Chart* chart, char* output_str) {
    Entry* entry;
    
    for (int j = 0; (entry = get_entry_at_index(chart->_cur_col, j)); j++) {
        if (!strcmp(get_rule(entry)->lhs, chart->_root)) {
            strcpy(output_str, "(ROOT ");
            recurse_print(chart, get_id(entry), output_str);
            strcat(output_str, ")");
            output_str[strlen(output_str)] = '\0';
            break;
        }
    }
}

int get_num_of_elychart_tokens(Chart* chart) {
    return chart->_num_of_tokens;
}

void destroy_chart (Chart* chart) {
    free (chart->_root);
    
    for (int i = 0; i < chart->_num_of_cols; i++) {
        destroy_column(chart->_ELY[i]);
    }
    
    free(chart);
}
