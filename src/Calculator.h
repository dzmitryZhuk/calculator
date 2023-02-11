#ifndef _CALCULATOR_CALCULATOR_H_
#define _CALCULATOR_CALCULATOR_H_

#include <QtWidgets/QMainWindow>
#include <QWidget>
#include <QLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QMap>

class Calculator : public QMainWindow
{
    Q_OBJECT

public:
  enum Operation
  {
    plus      = '+',
    minus     = '-',
    multiply  = '*',
    divide    = '/',
  };

  Calculator(QWidget *parent = nullptr);
  ~Calculator();

  double calculate();

protected:
  QWidget* mainWidget;
  QLayout* mainLayout;

  QLineEdit* lineEdit;
  QPushButton* button_1;
  QPushButton* button_2;
  QPushButton* button_3;
  QPushButton* button_4;
  QPushButton* button_5;
  QPushButton* button_6;
  QPushButton* button_7;
  QPushButton* button_8;
  QPushButton* button_9;
  QPushButton* button_0;
  QPushButton* button_dot;
  QPushButton* button_equals;
  QPushButton* button_plus;
  QPushButton* button_minus;
  QPushButton* button_multiply;
  QPushButton* button_divide;

private:
  QString createChildObjectName(QString name);
  QPushButton* createButton(QChar name);
  void initLineEdit();
  void initButtons();
  void initMainLayout();
  QString addOperation(QString input, Operation operation);
  void addOperationToDisplay(Operation operation);
  void addOperationToDisplay(QChar operation);
  double doOperation(Operation operation, double operand1, double operand2);
  bool displayContainsAnyOperation();
  Operation findOperationOnDisplay();

private slots:

  void on_dotPressed();
};

#endif