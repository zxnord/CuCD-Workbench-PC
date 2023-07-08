#include "changevariablesdialog.h"
#include "ui_changevariablesdialog.h"
#include <QDebug>

ChangeVariablesDialog::ChangeVariablesDialog(QVector<AbstractVariable*> & inVarsVector, QVector<AbstractVariable *> & refVarsVector, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangeVariablesDialog),
    farRefVector(refVarsVector),
    farInVector(inVarsVector)
{
    ui->setupUi(this);
    setWindowTitle("Configure Variables");

    //////////// Conexiones para seleccion In-Out /////////////
    connect(ui->inRadioButton, SIGNAL(clicked()), this, SLOT(onInRadioPressed()));
    connect(ui->outRadioButton, SIGNAL(clicked()), this, SLOT(onOutRadioPressed()));
    ///////////////////////////////////////////////////////////
    //////////// Conexiones para habilitacion de botones //////
    connect(ui->nameLineEdit, SIGNAL(textEdited(QString)), this, SLOT(enableSaveButton()));
    connect(ui->boolComboBox, SIGNAL(activated(int)), this, SLOT(enableSaveButton()));
    connect(ui->intSpinBox, SIGNAL(valueChanged(int)), this, SLOT(enableSaveButton()));
    connect(ui->doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(enableSaveButton()));
    ///////////////////////////////////////////////////////////
    //////////// Conexion del boton ///////////////////////////
    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(onSaveButtonPressed()));
    ///////////////////////////////////////////////////////////
    connect(ui->outComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onOutComboBoxPressed(int)));
    connect(ui->inComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onOutComboBoxPressed(int)));

    //firstTimeOutCombo = true;
    intRefVarCounter = 0;
    boolRefVarCounter = 0;
    doubleRefVarCounter = 0;

    intInVarCounter = 0;
    boolInVarCounter = 0;
    doubleInVarCounter = 0;

    qDebug() << "before init";

    initRefVariables();
    initInVariables();
    updateRefCombos();

    ui->boolNumberComboBox->setCurrentIndex(-1);
    qDebug() << "All done =)";
}

ChangeVariablesDialog::~ChangeVariablesDialog()
{
    delete ui;
}

void ChangeVariablesDialog::initRefVariables(){
    for(int i=0; i<farRefVector.size(); i++){
        qDebug() << "loop cycle";
        switch(farRefVector[i]->getVariableType()){
        case AbstractVariable::VAR_ON_OFF:
            qDebug() << "creating bool variable";
            boolRefVarCounter++;
            //ui->boolNumberComboBox->addItem(QString("Variable %1").arg(boolRefVarCounter));
            farRefBoolVariable.push_back(reinterpret_cast<ReferenceVariable<bool>*>(farRefVector[i]));
            qDebug() << "bool variable created";
            break;
        case AbstractVariable::VAR_INTEGER:
            qDebug() << "creating int variable";
            intRefVarCounter++;
            //ui->intNumberComboBox->addItem(QString("Variable %1").arg(intRefVarCounter));
            farRefIntVariable.push_back(reinterpret_cast<ReferenceVariable<int>*>(farRefVector[i]));
            qDebug() << "int variable created";
            break;
        case AbstractVariable::VAR_DOUBLE:
            qDebug() << "creating double variable";
            doubleRefVarCounter++;
            //ui->doubleNumberComboBox->addItem(QString("Variable %1").arg(doubleRefVarCounter));
            farRefDoubleVariable.push_back(reinterpret_cast<ReferenceVariable<double>*>(farRefVector[i]));
            qDebug() << "double variable created";
            break;
        }
    }
    //////////// Conexiones para seleccion de ComboBoxes //////
    //connect(ui->outComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onOutComboBoxPressed(int)));
    //connect(ui->intNumberComboBox, SIGNAL(activated(int)), this, SLOT(loadIntVariable(int)));
    //connect(ui->boolNumberComboBox, SIGNAL(activated(int)), this, SLOT(loadBoolVariable(int)));
    //connect(ui->doubleNumberComboBox, SIGNAL(activated(int)), this, SLOT(loadDoubleVariable(int)));
    ///////////////////////////////////////////////////////////
}

