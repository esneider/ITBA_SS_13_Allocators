#include "parser.h"


Parser* Parser::INSTANCE = NULL;

void worker_thread(void* params){
    if(Parser::INSTANCE != NULL){
        Parser::INSTANCE->parse();
    }
}


Parser::Parser(char *path):sim(NULL){
    Parser::INSTANCE = this;
    input = fopen(path, "r");
    //worker = new thread(worker_thread,NULL);
    parseSimulation();
}

Parser::~Parser(){
    //worker->join();
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
    //lock_guard<mutex> lock(mtx);
    sim = (simulation*) malloc(sizeof(simulation));
    if(int argc=fscanf(input,
            "%s\n"
            "%zu\n"
            "%zu\n"
            "%lf\n"
            "%lf\n"
            "%lf\n"
            "%lf\n"
            "%lf\n"
            "%zu\n",
            sim->context,
            &(sim->heap_size),
            &(sim->time),
            &(sim->mean_fragmentation),
            &(sim->max_fragmentation),
            &(sim->mean_metadata),
            &(sim->mean_malloc_time),
            &(sim->mean_free_time),
            &(sim->num_events)
        )!=9)printf("arrchivo malformado(sim)[%d]!\n",argc);
    //cv.notify_one();
}

event* Parser::parseEvent(){
    event* ev = (event*)malloc(sizeof(event));

    if(int argc=fscanf(input,
       "%zu\n"
       "%s\n"
       "%zu\n"
       "%zu\n"
       "%zu\n"
       "%lf\n"
       "%lf\n"
       "%lf\n",
       &(ev->number),
       ev->type,
       &(ev->size),
       &(ev->adddress),
       &(ev->alternate),
       &(ev->metadata),
       &(ev->execution),
       &(ev->fragmentation)
   )!=8){
		free(ev);
		return NULL;
	}
   /* {
        lock_guard<mutex> lock(mtx);
        events.push(ev);
        cv.notify_one();
    }*/
    return ev;
}

void Parser::skipEvent(){
	fscanf(input,
       "%*u\n"
       "%*s\n"
       "%*u\n"
       "%*u\n"
       "%*u\n"
       "%*f\n"
       "%*f\n"
       "%*f\n"
   );
}

event* Parser::getNextEvent(int skip){
    //lock_guard<mutex> lock(mtx);
    for(int i=0;i<skip;i++){
		/*while(events.empty()){
		    cv.wait(mtx);
		}
		events.pop();*/
		skipEvent();
	}
	
	/*while(events.empty()){
	    cv.wait(mtx);
	}
	
	
	event* tmp = events.front();
	events.pop();*/
    return parseEvent();
}

simulation* Parser::getSimulation(){
    /*lock_guard<mutex> lock(mtx);
    while(sim == NULL){
        cv.wait(mtx);
    }*/
    return sim;
}




