#include "refdialog.h"
#include "ui_refdialog.h"
#include <QDebug>

RefDialog::RefDialog(QVector<AbstractVariable *> varVector, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RefDialog),
    farVarVector(varVector)
{
    ui->setupUi(this);

    lastSelectedIndex = -1;

    initUI();
    initConnects();
}

RefDialog::~RefDialog()
{
    delete ui;
}

void RefDialog::initUI(){
    scrollWidget = new QWidget();
    selectedVarsLayout = new QVBoxLayout(scrollWidget);
    //selectedVarsLayout->addStretch();
    for(int i=0; i<farVarVector.size(); i++){
        ui->varsComboBox->addItem(farVarVector[i]->getVariableName());
    }
    ui->varsComboBox->setCurrentIndex(-1);
    ui->addPushButton->setEnabled(false);
    ui->refVarScrollArea->setWidget(scrollWidget);
}

void RefDialog::initConnects(){
    connect(ui->varsComboBox, SIGNAL(activated(int)), this, SLOT(onComboBoxActivated(int)));
    connect(ui->addPushButton, SIGNAL(pressed()), this, SLOT(onAddButtonPressed()));
}

void RefDialog::onComboBoxActivated(int item){
    if(item >= 0){
        for(int i=0; i<farVarVector.size(); i++)
            if(ui->varsComboBox->currentText() == farVarVector[i]->getVariableName())
                lastSelectedIndex = i;
        ui->addPushButton->setEnabled(true);
    }
}

void RefDialog::onAddButtonPressed(){
    qDebug() << "init";
    clearStretch(selectedVarsLayout);
    qDebug() << "cleared";
    qDebug() << "item no: " << ui->varsComboBox->currentIndex();
    selectedVarsLayout->addWidget(new RefDialogItem(typeToString(farVarVector[lastSelectedIndex]->getVariableType()), farVarVector[lastSelectedIndex]->getVariableName(), this));
    selectedVarsLayout->addStretch();
    ui->addPushButton->setEnabled(false);
    ui->varsComboBox->removeItem(ui->varsComboBox->currentIndex());
    ui->varsComboBox->setCurrentIndex(-1);
    panelVarVector.push_back(farVarVector[lastSelectedIndex]);
}

void RefDialog::clearStretch(QLayout *layout){
    QLayoutItem *child;
    if((child = layout->takeAt(layout->count())) != 0){
        delete child->spacerItem();
        delete child;
    }
    else
        return;
}

QString RefDialog::typeToString(AbstractVariable::DataType type){
    switch(type){
    case AbstractVariable::VAR_ON_OFF:
        return "Interruptor";
    case AbstractVariable::VAR_INTEGER:
        return "Integer";
    case AbstractVariable::VAR_DOUBLE:
        return "Double";
    }
    return "fail =(";
}

QVector<AbstractVariable *> RefDialog::getPanelVector(){
    return panelVarVector;
}
