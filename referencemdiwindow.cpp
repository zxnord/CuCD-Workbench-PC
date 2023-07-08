#include "referencemdiwindow.h"

ReferenceMdiWindow::ReferenceMdiWindow(QVector<AbstractVariable *> farVarVector, QWidget *parent) :
    QMdiSubWindow(parent)
{
    refWidget = new RefControlPanel(farVarVector, this);
    connect(refWidget, SIGNAL(dataReadyForUpdate()), this, SLOT(reEmitSignal()));
    QString title = "References Control Panel";
    setWindowTitle(title);
    setWidget(refWidget);
    setWindowIcon(QIcon(":/images/icon-future.gif"));
}

void ReferenceMdiWindow::reEmitSignal()
{
    emit widgetReadyForUpdate();
}
