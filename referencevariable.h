#ifndef REFERENCEVARIABLE_H
#define REFERENCEVARIABLE_H
#include "abstractvariable.h"

template<typename TYPE>
class ReferenceVariable : public AbstractVariable
{

public:
    ReferenceVariable(AbstractVariable::DataType variableType, TYPE minValue, TYPE maxValue, QString name):
        varType(variableType), minRange(minValue), maxRange(maxValue), varName(name){ }
    TYPE getMaxValue(void){ return maxRange; }
    TYPE getMinValue(void){ return minRange; }
    TYPE getVariableValue(void){ return varValue; }
    QString getVariableName(void){ return varName; }
    AbstractVariable::DataType getVariableType(void){ return varType; }
    void setVariableName(QString name){ varName = name; }
    void setVariableValue(TYPE value){ varValue = value; emitDynamicSignal("valueChanged()", 0); }

private:
    TYPE varValue;
    TYPE minRange;
    TYPE maxRange;
    QString varName;
    AbstractVariable::DataType varType;
};

#endif // REFERENCEVARIABLE_H
