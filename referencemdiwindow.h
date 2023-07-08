#ifndef REFERENCEMDIWINDOW_H
#define REFERENCEMDIWINDOW_H

#include <QMdiSubWindow>
#include <QVector>
#include <QIcon>
#include "refcontrolpanel.h"

class ReferenceMdiWindow : public QMdiSubWindow
{
    Q_OBJECT
public:
    explicit ReferenceMdiWindow(QVector<AbstractVariable *> farVarVector, QWidget *parent = 0);
    
signals:
    void widgetReadyForUpdate(void);
public slots:
    void reEmitSignal(void);
private:
    RefControlPanel *refWidget;
};

#endif // REFERENCEMDIWINDOW_H
