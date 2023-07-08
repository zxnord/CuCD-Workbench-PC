#ifndef SUBWINDOWPLOT_H
#define SUBWINDOWPLOT_H

#include <QWidget>
#include <QtCore>
#include <QtGui>
#include "3rd-party/qcustomplot.h"

namespace Ui {
class SubWindowPlot;
}

class SubWindowPlot : public QWidget
{
    Q_OBJECT
    
public slots:
    //void updatePlot(void);
    void mousePress(void);
    void mouseWheel(void);
    void titleDoubleClick(void);
    void axisLabelDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part);
    void legendDoubleClick(QCPLegend *legend, QCPAbstractLegendItem *item);
    void contextMenuRequest(QPoint pos);
    void selectionChanged(void);
    void graphClicked(QCPAbstractPlottable *plottable);
    void removeSelectedGraph(void);
    void removeAllGraphs(void);
    void moveLegend(void);

    void addRandomGraph(void); //must be changed!!

public:
    explicit SubWindowPlot(QWidget *parent = 0);
    ~SubWindowPlot();
    
private:
    Ui::SubWindowPlot *ui;
};

#endif // SUBWINDOWPLOT_H
