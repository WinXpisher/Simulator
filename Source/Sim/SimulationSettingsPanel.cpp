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

    channelCountLabel = new QLabel("Number of communication channels:");
    channelCountInput = new QLineEdit();
    QIntValidator *chanValidator = new QIntValidator(0, 10000);
    channelCountInput->setValidator(chanValidator);
    // Value by default
    channelCountInput->setText("1");

    timeUnitLabel = new QLabel("Time unit:");
    timeUnitInput = new QLineEdit();
    // Value by default
    timeUnitInput->setText("minutes");

    timeUnitToSkipLabel = new QLabel("Number of time units to skip (may affect simulation accuracy):");
    timeUnitToSkipInput = new QLineEdit();
    QIntValidator *timeUnitValidator = new QIntValidator(0, 999999);
    timeUnitToSkipInput->setValidator(timeUnitValidator);
    // Value by default
    timeUnitToSkipInput->setText("1");

    waitMilliSecLabel = new QLabel("Number of milliseconds to wait in simulation:");
    waitMilliSecInput = new QLineEdit();
    QIntValidator *waitMilliSecValidator = new QIntValidator(0, 999999);
    waitMilliSecInput->setValidator(waitMilliSecValidator);
    // Value by default
    waitMilliSecInput->setText("400");

    updatePeriodLabel = new QLabel("Screen update period in milliseconds:");
    updatePeriodInput = new QLineEdit();
    QIntValidator *updatePeriodValidator = new QIntValidator(100, 5000);
    updatePeriodInput->setValidator(updatePeriodValidator);
    // Value by default
    updatePeriodInput->setText("390");

    confirmButton = new QPushButton("Ok");
    backButton = new QPushButton("<- Back");

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
    vertLayout->addWidget(backButton);

    container->setLayout(vertLayout);

    mainLayout->addStretch();
    mainLayout->addWidget(container);
    mainLayout->addStretch();
}

void SimulationSettingsPanel::retranslateui() {
    channelCountLabel->setText(tr("Number of communication channels:"));
    timeUnitLabel->setText(tr("Time unit:"));
    timeUnitToSkipLabel->setText(tr("Number of time units to skip (may affect simulation accuracy):"));
    waitMilliSecLabel->setText(tr("Number of milliseconds to wait in simulation:"));
    updatePeriodLabel->setText(tr("Screen update period in milliseconds:"));
    confirmButton->setText(tr("Ok"));
    backButton->setText(tr("<- Back"));
}
