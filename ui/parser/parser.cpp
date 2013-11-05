#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

Parser* Parser::INSTANCE = NULL;

void worker_thread(void* params){
    if(Parser::INSTANCE != NULL){
        Parser::INSTANCE->parse();
    }
}


Parser::Parser(char *path):sim(NULL){
    Parser::INSTANCE = this;
    input = fopen(path, "r");
    worker = new thread(worker_thread,NULL);
}

Parser::~Parser(){
    worker->join();
    while(!events.empty()){
        free(events.front());
        events.pop();
    }
    free(sim);
}


void Parser::parse(){
    parseSimulation();
    for (size_t i = 0; i < sim->num_events; i++) {
        parseEvent();
    }
    fclose(input);
}


void Parser::parseSimulation(){
    lock_guard<mutex> lock(mtx);
    sim = (simulation*) malloc(sizeof(simulation));
    if(fscanf(input,
            "simulation = {\n"
                "\tcontext = %s\n"
                "\theap_size = %zu\n"
                "\ttime = %zu\n"
                "\tnum_events = %zu\n"
            "}\n",
            sim->context,
            &(sim->heap_size),
            &(sim->time),
            &(sim->num_events)
        )!=4)printf("arrchivo malformado!\n");
    cv.notify_one();
}

void Parser::parseEvent(){
    event* ev = (event*)malloc(sizeof(event));

    if(fscanf(input,
       "event %zu = {\n"
           "\ttype = %s\n"
           "\tsize = %zu\n"
           "\ttime = %lf\n"
       "}\n",
       &(ev->number),
       ev->type,
       &(ev->size),
       &(ev->time)
   )!=4)printf("archivo malformado!\n");

    {
        lock_guard<mutex> lock(mtx);
        events.push(ev);
        cv.notify_one();
    }
}

event Parser::getNextEvent(){
    lock_guard<mutex> lock(mtx);
    while(events.empty()){
        cv.wait(mtx);
    }
    event* tmp = events.front();
    events.pop();
    return *tmp;
}

simulation* Parser::getSimulation(){
    lock_guard<mutex> lock(mtx);
    while(sim == NULL){
        cv.wait(mtx);
    }
    return sim;
}




