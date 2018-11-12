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
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_MarangoniTrackingDialog
{
public:
    QGridLayout *gridLayout_6;
    QHBoxLayout *horizontalLayout_7;
    QCheckBox *activateBox;
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
    QCheckBox *checkBox_orientation;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_3;
    QLabel *label_8;
    QComboBox *cbox_LasOrient;
    QLabel *label_9;
    QLineEdit *line_LasPartD;
    QLabel *label_26;
    QLineEdit *line_PatternR;
    QLabel *label_27;
    QLineEdit *line_PatternA;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_28;
    QLineEdit *line_PathL;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_29;
    QLineEdit *line_PathA;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_30;
    QLineEdit *line_PathD;
    QGroupBox *groupBox_4;
    QVBoxLayout *verticalLayout_2;
    QGridLayout *gridLayout_4;
    QLabel *label_3;
    QSlider *horPosition;
    QLineEdit *line_Xtarg;
    QLabel *label_4;
    QSlider *vertPosition;
    QLineEdit *line_Ytarg;
    QCheckBox *chkbox_Tracking;
    QGridLayout *gridLayout_5;
    QLabel *label_13;
    QLineEdit *line_kp;
    QLabel *label_16;
    QLineEdit *line_Usat;
    QLabel *label_14;
    QLineEdit *line_Ti;
    QLabel *label_17;
    QLineEdit *line_Tt;
    QLabel *label_15;
    QLineEdit *line_Td;
    QLabel *label_18;
    QLineEdit *line_Tsamp;
    QLabel *label_19;
    QLineEdit *line_Kff;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout;
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
    QSlider *Rpattern_slider;
    QLabel *Rpattern_label;
    QLabel *label_25;
    QSlider *Aux_slider;
    QLabel *Aux_label;
    QLabel *label_7;
    QComboBox *cbox_SampleSense;
    QLineEdit *line_SampleRate;
    QLabel *label_12;

    void setupUi(QDialog *MarangoniTrackingDialog)
    {
        if (MarangoniTrackingDialog->objectName().isEmpty())
            MarangoniTrackingDialog->setObjectName(QStringLiteral("MarangoniTrackingDialog"));
        MarangoniTrackingDialog->resize(562, 411);
        MarangoniTrackingDialog->setWindowOpacity(0.8);
        gridLayout_6 = new QGridLayout(MarangoniTrackingDialog);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));

        gridLayout_6->addLayout(horizontalLayout_7, 0, 0, 1, 1);

        activateBox = new QCheckBox(MarangoniTrackingDialog);
        activateBox->setObjectName(QStringLiteral("activateBox"));

        gridLayout_6->addWidget(activateBox, 1, 1, 1, 1);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        label_5 = new QLabel(MarangoniTrackingDialog);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout_2->addWidget(label_5, 1, 0, 1, 1);

        cbox_PatternType = new QComboBox(MarangoniTrackingDialog);
        cbox_PatternType->setObjectName(QStringLiteral("cbox_PatternType"));
        cbox_PatternType->setMaximumSize(QSize(100, 16777215));

        gridLayout_2->addWidget(cbox_PatternType, 1, 1, 1, 1);

        label_6 = new QLabel(MarangoniTrackingDialog);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout_2->addWidget(label_6, 2, 0, 1, 1);

        cbox_OpMode = new QComboBox(MarangoniTrackingDialog);
        cbox_OpMode->setObjectName(QStringLiteral("cbox_OpMode"));
        cbox_OpMode->setMaximumSize(QSize(100, 16777215));

        gridLayout_2->addWidget(cbox_OpMode, 2, 1, 1, 1);

        thresholdSlider = new QSlider(MarangoniTrackingDialog);
        thresholdSlider->setObjectName(QStringLiteral("thresholdSlider"));
        thresholdSlider->setMaximumSize(QSize(100, 16777215));
        thresholdSlider->setMaximum(255);
        thresholdSlider->setPageStep(10);
        thresholdSlider->setValue(70);
        thresholdSlider->setTracking(true);
        thresholdSlider->setOrientation(Qt::Horizontal);
        thresholdSlider->setTickPosition(QSlider::NoTicks);

        gridLayout_2->addWidget(thresholdSlider, 0, 1, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_21 = new QLabel(MarangoniTrackingDialog);
        label_21->setObjectName(QStringLiteral("label_21"));

        horizontalLayout_3->addWidget(label_21);

        spinBox_NroPart = new QSpinBox(MarangoniTrackingDialog);
        spinBox_NroPart->setObjectName(QStringLiteral("spinBox_NroPart"));
        spinBox_NroPart->setMaximumSize(QSize(30, 16777215));
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
        label = new QLabel(MarangoniTrackingDialog);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_4->addWidget(label);

        threshEdit = new QLineEdit(MarangoniTrackingDialog);
        threshEdit->setObjectName(QStringLiteral("threshEdit"));
        threshEdit->setMaximumSize(QSize(30, 16777215));
        threshEdit->setEchoMode(QLineEdit::Normal);
        threshEdit->setCursorPosition(2);
        threshEdit->setClearButtonEnabled(false);

        horizontalLayout_4->addWidget(threshEdit);


        gridLayout_2->addLayout(horizontalLayout_4, 0, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer, 3, 0, 1, 1);

        checkBox_orientation = new QCheckBox(MarangoniTrackingDialog);
        checkBox_orientation->setObjectName(QStringLiteral("checkBox_orientation"));

        gridLayout_2->addWidget(checkBox_orientation, 4, 1, 1, 1);


        gridLayout_6->addLayout(gridLayout_2, 2, 1, 1, 1);

        groupBox = new QGroupBox(MarangoniTrackingDialog);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setAlignment(Qt::AlignCenter);
        groupBox->setFlat(false);
        gridLayout_3 = new QGridLayout(groupBox);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        label_8 = new QLabel(groupBox);
        label_8->setObjectName(QStringLiteral("label_8"));

        gridLayout_3->addWidget(label_8, 0, 0, 1, 1);

        cbox_LasOrient = new QComboBox(groupBox);
        cbox_LasOrient->setObjectName(QStringLiteral("cbox_LasOrient"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(cbox_LasOrient->sizePolicy().hasHeightForWidth());
        cbox_LasOrient->setSizePolicy(sizePolicy);
        cbox_LasOrient->setMaximumSize(QSize(70, 16777215));

        gridLayout_3->addWidget(cbox_LasOrient, 0, 1, 2, 1);

        label_9 = new QLabel(groupBox);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setWordWrap(true);

        gridLayout_3->addWidget(label_9, 1, 0, 2, 1);

        line_LasPartD = new QLineEdit(groupBox);
        line_LasPartD->setObjectName(QStringLiteral("line_LasPartD"));
        sizePolicy.setHeightForWidth(line_LasPartD->sizePolicy().hasHeightForWidth());
        line_LasPartD->setSizePolicy(sizePolicy);
        line_LasPartD->setMaximumSize(QSize(70, 16777215));

        gridLayout_3->addWidget(line_LasPartD, 2, 1, 1, 1);

        label_26 = new QLabel(groupBox);
        label_26->setObjectName(QStringLiteral("label_26"));
        label_26->setWordWrap(true);

        gridLayout_3->addWidget(label_26, 3, 0, 1, 1);

        line_PatternR = new QLineEdit(groupBox);
        line_PatternR->setObjectName(QStringLiteral("line_PatternR"));
        sizePolicy.setHeightForWidth(line_PatternR->sizePolicy().hasHeightForWidth());
        line_PatternR->setSizePolicy(sizePolicy);
        line_PatternR->setMaximumSize(QSize(70, 16777215));

        gridLayout_3->addWidget(line_PatternR, 3, 1, 1, 1);

        label_27 = new QLabel(groupBox);
        label_27->setObjectName(QStringLiteral("label_27"));
        label_27->setWordWrap(true);

        gridLayout_3->addWidget(label_27, 4, 0, 1, 1);

        line_PatternA = new QLineEdit(groupBox);
        line_PatternA->setObjectName(QStringLiteral("line_PatternA"));
        sizePolicy.setHeightForWidth(line_PatternA->sizePolicy().hasHeightForWidth());
        line_PatternA->setSizePolicy(sizePolicy);
        line_PatternA->setMaximumSize(QSize(70, 16777215));

        gridLayout_3->addWidget(line_PatternA, 4, 1, 1, 1);


        gridLayout_6->addWidget(groupBox, 2, 2, 1, 2);

        groupBox_3 = new QGroupBox(MarangoniTrackingDialog);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setAlignment(Qt::AlignCenter);
        groupBox_3->setFlat(false);
        verticalLayout = new QVBoxLayout(groupBox_3);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        label_28 = new QLabel(groupBox_3);
        label_28->setObjectName(QStringLiteral("label_28"));
        label_28->setWordWrap(true);

        horizontalLayout_5->addWidget(label_28);

        line_PathL = new QLineEdit(groupBox_3);
        line_PathL->setObjectName(QStringLiteral("line_PathL"));
        line_PathL->setMaximumSize(QSize(50, 16777215));

        horizontalLayout_5->addWidget(line_PathL);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        label_29 = new QLabel(groupBox_3);
        label_29->setObjectName(QStringLiteral("label_29"));
        label_29->setWordWrap(true);

        horizontalLayout_6->addWidget(label_29);

        line_PathA = new QLineEdit(groupBox_3);
        line_PathA->setObjectName(QStringLiteral("line_PathA"));
        line_PathA->setMaximumSize(QSize(50, 16777215));

        horizontalLayout_6->addWidget(line_PathA);


        verticalLayout->addLayout(horizontalLayout_6);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        label_30 = new QLabel(groupBox_3);
        label_30->setObjectName(QStringLiteral("label_30"));
        label_30->setWordWrap(true);

        horizontalLayout_8->addWidget(label_30);

        line_PathD = new QLineEdit(groupBox_3);
        line_PathD->setObjectName(QStringLiteral("line_PathD"));
        line_PathD->setMaximumSize(QSize(50, 16777215));

        horizontalLayout_8->addWidget(line_PathD);


        verticalLayout->addLayout(horizontalLayout_8);


        gridLayout_6->addWidget(groupBox_3, 2, 4, 1, 1);

        groupBox_4 = new QGroupBox(MarangoniTrackingDialog);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setAlignment(Qt::AlignCenter);
        verticalLayout_2 = new QVBoxLayout(groupBox_4);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        label_3 = new QLabel(groupBox_4);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout_4->addWidget(label_3, 0, 0, 1, 1);

        horPosition = new QSlider(groupBox_4);
        horPosition->setObjectName(QStringLiteral("horPosition"));
        horPosition->setMaximumSize(QSize(100, 16777215));
        horPosition->setMaximum(1024);
        horPosition->setValue(500);
        horPosition->setSliderPosition(500);
        horPosition->setOrientation(Qt::Horizontal);
        horPosition->setTickPosition(QSlider::NoTicks);
        horPosition->setTickInterval(10);

        gridLayout_4->addWidget(horPosition, 0, 1, 1, 1);

        line_Xtarg = new QLineEdit(groupBox_4);
        line_Xtarg->setObjectName(QStringLiteral("line_Xtarg"));
        line_Xtarg->setMaximumSize(QSize(30, 16777215));

        gridLayout_4->addWidget(line_Xtarg, 0, 2, 1, 1);

        label_4 = new QLabel(groupBox_4);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout_4->addWidget(label_4, 1, 0, 1, 1);

        vertPosition = new QSlider(groupBox_4);
        vertPosition->setObjectName(QStringLiteral("vertPosition"));
        vertPosition->setMaximumSize(QSize(100, 16777215));
        vertPosition->setMaximum(1024);
        vertPosition->setValue(500);
        vertPosition->setSliderPosition(500);
        vertPosition->setOrientation(Qt::Horizontal);

        gridLayout_4->addWidget(vertPosition, 1, 1, 1, 1);

        line_Ytarg = new QLineEdit(groupBox_4);
        line_Ytarg->setObjectName(QStringLiteral("line_Ytarg"));
        line_Ytarg->setMaximumSize(QSize(30, 16777215));

        gridLayout_4->addWidget(line_Ytarg, 1, 2, 1, 1);


        verticalLayout_2->addLayout(gridLayout_4);

        chkbox_Tracking = new QCheckBox(groupBox_4);
        chkbox_Tracking->setObjectName(QStringLiteral("chkbox_Tracking"));

        verticalLayout_2->addWidget(chkbox_Tracking);

        gridLayout_5 = new QGridLayout();
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        label_13 = new QLabel(groupBox_4);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_5->addWidget(label_13, 0, 0, 1, 1);

        line_kp = new QLineEdit(groupBox_4);
        line_kp->setObjectName(QStringLiteral("line_kp"));
        line_kp->setMaximumSize(QSize(50, 16777215));

        gridLayout_5->addWidget(line_kp, 0, 1, 1, 1);

        label_16 = new QLabel(groupBox_4);
        label_16->setObjectName(QStringLiteral("label_16"));

        gridLayout_5->addWidget(label_16, 0, 2, 1, 1);

        line_Usat = new QLineEdit(groupBox_4);
        line_Usat->setObjectName(QStringLiteral("line_Usat"));
        line_Usat->setMaximumSize(QSize(50, 16777215));

        gridLayout_5->addWidget(line_Usat, 0, 3, 1, 1);

        label_14 = new QLabel(groupBox_4);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_5->addWidget(label_14, 1, 0, 1, 1);

        line_Ti = new QLineEdit(groupBox_4);
        line_Ti->setObjectName(QStringLiteral("line_Ti"));
        line_Ti->setMaximumSize(QSize(50, 16777215));

        gridLayout_5->addWidget(line_Ti, 1, 1, 1, 1);

        label_17 = new QLabel(groupBox_4);
        label_17->setObjectName(QStringLiteral("label_17"));

        gridLayout_5->addWidget(label_17, 1, 2, 1, 1);

        line_Tt = new QLineEdit(groupBox_4);
        line_Tt->setObjectName(QStringLiteral("line_Tt"));
        line_Tt->setMaximumSize(QSize(50, 16777215));

        gridLayout_5->addWidget(line_Tt, 1, 3, 1, 1);

        label_15 = new QLabel(groupBox_4);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_5->addWidget(label_15, 2, 0, 1, 1);

        line_Td = new QLineEdit(groupBox_4);
        line_Td->setObjectName(QStringLiteral("line_Td"));
        line_Td->setMaximumSize(QSize(50, 16777215));

        gridLayout_5->addWidget(line_Td, 2, 1, 1, 1);

        label_18 = new QLabel(groupBox_4);
        label_18->setObjectName(QStringLiteral("label_18"));

        gridLayout_5->addWidget(label_18, 2, 2, 1, 1);

        line_Tsamp = new QLineEdit(groupBox_4);
        line_Tsamp->setObjectName(QStringLiteral("line_Tsamp"));
        line_Tsamp->setEnabled(false);
        line_Tsamp->setMaximumSize(QSize(50, 16777215));
        line_Tsamp->setReadOnly(true);

        gridLayout_5->addWidget(line_Tsamp, 2, 3, 1, 1);

        label_19 = new QLabel(groupBox_4);
        label_19->setObjectName(QStringLiteral("label_19"));
        label_19->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_5->addWidget(label_19, 3, 0, 1, 1);

        line_Kff = new QLineEdit(groupBox_4);
        line_Kff->setObjectName(QStringLiteral("line_Kff"));
        line_Kff->setMaximumSize(QSize(50, 16777215));

        gridLayout_5->addWidget(line_Kff, 3, 1, 1, 1);


        verticalLayout_2->addLayout(gridLayout_5);


        gridLayout_6->addWidget(groupBox_4, 3, 1, 1, 2);

        groupBox_2 = new QGroupBox(MarangoniTrackingDialog);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setAlignment(Qt::AlignCenter);
        groupBox_2->setCheckable(false);
        gridLayout = new QGridLayout(groupBox_2);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label_22 = new QLabel(groupBox_2);
        label_22->setObjectName(QStringLiteral("label_22"));

        gridLayout->addWidget(label_22, 0, 0, 1, 1);

        Xlaser_slider = new QSlider(groupBox_2);
        Xlaser_slider->setObjectName(QStringLiteral("Xlaser_slider"));
        Xlaser_slider->setMaximumSize(QSize(100, 16777215));
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
        Ylaser_slider->setMaximumSize(QSize(100, 16777215));
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
        Alaser_slider->setMaximumSize(QSize(100, 16777215));
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

        Rpattern_slider = new QSlider(groupBox_2);
        Rpattern_slider->setObjectName(QStringLiteral("Rpattern_slider"));
        Rpattern_slider->setMaximumSize(QSize(100, 16777215));
        Rpattern_slider->setMinimum(0);
        Rpattern_slider->setMaximum(300);
        Rpattern_slider->setPageStep(5);
        Rpattern_slider->setValue(0);
        Rpattern_slider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(Rpattern_slider, 3, 1, 1, 1);

        Rpattern_label = new QLabel(groupBox_2);
        Rpattern_label->setObjectName(QStringLiteral("Rpattern_label"));

        gridLayout->addWidget(Rpattern_label, 3, 2, 1, 1);

        label_25 = new QLabel(groupBox_2);
        label_25->setObjectName(QStringLiteral("label_25"));

        gridLayout->addWidget(label_25, 4, 0, 1, 1);

        Aux_slider = new QSlider(groupBox_2);
        Aux_slider->setObjectName(QStringLiteral("Aux_slider"));
        Aux_slider->setMaximumSize(QSize(100, 16777215));
        Aux_slider->setMinimum(0);
        Aux_slider->setMaximum(100);
        Aux_slider->setPageStep(5);
        Aux_slider->setValue(100);
        Aux_slider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(Aux_slider, 4, 1, 1, 1);

        Aux_label = new QLabel(groupBox_2);
        Aux_label->setObjectName(QStringLiteral("Aux_label"));

        gridLayout->addWidget(Aux_label, 4, 2, 1, 1);

        label_7 = new QLabel(groupBox_2);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout->addWidget(label_7, 5, 0, 1, 1);

        cbox_SampleSense = new QComboBox(groupBox_2);
        cbox_SampleSense->setObjectName(QStringLiteral("cbox_SampleSense"));
        cbox_SampleSense->setMaximumSize(QSize(100, 16777215));

        gridLayout->addWidget(cbox_SampleSense, 5, 1, 1, 1);

        line_SampleRate = new QLineEdit(groupBox_2);
        line_SampleRate->setObjectName(QStringLiteral("line_SampleRate"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(line_SampleRate->sizePolicy().hasHeightForWidth());
        line_SampleRate->setSizePolicy(sizePolicy1);
        line_SampleRate->setMaximumSize(QSize(50, 16777215));
        line_SampleRate->setClearButtonEnabled(false);

        gridLayout->addWidget(line_SampleRate, 5, 2, 1, 1);

        label_12 = new QLabel(groupBox_2);
        label_12->setObjectName(QStringLiteral("label_12"));

        gridLayout->addWidget(label_12, 6, 1, 1, 1);


        gridLayout_6->addWidget(groupBox_2, 3, 3, 1, 2);

        activateBox->raise();
        groupBox->raise();
        groupBox_2->raise();
        groupBox_3->raise();
        groupBox_4->raise();

        retranslateUi(MarangoniTrackingDialog);

        QMetaObject::connectSlotsByName(MarangoniTrackingDialog);
    } // setupUi

    void retranslateUi(QDialog *MarangoniTrackingDialog)
    {
        MarangoniTrackingDialog->setWindowTitle(QApplication::translate("MarangoniTrackingDialog", "Tracking", 0));
        activateBox->setText(QApplication::translate("MarangoniTrackingDialog", "Turn ON mirror and Activate Processing", 0));
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
         << QApplication::translate("MarangoniTrackingDialog", "Manual Mode", 0)
         << QApplication::translate("MarangoniTrackingDialog", "Mirror Calibration", 0)
         << QApplication::translate("MarangoniTrackingDialog", "Identification", 0)
         << QApplication::translate("MarangoniTrackingDialog", "Control FeedBack", 0)
         << QApplication::translate("MarangoniTrackingDialog", "Control FeedForward", 0)
         << QApplication::translate("MarangoniTrackingDialog", "Shielding", 0)
         << QApplication::translate("MarangoniTrackingDialog", "Closed Path", 0)
         << QApplication::translate("MarangoniTrackingDialog", "Path Finding", 0)
         << QApplication::translate("MarangoniTrackingDialog", "Multiplex test", 0)
        );
        label_21->setText(QApplication::translate("MarangoniTrackingDialog", "Nro particles", 0));
        label->setText(QApplication::translate("MarangoniTrackingDialog", "Threshold", 0));
        threshEdit->setText(QApplication::translate("MarangoniTrackingDialog", "70", 0));
        checkBox_orientation->setText(QApplication::translate("MarangoniTrackingDialog", "Orientation", 0));
        groupBox->setTitle(QApplication::translate("MarangoniTrackingDialog", "Identification", 0));
        label_8->setText(QApplication::translate("MarangoniTrackingDialog", "Laser position", 0));
        cbox_LasOrient->clear();
        cbox_LasOrient->insertItems(0, QStringList()
         << QApplication::translate("MarangoniTrackingDialog", "Bottom", 0)
         << QApplication::translate("MarangoniTrackingDialog", "Right", 0)
         << QApplication::translate("MarangoniTrackingDialog", "Top", 0)
         << QApplication::translate("MarangoniTrackingDialog", "Left", 0)
        );
        label_9->setText(QApplication::translate("MarangoniTrackingDialog", "Laser Dist. [mm]", 0));
        line_LasPartD->setText(QApplication::translate("MarangoniTrackingDialog", "1.0", 0));
        label_26->setText(QApplication::translate("MarangoniTrackingDialog", "Pattern radius [mm]", 0));
        line_PatternR->setText(QApplication::translate("MarangoniTrackingDialog", "5.0", 0));
        label_27->setText(QApplication::translate("MarangoniTrackingDialog", "Aux param", 0));
        line_PatternA->setText(QApplication::translate("MarangoniTrackingDialog", "0.5", 0));
        groupBox_3->setTitle(QApplication::translate("MarangoniTrackingDialog", "Path", 0));
        label_28->setText(QApplication::translate("MarangoniTrackingDialog", "L [mm]", 0));
        line_PathL->setText(QApplication::translate("MarangoniTrackingDialog", "1.0", 0));
        label_29->setText(QApplication::translate("MarangoniTrackingDialog", "A [mm]", 0));
        line_PathA->setText(QApplication::translate("MarangoniTrackingDialog", "5.0", 0));
        label_30->setText(QApplication::translate("MarangoniTrackingDialog", "D [mm]", 0));
        line_PathD->setText(QApplication::translate("MarangoniTrackingDialog", "2.0", 0));
        groupBox_4->setTitle(QApplication::translate("MarangoniTrackingDialog", "Contro options", 0));
        label_3->setText(QApplication::translate("MarangoniTrackingDialog", "Target X-Position", 0));
        line_Xtarg->setText(QApplication::translate("MarangoniTrackingDialog", "512", 0));
        label_4->setText(QApplication::translate("MarangoniTrackingDialog", "Target Y-Position", 0));
        line_Ytarg->setText(QApplication::translate("MarangoniTrackingDialog", "512", 0));
        chkbox_Tracking->setText(QApplication::translate("MarangoniTrackingDialog", "Activate linear Tracking", 0));
        label_13->setText(QApplication::translate("MarangoniTrackingDialog", "Kp", 0));
        label_16->setText(QApplication::translate("MarangoniTrackingDialog", "u sat [mm/s]", 0));
        line_Usat->setText(QApplication::translate("MarangoniTrackingDialog", "5.4", 0));
        label_14->setText(QApplication::translate("MarangoniTrackingDialog", "Ti", 0));
        label_17->setText(QApplication::translate("MarangoniTrackingDialog", "Windup Tt", 0));
        label_15->setText(QApplication::translate("MarangoniTrackingDialog", "Td", 0));
        label_18->setText(QApplication::translate("MarangoniTrackingDialog", "Tsamp=1/FPS", 0));
        line_Tsamp->setText(QApplication::translate("MarangoniTrackingDialog", "0.1", 0));
        label_19->setText(QApplication::translate("MarangoniTrackingDialog", "Kff", 0));
        groupBox_2->setTitle(QApplication::translate("MarangoniTrackingDialog", "MANUAL", 0));
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
        label_7->setText(QApplication::translate("MarangoniTrackingDialog", "s rate", 0));
        cbox_SampleSense->clear();
        cbox_SampleSense->insertItems(0, QStringList()
         << QApplication::translate("MarangoniTrackingDialog", "Anti clockwise", 0)
         << QApplication::translate("MarangoniTrackingDialog", "Clockwise", 0)
        );
        line_SampleRate->setText(QApplication::translate("MarangoniTrackingDialog", "10000", 0));
        label_12->setText(QApplication::translate("MarangoniTrackingDialog", "Min: 200 Max:100k", 0));
    } // retranslateUi

};

namespace Ui {
    class MarangoniTrackingDialog: public Ui_MarangoniTrackingDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MARANGONITRACKINGDIALOG_H
