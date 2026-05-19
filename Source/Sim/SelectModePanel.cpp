#include "SelectModePanel.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

SelectModePanel::SelectModePanel()
{
    selectModePanel = new QWidget();
    QHBoxLayout *mainLayout = new QHBoxLayout(selectModePanel);

    QWidget *container = new QWidget();
    QVBoxLayout *vertLayout = new QVBoxLayout();

    label = new QLabel("Choose how to obtain a new set:");
    genlabel = new QLabel("Generate");
    chlabel = new QLabel("Choose");

    comboBox = new QComboBox();
    comboBox->addItem(genlabel->text(), "generate_key");
    comboBox->addItem(chlabel->text(), "choose_key");
    comboBox->setCurrentIndex(0);

    langComboBox = new QComboBox();
    langComboBox->addItem("En", "en");
    langComboBox->addItem("Укр", "ukr");
    langComboBox->setCurrentIndex(0);
    langComboBox->setFixedWidth(60);

    confirmButton = new QPushButton("Ok");

    vertLayout->addWidget(langComboBox, 0, Qt::AlignLeft);
    vertLayout->addStretch(2);
    vertLayout->addWidget(label);
    vertLayout->addWidget(comboBox);
    vertLayout->addSpacing(10);
    vertLayout->addWidget(confirmButton);
    vertLayout->addStretch(3);

    container->setLayout(vertLayout);

    mainLayout->addStretch();
    mainLayout->addWidget(container);
    mainLayout->addStretch();
}

void SelectModePanel::retranslateui() {
    label->setText(tr("Choose how to obtain a new set:"));
    genlabel->setText(tr("Generate"));
    chlabel->setText(tr("Choose"));
    comboBox->removeItem(1);
    comboBox->removeItem(0);
    comboBox->addItem(genlabel->text(), "generate_key");
    comboBox->addItem(chlabel->text(), "choose_key");
    comboBox->setCurrentIndex(0);
    confirmButton->setText(tr("Ok"));
}

