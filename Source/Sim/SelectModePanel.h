#ifndef SELECTMODEPANEL_H
#define SELECTMODEPANEL_H

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QString>
#include <QLabel>

class SelectModePanel : public QWidget
{
    Q_OBJECT
public:
    SelectModePanel();
    QWidget* getPanel() { return selectModePanel; }
    QPushButton* getConfirmButton() { return confirmButton; }
    QString getSelectedMode() { return comboBox->currentData().toString(); }

    QComboBox *langComboBox;

    void retranslateui();
private:
    QWidget *selectModePanel;
    QPushButton *confirmButton;
    QComboBox *comboBox;
    QLabel *label;
    QLabel *genlabel;
    QLabel *chlabel;
};

#endif // SELECTMODEPANEL_H
