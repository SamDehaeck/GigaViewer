/********************************************************************************
** Form generated from reading UI file 'fileinputdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILEINPUTDIALOG_H
#define UI_FILEINPUTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_FileInputDialog
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QPushButton *camButton;
    QPushButton *MovieButton;
    QPushButton *pushButton_2;
    QPushButton *AvtButton;
    QPushButton *filePushButton;
    QSpacerItem *verticalSpacer;
    QPushButton *vimbaButton;
    QPushButton *idsButton;

    void setupUi(QDialog *FileInputDialog)
    {
        if (FileInputDialog->objectName().isEmpty())
            FileInputDialog->setObjectName(QStringLiteral("FileInputDialog"));
        FileInputDialog->resize(127, 330);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(FileInputDialog->sizePolicy().hasHeightForWidth());
        FileInputDialog->setSizePolicy(sizePolicy);
        FileInputDialog->setWindowOpacity(0.8);
        verticalLayout = new QVBoxLayout(FileInputDialog);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        camButton = new QPushButton(FileInputDialog);
        camButton->setObjectName(QStringLiteral("camButton"));
        camButton->setCheckable(false);

        gridLayout->addWidget(camButton, 2, 0, 1, 1);

        MovieButton = new QPushButton(FileInputDialog);
        MovieButton->setObjectName(QStringLiteral("MovieButton"));

        gridLayout->addWidget(MovieButton, 1, 0, 1, 1);

        pushButton_2 = new QPushButton(FileInputDialog);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        gridLayout->addWidget(pushButton_2, 7, 0, 1, 1);

        AvtButton = new QPushButton(FileInputDialog);
        AvtButton->setObjectName(QStringLiteral("AvtButton"));
        AvtButton->setCheckable(false);

        gridLayout->addWidget(AvtButton, 3, 0, 1, 1);

        filePushButton = new QPushButton(FileInputDialog);
        filePushButton->setObjectName(QStringLiteral("filePushButton"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(filePushButton->sizePolicy().hasHeightForWidth());
        filePushButton->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(filePushButton, 0, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 6, 0, 1, 1);

        vimbaButton = new QPushButton(FileInputDialog);
        vimbaButton->setObjectName(QStringLiteral("vimbaButton"));
        vimbaButton->setCheckable(false);

        gridLayout->addWidget(vimbaButton, 4, 0, 1, 1);

        idsButton = new QPushButton(FileInputDialog);
        idsButton->setObjectName(QStringLiteral("idsButton"));

        gridLayout->addWidget(idsButton, 5, 0, 1, 1);


        verticalLayout->addLayout(gridLayout);

        QWidget::setTabOrder(filePushButton, MovieButton);
        QWidget::setTabOrder(MovieButton, camButton);
        QWidget::setTabOrder(camButton, AvtButton);
        QWidget::setTabOrder(AvtButton, pushButton_2);

        retranslateUi(FileInputDialog);

        QMetaObject::connectSlotsByName(FileInputDialog);
    } // setupUi

    void retranslateUi(QDialog *FileInputDialog)
    {
        FileInputDialog->setWindowTitle(QApplication::translate("FileInputDialog", "Select Input", 0));
        camButton->setText(QApplication::translate("FileInputDialog", "Opencv Feed", 0));
        MovieButton->setText(QApplication::translate("FileInputDialog", "Load Movie", 0));
        pushButton_2->setText(QApplication::translate("FileInputDialog", "Exit App", 0));
        AvtButton->setText(QApplication::translate("FileInputDialog", "AVT Feed", 0));
        filePushButton->setText(QApplication::translate("FileInputDialog", "Load Picture", 0));
        vimbaButton->setText(QApplication::translate("FileInputDialog", "Vimba Feed", 0));
        idsButton->setText(QApplication::translate("FileInputDialog", "Ids Feed", 0));
    } // retranslateUi

};

namespace Ui {
    class FileInputDialog: public Ui_FileInputDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILEINPUTDIALOG_H
