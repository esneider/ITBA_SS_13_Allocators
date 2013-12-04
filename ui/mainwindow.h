#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_ 1

#include "simulationplotmanager.h"
#include "incrementalplot.h"
#include <qmainwindow.h>
#include <qaction.h>
#include <QStackedWidget>
#include <QComboBox>

class QSpinBox;
class QPushButton;
class Counter;
class QCheckBox;
class QLabel;

class MainWindow: public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();


Q_SIGNALS:
    void changeWindow( int );

private Q_SLOTS:
    void showRunning( bool );
    void showElapsed( int );
    void on_plot();
    void newSimulation();
    void appendPoints();
    void waitProgram();

private:
    QToolBar *toolBar();
	QWidget* Menu();
	QWidget* SingleMenu(int color);
	QWidget* CommonMenu();

private:
    Counter* d_skipCount;
    Counter* d_heapsizeCount;
    Counter* d_timerCount;
    QCheckBox *d_symbolType;
    QAction *d_startAction;
    QAction *d_clearAction;
    IncrementalPlot *d_plot1;
    IncrementalPlot *d_plot2;
    IncrementalPlot *d_plot3;
    IncrementalPlot *d_plot4;
    SimulationPlotManager* manager;
    
    QStackedWidget selector;
    
    QComboBox* strategy_combo[2];
    QComboBox* context_combo[2];
    
    QLabel* d_title;
    
    pid_t pID1;
    pid_t pID2;
	int success1;
	int success2;
    
};

#endif
