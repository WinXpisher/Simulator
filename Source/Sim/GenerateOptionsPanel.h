#ifndef GENERATEOPTIONSPANEL_H
#define GENERATEOPTIONSPANEL_H

#include <QWidget>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>

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
    QPushButton* getBackButton() { return backButton; }

    void retranslateui();
private:
    QWidget *generateOptionsPanel;
    QPushButton *confirmButton;
    QLabel *methodLabel;
    QLabel *taskCountLabel;
    QLabel *resCountLabel;

    QComboBox *chooseMethod;
    QLineEdit *taskCount;
    QLineEdit *resCount;
    QPushButton* backButton;
};

#endif // GENERATEOPTIONSPANEL_H
