/********************************************************************************
** Form generated from reading UI file 'loadprofiledialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOADPROFILEDIALOG_H
#define UI_LOADPROFILEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoadProfileDialog
{
public:
    QDialogButtonBox *buttonBox;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;

    void setupUi(QDialog *LoadProfileDialog)
    {
        if (LoadProfileDialog->objectName().isEmpty())
            LoadProfileDialog->setObjectName(QString::fromUtf8("LoadProfileDialog"));
        LoadProfileDialog->resize(412, 399);
        buttonBox = new QDialogButtonBox(LoadProfileDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(30, 350, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(false);
        verticalLayoutWidget = new QWidget(LoadProfileDialog);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 10, 391, 381));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);

        retranslateUi(LoadProfileDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), LoadProfileDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), LoadProfileDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(LoadProfileDialog);
    } // setupUi

    void retranslateUi(QDialog *LoadProfileDialog)
    {
        LoadProfileDialog->setWindowTitle(QApplication::translate("LoadProfileDialog", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LoadProfileDialog: public Ui_LoadProfileDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOADPROFILEDIALOG_H
