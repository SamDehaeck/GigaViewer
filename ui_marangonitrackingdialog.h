/********************************************************************************
** Form generated from reading UI file 'marangonitrackingdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MARANGONITRACKINGDIALOG_H
#define UI_MARANGONITRACKINGDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MarangoniTrackingDialog
{
public:
    QLabel *label_2;
    QLabel *label_4;
    QCheckBox *activateBox;
    QSlider *horPosition;
    QLabel *label_3;
    QSlider *vertPosition;
    QLabel *label_11;
    QLabel *label_13;
    QLabel *label_14;
    QLabel *label_15;
    QLabel *label_16;
    QLabel *label_17;
    QLineEdit *line_kp;
    QLineEdit *line_Ti;
    QLineEdit *line_Td;
    QLineEdit *line_Usat;
    QLineEdit *line_Tt;
    QLabel *label_18;
    QLineEdit *line_Tsamp;
    QLabel *label_19;
    QLineEdit *line_Kff;
    QLabel *label_20;
    QCheckBox *chkbox_Tracking;
    QLineEdit *line_Xtarg;
    QLineEdit *line_Ytarg;
    QGroupBox *groupBox;
    QLabel *label_8;
    QComboBox *cbox_LasOrient;
    QLabel *label_26;
    QLabel *label_9;
    QLineEdit *line_PatternA;
    QLineEdit *line_LasPartD;
    QLabel *label_27;
    QLineEdit *line_PatternR;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout;
    QLabel *label_7;
    QHBoxLayout *horizontalLayout;
    QHBoxLayout *horizontalLayout_2;
    QComboBox *cbox_SampleSense;
    QLabel *label_12;
    QLabel *label_22;
    QSlider *Xlaser_slider;
    QLabel *Xlaser_label;
    QLabel *label_23;
    QSlider *Ylaser_slider;
    QLabel *Ylaser_label;
    QLabel *label_10;
    QSlider *Alaser_slider;
    QLabel *Alaser_label;
    QLabel *label_24;
    QLabel *Rpattern_label;
    QLabel *label_25;
    QSlider *Aux_slider;
    QLabel *Aux_label;
    QSlider *Rpattern_slider;
    QLineEdit *line_SampleRate;
    QWidget *layoutWidget;
    QGridLayout *gridLayout_2;
    QLabel *label_5;
    QComboBox *cbox_PatternType;
    QLabel *label_6;
    QComboBox *cbox_OpMode;
    QSlider *thresholdSlider;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_21;
    QSpinBox *spinBox_NroPart;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label;
    QLineEdit *threshEdit;
    QSpacerItem *verticalSpacer;

    void setupUi(QDialog *MarangoniTrackingDialog)
    {
        if (MarangoniTrackingDialog->objectName().isEmpty())
            MarangoniTrackingDialog->setObjectName(QStringLiteral("MarangoniTrackingDialog"));
        MarangoniTrackingDialog->resize(593, 414);
        MarangoniTrackingDialog->setWindowOpacity(0.8);
        label_2 = new QLabel(MarangoniTrackingDialog);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(60, 200, 91, 16));
        label_4 = new QLabel(MarangoniTrackingDialog);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(16, 250, 81, 20));
        activateBox = new QCheckBox(MarangoniTrackingDialog);
        activateBox->setObjectName(QStringLiteral("activateBox"));
        activateBox->setGeometry(QRect(10, 10, 211, 17));
        horPosition = new QSlider(MarangoniTrackingDialog);
        horPosition->setObjectName(QStringLiteral("horPosition"));
        horPosition->setGeometry(QRect(113, 231, 121, 19));
        horPosition->setMaximum(1024);
        horPosition->setValue(500);
        horPosition->setSliderPosition(500);
        horPosition->setOrientation(Qt::Horizontal);
        horPosition->setTickPosition(QSlider::NoTicks);
        horPosition->setTickInterval(10);
        label_3 = new QLabel(MarangoniTrackingDialog);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(16, 225, 81, 20));
        vertPosition = new QSlider(MarangoniTrackingDialog);
        vertPosition->setObjectName(QStringLiteral("vertPosition"));
        vertPosition->setGeometry(QRect(113, 256, 121, 19));
        vertPosition->setMaximum(1024);
        vertPosition->setValue(500);
        vertPosition->setSliderPosition(500);
        vertPosition->setOrientation(Qt::Horizontal);
        label_11 = new QLabel(MarangoniTrackingDialog);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(100, 380, 341, 31));
        label_13 = new QLabel(MarangoniTrackingDialog);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setGeometry(QRect(10, 310, 21, 20));
        label_14 = new QLabel(MarangoniTrackingDialog);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setGeometry(QRect(10, 340, 21, 16));
        label_15 = new QLabel(MarangoniTrackingDialog);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setGeometry(QRect(10, 360, 21, 16));
        label_16 = new QLabel(MarangoniTrackingDialog);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setGeometry(QRect(100, 310, 71, 16));
        label_17 = new QLabel(MarangoniTrackingDialog);
        label_17->setObjectName(QStringLiteral("label_17"));
        label_17->setGeometry(QRect(100, 340, 61, 20));
        line_kp = new QLineEdit(MarangoniTrackingDialog);
        line_kp->setObjectName(QStringLiteral("line_kp"));
        line_kp->setGeometry(QRect(30, 310, 61, 20));
        line_Ti = new QLineEdit(MarangoniTrackingDialog);
        line_Ti->setObjectName(QStringLiteral("line_Ti"));
        line_Ti->setGeometry(QRect(30, 340, 61, 20));
        line_Td = new QLineEdit(MarangoniTrackingDialog);
        line_Td->setObjectName(QStringLiteral("line_Td"));
        line_Td->setGeometry(QRect(30, 360, 61, 20));
        line_Usat = new QLineEdit(MarangoniTrackingDialog);
        line_Usat->setObjectName(QStringLiteral("line_Usat"));
        line_Usat->setGeometry(QRect(160, 310, 61, 20));
        line_Tt = new QLineEdit(MarangoniTrackingDialog);
        line_Tt->setObjectName(QStringLiteral("line_Tt"));
        line_Tt->setGeometry(QRect(170, 340, 61, 20));
        label_18 = new QLabel(MarangoniTrackingDialog);
        label_18->setObjectName(QStringLiteral("label_18"));
        label_18->setGeometry(QRect(100, 360, 71, 20));
        line_Tsamp = new QLineEdit(MarangoniTrackingDialog);
        line_Tsamp->setObjectName(QStringLiteral("line_Tsamp"));
        line_Tsamp->setGeometry(QRect(170, 360, 61, 20));
        label_19 = new QLabel(MarangoniTrackingDialog);
        label_19->setObjectName(QStringLiteral("label_19"));
        label_19->setGeometry(QRect(6, 380, 21, 16));
        line_Kff = new QLineEdit(MarangoniTrackingDialog);
        line_Kff->setObjectName(QStringLiteral("line_Kff"));
        line_Kff->setGeometry(QRect(30, 380, 61, 20));
        label_20 = new QLabel(MarangoniTrackingDialog);
        label_20->setObjectName(QStringLiteral("label_20"));
        label_20->setGeometry(QRect(240, 340, 131, 16));
        chkbox_Tracking = new QCheckBox(MarangoniTrackingDialog);
        chkbox_Tracking->setObjectName(QStringLiteral("chkbox_Tracking"));
        chkbox_Tracking->setGeometry(QRect(20, 280, 151, 17));
        line_Xtarg = new QLineEdit(MarangoniTrackingDialog);
        line_Xtarg->setObjectName(QStringLiteral("line_Xtarg"));
        line_Xtarg->setGeometry(QRect(240, 230, 41, 20));
        line_Ytarg = new QLineEdit(MarangoniTrackingDialog);
        line_Ytarg->setObjectName(QStringLiteral("line_Ytarg"));
        line_Ytarg->setGeometry(QRect(240, 250, 41, 20));
        groupBox = new QGroupBox(MarangoniTrackingDialog);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(370, 200, 221, 141));
        groupBox->setAlignment(Qt::AlignCenter);
        groupBox->setFlat(false);
        label_8 = new QLabel(groupBox);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(10, 20, 71, 16));
        cbox_LasOrient = new QComboBox(groupBox);
        cbox_LasOrient->setObjectName(QStringLiteral("cbox_LasOrient"));
        cbox_LasOrient->setGeometry(QRect(140, 20, 69, 22));
        label_26 = new QLabel(groupBox);
        label_26->setObjectName(QStringLiteral("label_26"));
        label_26->setGeometry(QRect(10, 70, 131, 31));
        label_26->setWordWrap(true);
        label_9 = new QLabel(groupBox);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(10, 40, 131, 31));
        label_9->setWordWrap(true);
        line_PatternA = new QLineEdit(groupBox);
        line_PatternA->setObjectName(QStringLiteral("line_PatternA"));
        line_PatternA->setGeometry(QRect(140, 110, 71, 20));
        line_LasPartD = new QLineEdit(groupBox);
        line_LasPartD->setObjectName(QStringLiteral("line_LasPartD"));
        line_LasPartD->setGeometry(QRect(140, 50, 71, 20));
        label_27 = new QLabel(groupBox);
        label_27->setObjectName(QStringLiteral("label_27"));
        label_27->setGeometry(QRect(10, 100, 131, 31));
        label_27->setWordWrap(true);
        line_PatternR = new QLineEdit(groupBox);
        line_PatternR->setObjectName(QStringLiteral("line_PatternR"));
        line_PatternR->setGeometry(QRect(140, 80, 71, 20));
        groupBox_2 = new QGroupBox(MarangoniTrackingDialog);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(260, 10, 331, 181));
        groupBox_2->setAlignment(Qt::AlignCenter);
        groupBox_2->setCheckable(false);
        gridLayout = new QGridLayout(groupBox_2);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label_7 = new QLabel(groupBox_2);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout->addWidget(label_7, 5, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        cbox_SampleSense = new QComboBox(groupBox_2);
        cbox_SampleSense->setObjectName(QStringLiteral("cbox_SampleSense"));

        horizontalLayout_2->addWidget(cbox_SampleSense);

        label_12 = new QLabel(groupBox_2);
        label_12->setObjectName(QStringLiteral("label_12"));

        horizontalLayout_2->addWidget(label_12);


        horizontalLayout->addLayout(horizontalLayout_2);


        gridLayout->addLayout(horizontalLayout, 5, 1, 1, 1);

        label_22 = new QLabel(groupBox_2);
        label_22->setObjectName(QStringLiteral("label_22"));

        gridLayout->addWidget(label_22, 0, 0, 1, 1);

        Xlaser_slider = new QSlider(groupBox_2);
        Xlaser_slider->setObjectName(QStringLiteral("Xlaser_slider"));
        Xlaser_slider->setMaximum(1024);
        Xlaser_slider->setValue(512);
        Xlaser_slider->setOrientation(Qt::Horizontal);
        Xlaser_slider->setTickPosition(QSlider::NoTicks);

        gridLayout->addWidget(Xlaser_slider, 0, 1, 1, 1);

        Xlaser_label = new QLabel(groupBox_2);
        Xlaser_label->setObjectName(QStringLiteral("Xlaser_label"));

        gridLayout->addWidget(Xlaser_label, 0, 2, 1, 1);

        label_23 = new QLabel(groupBox_2);
        label_23->setObjectName(QStringLiteral("label_23"));

        gridLayout->addWidget(label_23, 1, 0, 1, 1);

        Ylaser_slider = new QSlider(groupBox_2);
        Ylaser_slider->setObjectName(QStringLiteral("Ylaser_slider"));
        Ylaser_slider->setMaximum(1024);
        Ylaser_slider->setValue(512);
        Ylaser_slider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(Ylaser_slider, 1, 1, 1, 1);

        Ylaser_label = new QLabel(groupBox_2);
        Ylaser_label->setObjectName(QStringLiteral("Ylaser_label"));

        gridLayout->addWidget(Ylaser_label, 1, 2, 1, 1);

        label_10 = new QLabel(groupBox_2);
        label_10->setObjectName(QStringLiteral("label_10"));

        gridLayout->addWidget(label_10, 2, 0, 1, 1);

        Alaser_slider = new QSlider(groupBox_2);
        Alaser_slider->setObjectName(QStringLiteral("Alaser_slider"));
        Alaser_slider->setMinimum(-180);
        Alaser_slider->setMaximum(180);
        Alaser_slider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(Alaser_slider, 2, 1, 1, 1);

        Alaser_label = new QLabel(groupBox_2);
        Alaser_label->setObjectName(QStringLiteral("Alaser_label"));

        gridLayout->addWidget(Alaser_label, 2, 2, 1, 1);

        label_24 = new QLabel(groupBox_2);
        label_24->setObjectName(QStringLiteral("label_24"));

        gridLayout->addWidget(label_24, 3, 0, 1, 1);

        Rpattern_label = new QLabel(groupBox_2);
        Rpattern_label->setObjectName(QStringLiteral("Rpattern_label"));

        gridLayout->addWidget(Rpattern_label, 3, 2, 1, 1);

        label_25 = new QLabel(groupBox_2);
        label_25->setObjectName(QStringLiteral("label_25"));

        gridLayout->addWidget(label_25, 4, 0, 1, 1);

        Aux_slider = new QSlider(groupBox_2);
        Aux_slider->setObjectName(QStringLiteral("Aux_slider"));
        Aux_slider->setMinimum(0);
        Aux_slider->setMaximum(100);
        Aux_slider->setPageStep(5);
        Aux_slider->setValue(100);
        Aux_slider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(Aux_slider, 4, 1, 1, 1);

        Aux_label = new QLabel(groupBox_2);
        Aux_label->setObjectName(QStringLiteral("Aux_label"));

        gridLayout->addWidget(Aux_label, 4, 2, 1, 1);

        Rpattern_slider = new QSlider(groupBox_2);
        Rpattern_slider->setObjectName(QStringLiteral("Rpattern_slider"));
        Rpattern_slider->setMinimum(0);
        Rpattern_slider->setMaximum(300);
        Rpattern_slider->setPageStep(5);
        Rpattern_slider->setValue(0);
        Rpattern_slider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(Rpattern_slider, 3, 1, 1, 1);

        line_SampleRate = new QLineEdit(groupBox_2);
        line_SampleRate->setObjectName(QStringLiteral("line_SampleRate"));
        line_SampleRate->setClearButtonEnabled(false);

        gridLayout->addWidget(line_SampleRate, 5, 2, 1, 1);

        layoutWidget = new QWidget(MarangoniTrackingDialog);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 40, 241, 131));
        gridLayout_2 = new QGridLayout(layoutWidget);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        label_5 = new QLabel(layoutWidget);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout_2->addWidget(label_5, 1, 0, 1, 1);

        cbox_PatternType = new QComboBox(layoutWidget);
        cbox_PatternType->setObjectName(QStringLiteral("cbox_PatternType"));

        gridLayout_2->addWidget(cbox_PatternType, 1, 1, 1, 1);

        label_6 = new QLabel(layoutWidget);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout_2->addWidget(label_6, 2, 0, 1, 1);

        cbox_OpMode = new QComboBox(layoutWidget);
        cbox_OpMode->setObjectName(QStringLiteral("cbox_OpMode"));

        gridLayout_2->addWidget(cbox_OpMode, 2, 1, 1, 1);

        thresholdSlider = new QSlider(layoutWidget);
        thresholdSlider->setObjectName(QStringLiteral("thresholdSlider"));
        thresholdSlider->setMaximum(255);
        thresholdSlider->setPageStep(10);
        thresholdSlider->setValue(70);
        thresholdSlider->setTracking(true);
        thresholdSlider->setOrientation(Qt::Horizontal);
        thresholdSlider->setTickPosition(QSlider::NoTicks);

        gridLayout_2->addWidget(thresholdSlider, 0, 1, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_21 = new QLabel(layoutWidget);
        label_21->setObjectName(QStringLiteral("label_21"));

        horizontalLayout_3->addWidget(label_21);

        spinBox_NroPart = new QSpinBox(layoutWidget);
        spinBox_NroPart->setObjectName(QStringLiteral("spinBox_NroPart"));
        spinBox_NroPart->setWrapping(false);
        spinBox_NroPart->setFrame(true);
        spinBox_NroPart->setReadOnly(false);
        spinBox_NroPart->setMinimum(0);
        spinBox_NroPart->setMaximum(10);
        spinBox_NroPart->setValue(1);

        horizontalLayout_3->addWidget(spinBox_NroPart);


        gridLayout_2->addLayout(horizontalLayout_3, 4, 0, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label = new QLabel(layoutWidget);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_4->addWidget(label);

        threshEdit = new QLineEdit(layoutWidget);
        threshEdit->setObjectName(QStringLiteral("threshEdit"));
        threshEdit->setEchoMode(QLineEdit::Normal);
        threshEdit->setCursorPosition(2);
        threshEdit->setClearButtonEnabled(false);

        horizontalLayout_4->addWidget(threshEdit);


        gridLayout_2->addLayout(horizontalLayout_4, 0, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer, 3, 0, 1, 1);

        layoutWidget->raise();
        label_2->raise();
        label_4->raise();
        activateBox->raise();
        horPosition->raise();
        label_3->raise();
        vertPosition->raise();
        label_11->raise();
        label_13->raise();
        label_14->raise();
        label_15->raise();
        label_16->raise();
        label_17->raise();
        line_kp->raise();
        line_Ti->raise();
        line_Td->raise();
        line_Usat->raise();
        line_Tt->raise();
        label_18->raise();
        line_Tsamp->raise();
        label_19->raise();
        line_Kff->raise();
        label_20->raise();
        chkbox_Tracking->raise();
        line_Xtarg->raise();
        line_Ytarg->raise();
        groupBox->raise();
        groupBox_2->raise();

        retranslateUi(MarangoniTrackingDialog);

        QMetaObject::connectSlotsByName(MarangoniTrackingDialog);
    } // setupUi

    void retranslateUi(QDialog *MarangoniTrackingDialog)
    {
        MarangoniTrackingDialog->setWindowTitle(QApplication::translate("MarangoniTrackingDialog", "Tracking", 0));
        label_2->setText(QApplication::translate("MarangoniTrackingDialog", "Control options", 0));
        label_4->setText(QApplication::translate("MarangoniTrackingDialog", "Target Y-Position", 0));
        activateBox->setText(QApplication::translate("MarangoniTrackingDialog", "Turn ON mirror and Activate Processing", 0));
        label_3->setText(QApplication::translate("MarangoniTrackingDialog", "Target X-Position", 0));
        label_11->setText(QApplication::translate("MarangoniTrackingDialog", "U(s)=Kp(E(s)+1/(Ti s) E(s)+Td s^2 E(s))+ 1/(Tt s) (Usat(s)-U(s))", 0));
        label_13->setText(QApplication::translate("MarangoniTrackingDialog", "Kp", 0));
        label_14->setText(QApplication::translate("MarangoniTrackingDialog", "Ti", 0));
        label_15->setText(QApplication::translate("MarangoniTrackingDialog", "Td", 0));
        label_16->setText(QApplication::translate("MarangoniTrackingDialog", "u sat [mm/s]", 0));
        label_17->setText(QApplication::translate("MarangoniTrackingDialog", "Windup Tt", 0));
        line_Usat->setText(QApplication::translate("MarangoniTrackingDialog", "5.4", 0));
        label_18->setText(QApplication::translate("MarangoniTrackingDialog", "Tsamp=1/FPS", 0));
        line_Tsamp->setText(QApplication::translate("MarangoniTrackingDialog", "0.0666", 0));
        label_19->setText(QApplication::translate("MarangoniTrackingDialog", "K ff", 0));
        label_20->setText(QApplication::translate("MarangoniTrackingDialog", "Tt=f( Tsamp,Ti)", 0));
        chkbox_Tracking->setText(QApplication::translate("MarangoniTrackingDialog", "Activate linear Tracking", 0));
        groupBox->setTitle(QApplication::translate("MarangoniTrackingDialog", "Identification", 0));
        label_8->setText(QApplication::translate("MarangoniTrackingDialog", "Laser position", 0));
        cbox_LasOrient->clear();
        cbox_LasOrient->insertItems(0, QStringList()
         << QApplication::translate("MarangoniTrackingDialog", "Bottom", 0)
         << QApplication::translate("MarangoniTrackingDialog", "Right", 0)
         << QApplication::translate("MarangoniTrackingDialog", "Top", 0)
         << QApplication::translate("MarangoniTrackingDialog", "Left", 0)
        );
        label_26->setText(QApplication::translate("MarangoniTrackingDialog", "Pattern radius [mm]", 0));
        label_9->setText(QApplication::translate("MarangoniTrackingDialog", "Laser-Particle Dist. [mm]", 0));
        label_27->setText(QApplication::translate("MarangoniTrackingDialog", "Aux param", 0));
        groupBox_2->setTitle(QApplication::translate("MarangoniTrackingDialog", "MANUAL", 0));
        label_7->setText(QApplication::translate("MarangoniTrackingDialog", "s rate", 0));
        cbox_SampleSense->clear();
        cbox_SampleSense->insertItems(0, QStringList()
         << QApplication::translate("MarangoniTrackingDialog", "Anti clockwise", 0)
         << QApplication::translate("MarangoniTrackingDialog", "Clockwise", 0)
        );
        label_12->setText(QApplication::translate("MarangoniTrackingDialog", "Min: 200 Max:100k", 0));
        label_22->setText(QApplication::translate("MarangoniTrackingDialog", "x laser", 0));
        Xlaser_label->setText(QApplication::translate("MarangoniTrackingDialog", "512px", 0));
        label_23->setText(QApplication::translate("MarangoniTrackingDialog", "y laser", 0));
        Ylaser_label->setText(QApplication::translate("MarangoniTrackingDialog", "512px", 0));
        label_10->setText(QApplication::translate("MarangoniTrackingDialog", "a laser", 0));
        Alaser_label->setText(QApplication::translate("MarangoniTrackingDialog", "0\302\260", 0));
        label_24->setText(QApplication::translate("MarangoniTrackingDialog", "r pattern", 0));
        Rpattern_label->setText(QApplication::translate("MarangoniTrackingDialog", "0mm", 0));
        label_25->setText(QApplication::translate("MarangoniTrackingDialog", "aux param", 0));
        Aux_label->setText(QApplication::translate("MarangoniTrackingDialog", "100%", 0));
        line_SampleRate->setText(QApplication::translate("MarangoniTrackingDialog", "10000", 0));
        label_5->setText(QApplication::translate("MarangoniTrackingDialog", "Pattern type", 0));
        cbox_PatternType->clear();
        cbox_PatternType->insertItems(0, QStringList()
         << QApplication::translate("MarangoniTrackingDialog", "Point-wise spot", 0)
         << QApplication::translate("MarangoniTrackingDialog", "Arc (+circle)", 0)
         << QApplication::translate("MarangoniTrackingDialog", "Ellipse", 0)
         << QApplication::translate("MarangoniTrackingDialog", "Rectangle", 0)
         << QApplication::translate("MarangoniTrackingDialog", "Line", 0)
        );
        label_6->setText(QApplication::translate("MarangoniTrackingDialog", "Mode", 0));
        cbox_OpMode->clear();
        cbox_OpMode->insertItems(0, QStringList()
         << QApplication::translate("MarangoniTrackingDialog", "Identification", 0)
         << QApplication::translate("MarangoniTrackingDialog", "Control FeedBack", 0)
         << QApplication::translate("MarangoniTrackingDialog", "Control FeedForward", 0)
         << QApplication::translate("MarangoniTrackingDialog", "Manual Mode", 0)
         << QApplication::translate("MarangoniTrackingDialog", "Mirror Calibration", 0)
         << QApplication::translate("MarangoniTrackingDialog", "2 Particles Control", 0)
         << QApplication::translate("MarangoniTrackingDialog", "Multiplex test", 0)
        );
        label_21->setText(QApplication::translate("MarangoniTrackingDialog", "Nro particles", 0));
        label->setText(QApplication::translate("MarangoniTrackingDialog", "Threshold", 0));
        threshEdit->setText(QApplication::translate("MarangoniTrackingDialog", "70", 0));
    } // retranslateUi

};

namespace Ui {
    class MarangoniTrackingDialog: public Ui_MarangoniTrackingDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MARANGONITRACKINGDIALOG_H
