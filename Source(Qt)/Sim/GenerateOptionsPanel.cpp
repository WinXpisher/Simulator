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

    methodLabel = new QLabel("Method:");

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

    taskCountLabel = new QLabel("Tasks count:");

    taskCount = new QLineEdit();
    QIntValidator *tValidator = new QIntValidator(0, 99999);
    taskCount->setValidator(tValidator);

    resCountLabel = new QLabel("Resources count:");

    resCount = new QLineEdit();
    QIntValidator *rValidator = new QIntValidator(0, 99999);
    resCount->setValidator(rValidator);

    confirmButton = new QPushButton("Ok");
    backButton = new QPushButton("<- Back");

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
    vertLayout->addWidget(backButton);

    container->setLayout(vertLayout);

    mainLayout->addStretch();
    mainLayout->addWidget(container);
    mainLayout->addStretch();
}

void GenerateOptionsPanel::retranslateui() {
    methodLabel->setText(tr("Method"));
    taskCountLabel->setText(tr("Tasks count:"));
    resCountLabel->setText(tr("Resources count:"));
    confirmButton->setText(tr("Ok"));
    backButton->setText(tr("<- Back"));
}


