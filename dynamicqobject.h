#ifndef DYNAMICQOBJECT_H
#define DYNAMICQOBJECT_H

#include <QHash>
#include <QList>
#include <QMetaObject>
#include <QObject>

class DynamicQObject : public QObject
{
public:
    DynamicQObject(QObject *parent = 0):QObject(parent){ }
    virtual int qt_metacall(QMetaObject::Call c, int id, void **arguments);

    bool emitDynamicSignal(char *signal, void **arguments);
    //bool connectDynamicSlot(QObject *obj, char *signal, char *slot);
    bool connectDynamicSignal(char *signal, QObject *obj, char *slot);

    //virtual DynamicSlot *createSlot(char *slot) = 0;

private:
    //QHash<QByteArray, int> slotIndices;
    //QList<DynamicSlot *> slotList;
    QHash<QByteArray, int> signalIndices;
    
};

#endif // DYNAMICQOBJECT_H
