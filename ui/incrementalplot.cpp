#include <qwt_plot.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>
#include <qwt_plot_directpainter.h>
#include <qwt_painter.h>
#include "incrementalplot.h"
#include <qpaintengine.h>



#include <qwt_plot_grid.h>
#include <qwt_plot_layout.h>
#include <qwt_scale_draw.h>

class CurveData: public QwtArraySeriesData<QPointF>
{
public:
    CurveData()
    {
    }

    virtual QRectF boundingRect() const
    {
        if ( d_boundingRect.width() < 0.0 )
            d_boundingRect = qwtBoundingRect( *this );

        return d_boundingRect;
    }

    inline void append( const QPointF &point )
    {
        d_samples += point;
    }

    void clear()
    {
        d_samples.clear();
        d_samples.squeeze();
        d_boundingRect = QRectF( 0.0, 0.0, -1.0, -1.0 );
    }
};

IncrementalPlot::IncrementalPlot( QWidget *parent ):
    QwtPlot( parent ),
    d_curve1( NULL ),
    d_curve2( NULL )
{
    d_directPainter = new QwtPlotDirectPainter( this );

    if ( QwtPainter::isX11GraphicsSystem() )
    {
#if QT_VERSION < 0x050000
        canvas()->setAttribute( Qt::WA_PaintOutsidePaintEvent, true );
#endif
        canvas()->setAttribute( Qt::WA_PaintOnScreen, true );
    }

    d_curve1 = new QwtPlotCurve( "Curve1" );
    d_curve1->setData( new CurveData() );
    d_curve2 = new QwtPlotCurve( "Curve2" );
    d_curve2->setData( new CurveData() );
    showSymbols( true );

    d_curve1->attach( this );
    d_curve2->attach( this );

    setAutoReplot( false );
    
    setFrameStyle( QFrame::NoFrame );
    setLineWidth( 0 );

   plotLayout()->setAlignCanvasToScales( true );

    QwtPlotGrid *grid = new QwtPlotGrid;
    grid->setMajorPen( Qt::gray, 0, Qt::DotLine );
    grid->attach( this );

    setCanvasBackground( QColor( 29, 100, 141 ) ); // nice blue
    
    replot();
}

IncrementalPlot::~IncrementalPlot()
{
    delete d_curve1;
    delete d_curve2;
}

void IncrementalPlot::appendPoint( const QPointF &point , int curve )
{

	QwtPlotCurve* d_curve;
	if(curve == 1){
		d_curve = d_curve1;
	}
	else if(curve == 2){
		d_curve = d_curve2;
	}
	else{
		return;
	}

    CurveData *data = static_cast<CurveData *>( d_curve->data() );
    data->append( point );

    const bool doClip = !canvas()->testAttribute( Qt::WA_PaintOnScreen );
    if ( doClip )
    {
        /*
           Depending on the platform setting a clip might be an important
           performance issue. F.e. for Qt Embedded this reduces the
           part of the backing store that has to be copied out - maybe
           to an unaccelerated frame buffer device.
         */
        const QwtScaleMap xMap = canvasMap( d_curve->xAxis() );
        const QwtScaleMap yMap = canvasMap( d_curve->yAxis() );

        QRegion clipRegion;

        const QSize symbolSize = d_curve->symbol()->size();
        QRect r( 0, 0, symbolSize.width() + 2, symbolSize.height() + 2 );

        const QPointF center =
            QwtScaleMap::transform( xMap, yMap, point );
        r.moveCenter( center.toPoint() );
        clipRegion += r;

        d_directPainter->setClipRegion( clipRegion );
    }

    d_directPainter->drawSeries( d_curve,
        data->size() - 1, data->size() - 1 );
}

void IncrementalPlot::clearPoints(int curve)
{

	QwtPlotCurve* d_curve;
	if(curve == 1){
		d_curve = d_curve1;
	}
	else if(curve == 2){
		d_curve = d_curve2;
	}
	else{
		return;
	}

    CurveData *data = static_cast<CurveData *>( d_curve->data() );
    data->clear();

    replot();
}

void IncrementalPlot::showSymbols( bool on )
{
    d_curve1->setStyle( QwtPlotCurve::Dots );
	d_curve1->setRenderHint( QwtPlotItem::RenderAntialiased, true );
    d_curve1->setSymbol( new QwtSymbol( QwtSymbol::XCross,
    	Qt::NoBrush, QPen( Qt::white ), QSize( 4, 4 ) ) );

	d_curve2->setStyle( QwtPlotCurve::Dots );
	d_curve2->setRenderHint( QwtPlotItem::RenderAntialiased, true );
    d_curve2->setSymbol( new QwtSymbol( QwtSymbol::XCross,
    	Qt::NoBrush, QPen( Qt::red ), QSize( 4, 4 ) ) );

    replot();
}
