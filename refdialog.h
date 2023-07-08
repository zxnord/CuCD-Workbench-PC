#ifndef REFDIALOG_H
#define REFDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QWidget>
#include "referencevariable.h"
#include "refdialogitem.h"
#include <QVector>

namespace Ui {
class RefDialog;
}

class RefDialog : public QDialog
{
    Q_OBJECT
    
public slots:
    void onComboBoxActivated(int);
    void onAddButtonPressed(void);

public:
    explicit RefDialog(QVector<AbstractVariable*> varVector, QWidget *parent = 0);
    QVector<AbstractVariable *> getPanelVector(void);
    ~RefDialog();
    
private:
    void initUI(void);
    void initConnects(void);
    QString typeToString(AbstractVariable::DataType type);
    void clearStretch(QLayout *layout);
    Ui::RefDialog *ui;
    QVector<AbstractVariable *> farVarVector;
    QVector<AbstractVariable *> panelVarVector;
    QVBoxLayout *selectedVarsLayout;
    QWidget *scrollWidget;
    unsigned int lastSelectedIndex;

};

#endif // REFDIALOG_H
