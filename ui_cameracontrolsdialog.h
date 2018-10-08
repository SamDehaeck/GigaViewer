/********************************************************************************
** Form generated from reading UI file 'cameracontrolsdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CAMERACONTROLSDIALOG_H
#define UI_CAMERACONTROLSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_CameraControlsDialog
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QPushButton *pushButton_2;
    QPushButton *IntensityButton;
    QPushButton *FitRangeButton;
    QPushButton *FitMeanButton;
    QSpinBox *shutterSpinBox;
    QLineEdit *ROIRows;
    QLabel *label_4;
    QLineEdit *ROICols;
    QLabel *IntensLabel;

    void setupUi(QDialog *CameraControlsDialog)
    {
        if (CameraControlsDialog->objectName().isEmpty())
            CameraControlsDialog->setObjectName(QStringLiteral("CameraControlsDialog"));
        CameraControlsDialog->resize(468, 125);
        CameraControlsDialog->setWindowOpacity(0.8);
        gridLayout = new QGridLayout(CameraControlsDialog);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label = new QLabel(CameraControlsDialog);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        pushButton_2 = new QPushButton(CameraControlsDialog);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setEnabled(true);
        pushButton_2->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(pushButton_2, 4, 0, 1, 1);

        IntensityButton = new QPushButton(CameraControlsDialog);
        IntensityButton->setObjectName(QStringLiteral("IntensityButton"));
        IntensityButton->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(IntensityButton, 2, 0, 1, 1);

        FitRangeButton = new QPushButton(CameraControlsDialog);
        FitRangeButton->setObjectName(QStringLiteral("FitRangeButton"));
        FitRangeButton->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(FitRangeButton, 0, 2, 1, 1);

        FitMeanButton = new QPushButton(CameraControlsDialog);
        FitMeanButton->setObjectName(QStringLiteral("FitMeanButton"));
        FitMeanButton->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(FitMeanButton, 0, 3, 1, 1);

        shutterSpinBox = new QSpinBox(CameraControlsDialog);
        shutterSpinBox->setObjectName(QStringLiteral("shutterSpinBox"));
        shutterSpinBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        shutterSpinBox->setKeyboardTracking(true);
        shutterSpinBox->setMaximum(9999999);
        shutterSpinBox->setSingleStep(100);
        shutterSpinBox->setValue(100);

        gridLayout->addWidget(shutterSpinBox, 0, 1, 1, 1);

        ROIRows = new QLineEdit(CameraControlsDialog);
        ROIRows->setObjectName(QStringLiteral("ROIRows"));

        gridLayout->addWidget(ROIRows, 4, 1, 1, 1);

        label_4 = new QLabel(CameraControlsDialog);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_4, 4, 2, 1, 1);

        ROICols = new QLineEdit(CameraControlsDialog);
        ROICols->setObjectName(QStringLiteral("ROICols"));

        gridLayout->addWidget(ROICols, 4, 3, 1, 1);

        IntensLabel = new QLabel(CameraControlsDialog);
        IntensLabel->setObjectName(QStringLiteral("IntensLabel"));
        IntensLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(IntensLabel, 2, 1, 1, 3);

        QWidget::setTabOrder(shutterSpinBox, FitRangeButton);
        QWidget::setTabOrder(FitRangeButton, FitMeanButton);
        QWidget::setTabOrder(FitMeanButton, IntensityButton);
        QWidget::setTabOrder(IntensityButton, ROIRows);
        QWidget::setTabOrder(ROIRows, ROICols);
        QWidget::setTabOrder(ROICols, pushButton_2);

        retranslateUi(CameraControlsDialog);

        QMetaObject::connectSlotsByName(CameraControlsDialog);
    } // setupUi

    void retranslateUi(QDialog *CameraControlsDialog)
    {
        CameraControlsDialog->setWindowTitle(QApplication::translate("CameraControlsDialog", "Camera Controls", 0));
        label->setText(QApplication::translate("CameraControlsDialog", "Shutter Time (us)", 0));
        pushButton_2->setText(QApplication::translate("CameraControlsDialog", "Reset ROI", 0));
        IntensityButton->setText(QApplication::translate("CameraControlsDialog", "Analyze Image", 0));
        FitRangeButton->setText(QApplication::translate("CameraControlsDialog", "Fit Range", 0));
        FitMeanButton->setText(QApplication::translate("CameraControlsDialog", "Fit Mean", 0));
        label_4->setText(QApplication::translate("CameraControlsDialog", "x", 0));
        IntensLabel->setText(QApplication::translate("CameraControlsDialog", "0", 0));
    } // retranslateUi

};

namespace Ui {
    class CameraControlsDialog: public Ui_CameraControlsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CAMERACONTROLSDIALOG_H
