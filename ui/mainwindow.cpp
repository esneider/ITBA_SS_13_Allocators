#include <qlabel.h>
#include <qlayout.h>
#include <qstatusbar.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qspinbox.h>
#include <qcheckbox.h>
#include <qwhatsthis.h>
#include <qpixmap.h>
#include <QFormLayout>
#include <QPushButton>

#include <sys/wait.h>

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

MainWindow::MainWindow()
{
    addToolBar( toolBar() );
#ifndef QT_NO_STATUSBAR
    ( void )statusBar();
#endif

    d_plot1 = new IncrementalPlot( this ); 
    d_plot2 = new IncrementalPlot( this ); 
    d_plot3 = new IncrementalPlot( this ); 
    d_plot4 = new IncrementalPlot( this );   
    
    QWidget *box = new QWidget( this);
    QGridLayout *layout = new QGridLayout( box );
    layout->addWidget( d_plot1 ,0,0 );
    layout->addWidget( d_plot2 ,0,1);
    layout->addWidget( d_plot3 ,1,0);
    layout->addWidget( d_plot4 ,1,1);
    
    
    manager = new SimulationPlotManager(d_plot1,d_plot2,d_plot3,d_plot4);
    
    selector.addWidget(Menu());
    selector.addWidget(box);
    
    QWidget* loading = new QLabel("Calculationg...",this);
    
    selector.addWidget(loading);
    
    QWidget* error = new QLabel("ERROR",this);
    
    selector.addWidget(error);
    
    
    connect(this,SIGNAL(changeWindow(int)),&selector,SLOT(setCurrentIndex(int)));
    
    changeWindow(0);
    
    const int margin = 4;
    d_plot1->setContentsMargins( margin, margin, margin, margin );
    d_plot2->setContentsMargins( margin, margin, margin, margin );
    d_plot3->setContentsMargins( margin, margin, margin, margin );
    d_plot4->setContentsMargins( margin, margin, margin, margin );

    setCentralWidget( &selector );

    connect( d_startAction, SIGNAL( triggered() ), this, SLOT( newSimulation() ) );
    connect( manager, SIGNAL( running( bool ) ), this, SLOT( showRunning( bool ) ) );
    connect( manager, SIGNAL( elapsed( int ) ), this, SLOT( showElapsed( int ) ) );
    
    setContextMenuPolicy( Qt::NoContextMenu );
}

QWidget *MainWindow::Menu()
{
	QWidget *box =new QWidget(this);
	
	
    d_skipCount = new Counter( box, "Skip events", "", 0, 100000, 1000 );
    d_skipCount->setValue( 0 );
    
    d_heapsizeCount = new Counter( box, "Heap size", "", 1000000, 10000000, 1000000 );
    d_heapsizeCount->setValue( 1000000 );
    
    d_timerCount = new Counter( box, "Simulation time", "", 600, 60000, 100 );
    d_timerCount->setValue( 600 );
	
    
    strategy_combo = new QComboBox(this);
    context_combo = new QComboBox(this);
    curve_combo = new QComboBox(this);
    
    strategy_combo->addItem("lifo");
    strategy_combo->addItem("best");
    
    context_combo->addItem("excel");
    context_combo->addItem("photoshop");
    context_combo->addItem("safari");
    context_combo->addItem("skype");
    context_combo->addItem("vim");
    
    
    curve_combo->addItem("white");
    curve_combo->addItem("red");
    
   	QPushButton* plot_button = new QPushButton("Plot",box);
   	connect(plot_button,SIGNAL(clicked()),this,SLOT(on_plot()));
    
    QFormLayout *layout = new QFormLayout( box );
    layout->addWidget( d_skipCount );
    layout->addWidget( d_heapsizeCount );
    layout->addWidget( d_timerCount );
    layout->addWidget( strategy_combo );
    layout->addWidget( context_combo );
    layout->addWidget( curve_combo );
    layout->addWidget( plot_button );
    
    return box;
    
}

QToolBar *MainWindow::toolBar()
{
    MyToolBar *toolBar = new MyToolBar( this );

    toolBar->setAllowedAreas( Qt::TopToolBarArea | Qt::BottomToolBarArea );
    setToolButtonStyle( Qt::ToolButtonTextUnderIcon );

    d_startAction = new QAction( "New simulation", toolBar );

	toolBar->addAction(d_startAction);
   
    return toolBar;
}

void MainWindow::on_plot(){
	changeWindow(2);
	
	bool ok = true;
	
	pid_t pID = fork();
	
	if(pID == 0){
		char program[256];
		char sample[256];
		char heapsize[256];
		char simtime[256];
		
		sprintf(program,"bin/simulator/run_%s",strategy_combo->currentText().toStdString().c_str());
		sprintf(sample,"context/samples/%s.txt",context_combo->currentText().toStdString().c_str());
		sprintf(heapsize,"%d",d_heapsizeCount->value());
		sprintf(simtime,"%d",d_timerCount->value());
		
		printf("%s -c %s ...\n",program,sample);
	
		execlp(program ,program,"-c",sample,"-h",heapsize, "-t",simtime,"-o","bin/tmp.txt",NULL);
	}
	else if(pID < 0){
		changeWindow(3);
		ok = false;
	}
	
	int success;
	waitpid(pID,&success,0);
	if(WEXITSTATUS(success)){
		changeWindow(3);
		ok = false;
	}
	
	
	if(ok){
		changeWindow(1);
		appendPoints();
	}
}


void MainWindow::newSimulation(){
	changeWindow(0);
}

void MainWindow::appendPoints()
{
	manager->append("bin/tmp.txt",d_skipCount->value(),curve_combo->currentIndex()+1);
}

void MainWindow::showRunning( bool running )
{
    d_startAction->setEnabled( running );
}

void MainWindow::showElapsed( int ms )
{
    QString text;
    text.setNum( ms );
    text += " ms";

    statusBar()->showMessage( text );
}


