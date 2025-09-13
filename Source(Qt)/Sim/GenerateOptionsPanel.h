#ifndef GENERATEOPTIONSPANEL_H
#define GENERATEOPTIONSPANEL_H


#include <QWidget>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>

class GenerateOptionsPanel : public QWidget
{
    Q_OBJECT
public:
    GenerateOptionsPanel();
    QWidget* getPanel() { return generateOptionsPanel; }
    QPushButton* getConfirmButton() { return confirmButton; }

    QString getSelectedDMethod() { return chooseMethod->currentText(); }
    QString getTaskCount() { return taskCount->text(); }
    QString getResCount() { return resCount->text(); }
private:
    QWidget *generateOptionsPanel;
    QPushButton *confirmButton;

    QComboBox *chooseMethod;
    QLineEdit *taskCount;
    QLineEdit *resCount;
};

#endif // GENERATEOPTIONSPANEL_H
