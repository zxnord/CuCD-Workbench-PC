#ifndef CHANGEVARIABLESDIALOG_H
#define CHANGEVARIABLESDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QVector>
#include "referencevariable.h"

namespace Ui {
class ChangeVariablesDialog;
}

class ChangeVariablesDialog : public QDialog
{
    Q_OBJECT
    
public:
    enum {
        OUT_DATA_PAGE,
        IN_DATA_PAGE
    };

    enum {
        INT_TYPE_PAGE,
        DOUBLE_TYPE_PAGE,
        BOOL_TYPE_PAGE
    };

    enum {
        BOOL_NUMBER_PAGE,
        DOUBLE_NUMBER_PAGE,
        INT_NUMBER_PAGE
    };

    explicit ChangeVariablesDialog(QVector<AbstractVariable*> & inVarsVector, QVector<AbstractVariable *> & refVarsVector, QWidget *parent = 0);
    ~ChangeVariablesDialog();

public slots:
    void onOutRadioPressed(void);
    void onInRadioPressed(void);
    void onOutComboBoxPressed(int);
    void loadInIntVariable(int);
    void loadInBoolVariable(int);
    void loadInDoubleVariable(int);
    void loadRefIntVariable(int);
    void loadRefBoolVariable(int);
    void loadRefDoubleVariable(int);
    void enableSaveButton(void);
    void onSaveButtonPressed(void);
    
private:
    void initRefVariables(void);
    void initInVariables(void);
    void updateRefCombos(void);
    void updateInCombos(void);
    void clearFields(int);
    void clearCombos(QComboBox *combo, int lenght);
    Ui::ChangeVariablesDialog *ui;
    //bool firstTimeOutCombo;
    //bool firstTimeInCombo;
    int intRefVarCounter;
    int boolRefVarCounter;
    int doubleRefVarCounter;
    int intInVarCounter;
    int boolInVarCounter;
    int doubleInVarCounter;
    QVector<AbstractVariable*> farRefVector;
    QVector<AbstractVariable*> farInVector;
    QVector<ReferenceVariable<int>*> farRefIntVariable;
    QVector<ReferenceVariable<bool>*> farRefBoolVariable;
    QVector<ReferenceVariable<double>*> farRefDoubleVariable;
    QVector<ReferenceVariable<int>*> farInIntVariable;
    QVector<ReferenceVariable<bool>*> farInBoolVariable;
    QVector<ReferenceVariable<double>*> farInDoubleVariable;
};

#endif // CHANGEVARIABLESDIALOG_H
