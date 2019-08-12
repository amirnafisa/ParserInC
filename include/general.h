//
//  general.h
//  FastParser
//
//  Created by Nafisa Ali Amir on 26/06/2019.
//  Copyright © 2019 Fasana. All rights reserved.
//

#ifndef general_h
#define general_h

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define True    1
#define False   0
#define MAX_SIZE_OF_INPUT_STR 500

int exists_element_in_arr(char** arr, char* element, int num_of_tokens);
void add_to_arr(char** arr, char* element, int num_of_tokens);
bool delete_from_array (char** arr, int len_of_arr, char* token2delete);

#endif /* general_h */
