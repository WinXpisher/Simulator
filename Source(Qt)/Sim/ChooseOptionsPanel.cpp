#include "ChooseOptionsPanel.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>

ChooseOptionsPanel::ChooseOptionsPanel()
{
    chooseOptionsPanel = new QWidget();
    QHBoxLayout *mainLayout = new QHBoxLayout(chooseOptionsPanel);

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

    QLabel *setNumberLabel = new QLabel("Set number:");

    setNumber = new QLineEdit();

    confirmButton = new QPushButton("Ok");

    vertLayout->addStretch(2);
    vertLayout->addWidget(methodLabel);
    vertLayout->addWidget(chooseMethod);
    vertLayout->addWidget(setNumberLabel);
    vertLayout->addWidget(setNumber);
    vertLayout->addSpacing(10);
    vertLayout->addWidget(confirmButton);
    vertLayout->addStretch(3);

    container->setLayout(vertLayout);

    mainLayout->addStretch();
    mainLayout->addWidget(container);
    mainLayout->addStretch();
}

