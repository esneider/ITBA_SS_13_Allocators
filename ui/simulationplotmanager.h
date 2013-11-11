#ifndef _SIMULATIONPLOTMANAGER_H_
#define _SIMULATIONPLOTMANAGER_H_ 1

#include "incrementalplot.h"
#include <qdatetime.h>
#include "parser/parser.h"

class QTimer;

class SimulationPlotManager: public QObject
{

    Q_OBJECT
    
public:
    SimulationPlotManager(IncrementalPlot* plot1, IncrementalPlot* plot2, IncrementalPlot* plot3, IncrementalPlot* plot4);


Q_SIGNALS:
    void running( bool );
    void elapsed( int ms );


public:
    void append(char*,int);


private Q_SLOTS:
    void appendPoint();

private:

	IncrementalPlot* plot1;
	IncrementalPlot* plot2;
	IncrementalPlot* plot3;
	IncrementalPlot* plot4;

    QTimer *d_timer;
    int d_eventCount;

    Parser* parser;
    
    struct event* nextEvent;
    int currentTime;
    double eventsToSkip;
};

#endif // _SIMULATIONPLOTMANAGER_H_
