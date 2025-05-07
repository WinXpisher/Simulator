#ifndef SELECTMODEPANEL_H
#define SELECTMODEPANEL_H

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QString>

class SelectModePanel : public QWidget
{
    Q_OBJECT
public:
    SelectModePanel();
    QWidget* getPanel() { return selectModePanel; }
    QPushButton* getConfirmButton() { return confirmButton; }
    QString getSelectedMode() { return comboBox->currentText(); }
private:
    QWidget *selectModePanel;
    QPushButton *confirmButton;
    QComboBox *comboBox;
};

#endif // SELECTMODEPANEL_H
