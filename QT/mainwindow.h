#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "SelectModePanel.h"
#include "GenerateOptionsPanel.h"
#include "ChooseOptionsPanel.h"
#include "SimulationSettingsPanel.h"
#include "SimulationMainWidget.h"
#include "ResourcesDockWidget.h"
#include "SimulationDispatcher.h"

#include "SimulatorFiles/SimulatorPrimitives.h"
#include "SimulatorFiles/DistributionMethod.h"
#include "SimulatorFiles/Log.h"
#include "SimulatorFiles/DataSetManager.h"
#include "SimulatorFiles/SetGenerator.h"

#include <QMainWindow>
#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QString>
#include <QComboBox>
#include <QMessageBox>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QProcess>
#include <QStringList>

#include <atomic>
#include <thread>
#include <vector>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    struct GeneratedSetInfo
    {
        int taskSetNumber;
        int resSetNumber;
    };

public:
    MainWindow(QWidget *parent = nullptr) : QMainWindow(parent)
    {
        this->setWindowIcon(QIcon(":/icons/icon.jpg"));
        this->setWindowTitle("Select mode");
        this->resize(300, 350);
        stackedWidget = new QStackedWidget(this);

        connect(selectModePanel.getConfirmButton(), &QPushButton::clicked, this, &MainWindow::onSelModePanelConfirmButtonClicked);
        connect(generateOptionsPanel.getConfirmButton(), &QPushButton::clicked, this, &MainWindow::onGenerateOptPanelConfirmButtonClicked);
        connect(chooseOptionsPanel.getConfirmButton(), &QPushButton::clicked, this, &MainWindow::onChooseOptPanelConfirmButtonClicked);
        connect(simulationSettingsPanel.getConfirmButton(), &QPushButton::clicked, this, &MainWindow::onSimulationSettingsConfirmButtonClicked);

        stackedWidget->addWidget(simulationMainWidget.getPanel());
        stackedWidget->addWidget(selectModePanel.getPanel());
        stackedWidget->addWidget(generateOptionsPanel.getPanel());
        stackedWidget->addWidget(chooseOptionsPanel.getPanel());

        stackedWidget->addWidget(simulationSettingsPanel.getPanel());

        QWidget *centralWidget = new QWidget(this);
        QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
        mainLayout->addWidget(stackedWidget);
        setCentralWidget(centralWidget);

        stackedWidget->setCurrentWidget(selectModePanel.getPanel());
    }

private slots:
    void onSelModePanelConfirmButtonClicked()
    {
        QString selectedMode = selectModePanel.getSelectedMode();
        if (selectedMode == "Generate")
        {
            this->setWindowTitle("Generate set");
            stackedWidget->setCurrentWidget(generateOptionsPanel.getPanel());
        } else if (selectedMode == "Choose")
        {
            this->setWindowTitle("Choose set");
            stackedWidget->setCurrentWidget(chooseOptionsPanel.getPanel());
        }
    }
    void onGenerateOptPanelConfirmButtonClicked()
    {
        dMethodStr = generateOptionsPanel.getSelectedDMethod();
        QString taskCountStr = generateOptionsPanel.getTaskCount();
        QString resCountStr = generateOptionsPanel.getResCount();

        int taskCount = parsePositiveNumber(taskCountStr, "tasks count");
        if (taskCount == -1)
            return;

        int resCount = parsePositiveNumber(resCountStr, "resources count");
        if (resCount == -1)
            return;

        DataSetManager dsm;
        SetGenerator sg(procArchs, osTypes);

        sg.fillRandomTasks(db.tasks, taskCount);
        sg.fillRandomResources(db.availableResources, resCount);

        setInfo.taskSetNumber = dsm.writeTasks(db.tasks);
        setInfo.resSetNumber = dsm.writeResources(db.availableResources);

        switchToSimulationSettingsPanel();
    }
    void onChooseOptPanelConfirmButtonClicked()
    {
        dMethodStr = chooseOptionsPanel.getSelectedDMethod();
        QString setNumberStr = chooseOptionsPanel.getSetNumber();

        int setNumber = parsePositiveNumber(setNumberStr, "set number");
        if (setNumber == -1)
            return;

        DataSetManager dsm;
        dsm.loadTasks(db.tasks, setNumber);
        dsm.loadResources(db.availableResources, setNumber);

        setInfo.taskSetNumber = setNumber;
        setInfo.resSetNumber = setNumber;

        switchToSimulationSettingsPanel();
    }

    void onSimulationSettingsConfirmButtonClicked()
    {
        this->setWindowTitle("Simulation");
        QString channelCountStr = simulationSettingsPanel.getChannelCount();
        QString timeUnitStr = simulationSettingsPanel.getTimeUnit();
        QString timeUnitToSkipStr = simulationSettingsPanel.getTimeUnitToSkip();
        QString waitMilliSecStr = simulationSettingsPanel.getWaitMilliSec();
        QString updatePeriodStr = simulationSettingsPanel.getUpdatePeriod();

        int channelCount = parsePositiveNumber(channelCountStr, "number of channels");
        if (channelCount == -1)
            return;

        int timeUnitToSkip = parsePositiveNumber(timeUnitToSkipStr, "time unit to skip");
        if (timeUnitToSkip == -1)
            return;

        int waitMilliSec = parsePositiveNumber(waitMilliSecStr, "number of milliseconds to wait");
        if (waitMilliSec == -1)
            return;

        int updatePeriod = parsePositiveNumber(updatePeriodStr, "update period");
        if (updatePeriod == -1)
            return;

        simulationMainWidget.setTimeUnit(timeUnitStr);
        initDMethod();
        logger = new Logger(timeUnitStr.toStdString());
        logger->selectDMethod(dm->getId());
        logger->logSimulationMetaData(SimulationMetaData{
            setInfo.taskSetNumber,
            setInfo.resSetNumber,
            dm->getId(),
            channelCount
        });

        simEnv = new SimulationEnvironment(&db, logger);
        simEnv->prepareForSimulation(channelCount, timeUnitToSkip, waitMilliSec);

        std::thread startMainthr([this, &updatePeriod]() {
            this->startMainThreads(updatePeriod);
        });
        startMainthr.detach();

        this->resize(900, 500);

        addDockWidget(Qt::BottomDockWidgetArea, resourcesDockWidget.getDock());
        stackedWidget->setCurrentWidget(simulationMainWidget.getPanel());

        addMenuToSimulationWindow();
    }

    void toggleResDockVisibility(bool visible)
    {
        resourcesDockWidget.getDock()->setVisible(visible);
    }

    void showGraphAction()
    {
        QString program = "python";
        QStringList arguments;
        arguments << "show_graph.py";
        QProcess *startGraphProc = new QProcess();
        startGraphProc->start(program, arguments);

    }
