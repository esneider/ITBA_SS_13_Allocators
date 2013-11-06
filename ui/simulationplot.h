#ifndef _SIMULATIONPLOT_H_
#define _SIMULATIONPLOT_H_ 1

#include "incrementalplot.h"
#include <qdatetime.h>
#include "parser/parser.h"

class QTimer;

class SimulationPlot: public IncrementalPlot
{
    Q_OBJECT

public:
    SimulationPlot( QWidget *parent,char* simulation_data );

    virtual QSize sizeHint() const;

Q_SIGNALS:
    void running( bool );
    void elapsed( int ms );

public Q_SLOTS:
    void clear();
    void stop();
    void append();

private Q_SLOTS:
    void appendPoint();

private:
    void initCurve();

    QTimer *d_timer;
    int d_eventCount;

    QTime d_timeStamp;

    Parser* parser;
    
    struct event* nextEvent;
    int currentTime;
    double timeScale;
};

#endif // _SIMULATIONPLOT_H_