void ChangeVariablesDialog::initInVariables(){
    for(int i=0; i<farInVector.size(); i++){
        qDebug() << "loop cycle";
        switch(farInVector[i]->getVariableType()){
        case AbstractVariable::VAR_ON_OFF:
            qDebug() << "creating bool variable";
            boolInVarCounter++;
            //ui->boolNumberComboBox->addItem(QString("Variable %1").arg(boolInVarCounter));
            farInBoolVariable.push_back(reinterpret_cast<ReferenceVariable<bool>*>(farInVector[i]));
            qDebug() << "bool variable created";
            break;
        case AbstractVariable::VAR_INTEGER:
            qDebug() << "creating int variable";
            intInVarCounter++;
            //ui->intNumberComboBox->addItem(QString("Variable %1").arg(intInVarCounter));
            farInIntVariable.push_back(reinterpret_cast<ReferenceVariable<int>*>(farInVector[i]));
            qDebug() << "int variable created";
            break;
        case AbstractVariable::VAR_DOUBLE:
            qDebug() << "creating double variable";
            doubleInVarCounter++;
            //ui->doubleNumberComboBox->addItem(QString("Variable %1").arg(doubleInVarCounter));
            farInDoubleVariable.push_back(reinterpret_cast<ReferenceVariable<double>*>(farInVector[i]));
            qDebug() << "double variable created";
            break;
        }

    }

}

void ChangeVariablesDialog::updateInCombos(){
    clearCombos(ui->boolNumberComboBox, boolRefVarCounter);
    clearCombos(ui->intNumberComboBox, intRefVarCounter);
    clearCombos(ui->doubleNumberComboBox, doubleRefVarCounter);
    ui->intNumberComboBox->disconnect();
    ui->boolNumberComboBox->disconnect();
    ui->doubleNumberComboBox->disconnect();
    for(int i=0; i<boolInVarCounter; i++)
        ui->boolNumberComboBox->addItem(QString("Variable %1").arg(i+1));
    for(int i=0; i<intInVarCounter; i++)
        ui->intNumberComboBox->addItem(QString("Variable %1").arg(i+1));
    for(int i=0; i<doubleInVarCounter; i++)
        ui->doubleNumberComboBox->addItem(QString("Variable %1").arg(i+1));
    //////////// Conexiones para seleccion de ComboBoxes //////
    //connect(ui->inComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onOutComboBoxPressed(int)));
    connect(ui->intNumberComboBox, SIGNAL(activated(int)), this, SLOT(loadInIntVariable(int)));
    connect(ui->boolNumberComboBox, SIGNAL(activated(int)), this, SLOT(loadInBoolVariable(int)));
    connect(ui->doubleNumberComboBox, SIGNAL(activated(int)), this, SLOT(loadInDoubleVariable(int)));
    ///////////////////////////////////////////////////////////
}

void ChangeVariablesDialog::updateRefCombos(){
    clearCombos(ui->boolNumberComboBox, boolInVarCounter);
    clearCombos(ui->intNumberComboBox, intInVarCounter);
    clearCombos(ui->doubleNumberComboBox, doubleInVarCounter);
    ui->intNumberComboBox->disconnect();
    ui->boolNumberComboBox->disconnect();
    ui->doubleNumberComboBox->disconnect();
    for(int i=0; i<boolRefVarCounter; i++)
        ui->boolNumberComboBox->addItem(QString("Variable %1").arg(i+1));
    for(int i=0; i<intRefVarCounter; i++)
        ui->intNumberComboBox->addItem(QString("Variable %1").arg(i+1));
    for(int i=0; i<doubleRefVarCounter; i++)
        ui->doubleNumberComboBox->addItem(QString("Variable %1").arg(i+1));
    //////////// Conexiones para seleccion de ComboBoxes //////
    //connect(ui->outComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onOutComboBoxPressed(int)));
    connect(ui->intNumberComboBox, SIGNAL(activated(int)), this, SLOT(loadRefIntVariable(int)));
    connect(ui->boolNumberComboBox, SIGNAL(activated(int)), this, SLOT(loadRefBoolVariable(int)));
    connect(ui->doubleNumberComboBox, SIGNAL(activated(int)), this, SLOT(loadRefDoubleVariable(int)));
    ///////////////////////////////////////////////////////////
}

void ChangeVariablesDialog::clearFields(int page){
    ui->attrGroupBox->setEnabled(false);
    ui->nameLineEdit->clear();
    ui->minLineEdit->clear();
    ui->maxLineEdit->clear();
    switch(page){
    case BOOL_NUMBER_PAGE:
        ui->boolComboBox->setCurrentIndex(-1);
        break;
    case INT_NUMBER_PAGE:
        ui->intSpinBox->setValue(0);
        break;
    case DOUBLE_NUMBER_PAGE:
        ui->doubleSpinBox->setValue(0.0);
        break;
    }
}

void ChangeVariablesDialog::onOutRadioPressed(){
    ui->ioStackedWidget->setCurrentIndex(OUT_DATA_PAGE);
    ui->typeStackedWidget->setCurrentIndex(BOOL_TYPE_PAGE);
    ui->numVarStackedWidget->setCurrentIndex(BOOL_NUMBER_PAGE);
    clearFields(BOOL_NUMBER_PAGE);
    ui->outComboBox->setCurrentIndex(-1);
    ui->boolNumberComboBox->setCurrentIndex(-1);
    updateRefCombos();

}

