#ifndef _PARSER_H_
#define _PARSER_H_ 1

//#include <mutex>
#include <queue>
//#include <thread>
//#include <condition_variable>
#include <tinythread.h>
#include <fast_mutex.h>

using namespace std;
using namespace tthread;

struct simulation{
    char context[256];
    size_t heap_size;
    size_t time;
    double mean_fragmentation;
    double max_fragmentation;
    double mean_metadata;
    double mean_malloc_time;
    double mean_free_time;
    size_t num_events;
};


struct event{
    size_t number;
    char type[256];
    size_t size;       
    size_t adddress;
    size_t alternate;
    double metadata;
    double execution;
    double fragmentation;
};

class Parser
{


public:
    Parser( char* path );
    ~Parser();

    simulation* getSimulation();
    
    event* getNextEvent(int skeep);

    static Parser* INSTANCE;

    void parse();
private:

    void parseSimulation();
    event* parseEvent();
    void skipEvent();

    thread* worker;
    mutex mtx;
    condition_variable cv;

    FILE* input;

    simulation* sim;
    queue<event*> events;
};

#endif // _PARSER_H_
