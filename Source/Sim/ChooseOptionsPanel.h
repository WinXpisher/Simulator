#ifndef CHOOSEOPTIONSPANEL_H
#define CHOOSEOPTIONSPANEL_H

#include <QWidget>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>

class ChooseOptionsPanel : public QWidget
{
    Q_OBJECT
public:
    ChooseOptionsPanel();
    QWidget* getPanel() { return chooseOptionsPanel; }
    QPushButton* getConfirmButton() { return confirmButton; }

    QString getSelectedDMethod() { return chooseMethod->currentText(); }
    QString getSetNumber() { return setNumber->text(); }
    QPushButton* getBackButton() { return backButton; }

    void retranslateui();
private:
    QWidget *chooseOptionsPanel;
    QPushButton *confirmButton;
    QComboBox *chooseMethod;
    QLineEdit *setNumber;
    QLabel *methodLabel;
    QLabel *setNumberLabel;
    QPushButton* backButton;
};


#endif // CHOOSEOPTIONSPANEL_H