void ChangeVariablesDialog::onInRadioPressed(){
    ui->ioStackedWidget->setCurrentIndex(IN_DATA_PAGE);
    ui->typeStackedWidget->setCurrentIndex(BOOL_TYPE_PAGE);
    ui->numVarStackedWidget->setCurrentIndex(BOOL_NUMBER_PAGE);
    clearFields(BOOL_NUMBER_PAGE);
    ui->inComboBox->setCurrentIndex(-1);
    ui->boolNumberComboBox->setCurrentIndex(-1);
    updateInCombos();

}

void ChangeVariablesDialog::onOutComboBoxPressed(int index){
    ui->saveButton->setEnabled(false);
    switch(index){
    case 0:
        ui->numVarStackedWidget->setCurrentIndex(BOOL_NUMBER_PAGE);
        ui->typeStackedWidget->setCurrentIndex(BOOL_TYPE_PAGE);
        ui->boolNumberComboBox->setCurrentIndex(-1);
        clearFields(BOOL_NUMBER_PAGE);
        break;
    case 1:
        ui->numVarStackedWidget->setCurrentIndex(INT_NUMBER_PAGE);
        ui->typeStackedWidget->setCurrentIndex(INT_TYPE_PAGE);
        ui->intNumberComboBox->setCurrentIndex(-1);
        clearFields(INT_NUMBER_PAGE);
        break;
    case 2:
        ui->numVarStackedWidget->setCurrentIndex(DOUBLE_NUMBER_PAGE);
        ui->typeStackedWidget->setCurrentIndex(DOUBLE_TYPE_PAGE);
        ui->doubleNumberComboBox->setCurrentIndex(-1);
        clearFields(DOUBLE_NUMBER_PAGE);
        break;
    }
}

void ChangeVariablesDialog::loadInDoubleVariable(int index){
    ui->attrGroupBox->setEnabled(true);
    ui->minLineEdit->setEnabled(true);
    ui->minLineEdit->setReadOnly(true);
    ui->maxLineEdit->setEnabled(true);
    ui->maxLineEdit->setReadOnly(true);
    ui->nameLineEdit->setText(farInDoubleVariable[index]->getVariableName());
    ui->minLineEdit->setText(QString("%1").arg(farInDoubleVariable[index]->getMinValue()));
    ui->maxLineEdit->setText(QString("%1").arg(farInDoubleVariable[index]->getMaxValue()));
    ui->doubleSpinBox->setValue(farInDoubleVariable[index]->getVariableValue());
}

void ChangeVariablesDialog::loadInIntVariable(int index){
    ui->attrGroupBox->setEnabled(true);
    ui->minLineEdit->setEnabled(true);
    ui->minLineEdit->setReadOnly(true);
    ui->maxLineEdit->setEnabled(true);
    ui->maxLineEdit->setReadOnly(true);
    ui->nameLineEdit->setText(farInIntVariable[index]->getVariableName());
    ui->minLineEdit->setText(QString("%1").arg(farInIntVariable[index]->getMinValue()));
    ui->maxLineEdit->setText(QString("%1").arg(farInIntVariable[index]->getMaxValue()));
    ui->intSpinBox->setValue(farInIntVariable[index]->getVariableValue());
}

void ChangeVariablesDialog::loadInBoolVariable(int index){
    ui->attrGroupBox->setEnabled(true);
    ui->minLineEdit->setEnabled(true);
    ui->minLineEdit->setReadOnly(true);
    ui->maxLineEdit->setEnabled(true);
    ui->maxLineEdit->setReadOnly(true);
    ui->nameLineEdit->setText(farInBoolVariable[index]->getVariableName());
    ui->minLineEdit->setText("False");
    ui->maxLineEdit->setText("True");
    if(farRefBoolVariable[index]->getVariableValue())
        ui->boolComboBox->setCurrentIndex(0);
    else
        ui->boolComboBox->setCurrentIndex(1);
}

void ChangeVariablesDialog::loadRefDoubleVariable(int index){
    ui->attrGroupBox->setEnabled(true);
    ui->minLineEdit->setEnabled(true);
    ui->minLineEdit->setReadOnly(true);
    ui->maxLineEdit->setEnabled(true);
    ui->maxLineEdit->setReadOnly(true);
    ui->nameLineEdit->setText(farRefDoubleVariable[index]->getVariableName());
    ui->minLineEdit->setText(QString("%1").arg(farRefDoubleVariable[index]->getMinValue()));
    ui->maxLineEdit->setText(QString("%1").arg(farRefDoubleVariable[index]->getMaxValue()));
    ui->doubleSpinBox->setValue(farRefDoubleVariable[index]->getVariableValue());
}

