#include "plotdialog.h"
#include "ui_plotdialog.h"

#include "abstractvariablehandler.h"

#include <QDebug>
#include <QStringList>
#include <QLabel>
#include <QColorDialog>

static int PLOT_COUNTER = 1;

PlotDialog::PlotDialog(QWidget *parent) :
  QDialog(parent),
  _ui(new Ui::PlotDialog)
{
  _ui->setupUi(this);
  QString title = QString("New Plot %1").arg(PLOT_COUNTER++);
  _ui->plotNameLE->setText(title);

  //Connects
  connect(_ui->buttonGroup, SIGNAL(buttonClicked(QAbstractButton*)),
          this, SLOT(onRadioButtonsChanged(QAbstractButton*)));

  initInVars();
  initUI();
}


void PlotDialog::initUI()
{
  lineColour1 = QColor(Qt::red);
  lineColour2 = QColor(Qt::green);
  lineColour3 = QColor(Qt::blue);

  oneColorButton   = NULL;
  twoColorButton   = NULL;
  threeColorButton = NULL;

  oneColorLabel    = NULL;
  twoColorLabel    = NULL;
  threeColorLabel  = NULL;

  oneVarComboBox = createComboBox();
  oneVarComboBox->setCurrentIndex(-1);
  connect(oneVarComboBox, SIGNAL(activated(QString)), this, SLOT(onOneVarActivated(QString)));
  _combosLayout = new QHBoxLayout(_ui->combosWidget);
  _combosLayout->addWidget(oneVarComboBox);
  _combosLayout->addStretch();

  _buttonsLayout = new QHBoxLayout(_ui->buttonsWidget);
  _buttonsLayout->addWidget(createPushButton("Color 1", lineColour1, oneColorLabel, oneColorButton));
  connect(oneColorButton, SIGNAL(pressed()), this, SLOT(onOneButtonPressed()));
  _buttonsLayout->addStretch();

  _ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}


void PlotDialog::initInVars()
{
  const QList<QPair<QString, QString> > variables = AbstractVariableHandler::Instance()->getDSPtoPCVarList();

  oneVarText = "";
  twoVarText = "";
  threeVarText = "";
  oneVarComboBox = 0;
  twoVarComboBox = 0;
  threeVarComboBox = 0;
  originalComboBox = new QComboBox(this);


  for( int i = 0; i < variables.size(); ++i )
  {
    originalComboBox->addItem(variables.at(i).first);
  }

  originalComboBox->hide();
}


PlotDialog::~PlotDialog()
{
  delete _ui;
}


QString PlotDialog::getPlotName()
{
  if( _ui->plotNameLE->text().isEmpty() )
  {
    return QString("New plot %1").arg(PLOT_COUNTER - 1);
  }
  else
  {
    return _ui->plotNameLE->text();
  }
}


int PlotDialog::getBufferSize()
{
  return _ui->bufferSizeSP->text().toInt();
}


double PlotDialog::getTimeStamp()
{
  return _ui->timeStampSP->value();
}


int PlotDialog::getPlotLinesNumber()
{
  return ( _ui->singlePlotRB->isChecked() )? 1:
         ( _ui->doublePlotRB->isChecked() )? 2:
         ( _ui->triplePlotRB->isChecked() )? 3: -1;
}


const QMap<QString, QColor> PlotDialog::getPlotVariables()
{
  QMap<QString, QColor> plotVarList = QMap<QString, QColor>();

  switch( getPlotLinesNumber() )
  {
  case 1:
    plotVarList[oneVarComboBox->currentText()] = lineColour1;
    break;
  case 2:
    plotVarList[oneVarComboBox->currentText()] = lineColour1;
    plotVarList[twoVarComboBox->currentText()] = lineColour2;
    break;
  case 3:
    plotVarList[oneVarComboBox->currentText()] = lineColour1;
    plotVarList[twoVarComboBox->currentText()] = lineColour2;
    plotVarList[threeVarComboBox->currentText()] = lineColour3;
    break;
  default:
    plotVarList.clear();
    break;
  }

  return plotVarList;
}


