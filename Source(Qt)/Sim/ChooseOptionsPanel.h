#ifndef CHOOSEOPTIONSPANEL_H
#define CHOOSEOPTIONSPANEL_H

#include <QWidget>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>

class ChooseOptionsPanel : public QWidget
{
    Q_OBJECT
public:
    ChooseOptionsPanel();
    QWidget* getPanel() { return chooseOptionsPanel; }
    QPushButton* getConfirmButton() { return confirmButton; }

    QString getSelectedDMethod() { return chooseMethod->currentText(); }
    QString getSetNumber() { return setNumber->text(); }
private:
    QWidget *chooseOptionsPanel;
    QPushButton *confirmButton;
    QComboBox *chooseMethod;
    QLineEdit *setNumber;
};


#endif // CHOOSEOPTIONSPANEL_H
