//
//  ely_parser.h
//  FastParser
//
//  Created by Nafisa Ali Amir on 27/06/2019.
//  Copyright © 2019 Fasana. All rights reserved.
//

#ifndef ely_parser_h
#define ely_parser_h

#include <stdio.h>
#include <stdbool.h>
#include "speedup.h"
#include "ely_chart.h"
#include "grammar.h"
#include "general.h"

bool parse_sen (Grammar* gr, Sentence* sen2parse, bool speedup, RTable* rtable, LAncestorTable* latable, char* output_str);

#endif /* ely_parser_h */
