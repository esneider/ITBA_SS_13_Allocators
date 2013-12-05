#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "../../ui/parser/parser.h"

int main(int argc, char **argv) {

	if(argc<4){
		printf("usage: analyze_data output{1:mean frag; 2: max frag; 3: mean metadata; 4:mean malloc time; 5: mean free time 6: all} [imputfile1] [imputfile2] ...");
		exit(2);
	}
	
	int num_tests = argc -2;
	
	long double* mean_frag  = new long double[num_tests];
	long double* max_frag = new long double[num_tests];
	long double* mean_meta = new long double[num_tests];
	long double* mean_malloc_time = new long double[num_tests];
	long double* mean_free_time = new long double[num_tests];


	
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
	

	for(int i =0; i < num_tests;i++){
		mean_frag[i] = 0;
		max_frag[i] = 1;
		mean_meta[i] = 0;
		mean_malloc_time[i] = 0;
		mean_free_time[i] = 0;


		Parser* parser = new Parser(argv[i+2]);

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
		"}\n",argv[i+2],(1-mean_frag[i]),(1-max_frag[i]),mean_meta[i],mean_malloc_time[i],mean_free_time[i]);
		*/
		
		mean_mean_frag += mean_frag[i]/(double)(num_tests);
		mean_max_frag += max_frag[i]/(double)(num_tests);
		mean_mean_meta += mean_meta[i]/(double)(num_tests);
		mean_mean_malloc_time += mean_malloc_time[i]/(double)(num_tests);
		mean_mean_free_time += mean_free_time[i]/(double)(num_tests);
	}
	
	for(int i =0; i < num_tests;i++){
		var_mean_frag += pow(mean_frag[i]-mean_mean_frag,2)/(double)(num_tests-1);
		var_max_frag += pow(max_frag[i]-mean_max_frag,2)/(double)(num_tests-1);
		var_mean_meta += pow(mean_meta[i]-mean_mean_meta,2)/(double)(num_tests-1);
		var_mean_malloc_time += pow(mean_malloc_time[i]-mean_mean_malloc_time,2)/(double)(num_tests-1);
		var_mean_free_time += pow(mean_free_time[i]-mean_mean_free_time,2)/(double)(num_tests-1);
	}
	
	std_mean_frag = sqrt(var_mean_frag);
	std_max_frag = sqrt(var_max_frag);
	std_mean_meta = sqrt(var_mean_meta);
	std_mean_malloc_time = sqrt(var_mean_malloc_time);
	std_mean_free_time = sqrt(var_mean_free_time);
	
	/*printf(
	"mean_std %s= {\n"
			"\tmean_frag = %LF %LF\n"
			"\tmax_frag = %LF %LF\n"
			"\tmean_meta = %LF %LF\n"
			"\tmean_malloc_time = %LF %LF\n"
			"\tmean_free_time = %LF %LF\n"
	"}\n",argv[2],(1-mean_mean_frag), std_mean_frag,
	 (1-mean_max_frag), std_max_frag,
	 mean_mean_meta, std_mean_meta,
	 mean_mean_malloc_time, std_mean_malloc_time,
	 mean_mean_free_time, std_mean_free_time);*/
	 
	switch(argv[1][0]){
		case'1': 
			printf("%LF %LF\n",
			 (1-mean_mean_frag), std_mean_frag);
			break;
		case'2': 
			printf("%LF %LF\n",
			 (1-mean_max_frag), std_max_frag);
			break;
		case'3': 
			printf("%LF %LF\n",
			 mean_mean_meta, std_mean_meta);
			break;
		case'4': 
			printf("%LF %LF\n",
			 mean_mean_malloc_time, std_mean_malloc_time);
			break;
		case'5': 
			printf("%LF %LF\n",
			 mean_mean_free_time, std_mean_free_time);
			break;
		case'6': 
			printf(
			"mean_std %s= {\n"
					"\tmean_frag = %LF %LF\n"
					"\tmax_frag = %LF %LF\n"
					"\tmean_meta = %LF %LF\n"
					"\tmean_malloc_time = %LF %LF\n"
					"\tmean_free_time = %LF %LF\n"
			"}\n",argv[2],(1-mean_mean_frag), std_mean_frag,
			 (1-mean_max_frag), std_max_frag,
			 mean_mean_meta, std_mean_meta,
			 mean_mean_malloc_time, std_mean_malloc_time,
			 mean_mean_free_time, std_mean_free_time);
			break;
		default:
			printf("usage: analyze_data output{1:mean frag; 2: max frag; 3: mean metadata; 4:mean malloc time; 5: mean free time 6: all} [imputfile1] [imputfile2] ...");
			exit(2);
			break;
	}	
}
