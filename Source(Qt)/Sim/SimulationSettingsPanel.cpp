#include "SimulationSettingsPanel.h"

#include <QIntValidator>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>

SimulationSettingsPanel::SimulationSettingsPanel()
{
    simulationSettingsPanel = new QWidget();
    QHBoxLayout *mainLayout = new QHBoxLayout(simulationSettingsPanel);

    QWidget *container = new QWidget();
    QVBoxLayout *vertLayout = new QVBoxLayout();

    QLabel *channelCountLabel = new QLabel("Number of communication channels:");
    channelCountInput = new QLineEdit();
    QIntValidator *chanValidator = new QIntValidator(0, 10000);
    channelCountInput->setValidator(chanValidator);
    // значення за замовчуванням
    channelCountInput->setText("1");

    QLabel *timeUnitLabel = new QLabel("Time unit:");
    timeUnitInput = new QLineEdit();
    // значення за замовчуванням
    timeUnitInput->setText("minutes");

    QLabel *timeUnitToSkipLabel = new QLabel("Number of time units to skip (may affect simulation accuracy):");
    timeUnitToSkipInput = new QLineEdit();
    QIntValidator *timeUnitValidator = new QIntValidator(0, 999999);
    timeUnitToSkipInput->setValidator(timeUnitValidator);
    // значення за замовчуванням
    timeUnitToSkipInput->setText("1");

    QLabel *waitMilliSecLabel = new QLabel("Number of milliseconds to wait in simulation:");
    waitMilliSecInput = new QLineEdit();
    QIntValidator *waitMilliSecValidator = new QIntValidator(0, 999999);
    waitMilliSecInput->setValidator(waitMilliSecValidator);
    // значення за замовчуванням
    waitMilliSecInput->setText("400");

    QLabel *updatePeriodLabel = new QLabel("Screen update period in milliseconds:");
    updatePeriodInput = new QLineEdit();
    QIntValidator *updatePeriodValidator = new QIntValidator(100, 5000);
    updatePeriodInput->setValidator(updatePeriodValidator);
    // значення за замовчуванням
    updatePeriodInput->setText("390");

    confirmButton = new QPushButton("Ok");

    vertLayout->addStretch(2);
    vertLayout->addWidget(channelCountLabel);
    vertLayout->addWidget(channelCountInput);
    vertLayout->addWidget(timeUnitLabel);
    vertLayout->addWidget(timeUnitInput);
    vertLayout->addWidget(timeUnitToSkipLabel);
    vertLayout->addWidget(timeUnitToSkipInput);
    vertLayout->addWidget(waitMilliSecLabel);
    vertLayout->addWidget(waitMilliSecInput);
    vertLayout->addWidget(updatePeriodLabel);
    vertLayout->addWidget(updatePeriodInput);
    vertLayout->addSpacing(10);
    vertLayout->addWidget(confirmButton);
    vertLayout->addStretch(3);

    container->setLayout(vertLayout);

    mainLayout->addStretch();
    mainLayout->addWidget(container);
    mainLayout->addStretch();
}
