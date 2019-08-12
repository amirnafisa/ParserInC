//
//  general.c
//  FastParser
//
//  Created by Nafisa Ali Amir on 27/06/2019.
//  Copyright © 2019 Fasana. All rights reserved.
//

#include "general.h"

int exists_element_in_arr(char** arr, char* element, int num_of_tokens) {
    
    for (int i = 0; i < num_of_tokens; i++) {
        if (!strcmp(arr[i], element)) {
            return i;
        }
    }
    return -1;
}

void add_to_arr(char** arr, char* element, int num_of_tokens) {
    arr[num_of_tokens] = (char*) malloc(strlen(element)*sizeof(char));
    strcpy(arr[num_of_tokens], element);
}

bool delete_from_array (char** arr, int len_of_arr, char* token2delete) {

    bool shift = False;
    for (int i = 0; i < len_of_arr; i++) {
        if(!strcmp(arr[i], token2delete)) {
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
