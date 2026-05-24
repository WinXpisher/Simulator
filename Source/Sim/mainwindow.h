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
#include <QtGlobal>
#include <QTranslator>
#include <QEvent>
#include <QStack>
#include <QDir>

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
        writeTranslFile("en");

        connect(selectModePanel.getConfirmButton(), &QPushButton::clicked, this, &MainWindow::onSelModePanelConfirmButtonClicked);
        connect(generateOptionsPanel.getConfirmButton(), &QPushButton::clicked, this, &MainWindow::onGenerateOptPanelConfirmButtonClicked);
        connect(chooseOptionsPanel.getConfirmButton(), &QPushButton::clicked, this, &MainWindow::onChooseOptPanelConfirmButtonClicked);
        connect(simulationSettingsPanel.getConfirmButton(), &QPushButton::clicked, this, &MainWindow::onSimulationSettingsConfirmButtonClicked);
        connect(selectModePanel.langComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::on_langComboBox_currentIndexChanged);

        connect(generateOptionsPanel.getBackButton(), &QPushButton::clicked, this, &MainWindow::onBackRequested);
        connect(chooseOptionsPanel.getBackButton(), &QPushButton::clicked, this, &MainWindow::onBackRequested);
        connect(simulationSettingsPanel.getBackButton(), &QPushButton::clicked, this, &MainWindow::onBackRequested);

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

    void on_langComboBox_currentIndexChanged(int index) {
        QString lang = selectModePanel.langComboBox->currentData().toString();;

        qApp->removeTranslator(&m_translator);

        if (lang == "ukr") {
            if (m_translator.load(":/translations/ukr_transl.qm")) {
                qApp->installTranslator(&m_translator);
            }
            writeTranslFile("ukr");
        }
        else writeTranslFile("en");
    }

    void onBackRequested()
    {
        if (!m_history.isEmpty())
        {
            QWidget* prevPanel = m_history.pop();
            stackedWidget->setCurrentWidget(prevPanel);

            if (prevPanel == selectModePanel.getPanel()) {
                this->resize(300, 350);
            } else if (prevPanel == generateOptionsPanel.getPanel() || prevPanel == chooseOptionsPanel.getPanel()) {
                this->resize(300, 350);
            } else if (prevPanel == simulationSettingsPanel.getPanel()) {
                this->resize(400, 450);
            }

            retranslateui();
        }
    }

    void onSelModePanelConfirmButtonClicked()
    {
        QString selectedMode = selectModePanel.getSelectedMode();
        if (selectedMode == "generate_key")
        {
            m_history.push(stackedWidget->currentWidget());
            this->setWindowTitle("Generate set");
            stackedWidget->setCurrentWidget(generateOptionsPanel.getPanel());
            retranslateui();
        } else if (selectedMode == "choose_key")
        {
            m_history.push(stackedWidget->currentWidget());
            this->setWindowTitle("Choose set");
            stackedWidget->setCurrentWidget(chooseOptionsPanel.getPanel());
            retranslateui();
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

        sg.fillRandomResources(db.availableResources, resCount);
        sg.fillRandomTasks(
            db.tasks,
            taskCount,
            db.availableResources,
            0.1
            );

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

        m_history.clear();
        this->resize(900, 500);

        addDockWidget(Qt::BottomDockWidgetArea, resourcesDockWidget.getDock());
        stackedWidget->setCurrentWidget(simulationMainWidget.getPanel());

        addMenuToSimulationWindow();
        retranslateui();
    }

    void toggleResDockVisibility(bool visible)
    {
        resourcesDockWidget.getDock()->setVisible(visible);
    }

    void showGraphAction()
    {
        #ifdef Q_OS_WIN
            QString program = "dist/Wshow_graph.exe";
        #elif defined(Q_OS_UNIX)
            QString program = "./dist/Lshow_graph";
        #endif

        QStringList arguments;
        QProcess *startGraphProc = new QProcess();
        startGraphProc->start(program, arguments);
    }

    void showGraphsAction()
    {
        #ifdef Q_OS_WIN
                QString program = "dist/Wshow_graphs.exe";
        #elif defined(Q_OS_UNIX)
                QString program = "./dist/Lshow_graphs";
        #endif
        QStringList arguments;
        QProcess *startGraphProc = new QProcess();
        startGraphProc->start(program, arguments);
    }

protected:
    void changeEvent(QEvent *event)
    {

        if (event->type() == QEvent::LanguageChange) {
            retranslateui();
        }
        QMainWindow::changeEvent(event);
    }

private:

    void retranslateui() {
        QWidget* current = stackedWidget->currentWidget();
        selectModePanel.retranslateui();
        generateOptionsPanel.retranslateui();
        chooseOptionsPanel.retranslateui();
        simulationSettingsPanel.retranslateui();
        if (current == simulationMainWidget.getPanel()) {
            viewMenu->setTitle(tr("View"));
            toggleResPanel->setText(tr("Show resources"));
            actionsMenu->setTitle(tr("Actions"));
            showOneGraphAction->setText(tr("Show graph"));
            showManyGraphsAction->setText(tr("Show graphs"));
        }
        updateWindowTitle();
    }

    void updateWindowTitle() {
        QWidget* current = stackedWidget->currentWidget();

        if (current == selectModePanel.getPanel()) {
            this->setWindowTitle(tr("Select mode"));
        } else if (current == generateOptionsPanel.getPanel()) {
            this->setWindowTitle(tr("Generate set"));
        } else if (current == chooseOptionsPanel.getPanel()) {
            this->setWindowTitle(tr("Choose set"));
        } else if (current == simulationSettingsPanel.getPanel()) {
            this->setWindowTitle(tr("Simulation settings"));
        } else if (current == simulationMainWidget.getPanel()) {
            this->setWindowTitle(tr("Simulation"));
        }
    }

    void writeTranslFile(const QString &lang)
    {
        QString workingDir = QDir::currentPath();
        QString distPath = workingDir + "/dist";

        QDir dir;
        if (!dir.exists(distPath)) {
            if (!dir.mkpath(distPath)) {
                return;
            }
        }

        QString filePath = distPath + "/lang.conf";
        QFile file(filePath);

        if (file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
            QTextStream out(&file);
            out << lang.trimmed().toLower();
            file.close();
        }
    }

    void addMenuToSimulationWindow()
    {
        mBar = QMainWindow::menuBar();
        // --- VIEW ---
        viewMenu = new QMenu("View", mBar);
        mBar->addMenu(viewMenu);

        toggleResPanel = new QAction("Show resources", viewMenu);
        viewMenu->addAction(toggleResPanel);


        toggleResPanel->setCheckable(true);
        toggleResPanel->setChecked(true);

        connect(toggleResPanel, &QAction::triggered, this, &MainWindow::toggleResDockVisibility);

        // --- ACTIONS ---
        actionsMenu = new QMenu("Actions", mBar);
        mBar->addMenu(actionsMenu);

        showOneGraphAction = new QAction("Show graph", actionsMenu);
        actionsMenu->addAction(showOneGraphAction);
        connect(showOneGraphAction, &QAction::triggered, this, &MainWindow::showGraphAction);

        showManyGraphsAction = new QAction("Show graphs", actionsMenu);
        actionsMenu->addAction(showManyGraphsAction);
        connect(showManyGraphsAction, &QAction::triggered, this, &MainWindow::showGraphsAction);
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
        m_history.push(stackedWidget->currentWidget());
        this->resize(400, 450);
        this->setWindowTitle("Simulation settings");
        stackedWidget->setCurrentWidget(simulationSettingsPanel.getPanel());
        retranslateui();
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
    QStack<QWidget*> m_history;

    SelectModePanel selectModePanel;
    GenerateOptionsPanel generateOptionsPanel;
    ChooseOptionsPanel chooseOptionsPanel;
    SimulationSettingsPanel simulationSettingsPanel;
    SimulationMainWidget simulationMainWidget;
    ResourcesDockWidget resourcesDockWidget;

    // --- SIMULATION OPTIONS ---
    QString dMethodStr;
    QString timeUnit;
    int channelCount;
    int timeUnitToSkip;
    int waitMilliSec;
    int updatePeriod;

    // --- SIMULATION VARIABLES ---
    DataBase db;
    SimulationEnvironment* simEnv;
    Logger* logger;
    DistributionMethod* dm;
    GeneratedSetInfo setInfo;
    std::vector<std::string> procArchs = { "x86" };
    std::vector<std::string> osTypes = { "Windows", "Linux" };

    // --- MENU ITEMS ---
    QAction *toggleResPanel;

    // --- TRANSLATION ---
    QTranslator m_translator;
    QMenuBar *mBar;
    QMenu *viewMenu;
    QMenu *actionsMenu;
    QAction *showOneGraphAction;
    QAction *showManyGraphsAction;
};


#endif // MAINWINDOW_H
