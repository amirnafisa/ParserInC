//
//  parser_main.h
//  FastParser
//
//  Created by Nafisa Ali Amir on 02/07/2019.
//  Copyright © 2019 Fasana. All rights reserved.
//

#ifndef parser_main_h
#define parser_main_h

#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "grammar.h"
#include "sentence.h"
#include "ely_parser.h"
#include "speedup.h"

int earley_parser_main(char sent_str[], char* output_str, char gr_file[]);

#endif /* parser_main_h */
