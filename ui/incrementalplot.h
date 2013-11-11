#ifndef _INCREMENTALPLOT_H_
#define _INCREMENTALPLOT_H_ 1

#include <qwt_plot.h>

class QwtPlotCurve;
class QwtPlotDirectPainter;

class IncrementalPlot : public QwtPlot
{
    Q_OBJECT

public:
    IncrementalPlot( QWidget *parent = NULL );
    virtual ~IncrementalPlot();

    void appendPoint( const QPointF & ,int curve);
    void clearPoints(int curve);

public Q_SLOTS:
    void showSymbols( bool );

private:
    QwtPlotCurve *d_curve1;
    QwtPlotCurve *d_curve2;
    QwtPlotDirectPainter *d_directPainter;
};

#endif // _INCREMENTALPLOT_H_
