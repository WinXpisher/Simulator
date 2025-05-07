#include "GenerateOptionsPanel.h"

#include <QIntValidator>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>

GenerateOptionsPanel::GenerateOptionsPanel()
{
    generateOptionsPanel = new QWidget();
    QHBoxLayout *mainLayout = new QHBoxLayout(generateOptionsPanel);

    QWidget *container = new QWidget();
    QVBoxLayout *vertLayout = new QVBoxLayout();

    QLabel *methodLabel = new QLabel("Method:");

    chooseMethod = new QComboBox();
    chooseMethod->addItem("LIFO");
    chooseMethod->addItem("FCFS");
    chooseMethod->addItem("HPF");
    chooseMethod->addItem("BACKFILL");
    chooseMethod->addItem("SIMPLEX");
    chooseMethod->addItem("SMART");
    chooseMethod->addItem("MFQS");
    chooseMethod->addItem("Penguin");
    chooseMethod->setCurrentIndex(0);

    QLabel *taskCountLabel = new QLabel("Tasks count:");

    taskCount = new QLineEdit();
    QIntValidator *tValidator = new QIntValidator(0, 99999);
    taskCount->setValidator(tValidator);

    QLabel *resCountLabel = new QLabel("Resources count:");

    resCount = new QLineEdit();
    QIntValidator *rValidator = new QIntValidator(0, 99999);
    resCount->setValidator(rValidator);

    confirmButton = new QPushButton("Ok");

    vertLayout->addStretch(2);
    vertLayout->addWidget(methodLabel);
    vertLayout->addWidget(chooseMethod);
    vertLayout->addWidget(taskCountLabel);
    vertLayout->addWidget(taskCount);
    vertLayout->addWidget(resCountLabel);
    vertLayout->addWidget(resCount);
    vertLayout->addSpacing(10);
    vertLayout->addWidget(confirmButton);
    vertLayout->addStretch(3);

    container->setLayout(vertLayout);

    mainLayout->addStretch();
    mainLayout->addWidget(container);
    mainLayout->addStretch();
}




