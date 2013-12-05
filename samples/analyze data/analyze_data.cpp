#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "../../ui/parser/parser.h"

int main(int argc, char **argv) {

	if(argc<3){
		printf("usage: analyze_data [imputfile1] [imputfile2] ...");
		exit(2);
	}
	
	long double* mean_frag  = new long double[argc-1];
	long double* max_frag = new long double[argc-1];
	long double* mean_meta = new long double[argc-1];
	long double* mean_malloc_time = new long double[argc-1];
	long double* mean_free_time = new long double[argc-1];


	
	long double mean_mean_frag = 0;
	long double mean_max_frag = 0;
	long double mean_mean_meta = 0;
	long double mean_mean_malloc_time = 0;
	long double mean_mean_free_time = 0;
	
	long double var_mean_frag = 0;
	long double var_max_frag = 0;
	long double var_mean_meta = 0;
	long double var_mean_malloc_time = 0;
	long double var_mean_free_time = 0;
	
	long double std_mean_frag = 0;
	long double std_max_frag = 0;
	long double std_mean_meta = 0;
	long double std_mean_malloc_time = 0;
	long double std_mean_free_time = 0;
	

	for(int i =0; i < argc-1;i++){
		mean_frag[i] = 0;
		max_frag[i] = 1;
		mean_meta[i] = 0;
		mean_malloc_time[i] = 0;
		mean_free_time[i] = 0;


		Parser* parser = new Parser(argv[i+1]);

		int num_events = parser->getSimulation()->num_events;
		
		for(int j =0; j < num_events;j++){
			event* _event = parser->getNextEvent(0);
		
			max_frag[i] = _event->fragmentation < max_frag[i]? _event->fragmentation: max_frag[i];
		
			mean_malloc_time[i] += strcmp(_event->type,"free")==0?0:_event->execution<0? 0 : _event->execution/(double)num_events;
			mean_free_time[i] += strcmp(_event->type,"free")==0?_event->execution<0? 0 : _event->execution/(double)num_events:0;
		
			free(_event);
		}
		
		mean_frag[i] = parser->getSimulation()->mean_fragmentation;
		mean_meta[i] = parser->getSimulation()->mean_metadata;
		
		/*
		printf(
		"measures %s= {\n"
				"\tmean_frag = %LF\n"
				"\tmax_frag = %LF\n"
				"\tmean_meta = %LF\n"
				"\tmean_malloc_time = %LF\n"
				"\tmean_free_time = %LF\n"
		"}\n",argv[i+1],(1-mean_frag[i]),(1-max_frag[i]),mean_meta[i],mean_malloc_time[i],mean_free_time[i]);
		*/
		
		mean_mean_frag += mean_frag[i]/(double)(argc-1);
		mean_max_frag += max_frag[i]/(double)(argc-1);
		mean_mean_meta += mean_meta[i]/(double)(argc-1);
		mean_mean_malloc_time += mean_malloc_time[i]/(double)(argc-1);
		mean_mean_free_time += mean_free_time[i]/(double)(argc-1);
	}
	
	for(int i =0; i < argc-1;i++){
		var_mean_frag += pow(mean_frag[i]-mean_mean_frag,2)/(double)(argc-2);
		var_max_frag += pow(max_frag[i]-mean_max_frag,2)/(double)(argc-2);
		var_mean_meta += pow(mean_meta[i]-mean_mean_meta,2)/(double)(argc-2);
		var_mean_malloc_time += pow(mean_malloc_time[i]-mean_mean_malloc_time,2)/(double)(argc-2);
		var_mean_free_time += pow(mean_free_time[i]-mean_mean_free_time,2)/(double)(argc-2);
	}
	
	std_mean_frag = sqrt(var_mean_frag);
	std_max_frag = sqrt(var_max_frag);
	std_mean_meta = sqrt(var_mean_meta);
	std_mean_malloc_time = sqrt(var_mean_malloc_time);
	std_mean_free_time = sqrt(var_mean_free_time);
	
	printf(
	"mean_std %s= {\n"
			"\tmean_frag = %LF %LF\n"
			"\tmax_frag = %LF %LF\n"
			"\tmean_meta = %LF %LF\n"
			"\tmean_malloc_time = %LF %LF\n"
			"\tmean_free_time = %LF %LF\n"
	"}\n",argv[1],(1-mean_mean_frag), std_mean_frag,
	 (1-mean_max_frag), std_max_frag,
	 mean_mean_meta, std_mean_meta,
	 mean_mean_malloc_time, std_mean_malloc_time,
	 mean_mean_free_time, std_mean_free_time);
		
	
}
