#include "dynamicqobject.h"

int DynamicQObject::qt_metacall(QMetaObject::Call c, int id, void **arguments)
{
    id = QObject::qt_metacall(c, id, arguments);
    if (id < 0 || c != QMetaObject::InvokeMetaMethod)
        return id;
    //Q_ASSERT(id < slotList.size());

    //slotList[id]->call(sender(), arguments);
    return -1;
}

bool DynamicQObject::emitDynamicSignal(char *signal, void **arguments)
{
    QByteArray theSignal = QMetaObject::normalizedSignature(signal);
    int signalId = signalIndices.value(theSignal, -1);
    if (signalId >= 0) {
        QMetaObject::activate(this, metaObject(), signalId + metaObject()->methodCount(),
            arguments);
        return true;
    } else {
        return false;
    }
}

bool DynamicQObject::connectDynamicSignal(char *signal, QObject *obj, char *slot)
{
    QByteArray theSignal = QMetaObject::normalizedSignature(signal);
    QByteArray theSlot = QMetaObject::normalizedSignature(slot);
    if (!QMetaObject::checkConnectArgs(theSignal, theSlot))
        return false;

    int slotId = obj->metaObject()->indexOfSlot(theSlot);
    if (slotId < 0)
        return false;

    int signalId = signalIndices.value(theSignal, -1);
    if (signalId < 0) {
        signalId = signalIndices.size();
        signalIndices[theSignal] = signalId;
    }

    return QMetaObject::connect(this, signalId + metaObject()->methodCount(), obj, slotId);
}
