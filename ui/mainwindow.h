#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_ 1

#include "simulationplotmanager.h"
#include "incrementalplot.h"
#include <qmainwindow.h>
#include <qaction.h>

class QSpinBox;
class QPushButton;
class Counter;
class QCheckBox;

class MainWindow: public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(char* simulationData);

private Q_SLOTS:
    void showRunning( bool );
    void appendPoints( bool );
    void showElapsed( int );

private:
    QToolBar *toolBar();

private:
    Counter *d_randomCount;
    Counter *d_timerCount;
    QCheckBox *d_symbolType;
    QAction *d_startAction;
    QAction *d_clearAction;
    IncrementalPlot *d_plot1;
    IncrementalPlot *d_plot2;
    IncrementalPlot *d_plot3;
    IncrementalPlot *d_plot4;
    SimulationPlotManager* manager;
};

#endif
