//
//  sentence.h
//  FastParser
//
//  Created by Nafisa Ali Amir on 26/06/2019.
//  Copyright © 2019 Fasana. All rights reserved.
//

#ifndef sentence_h
#define sentence_h

#include <stdio.h>

#define MAX_NUM_OF_TOKENS 100
#define MAX_SIZE_OF_TOKENS 100

typedef struct {
    char* str;
    char** token;
    int num_of_tokens;
} Sentence;

Sentence* create_sentence (char sentence_string[]);
void read_sentence_into_tokens(char sentence_str[], Sentence* sentence);
void print_sentence(Sentence* sentence);
void print_tokens(Sentence* sentence);
void destroy_sentence(Sentence* sentence);
#endif /* sentence_h */
