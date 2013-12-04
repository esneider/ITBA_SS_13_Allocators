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
#include <qtimer.h>

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
    
    QWidget* loading = new QLabel("Calculating...",this);
    
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

QWidget *MainWindow::SingleMenu(int color){

	QWidget *box =new QWidget(this);
	
    
    strategy_combo[color] = new QComboBox(this);
    context_combo[color] = new QComboBox(this);
    
    strategy_combo[color]->addItem("lifo");
    strategy_combo[color]->addItem("best");
    
    context_combo[color]->addItem("excel");
    context_combo[color]->addItem("photoshop");
    context_combo[color]->addItem("safari");
    context_combo[color]->addItem("skype");
    context_combo[color]->addItem("vim");
    
    QFormLayout *layout = new QFormLayout( box );
	layout->addWidget( new QLabel( color==0?"parameters white line:":"parameters red line:", this ) );
    layout->addWidget( strategy_combo[color] );
    layout->addWidget( context_combo[color] );
    
    return box;

}

QWidget *MainWindow::CommonMenu(){

	QWidget *box =new QWidget(this);
	
	
    d_skipCount = new Counter( box, "Skip events", "", 0, 100000, 1000 );
    d_skipCount->setValue( 0 );
    
    d_heapsizeCount = new Counter( box, "Heap size", "", 1000000, 10000000, 1000000 );
    d_heapsizeCount->setValue( 1000000 );
    
    d_timerCount = new Counter( box, "Simulation time", "", 600, 60000, 100 );
    d_timerCount->setValue( 600 );
    
    QFormLayout *layout = new QFormLayout( box );
    layout->addWidget( d_skipCount );
    layout->addWidget( d_heapsizeCount );
    layout->addWidget( d_timerCount );
    
    return box;

}

QWidget *MainWindow::Menu()
{
	QWidget *box1 =new QWidget(this);
	QWidget *box2 =new QWidget(this);
	
	QHBoxLayout * layout2 = new QHBoxLayout(box2);
	
	layout2->addWidget(SingleMenu(0));
	layout2->addWidget(SingleMenu(1));
	
   	QPushButton* plot_button = new QPushButton("Plot",box2);
   	connect(plot_button,SIGNAL(clicked()),this,SLOT(on_plot()));
    
    QVBoxLayout * layout1 = new QVBoxLayout(box1);
    
    layout1->addWidget(CommonMenu());
    layout1->addWidget(box2);
    layout1->addWidget(plot_button);
    
    return box1;
    
}

QToolBar *MainWindow::toolBar()
{
    MyToolBar *toolBar = new MyToolBar( this );

    toolBar->setAllowedAreas( Qt::TopToolBarArea | Qt::BottomToolBarArea );
    setToolButtonStyle( Qt::ToolButtonTextUnderIcon );

    d_startAction = new QAction( "New simulation", toolBar );

	toolBar->addAction(d_startAction);
    d_title = new QLabel( this );
    toolBar->addWidget( d_title );
   
    return toolBar;
}

void MainWindow::on_plot(){
	changeWindow(2);
	
	pID1 = fork();
	
	if(pID1 == 0){
		char program[256];
		char sample[256];
		char heapsize[256];
		char simtime[256];
		
		sprintf(program,"bin/simulator/run_%s",strategy_combo[0]->currentText().toStdString().c_str());
		sprintf(sample,"context/samples/%s.txt",context_combo[0]->currentText().toStdString().c_str());
		sprintf(heapsize,"%d",d_heapsizeCount->value());
		sprintf(simtime,"%d",d_timerCount->value());
		
		printf("%s -c %s ...\n",program,sample);
	
		execlp(program ,program,"-c",sample,"-h",heapsize, "-t",simtime,"-o","bin/tmp1.txt",NULL);
	}
	else if(pID1 < 0){
		changeWindow(3);
	}
	
	pID2 = fork();
	
	if(pID2 == 0){
		char program[256];
		char sample[256];
		char heapsize[256];
		char simtime[256];
		
		sprintf(program,"bin/simulator/run_%s",strategy_combo[1]->currentText().toStdString().c_str());
		sprintf(sample,"context/samples/%s.txt",context_combo[1]->currentText().toStdString().c_str());
		sprintf(heapsize,"%d",d_heapsizeCount->value());
		sprintf(simtime,"%d",d_timerCount->value());
		
		printf("%s -c %s ...\n",program,sample);
	
		execlp(program ,program,"-c",sample,"-h",heapsize, "-t",simtime,"-o","bin/tmp2.txt",NULL);
	}
	else if(pID2 < 0){
		changeWindow(3);
	}
	
	success1 = -54;
	success2 = -54;
	
	QTimer::singleShot(10,this,SLOT( waitProgram() ));
}

void MainWindow::waitProgram(){
	if(success1 == -54)waitpid(pID1,&success1,WNOHANG);
	if(success2 == -54)waitpid(pID2,&success2,WNOHANG);
	
	if(!WIFEXITED(success1) || !WIFEXITED(success2)){
		QTimer::singleShot(10,this,SLOT( waitProgram() ));
		return;
	}
	
	if(WEXITSTATUS(success1) && WEXITSTATUS(success2)){
		changeWindow(3);
		return;
	}
	
	QTimer::singleShot(10,this,SLOT( appendPoints() ));
}


void MainWindow::newSimulation(){
	changeWindow(0);
}

void MainWindow::appendPoints()
{
	changeWindow(1);
	
	char title[512];
	sprintf(title,"white: %s_%s ; red: %s_%s",strategy_combo[0]->currentText().toStdString().c_str(),context_combo[0]->currentText().toStdString().c_str(),strategy_combo[1]->currentText().toStdString().c_str(),context_combo[1]->currentText().toStdString().c_str());
	d_title->setText(title);
	manager->append("bin/tmp1.txt","bin/tmp2.txt",d_skipCount->value());
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


