/********************************************************************************
** Form generated from reading UI file 'showcal.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHOWCAL_H
#define UI_SHOWCAL_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ShowCal
{
public:
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *nextButton;
    QPushButton *computeButton;

    void setupUi(QDialog *ShowCal)
    {
        if (ShowCal->objectName().isEmpty())
            ShowCal->setObjectName(QString::fromUtf8("ShowCal"));
        ShowCal->resize(1308, 513);
        QIcon icon;
        icon.addFile(QString::fromUtf8("eye.png"), QSize(), QIcon::Normal, QIcon::Off);
        ShowCal->setWindowIcon(icon);
        ShowCal->setModal(false);
        horizontalLayoutWidget = new QWidget(ShowCal);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(9, 9, 1291, 481));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        nextButton = new QPushButton(ShowCal);
        nextButton->setObjectName(QString::fromUtf8("nextButton"));
        nextButton->setGeometry(QRect(30, 490, 80, 22));
        computeButton = new QPushButton(ShowCal);
        computeButton->setObjectName(QString::fromUtf8("computeButton"));
        computeButton->setGeometry(QRect(180, 490, 80, 22));

        retranslateUi(ShowCal);

        QMetaObject::connectSlotsByName(ShowCal);
    } // setupUi

    void retranslateUi(QDialog *ShowCal)
    {
        ShowCal->setWindowTitle(QApplication::translate("ShowCal", "\351\217\215\345\233\247\347\225\276", nullptr));
        nextButton->setText(QApplication::translate("ShowCal", "\344\270\213\344\270\200\347\273\204", nullptr));
        computeButton->setText(QApplication::translate("ShowCal", "\350\256\241\347\256\227", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ShowCal: public Ui_ShowCal {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHOWCAL_H
