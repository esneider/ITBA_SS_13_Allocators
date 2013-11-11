#include <qtimer.h>
#include "simulationplotmanager.h"

SimulationPlotManager::SimulationPlotManager(IncrementalPlot* p1, IncrementalPlot* p2, IncrementalPlot* p3, IncrementalPlot* p4 ):
    d_eventCount( 0 ),
    currentTime( 0 ),
    parser( 0 )
{
    
    plot1 = p1;
    plot2 = p2;
    plot3 = p3;
    plot4 = p4;
    
    plot1->setTitle("Time free");
    plot1->setAxisTitle(0,"time");
    plot1->setAxisTitle(2,"event");
    plot1->setAxisScale(0,0,10);
    
    plot2->setTitle("Time malloc");
    plot2->setAxisTitle(0,"time");
    plot2->setAxisScale(0,0,10);
    plot2->setAxisTitle(2,"event");
    
    plot3->setTitle("Metadata");
    plot3->setAxisTitle(0,"%");
    plot3->setAxisTitle(2,"event");
    plot3->setAxisScale(0,0,1);
    
    plot4->setTitle("External fragmentation");
    plot4->setAxisTitle(0,"%");
    plot4->setAxisTitle(2,"event");
    plot4->setAxisScale(0,0,1);
    
}



void SimulationPlotManager::appendPoint()
{

	currentTime= nextEvent->number;
	
    //Q_EMIT elapsed( currentTime );

    //plot1->appendPoint( QPointF( currentTime, nextEvent->size ) );
    if(strcmp(nextEvent->type,"free")==0){
	    plot1->appendPoint( QPointF( currentTime, nextEvent->execution ), curve);
	}
	else{
	    plot2->appendPoint( QPointF( currentTime, nextEvent->execution ), curve);
	}
    
    
    plot3->appendPoint( QPointF( currentTime, nextEvent->metadata ), curve);
    plot4->appendPoint( QPointF( currentTime, ( 1 - nextEvent->fragmentation) ), curve);

	free(nextEvent);
	d_eventCount-=eventsToSkip;
    if ( d_eventCount > 0 ){
		nextEvent = parser->getNextEvent(eventsToSkip-1);
	    QTimer::singleShot(10,this,SLOT( appendPoint() ));
	}
}

void SimulationPlotManager::append(char* simulation_data, int skip, int curv)
{
	curve = curv;

	plot1->clearPoints(curve);
    plot2->clearPoints(curve);
    plot3->clearPoints(curve);
    plot4->clearPoints(curve);

	if(parser){
		delete parser;
	}
   	parser = new Parser(simulation_data);
   	
   	
    plot1->setAxisScale(2,0,parser->getSimulation()->num_events);
    plot2->setAxisScale(2,0,parser->getSimulation()->num_events);
    plot3->setAxisScale(2,0,parser->getSimulation()->num_events);
    plot4->setAxisScale(2,0,parser->getSimulation()->num_events);
    plot1->replot();
    plot2->replot();
    plot3->replot();
    plot4->replot();
    
   	
	eventsToSkip = skip;
	
    Q_EMIT running( true );
    
    d_eventCount = parser->getSimulation()->num_events;

	nextEvent = parser->getNextEvent(0);
	QTimer::singleShot(10,this,SLOT( appendPoint() ));
}
