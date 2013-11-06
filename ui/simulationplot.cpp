#include <qglobal.h>
#include <qtimer.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_layout.h>
#include <qwt_scale_draw.h>
#include "scrollzoomer.h"
#include "simulationplot.h"

SimulationPlot::SimulationPlot( QWidget *parent, char* simulation_data ):
    IncrementalPlot( parent ),
    d_timer( 0 ),
    d_eventCount( 0 ),
    currentTime( 0 ),
    timeScale( 100.0 )
{
    
   	parser = new Parser(simulation_data);
    
    setFrameStyle( QFrame::NoFrame );
    setLineWidth( 0 );

    plotLayout()->setAlignCanvasToScales( true );

    QwtPlotGrid *grid = new QwtPlotGrid;
    grid->setMajorPen( Qt::gray, 0, Qt::DotLine );
    grid->attach( this );

    setCanvasBackground( QColor( 29, 100, 141 ) ); // nice blue

    //setAxisScale( xBottom, 0, c_rangeMax );
    //setAxisScale( yLeft, 0, c_rangeMax );

    replot();
}

QSize SimulationPlot::sizeHint() const
{
    return QSize( 540, 400 );
}



void SimulationPlot::appendPoint()
{
	d_timer->stop();
	currentTime= nextEvent->time;

    IncrementalPlot::appendPoint( QPointF( currentTime, nextEvent->size ) );

	free(nextEvent);
    if ( --d_eventCount > 0 ){
		nextEvent = parser->getNextEvent();
	    d_timer->start( (nextEvent->time - currentTime) * timeScale);
	}
}

void SimulationPlot::append(double ts)
{
	timeScale = ts;
    if ( !d_timer )
    {
        d_timer = new QTimer( this );
        connect( d_timer, SIGNAL( timeout() ), SLOT( appendPoint() ) );
    }

    d_eventCount = parser->getSimulation()->num_events;

    Q_EMIT running( true );
    d_timeStamp.start();

    QwtPlotCanvas *plotCanvas = qobject_cast<QwtPlotCanvas *>( canvas() );
    plotCanvas->setPaintAttribute( QwtPlotCanvas::BackingStore, false );

	nextEvent = parser->getNextEvent();
    d_timer->start( (nextEvent->time - currentTime) * timeScale);
}

void SimulationPlot::stop()
{
    Q_EMIT elapsed( d_timeStamp.elapsed() );

    if ( d_timer )
    {
        d_timer->stop();
        Q_EMIT running( false );
    }

    QwtPlotCanvas *plotCanvas = qobject_cast<QwtPlotCanvas *>( canvas() );
    plotCanvas->setPaintAttribute( QwtPlotCanvas::BackingStore, true );
}

void SimulationPlot::clear()
{
    clearPoints();
    replot();
}
