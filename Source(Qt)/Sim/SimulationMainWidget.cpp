#include "SimulationMainWidget.h"

SimulationMainWidget::SimulationMainWidget() : timeUnit("")
{
    simulationPanel = new QWidget();

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // === label layout ===
    QVBoxLayout *statusPanelLayout = new QVBoxLayout();

    clockLabel = new QLabel("Clock:");
    stagnationLabel = new QLabel("Resource stagnation:");
    waitingTimeLabel = new QLabel("Average waiting time:");

    clockLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
    stagnationLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
    waitingTimeLabel->setStyleSheet("font-weight: bold; font-size: 14px;");

    statusPanelLayout->addWidget(clockLabel);
    statusPanelLayout->addWidget(stagnationLabel);
    statusPanelLayout->addWidget(waitingTimeLabel);

    QHBoxLayout *tablePanelLayout = new QHBoxLayout();

    // === Waiting Block ===
    QVBoxLayout *waitingLayout = new QVBoxLayout();
    QLabel *waitingLabel = new QLabel("Waiting");
    waitingLabel->setStyleSheet("font-weight: bold; font-size: 14px; color: red;");
    waitingLabel->setAlignment(Qt::AlignCenter);
    waitingLayout->addWidget(waitingLabel);
    waitingLayout->addWidget(waitingPool.getTaskTable());
    tablePanelLayout->addLayout(waitingLayout);
    tablePanelLayout->addSpacing(20);

    // === Sending Block ===
    QVBoxLayout *sendingLayout = new QVBoxLayout();
    QLabel *sendingLabel = new QLabel("Sending");
    sendingLabel->setStyleSheet("font-weight: bold; font-size: 14px; color: blue;");
    sendingLabel->setAlignment(Qt::AlignCenter);
    sendingLayout->addWidget(sendingLabel);
    sendingLayout->addWidget(sendingPool.getTaskTable());
    tablePanelLayout->addLayout(sendingLayout);
    tablePanelLayout->addSpacing(20);

    // === Running Block ===
    QVBoxLayout *runningLayout = new QVBoxLayout();
    QLabel *runningLabel = new QLabel("Running");
    runningLabel->setStyleSheet("font-weight: bold; font-size: 14px; color: rgb(96, 235, 40);");
    runningLabel->setAlignment(Qt::AlignCenter);
    runningLayout->addWidget(runningLabel);
    runningLayout->addWidget(runningPool.getTaskTable());
    tablePanelLayout->addLayout(runningLayout);

    mainLayout->addLayout(statusPanelLayout);
    mainLayout->addLayout(tablePanelLayout);

    simulationPanel->setLayout(mainLayout);

    connect(this, &SimulationMainWidget::clockChanged, this, &SimulationMainWidget::updateClock);
    connect(this, &SimulationMainWidget::stagnationChanged, this, &SimulationMainWidget::updateStagnation);
    connect(this, &SimulationMainWidget::waitingTimeChanged, this, &SimulationMainWidget::updateWaitingTime);

    connect(this, &SimulationMainWidget::taskAddedToWaitingPool, this, &SimulationMainWidget::addTaskToWaitingPool);
    connect(this, &SimulationMainWidget::taskAddedToSendingPool, this, &SimulationMainWidget::addTaskToSendingPool);
    connect(this, &SimulationMainWidget::taskAddedToRunningPool, this, &SimulationMainWidget::addTaskToRunningPool);
    connect(this, &SimulationMainWidget::allPoolsCleared, this, &SimulationMainWidget::clearAllPools);
}


void SimulationMainWidget::setTimeUnit(QString timeUnitP)
{
    timeUnit = timeUnitP;
    waitingPool.setTimeUnit(timeUnitP);
    sendingPool.setTimeUnit(timeUnitP);
    runningPool.setTimeUnit(timeUnitP);
}
