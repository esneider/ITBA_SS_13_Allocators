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
    void append(char* simulation_data1,char* simulation_data2, int skip);


private Q_SLOTS:
    void appendPoint();

private:

	IncrementalPlot* plot1;
	IncrementalPlot* plot2;
	IncrementalPlot* plot3;
	IncrementalPlot* plot4;

    QTimer *d_timer;
    int d_eventCount;

    Parser* parser1;
    Parser* parser2;
    
    struct event* nextEvent1;
    struct event* nextEvent2;
    int currentTime;
    int eventsToSkip;
    int curve1;
    int curve2;
};

#endif // _SIMULATIONPLOTMANAGER_H_