void PlotDialog::onRadioButtonsChanged(QAbstractButton *button)
{
  if(button->text() == "Single plot")
  {
    removeLayoutWidgets(_buttonsLayout);
    _buttonsLayout->addWidget(createPushButton("Color 1", lineColour1, oneColorLabel, oneColorButton));
    connect(oneColorButton, SIGNAL(pressed()), this, SLOT(onOneButtonPressed()));
    _buttonsLayout->addStretch();

    removeLayoutWidgets(_combosLayout);
    oneVarComboBox = createComboBox();
    oneVarComboBox->setCurrentIndex(-1);
    connect(oneVarComboBox, SIGNAL(activated(QString)), this, SLOT(onOneVarActivated(QString)));
    _combosLayout->addWidget(oneVarComboBox);
    _combosLayout->addStretch();
    twoVarComboBox = 0;
    threeVarComboBox = 0;
    oneVarText = "";
    twoVarText = "";
    threeVarText = "";
    _ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
  }
  else if(button->text() == "Double plot")
  {
    removeLayoutWidgets(_buttonsLayout);
    qDebug()<< "removidos" ;
    _buttonsLayout->addWidget(createPushButton("Color 1", lineColour1, oneColorLabel, oneColorButton));
    connect(oneColorButton, SIGNAL(pressed()), this, SLOT(onOneButtonPressed()));
    _buttonsLayout->addWidget(createPushButton("Color 2", lineColour2, twoColorLabel, twoColorButton));
    connect(twoColorButton, SIGNAL(pressed()), this, SLOT(onTwoButtonPressed()));
    _buttonsLayout->addStretch();
    qDebug() << "creados";

    removeLayoutWidgets(_combosLayout);
    oneVarComboBox = createComboBox();
    oneVarComboBox->setCurrentIndex(-1);
    connect(oneVarComboBox, SIGNAL(activated(QString)), this, SLOT(onOneVarActivated(QString)));
    _combosLayout->addWidget(oneVarComboBox);
    twoVarComboBox = createComboBox();
    twoVarComboBox->setCurrentIndex(-1);
    connect(twoVarComboBox, SIGNAL(activated(QString)), this, SLOT(onTwoVarActivated(QString)));
    _combosLayout->addWidget(twoVarComboBox);
    _combosLayout->addStretch();
    threeVarComboBox = 0;
    oneVarText = "";
    twoVarText = "";
    threeVarText = "";
    _ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
  }
  else if(button->text() == "Triple plot")
  {
    removeLayoutWidgets(_buttonsLayout);
    _buttonsLayout->addWidget(createPushButton("Color 1", lineColour1, oneColorLabel, oneColorButton));
    connect(oneColorButton, SIGNAL(pressed()), this, SLOT(onOneButtonPressed()));
    _buttonsLayout->addWidget(createPushButton("Color 2", lineColour2, twoColorLabel, twoColorButton));
    connect(twoColorButton, SIGNAL(pressed()), this, SLOT(onTwoButtonPressed()));
    _buttonsLayout->addWidget(createPushButton("Color 3", lineColour3, threeColorLabel, threeColorButton));
    connect(threeColorButton, SIGNAL(pressed()), this, SLOT(onThreeButtonPressed()));
    _buttonsLayout->addStretch();

    removeLayoutWidgets(_combosLayout);
    oneVarComboBox = createComboBox();
    oneVarComboBox->setCurrentIndex(-1);
    connect(oneVarComboBox, SIGNAL(activated(QString)), this, SLOT(onOneVarActivated(QString)));
    _combosLayout->addWidget(oneVarComboBox);
    twoVarComboBox = createComboBox();
    twoVarComboBox->setCurrentIndex(-1);
    connect(twoVarComboBox, SIGNAL(activated(QString)), this, SLOT(onTwoVarActivated(QString)));
    _combosLayout->addWidget(twoVarComboBox);
    threeVarComboBox = createComboBox();
    threeVarComboBox->setCurrentIndex(-1);
    connect(threeVarComboBox, SIGNAL(activated(QString)), this, SLOT(onThreeVarActivated(QString)));
    _combosLayout->addWidget(threeVarComboBox);
    _combosLayout->addStretch();
    oneVarText = "";
    twoVarText = "";
    threeVarText = "";
    _ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
  }
  else
    return;
}


QComboBox* PlotDialog::createComboBox()
{
  QComboBox *combo = new QComboBox(this);

  for(int i=0; i<originalComboBox->count(); i++)
    combo->addItem(originalComboBox->itemText(i));

  combo->setMaximumWidth(100);
  combo->setMinimumWidth(100);
  return combo;
}


