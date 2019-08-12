//
//  sentence.c
//  FastParser
//
//  Created by Nafisa Ali Amir on 26/06/2019.
//  Copyright © 2019 Fasana. All rights reserved.
//

#include <stdlib.h>
#include <string.h>
#include "sentence.h"

Sentence* create_sentence (char sentence_string[]) {

    Sentence* sentence_to_parse = (Sentence *) malloc(sizeof(Sentence));
    
    read_sentence_into_tokens(sentence_string, sentence_to_parse);
    print_sentence(sentence_to_parse);
    return sentence_to_parse;
}

void read_sentence_into_tokens(char sentence_str[], Sentence* sentence) {
   
    sentence->str = (char* ) malloc(strlen(sentence_str)*sizeof(char));
    strcpy(sentence->str, sentence_str);

    sentence->token = (char **) malloc(MAX_NUM_OF_TOKENS*sizeof(char *));
    sentence->num_of_tokens = 0;
    
    char* token = (char* ) malloc(MAX_SIZE_OF_TOKENS*sizeof(char));
    char* rest = sentence_str;
    
    while ((token = strtok_r(rest, " ", &rest))) {
        sentence->token[sentence->num_of_tokens] = (char *) malloc(MAX_SIZE_OF_TOKENS*sizeof(char));
        strcpy(sentence->token[sentence->num_of_tokens], token);
        sentence->num_of_tokens++;
        if (rest == NULL) {
            break;
        }
    }

}

void print_sentence(Sentence* sentence) {
    printf("Sentence: %s\n", sentence->str);
}

void print_tokens(Sentence* sentence) {
    printf("Sentence [%d] Tokens: \n",sentence->num_of_tokens);
    for (int i = 0; i < sentence->num_of_tokens; i++) {
        printf("%s\n",sentence->token[i]);
    }
}

void destroy_sentence(Sentence* sentence) {
    for (int i = 0; i < sentence->num_of_tokens; i++) {
        free(sentence->token[i]);
    }
    free(sentence->str);
    free(sentence);
}
