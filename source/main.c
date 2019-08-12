#include <stdio.h>
#include <parser_main.h>

int main(int argc, const char * argv[]) {
    char sent_str[] = "John saw the man with my telescope";
    char output_str[200];
    earley_parser_main(sent_str, output_str, "test.gr");
    printf("Output: %s\n",output_str);
    return 0;
}
