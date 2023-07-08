#include "referencemditemplate.h"

ReferenceMdiTemplate::ReferenceMdiTemplate(QVector<AbstractVariable *> panelVector, QObject *parent) :
    QObject(parent), farPanelVector(panelVector)
{
    initPanels();
}

void ReferenceMdiTemplate::initPanels()
{
    for(int i=0; i<farPanelVector.size(); i++)
    {
        switch(farPanelVector[i]->getVariableType())
        {
        case AbstractVariable::VAR_ON_OFF:
            panelTypeVector.append(PANEL_BOOL);
            break;
        case AbstractVariable::VAR_INTEGER:
            panelTypeVector.append(PANEL_INT);
            break;
        case AbstractVariable::VAR_DOUBLE:
            panelTypeVector.append(PANEL_DOUBLE);
        }
    }
}
