#include <qtimer.h>
#include "simulationplotmanager.h"

SimulationPlotManager::SimulationPlotManager(IncrementalPlot* p1, IncrementalPlot* p2, IncrementalPlot* p3, IncrementalPlot* p4, char* simulation_data ):
    d_eventCount( 0 ),
    currentTime( 0 ),
    timeScale( 100.0 )
{
    
    plot1 = p1;
    plot2 = p2;
    plot3 = p3;
    plot4 = p4;
    
   	parser = new Parser(simulation_data);
}



void SimulationPlotManager::appendPoint()
{
	currentTime= nextEvent->time;
	
    Q_EMIT elapsed( currentTime );

    plot1->appendPoint( QPointF( currentTime, nextEvent->size ) );

	free(nextEvent);
    if ( --d_eventCount > 0 ){
		nextEvent = parser->getNextEvent();
	    QTimer::singleShot((nextEvent->time - currentTime) * timeScale,this,SLOT( appendPoint() ));
	}
}

void SimulationPlotManager::append(double ts)
{
	timeScale = ts;
	
    Q_EMIT running( true );
    
    d_eventCount = parser->getSimulation()->num_events;

	nextEvent = parser->getNextEvent();
	QTimer::singleShot((nextEvent->time - currentTime) * timeScale,this,SLOT( appendPoint() ));
}
