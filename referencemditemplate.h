#ifndef REFERENCEMDITEMPLATE_H
#define REFERENCEMDITEMPLATE_H

#include <QObject>
#include <QVector>
#include "abstractvariable.h"

enum PanelDataType{
    PANEL_BOOL,
    PANEL_INT,
    PANEL_DOUBLE
};

class ReferenceMdiTemplate : public QObject
{
    Q_OBJECT
public:
    explicit ReferenceMdiTemplate(QVector<AbstractVariable *> panelVector, QObject *parent = 0);
    
signals:
    
public slots:

private:
    void initPanels(void);

    QString label;
    bool enabled;
    QVector<PanelDataType> panelTypeVector;
    QVector<AbstractVariable *> farPanelVector;
    
};

#endif // REFERENCEMDITEMPLATE_H
