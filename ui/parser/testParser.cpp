#include "parser.h"
#include <stdio.h>

int main(int argc, char **argv){
    Parser* parser = new Parser("output.txt");
    
    printf("%zu\n",parser->getNextEvent().number);
    printf("%zu\n",parser->getNextEvent().number);
    printf("%zu\n",parser->getNextEvent().number);
    printf("%zu\n",parser->getNextEvent().number);
    
    delete parser;
}
