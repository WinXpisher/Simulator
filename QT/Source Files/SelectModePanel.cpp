#include "SelectModePanel.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

SelectModePanel::SelectModePanel()
{
    selectModePanel = new QWidget();
    QHBoxLayout *mainLayout = new QHBoxLayout(selectModePanel);

    QWidget *container = new QWidget();
    QVBoxLayout *vertLayout = new QVBoxLayout();

    QLabel *label = new QLabel("Choose how to obtain a new set:");

    comboBox = new QComboBox();
    comboBox->addItem("Generate");
    comboBox->addItem("Choose");
    comboBox->setCurrentIndex(0);

    confirmButton = new QPushButton("Ok");

    vertLayout->addStretch(2);
    vertLayout->addWidget(label);
    vertLayout->addWidget(comboBox);
    vertLayout->addSpacing(10);
    vertLayout->addWidget(confirmButton);
    vertLayout->addStretch(3);

    container->setLayout(vertLayout);

    mainLayout->addStretch();
    mainLayout->addWidget(container);
    mainLayout->addStretch();
}

