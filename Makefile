CC = gcc-9
SRCDIR = source/
INCDIR = include/
LIBDIR = lib/
SRCFILES =  main.c ely_chart.c ely_column.c ely_entry.c ely_parser.c general.c grammar_rule.c grammar.c parser_main.c sentence.c speedup.c
SRCS = $(addprefix $(SRCDIR),$(SRCFILES))

CCFLAGS = -O3 -I $(INCDIR)

.PHONY: all
all: ${TARGET_LIB}
	$(CC) $(CCFLAGS) $(SRCS)

run:
	./a.out

.PHONY: clean
clean:
	rm -rf *.o
