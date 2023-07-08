#ifndef PLOTDIALOG_H
#define PLOTDIALOG_H

#include <QDialog>
#include <QStringList>
#include <QAbstractButton>
#include <QComboBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QVector>
#include <QColor>
#include <QMap>

namespace Ui {
class PlotDialog;
}

class PlotDialog : public QDialog
{
    Q_OBJECT
    
public slots:
    void onRadioButtonsChanged(QAbstractButton *button);
    void onOneVarActivated(QString);
    void onTwoVarActivated(QString);
    void onThreeVarActivated(QString);
    void onOneButtonPressed();
    void onTwoButtonPressed();
    void onThreeButtonPressed();


public:
    explicit PlotDialog(QWidget *parent = 0);
    ~PlotDialog();

    QString                     getPlotName();
    double                      getTimeStamp();
    int                         getBufferSize();
    int                         getPlotLinesNumber();
    const QMap<QString, QColor> getPlotVariables();
    
private:
    QComboBox *createComboBox();
    QWidget* createPushButton(QString name, QColor color, QLabel*& label, QPushButton*& button);
    void updateComboBoxes(QString itemName, QComboBox *box1, QComboBox *box2, bool insert = false);
    void removeLayoutWidgets(QHBoxLayout *layout);
    void initUI();
    void initInVars();
    Ui::PlotDialog* _ui;
    QHBoxLayout*    _buttonsLayout;
    QHBoxLayout*    _combosLayout;
    QComboBox*      originalComboBox;
    QComboBox*      oneVarComboBox;
    QComboBox*      twoVarComboBox;
    QComboBox*      threeVarComboBox;
    QLabel*         oneColorLabel;
    QLabel*         twoColorLabel;
    QLabel*         threeColorLabel;
    QPushButton*    oneColorButton;
    QPushButton*    twoColorButton;
    QPushButton*    threeColorButton;
    QColor          lineColour1;
    QColor          lineColour2;
    QColor          lineColour3;
    QString         oneVarText;
    QString         twoVarText;
    QString         threeVarText;
};

#endif // PLOTDIALOG_H