private:
    void addMenuToSimulationWindow()
    {
        QMenuBar *mBar = QMainWindow::menuBar();
        // === View ===
        QMenu *viewMenu = new QMenu("View", mBar);
        mBar->addMenu(viewMenu);

        toggleResPanel = new QAction("Show resources", viewMenu);
        viewMenu->addAction(toggleResPanel);


        toggleResPanel->setCheckable(true);
        toggleResPanel->setChecked(true);

        connect(toggleResPanel, &QAction::triggered, this, &MainWindow::toggleResDockVisibility);

        // === Actions ===
        QMenu *actionsMenu = new QMenu("Actions", mBar);
        mBar->addMenu(actionsMenu);

        QAction *showAction = new QAction("Show graph", actionsMenu);
        actionsMenu->addAction(showAction);

        connect(showAction, &QAction::triggered, this, &MainWindow::showGraphAction);
    }

    void startMainThreads(int updatePeriod)
    {
        std::atomic<bool> stopThread(false);
        SimulationDispatcher simDis(
            simEnv,
            &stopThread,
            &simulationMainWidget,
            &resourcesDockWidget,
            &db,
            updatePeriod
        );
        std::thread dispatcher([&simDis]() {
            simDis.Start();
        });

        std::thread simulation([this]() {
            this->simEnv->runSimulation(this->dm);
        });

        simulation.join();
        stopThread = true;
        dispatcher.join();
    }
    void initDMethod()
    {
        if (dMethodStr == "FCFS")
            dm = new FCFS();
        else if (dMethodStr == "LIFO")
            dm = new LIFO();
        else if (dMethodStr == "HPF")
            dm = new HPF();
        else if (dMethodStr == "BACKFILL")
            dm = new BACKFILL();
        else if (dMethodStr == "SIMPLEX")
            dm = new SIMPLEX();
        else if (dMethodStr == "SMART")
            dm = new SMART();
        else if (dMethodStr == "MFQS")
            dm = new MFQS();
        else if (dMethodStr == "Penguin")
            dm = new Penguin();
        else
            dm = nullptr;
    }
    void switchToSimulationSettingsPanel()
    {
        this->resize(400, 450);
        this->setWindowTitle("Simulation settings");
        stackedWidget->setCurrentWidget(simulationSettingsPanel.getPanel());
    }
    int parsePositiveNumber(const QString& numberToConvert, const QString& fieldName)
    {
        bool ok;
        int number = numberToConvert.toInt(&ok);
        if (!ok)
        {
            QMessageBox::critical(nullptr, "Error", QString("Incorrect value for %1. Please, try again!").arg(fieldName));
            return -1;
        }
        return number;
    }
private:
    QStackedWidget *stackedWidget;

    SelectModePanel selectModePanel;
    GenerateOptionsPanel generateOptionsPanel;
    ChooseOptionsPanel chooseOptionsPanel;
    SimulationSettingsPanel simulationSettingsPanel;
    SimulationMainWidget simulationMainWidget;
    ResourcesDockWidget resourcesDockWidget;

    // === Simulation options ===
    QString dMethodStr;
    QString timeUnit;
    int channelCount;
    int timeUnitToSkip;
    int waitMilliSec;
    int updatePeriod;

    // === Simulation variables ===
    DataBase db;
    SimulationEnvironment* simEnv;
    Logger* logger;
    DistributionMethod* dm;
    GeneratedSetInfo setInfo;
    std::vector<std::string> procArchs = { "x86" };
    std::vector<std::string> osTypes = { "Windows", "Linux" };

    // === menu items ===
    QAction *toggleResPanel;
};


#endif // MAINWINDOW_H