QWidget* PlotDialog::createPushButton(QString name, QColor color, QLabel*& label, QPushButton*& button)
{
  QWidget* widget = new QWidget(this);
  QHBoxLayout* layout = new QHBoxLayout(widget);
  QPalette labelPalette;
  labelPalette.setColor(QPalette::Window, color);
  label = new QLabel(this);
  label->setAutoFillBackground(true);
  label->setMaximumWidth(20);
  label->setMinimumWidth(20);
  label->setPalette(labelPalette);

  button = new QPushButton(name, widget);
  button->setMaximumWidth(70);
  button->setMinimumWidth(70);

  layout->addWidget(label);
  layout->addWidget(button);
  widget->setLayout(layout);

  return widget;
}


void PlotDialog::removeLayoutWidgets(QHBoxLayout *layout)
{
  QLayoutItem* child;

  while( child = layout->takeAt(0) )
  {
    if( !child->spacerItem() )
      delete child->widget();
    else
      delete child->spacerItem();
  }
  delete child;
}


void PlotDialog::onOneVarActivated(QString name)
{
  updateComboBoxes(name, twoVarComboBox, threeVarComboBox);
  if( (oneVarText != "") && (name != oneVarText) )
    updateComboBoxes(oneVarText, twoVarComboBox, threeVarComboBox, true);

  oneVarText = name;
  if( (twoVarComboBox != 0) && (threeVarComboBox != 0) )
  {
    if( (twoVarComboBox->currentIndex() != -1) &&
        (threeVarComboBox->currentIndex() != -1) )
      _ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    else
      _ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
  }
  else if( twoVarComboBox != 0 )
  {
    if( twoVarComboBox->currentIndex() != -1 )
      _ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    else
      _ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
  }
  else if( oneVarComboBox->currentIndex() != -1 )
    _ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
  else
    _ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}


void PlotDialog::onTwoVarActivated(QString name)
{
  updateComboBoxes(name, oneVarComboBox, threeVarComboBox);
  if( (twoVarText != "") && (name != twoVarText) )
    updateComboBoxes(twoVarText, oneVarComboBox, threeVarComboBox, true);
  twoVarText = name;

  if(threeVarComboBox != 0)
  {
    if( (oneVarComboBox->currentIndex() != -1) &&
        (threeVarComboBox->currentIndex() != -1) )
      _ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    else
      _ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
  }
  else if( (oneVarComboBox->currentIndex() != -1) &&
           (twoVarComboBox->currentIndex() != -1) )
    _ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
  else
    _ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}


void PlotDialog::onThreeVarActivated(QString name)
{
  updateComboBoxes(name, oneVarComboBox, twoVarComboBox);
  if( (threeVarText != "") && (name != threeVarText) )
    updateComboBoxes(threeVarText, oneVarComboBox, twoVarComboBox, true);
  threeVarText = name;

  if( (oneVarComboBox->currentIndex() != -1) &&
      (twoVarComboBox->currentIndex() != -1) &&
      (threeVarComboBox->currentIndex() != -1) )
    _ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
  else
    _ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}


void PlotDialog::updateComboBoxes(QString itemName, QComboBox *box1, QComboBox *box2, bool insert)
{
  if( insert )
  {
    if( box1 != 0 )
    {
      box1->addItem(itemName);
      box1->update();
    }
    if( box2 != 0 )
    {
      box2->addItem(itemName);
      box2->update();
    }
  }
  else
  {
    if( box1 != 0 )
    {
      for( int i=0; i<box1->count(); i++ )
      {
        if( box1->itemText(i) == itemName )
          box1->removeItem(i);
      }
      box1->update();
    }
    if( box2 != 0 )
    {
      for( int i=0; i<box2->count(); i++ )
      {
        if( box2->itemText(i) == itemName )
          box2->removeItem(i);
      }
      box2->update();
    }
  }
}


void PlotDialog::onOneButtonPressed()
{
  lineColour1 = QColorDialog::getColor(Qt::red, this);

  QPalette palette;
  palette.setColor(QPalette::Window, lineColour1);

  oneColorLabel->setPalette(palette);
}


void PlotDialog::onTwoButtonPressed()
{
  lineColour2 = QColorDialog::getColor(Qt::green, this);

  QPalette palette;
  palette.setColor(QPalette::Window, lineColour2);

  twoColorLabel->setPalette(palette);
}


void PlotDialog::onThreeButtonPressed()
{
  lineColour3 = QColorDialog::getColor(Qt::blue, this);

  QPalette palette;
  palette.setColor(QPalette::Window, lineColour3);

  threeColorLabel->setPalette(palette);
}
