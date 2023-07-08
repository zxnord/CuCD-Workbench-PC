#ifndef ABSTRACTVARIABLE_H
#define ABSTRACTVARIABLE_H

#include "dynamicqobject.h"
#include <QString>

class AbstractVariable: public DynamicQObject
{

public:
    enum DataType{
        VAR_ON_OFF,
        VAR_INTEGER,
        VAR_DOUBLE
    };

    virtual DataType getVariableType(void)=0;
    virtual void setVariableName(QString name)=0;
    virtual QString getVariableName(void)=0;

};
#endif // ABSTRACTVARIABLE_H
