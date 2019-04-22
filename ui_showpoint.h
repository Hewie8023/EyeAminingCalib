/********************************************************************************
** Form generated from reading UI file 'showpoint.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHOWPOINT_H
#define UI_SHOWPOINT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>

QT_BEGIN_NAMESPACE

class Ui_ShowPoint
{
public:

    void setupUi(QDialog *ShowPoint)
    {
        if (ShowPoint->objectName().isEmpty())
            ShowPoint->setObjectName(QString::fromUtf8("ShowPoint"));
        ShowPoint->resize(1261, 790);

        retranslateUi(ShowPoint);

        QMetaObject::connectSlotsByName(ShowPoint);
    } // setupUi

    void retranslateUi(QDialog *ShowPoint)
    {
        ShowPoint->setWindowTitle(QApplication::translate("ShowPoint", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ShowPoint: public Ui_ShowPoint {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHOWPOINT_H
