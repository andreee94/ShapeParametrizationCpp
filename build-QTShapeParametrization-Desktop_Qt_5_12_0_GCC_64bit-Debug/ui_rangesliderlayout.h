/********************************************************************************
** Form generated from reading UI file 'rangesliderlayout.ui'
**
** Created by: Qt User Interface Compiler version 5.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RANGESLIDERLAYOUT_H
#define UI_RANGESLIDERLAYOUT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RangeSliderLayout
{
public:

    void setupUi(QWidget *RangeSliderLayout)
    {
        if (RangeSliderLayout->objectName().isEmpty())
            RangeSliderLayout->setObjectName(QString::fromUtf8("RangeSliderLayout"));
        RangeSliderLayout->resize(400, 300);

        retranslateUi(RangeSliderLayout);

        QMetaObject::connectSlotsByName(RangeSliderLayout);
    } // setupUi

    void retranslateUi(QWidget *RangeSliderLayout)
    {
        RangeSliderLayout->setWindowTitle(QApplication::translate("RangeSliderLayout", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RangeSliderLayout: public Ui_RangeSliderLayout {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RANGESLIDERLAYOUT_H
