#include <qtimer.h>
#include "simulationplotmanager.h"

SimulationPlotManager::SimulationPlotManager(IncrementalPlot* p1, IncrementalPlot* p2, IncrementalPlot* p3, IncrementalPlot* p4 ):
    d_eventCount( 0 ),
    currentTime( 0 ),
    parser1( 0 ),
    parser2( 0 ),
    curve1(1),
    curve2(2)
{
    
    plot1 = p1;
    plot2 = p2;
    plot3 = p3;
    plot4 = p4;
    
    plot1->setTitle("Time free");
    plot1->setAxisTitle(0,"time");
    plot1->setAxisTitle(2,"event");
    plot1->setAxisScale(0,0,25);
    
    plot2->setTitle("Time malloc");
    plot2->setAxisTitle(0,"time");
    plot2->setAxisScale(0,0,25);
    plot2->setAxisTitle(2,"event");
    
    plot3->setTitle("Metadata");
    plot3->setAxisTitle(0,"%");
    plot3->setAxisTitle(2,"event");
    plot3->setAxisScale(0,0,0.1);
    
    plot4->setTitle("External fragmentation");
    plot4->setAxisTitle(0,"%");
    plot4->setAxisTitle(2,"event");
    plot4->setAxisScale(0,0,1);
    
}



void SimulationPlotManager::appendPoint()
{

	currentTime= nextEvent1->number;
	
    //Q_EMIT elapsed( currentTime );

    //plot1->appendPoint( QPointF( currentTime, nextEvent->size ) );
    if(nextEvent1 !=NULL){
		currentTime= nextEvent1->number;
		if(strcmp(nextEvent1->type,"free")==0){
			plot1->appendPoint( QPointF( currentTime, nextEvent1->execution ), curve1);
		}
		else{
			plot2->appendPoint( QPointF( currentTime, nextEvent1->execution ), curve1);
		}
		plot3->appendPoint( QPointF( currentTime, nextEvent1->metadata ), curve1);
		plot4->appendPoint( QPointF( currentTime, ( 1 - nextEvent1->fragmentation) ), curve1);
		free(nextEvent1);
	}
	if(nextEvent2 !=NULL){
		currentTime= nextEvent2->number;
		if(strcmp(nextEvent2->type,"free")==0){
			plot1->appendPoint( QPointF( currentTime, nextEvent2->execution ), curve2);
		}
		else{
			plot2->appendPoint( QPointF( currentTime, nextEvent2->execution ), curve2);
		}
		plot3->appendPoint( QPointF( currentTime, nextEvent2->metadata ), curve2);
		plot4->appendPoint( QPointF( currentTime, ( 1 - nextEvent2->fragmentation) ), curve2);
		free(nextEvent2);
	}


	d_eventCount-=eventsToSkip;
    if ( d_eventCount > 0 ){
		nextEvent1 = parser1->getNextEvent(eventsToSkip-1);
		nextEvent2 = parser2->getNextEvent(eventsToSkip-1);
	    QTimer::singleShot(10,this,SLOT( appendPoint() ));
	}
}

void SimulationPlotManager::append(char* simulation_data1,char* simulation_data2, int skip)
{
	plot1->clearPoints(curve1);
    plot2->clearPoints(curve1);
    plot3->clearPoints(curve1);
    plot4->clearPoints(curve1);
	plot1->clearPoints(curve2);
    plot2->clearPoints(curve2);
    plot3->clearPoints(curve2);
    plot4->clearPoints(curve2);

	if(parser1){
		delete parser1;
	}
   	parser1 = new Parser(simulation_data1);
   	
   	if(parser2){
		delete parser2;
	}
   	parser2 = new Parser(simulation_data2);
   	
   	
    plot1->setAxisScale(2,0,max(parser1->getSimulation()->num_events,parser2->getSimulation()->num_events));
    plot2->setAxisScale(2,0,max(parser1->getSimulation()->num_events,parser2->getSimulation()->num_events));
    plot3->setAxisScale(2,0,max(parser1->getSimulation()->num_events,parser2->getSimulation()->num_events));
    plot4->setAxisScale(2,0,max(parser1->getSimulation()->num_events,parser2->getSimulation()->num_events));
    plot1->replot();
    plot2->replot();
    plot3->replot();
    plot4->replot();
    
   	
	eventsToSkip = skip;
	
    Q_EMIT running( true );
    
    d_eventCount = max(parser1->getSimulation()->num_events,parser2->getSimulation()->num_events);

	nextEvent1 = parser1->getNextEvent(0);
	nextEvent2 = parser2->getNextEvent(0);
	QTimer::singleShot(10,this,SLOT( appendPoint() ));
}
