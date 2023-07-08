#ifndef REFDIALOGITEM_H
#define REFDIALOGITEM_H

#include <QWidget>

namespace Ui {
class RefDialogItem;
}

class RefDialogItem : public QWidget
{
    Q_OBJECT
    
public:
    explicit RefDialogItem(QString type, QString name, QWidget *parent = 0);
    ~RefDialogItem();
    
private:
    Ui::RefDialogItem *ui;
};

#endif // REFDIALOGITEM_H
