#include "refdialogitem.h"
#include "ui_refdialogitem.h"

RefDialogItem::RefDialogItem(QString type, QString name, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RefDialogItem)
{
    ui->setupUi(this);
    ui->nameLineEdit->setText(name);
    ui->typeLineEdit->setText(type);
}

RefDialogItem::~RefDialogItem()
{
    delete ui;
}
