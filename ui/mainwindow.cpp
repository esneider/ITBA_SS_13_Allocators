#include <qlabel.h>
#include <qlayout.h>
#include <qstatusbar.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qspinbox.h>
#include <qcheckbox.h>
#include <qwhatsthis.h>
#include <qpixmap.h>
#include "mainwindow.h"
#include "start.xpm"
#include "clear.xpm"

class MyToolBar: public QToolBar
{
public:
    MyToolBar( MainWindow *parent ):
        QToolBar( parent )
    {
    }
    void addSpacing( int spacing )
    {
        QLabel *label = new QLabel( this );
        addWidget( label );
        label->setFixedWidth( spacing );
    }
};

class Counter: public QWidget
{
public:
    Counter( QWidget *parent,
            const QString &prefix, const QString &suffix,
            int min, int max, int step ):
        QWidget( parent )
    {
        QHBoxLayout *layout = new QHBoxLayout( this );

        if ( !prefix.isEmpty() )
            layout->addWidget( new QLabel( prefix + " ", this ) );

        d_counter = new QSpinBox( this );
        d_counter->setRange( min, max );
        d_counter->setSingleStep( step );
        layout->addWidget( d_counter );

        if ( !suffix.isEmpty() )
            layout->addWidget( new QLabel( QString( " " ) + suffix, this ) );
    }

    void setValue( int value ) { d_counter->setValue( value ); }
    int value() const { return d_counter->value(); }

private:
    QSpinBox *d_counter;
};

MainWindow::MainWindow(char* simulationData)
{
    addToolBar( toolBar() );
#ifndef QT_NO_STATUSBAR
    ( void )statusBar();
#endif

    d_plot = new SimulationPlot( this, simulationData );
    const int margin = 4;
    d_plot->setContentsMargins( margin, margin, margin, margin );

    setCentralWidget( d_plot );

    connect( d_startAction, SIGNAL( toggled( bool ) ), this, SLOT( appendPoints( bool ) ) );
    connect( d_plot, SIGNAL( running( bool ) ), this, SLOT( showRunning( bool ) ) );
    connect( d_plot, SIGNAL( elapsed( int ) ), this, SLOT( showElapsed( int ) ) );
    
    setContextMenuPolicy( Qt::NoContextMenu );
}

QToolBar *MainWindow::toolBar()
{
    MyToolBar *toolBar = new MyToolBar( this );

    toolBar->setAllowedAreas( Qt::TopToolBarArea | Qt::BottomToolBarArea );
    setToolButtonStyle( Qt::ToolButtonTextUnderIcon );

    d_startAction = new QAction( QPixmap( start_xpm ), "Start", toolBar );
    d_startAction->setCheckable( true );

    toolBar->addAction( d_startAction );

    setIconSize( QSize( 22, 22 ) );

    QWidget *hBox = new QWidget( toolBar );
    
    d_timerCount = new Counter( hBox, "Time scale", "", 1, 1000, 5 );
    d_timerCount->setValue( 0 );

    QHBoxLayout *layout = new QHBoxLayout( hBox );
    layout->addWidget( d_timerCount );

    showRunning( false );

    toolBar->addWidget( hBox );

    return toolBar;
}

void MainWindow::appendPoints( bool on )
{
    if ( on )
        d_plot->append((double)d_timerCount->value());
    else
        d_plot->stop();
}

void MainWindow::showRunning( bool running )
{
    d_timerCount->setEnabled( !running );
    d_startAction->setChecked( running );
    d_startAction->setText( running ? "Stop" : "Start" );
}

void MainWindow::showElapsed( int ms )
{
    QString text;
    text.setNum( ms );
    text += " ms";

    statusBar()->showMessage( text );
}


