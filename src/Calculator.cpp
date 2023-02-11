#include <QRegExpValidator>

#include "Calculator.h"
#include "macros.h"

Calculator::Calculator(QWidget *parent)
    : QMainWindow(parent)
{
    setFixedSize(400, 600);
    setObjectName(QString::fromUtf8("Calculator"));

    mainWidget = new QWidget(this);
    mainWidget->setObjectName(createChildObjectName("mainWidget"));
    setCentralWidget(mainWidget);

    mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(createChildObjectName("mainLayout"));
    mainWidget->setLayout(mainLayout);

    initLineEdit();
    initButtons();
    initMainLayout();
}

Calculator::~Calculator()
{}

double Calculator::calculate()
{
  if (displayContainsAnyOperation())
  {
    double result = 0;
    Operation operation = findOperationOnDisplay();
    auto numbers = lineEdit->text().split(static_cast<QChar>(operation));
    if (numbers.size() == 2)
      result = doOperation(operation, numbers[0].toDouble(), numbers[1].toDouble());
    lineEdit->setText(QString::number(result));
    auto s = lineEdit->text();
    return result;
  }
  else
    return lineEdit->text().toDouble();
}

QString Calculator::createChildObjectName(QString name)
{
  return objectName() + QString(DELIMITER) + name;
}

void Calculator::initLineEdit()
{
  lineEdit = new QLineEdit(this);
  lineEdit->setObjectName(createChildObjectName("lineEdit"));
  lineEdit->setFixedHeight(height() / 5);   // !dangerous
  lineEdit->setAlignment(Qt::AlignmentFlag::AlignRight);
  lineEdit->setReadOnly(true);
  connect(lineEdit, &QLineEdit::editingFinished, this, [=]() { calculate(); });
  //QString range = "";
  //QRegExp regex(range);
  //QRegExpValidator* validator = new QRegExpValidator(regex, this);
  //lineEdit->setValidator(validator);

  //QFont font = lineEdit->font();
  //font.setPointSize(lineEdit->height() / 3);
  //lineEdit->setFont(font);
}

QPushButton* Calculator::createButton(QChar name)
{
  QPushButton* button = new QPushButton(this);
  button->setText(name);
  button->setObjectName(createChildObjectName(QString::fromUtf8("button") + QString(DELIMITER) + name));
  button->setFixedSize(width() / 4, height() / 5);    // !dangerous
  if (name == '=')
    connect(button, &QPushButton::clicked, this, [=]() { calculate(); });
  /*else */if (name == '.')
    connect(button, &QPushButton::clicked, this, &Calculator::on_dotPressed);
  else if (name.isDigit())
    connect(button, &QPushButton::clicked, this, [=]() { lineEdit->setText(lineEdit->text() + name); });
  else
    connect(button, &QPushButton::clicked, this, [=]() { addOperationToDisplay(name);});
  return button;
}

void Calculator::initButtons()
{
  button_0 = createButton('0');
  button_1 = createButton('1');
  button_2 = createButton('2');
  button_3 = createButton('3');
  button_4 = createButton('4');
  button_5 = createButton('5');
  button_6 = createButton('6');
  button_7 = createButton('7');
  button_8 = createButton('8');
  button_9 = createButton('9');
  button_dot = createButton('.');
  button_plus = createButton('+');
  button_minus = createButton('-');
  button_multiply = createButton('*');
  button_divide = createButton('/');
  button_equals = createButton('=');
}

void Calculator::initMainLayout()
{
  auto layout = dynamic_cast<QVBoxLayout*>(mainLayout);

  layout->addWidget(lineEdit);

  auto hLayout1 = new QHBoxLayout(this);
  hLayout1->addWidget(button_7);
  hLayout1->addWidget(button_8);
  hLayout1->addWidget(button_9);
  hLayout1->addWidget(button_multiply);
  layout->addLayout(hLayout1);

  auto hLayout2 = new QHBoxLayout(this);
  hLayout2->addWidget(button_4);
  hLayout2->addWidget(button_5);
  hLayout2->addWidget(button_6);
  hLayout2->addWidget(button_divide);
  layout->addLayout(hLayout2);

  auto hLayout3 = new QHBoxLayout(this);
  hLayout3->addWidget(button_1);
  hLayout3->addWidget(button_2);
  hLayout3->addWidget(button_3);
  hLayout3->addWidget(button_plus);
  layout->addLayout(hLayout3);

  auto hLayout4 = new QHBoxLayout(this);
  hLayout4->addWidget(button_dot);
  hLayout4->addWidget(button_0);
  hLayout4->addWidget(button_equals);
  hLayout4->addWidget(button_minus);
  layout->addLayout(hLayout4);
}

double Calculator::doOperation(Operation operation, double operand1, double operand2)
{
  switch (operation)
  {
  case Operation::plus:
    return operand1 + operand2;
  case Operation::minus:
    return operand1 - operand2;
  case Operation::multiply:
    return operand1 * operand2;
  case Operation::divide:
    return operand1 / operand2;
  default:
    break;
  }
  return 0;
}

QString Calculator::addOperation(QString input, Operation operation)
{
  auto text = input;
  auto last = text.at(text.size() - 1);
  if (!last.isDigit())
    text.resize(text.size() - 1);
  text += operation;
  return text;
}

// we have to consider every element of enum cuz c++ can't enumerate enum
void Calculator::addOperationToDisplay(QChar operation)
{
  Operation _operation;
  if (operation == static_cast<QChar>(Operation::divide))
    _operation = Operation::divide;
  if (operation == static_cast<QChar>(Operation::multiply))
    _operation = Operation::multiply;
  if (operation == static_cast<QChar>(Operation::plus))
    _operation = Operation::plus;
  if (operation == static_cast<QChar>(Operation::minus))
    _operation = Operation::minus;
  addOperationToDisplay(_operation);
}

void Calculator::addOperationToDisplay(Operation operation)
{
  if (displayContainsAnyOperation())
  {
    auto text = lineEdit->text();
    switch (text.at(text.size()-1).toLatin1())
    {
    case Operation::divide:
    case Operation::minus:
    case Operation::multiply:
    case Operation::plus:
      text.resize(text.size() - 1);
      break;
    default:
      calculate();
      break;
    }
  }
  lineEdit->setText(addOperation(lineEdit->text(), operation));
}

// we have to consider every element of enum cuz c++ can't enumerate enum
bool Calculator::displayContainsAnyOperation()
{
  auto text = lineEdit->text();
  return text.contains(Operation::divide) || text.contains(Operation::multiply) || text.contains(Operation::plus) || text.contains(Operation::minus);
}

// we have to consider every element of enum cuz c++ can't enumerate enum
Calculator::Operation Calculator::findOperationOnDisplay()
{
  auto text = lineEdit->text();
  if (text.contains(Operation::divide))
    return Operation::divide;
  if (text.contains(Operation::multiply))
    return Operation::multiply;
  if (text.contains(Operation::plus))
    return Operation::plus;
  if (text.contains(Operation::minus))
    return Operation::minus;
}

void Calculator::on_dotPressed()
{
  auto text = lineEdit->text();
  if (displayContainsAnyOperation())
  {
    auto operation = findOperationOnDisplay();
    auto numbers = text.split(operation);
    if (numbers.count())
      text = numbers.last();
  }
  if(!text.contains(QChar('.')))
    lineEdit->setText(lineEdit->text() + QChar('.'));
}