void ChangeVariablesDialog::loadRefIntVariable(int index){
    ui->attrGroupBox->setEnabled(true);
    ui->minLineEdit->setEnabled(true);
    ui->minLineEdit->setReadOnly(true);
    ui->maxLineEdit->setEnabled(true);
    ui->maxLineEdit->setReadOnly(true);
    ui->nameLineEdit->setText(farRefIntVariable[index]->getVariableName());
    ui->minLineEdit->setText(QString("%1").arg(farRefIntVariable[index]->getMinValue()));
    ui->maxLineEdit->setText(QString("%1").arg(farRefIntVariable[index]->getMaxValue()));
    ui->intSpinBox->setValue(farRefIntVariable[index]->getVariableValue());
}

void ChangeVariablesDialog::loadRefBoolVariable(int index){
    ui->attrGroupBox->setEnabled(true);
    ui->minLineEdit->setEnabled(true);
    ui->minLineEdit->setReadOnly(true);
    ui->maxLineEdit->setEnabled(true);
    ui->maxLineEdit->setReadOnly(true);
    ui->nameLineEdit->setText(farRefBoolVariable[index]->getVariableName());
    ui->minLineEdit->setText("False");
    ui->maxLineEdit->setText("True");
    if(farRefBoolVariable[index]->getVariableValue())
        ui->boolComboBox->setCurrentIndex(0);
    else
        ui->boolComboBox->setCurrentIndex(1);
}

void ChangeVariablesDialog::enableSaveButton(){
    switch(ui->numVarStackedWidget->currentIndex()){
    case BOOL_NUMBER_PAGE:
        if(ui->boolNumberComboBox->currentIndex() == -1)
            return;
        else
            break;
    case INT_NUMBER_PAGE:
        if(ui->intNumberComboBox->currentIndex() == -1)
            return;
        else
            break;
    case DOUBLE_NUMBER_PAGE:
        if(ui->doubleNumberComboBox->currentIndex() == -1)
            return;
        else
            break;
    }
    ui->saveButton->setEnabled(true);
}

void ChangeVariablesDialog::onSaveButtonPressed(){
    int index;
    switch(ui->ioStackedWidget->currentIndex()){
    case IN_DATA_PAGE:
        switch(ui->numVarStackedWidget->currentIndex()){
        case BOOL_NUMBER_PAGE:
            index = ui->boolNumberComboBox->currentIndex();
            farInBoolVariable[index]->setVariableName(ui->nameLineEdit->text());
            switch(ui->boolComboBox->currentIndex()){
            case 0:
                farInBoolVariable[index]->setVariableValue(true);
                break;
            case 1:
                farInBoolVariable[index]->setVariableValue(false);
                break;
            }
            break;
        case INT_NUMBER_PAGE:
            index = ui->intNumberComboBox->currentIndex();
            farInIntVariable[index]->setVariableName(ui->nameLineEdit->text());
            farInIntVariable[index]->setVariableValue(ui->intSpinBox->value());
            break;
        case DOUBLE_NUMBER_PAGE:
            index = ui->doubleNumberComboBox->currentIndex();
            farInDoubleVariable[index]->setVariableName(ui->nameLineEdit->text());
            farInDoubleVariable[index]->setVariableValue(ui->doubleSpinBox->value());
            break;
        }

        break;
    case OUT_DATA_PAGE:
        switch(ui->numVarStackedWidget->currentIndex()){
        case BOOL_NUMBER_PAGE:
            index = ui->boolNumberComboBox->currentIndex();
            farRefBoolVariable[index]->setVariableName(ui->nameLineEdit->text());
            switch(ui->boolComboBox->currentIndex()){
            case 0:
                farRefBoolVariable[index]->setVariableValue(true);
                break;
            case 1:
                farRefBoolVariable[index]->setVariableValue(false);
                break;
            }
            break;
        case INT_NUMBER_PAGE:
            index = ui->intNumberComboBox->currentIndex();
            farRefIntVariable[index]->setVariableName(ui->nameLineEdit->text());
            farRefIntVariable[index]->setVariableValue(ui->intSpinBox->value());
            break;
        case DOUBLE_NUMBER_PAGE:
            index = ui->doubleNumberComboBox->currentIndex();
            farRefDoubleVariable[index]->setVariableName(ui->nameLineEdit->text());
            farRefDoubleVariable[index]->setVariableValue(ui->doubleSpinBox->value());
            break;
        }

        break;
    }
    ui->saveButton->setEnabled(false);
}

void ChangeVariablesDialog::clearCombos(QComboBox *combo, int lenght){
    for(int i=0; i<lenght; i++){
        combo->removeItem(i);
    }
}
