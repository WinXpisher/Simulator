#ifndef SIMULATIONSETTINGSPANEL_H
#define SIMULATIONSETTINGSPANEL_H

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QString>
#include <QLineEdit>
#include <QLabel>

class SimulationSettingsPanel : public QWidget
{
    Q_OBJECT
public:
    SimulationSettingsPanel();
    QWidget* getPanel() { return simulationSettingsPanel; }
    QPushButton* getConfirmButton() { return confirmButton; }

    QString getChannelCount() { return channelCountInput->text(); }
    QString getTimeUnit() { return timeUnitInput->text(); }
    QString getTimeUnitToSkip() { return timeUnitToSkipInput->text(); }
    QString getWaitMilliSec() { return waitMilliSecInput->text(); }
    QString getUpdatePeriod() { return updatePeriodInput->text(); }
    QPushButton* getBackButton() { return backButton; }

    void retranslateui();
private:
    QWidget *simulationSettingsPanel;
    QPushButton *confirmButton;

    QLineEdit *channelCountInput;
    QLineEdit *timeUnitInput;
    QLineEdit *timeUnitToSkipInput;
    QLineEdit *waitMilliSecInput;
    QLineEdit *updatePeriodInput;

    QLabel *channelCountLabel;
    QLabel *timeUnitLabel;
    QLabel *timeUnitToSkipLabel;
    QLabel *waitMilliSecLabel;
    QLabel *updatePeriodLabel;
    QPushButton* backButton;
};

#endif // SIMULATIONSETTINGSPANEL_H
