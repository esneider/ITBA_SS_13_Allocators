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
    
    plot1->setTitle("Block size");
    plot1->setAxisTitle(0,"size");
    plot1->setAxisTitle(2,"time");
    plot1->setAxisAutoScale(0,true);
    
    plot2->setTitle("Metadata");
    plot2->setAxisTitle(0,"%");
    plot2->setAxisScale(0,0,1);
    plot2->setAxisTitle(2,"time");
    
    plot3->setTitle("Time expended");
    plot3->setAxisTitle(0,"%");
    plot3->setAxisTitle(2,"time");
    plot3->setAxisScale(0,0,10);
    
    plot4->setTitle("External fragmentation");
    plot4->setAxisTitle(0,"time-processing");
    plot4->setAxisTitle(2,"%");
    plot4->setAxisScale(0,0,1);
    
}



void SimulationPlotManager::appendPoint()
{

	currentTime= nextEvent->number;
	
    //Q_EMIT elapsed( currentTime );

    plot1->appendPoint( QPointF( currentTime, nextEvent->size ) );
    plot2->appendPoint( QPointF( currentTime, nextEvent->metadata ) );
    plot3->appendPoint( QPointF( currentTime, nextEvent->execution ) );
    plot4->appendPoint( QPointF( currentTime, ( 1 - nextEvent->fragmentation) ) );

	free(nextEvent);
	d_eventCount-=eventsToSkip;
    if ( d_eventCount > 0 ){
		nextEvent = parser->getNextEvent(eventsToSkip);
	    QTimer::singleShot(10,this,SLOT( appendPoint() ));
	}
}

void SimulationPlotManager::append(char* simulation_data, int skip)
{


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
