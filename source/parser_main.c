//
//  parser_main.c
//  FastParser
//
//  Created by Nafisa Ali Amir on 02/07/2019.
//  Copyright © 2019 Fasana. All rights reserved.
//

#include "parser_main.h"

int earley_parser_main(char sent_str[], char* output_str, char gr_file[]) {
    clock_t begin = clock();
    // insert code here...
    bool speedup = True;
    Grammar* gr;
    gr = create_grammar(gr_file);
    
    RTable* rtable = NULL;
    LParentTable* lptable = NULL;
    LAncestorTable* latable = NULL;
    
    Sentence* sentence_to_parse;
    
    sentence_to_parse = create_sentence(sent_str);
    
    if (speedup) {
        filter_grammar(gr, sentence_to_parse);
        
        rtable = extract_RTable(gr);
        lptable = extract_LParentTable(gr);
        latable = extract_LAncestors (gr, lptable);
    }
    
    parse_sen(gr, sentence_to_parse, speedup, rtable, latable, output_str);
    
    destroy_grammar(gr);
    destroy_sentence(sentence_to_parse);
    
    if (speedup) {
        if (rtable) {
            destroy_RTable(rtable);
        }
        if (lptable) {
            destroy_LParentTable(lptable);
        }
        if (latable) {
            destroy_LAncestorTable(latable);
        }
    }
    
    clock_t end = clock();
    printf("Elapsed: %f milli-seconds\n", (double)(end - begin) * 1000 / CLOCKS_PER_SEC);
    
    return 0;
}
