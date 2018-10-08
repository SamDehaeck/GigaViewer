/********************************************************************************
** Form generated from reading UI file 'playbackdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PLAYBACKDIALOG_H
#define UI_PLAYBACKDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PlaybackDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_2;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QHBoxLayout *horizontalLayout_3;
    QToolButton *rwdButton;
    QToolButton *stopButton;
    QToolButton *playButton;
    QToolButton *ffwdButton;
    QToolButton *snapshotButton;
    QToolButton *recButton;
    QToolButton *recTimedButton;
    QToolButton *RecSettings;
    QLabel *label;
    QWidget *widget_3;
    QVBoxLayout *verticalLayout_3;
    QLineEdit *fpsEdit;
    QSlider *horizontalSlider;
    QWidget *page_2;
    QHBoxLayout *horizontalLayout_4;
    QToolButton *backButton;
    QLabel *label_2;
    QLineEdit *RecFolder;
    QToolButton *toolButton;
    QComboBox *codecBox;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout;
    QLabel *LeftStatus;
    QLabel *RightStatus;

    void setupUi(QDialog *PlaybackDialog)
    {
        if (PlaybackDialog->objectName().isEmpty())
            PlaybackDialog->setObjectName(QStringLiteral("PlaybackDialog"));
        PlaybackDialog->resize(571, 103);
        PlaybackDialog->setWindowOpacity(0.8);
        verticalLayout_2 = new QVBoxLayout(PlaybackDialog);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        widget = new QWidget(PlaybackDialog);
        widget->setObjectName(QStringLiteral("widget"));
        horizontalLayout_2 = new QHBoxLayout(widget);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(-1, -1, -1, 0);
        stackedWidget = new QStackedWidget(widget);
        stackedWidget->setObjectName(QStringLiteral("stackedWidget"));
        page = new QWidget();
        page->setObjectName(QStringLiteral("page"));
        horizontalLayout_3 = new QHBoxLayout(page);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(5, 2, 5, 2);
        rwdButton = new QToolButton(page);
        rwdButton->setObjectName(QStringLiteral("rwdButton"));
        rwdButton->setEnabled(true);
        QIcon icon;
        icon.addFile(QStringLiteral(":/new/png/icons/gtk-media-forward-rtl.png"), QSize(), QIcon::Normal, QIcon::Off);
        rwdButton->setIcon(icon);
        rwdButton->setIconSize(QSize(32, 32));

        horizontalLayout_3->addWidget(rwdButton);

        stopButton = new QToolButton(page);
        stopButton->setObjectName(QStringLiteral("stopButton"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/new/png/icons/gtk-media-stop.png"), QSize(), QIcon::Normal, QIcon::Off);
        stopButton->setIcon(icon1);
        stopButton->setIconSize(QSize(32, 32));

        horizontalLayout_3->addWidget(stopButton);

        playButton = new QToolButton(page);
        playButton->setObjectName(QStringLiteral("playButton"));
        playButton->setEnabled(true);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(playButton->sizePolicy().hasHeightForWidth());
        playButton->setSizePolicy(sizePolicy);
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/new/png/icons/gtk-media-play-ltr.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon2.addFile(QStringLiteral(":/new/png/icons/gtk-media-pause.png"), QSize(), QIcon::Normal, QIcon::On);
        playButton->setIcon(icon2);
        playButton->setIconSize(QSize(32, 32));
        playButton->setCheckable(true);
        playButton->setChecked(true);

        horizontalLayout_3->addWidget(playButton);

        ffwdButton = new QToolButton(page);
        ffwdButton->setObjectName(QStringLiteral("ffwdButton"));
        ffwdButton->setEnabled(true);
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/new/png/icons/gtk-media-forward-ltr.png"), QSize(), QIcon::Normal, QIcon::Off);
        ffwdButton->setIcon(icon3);
        ffwdButton->setIconSize(QSize(32, 32));

        horizontalLayout_3->addWidget(ffwdButton);

        snapshotButton = new QToolButton(page);
        snapshotButton->setObjectName(QStringLiteral("snapshotButton"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/new/png/icons/snapshot.png"), QSize(), QIcon::Normal, QIcon::Off);
        snapshotButton->setIcon(icon4);
        snapshotButton->setIconSize(QSize(32, 32));

        horizontalLayout_3->addWidget(snapshotButton);

        recButton = new QToolButton(page);
        recButton->setObjectName(QStringLiteral("recButton"));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/new/png/icons/gtk-media-record.png"), QSize(), QIcon::Normal, QIcon::Off);
        recButton->setIcon(icon5);
        recButton->setIconSize(QSize(32, 32));
        recButton->setCheckable(true);

        horizontalLayout_3->addWidget(recButton);

        recTimedButton = new QToolButton(page);
        recTimedButton->setObjectName(QStringLiteral("recTimedButton"));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/new/png/icons/gtk-media-record-timed.png"), QSize(), QIcon::Normal, QIcon::Off);
        recTimedButton->setIcon(icon6);
        recTimedButton->setIconSize(QSize(32, 32));
        recTimedButton->setCheckable(true);

        horizontalLayout_3->addWidget(recTimedButton);

        RecSettings = new QToolButton(page);
        RecSettings->setObjectName(QStringLiteral("RecSettings"));
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/new/png/icons/gtk-properties.png"), QSize(), QIcon::Normal, QIcon::Off);
        RecSettings->setIcon(icon7);
        RecSettings->setIconSize(QSize(32, 32));

        horizontalLayout_3->addWidget(RecSettings);

        label = new QLabel(page);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_3->addWidget(label);

        widget_3 = new QWidget(page);
        widget_3->setObjectName(QStringLiteral("widget_3"));
        verticalLayout_3 = new QVBoxLayout(widget_3);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        fpsEdit = new QLineEdit(widget_3);
        fpsEdit->setObjectName(QStringLiteral("fpsEdit"));
        fpsEdit->setLayoutDirection(Qt::LeftToRight);
        fpsEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout_3->addWidget(fpsEdit);

        horizontalSlider = new QSlider(widget_3);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setMinimum(1);
        horizontalSlider->setMaximum(89);
        horizontalSlider->setValue(19);
        horizontalSlider->setTracking(true);
        horizontalSlider->setOrientation(Qt::Horizontal);

        verticalLayout_3->addWidget(horizontalSlider);


        horizontalLayout_3->addWidget(widget_3);

        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QStringLiteral("page_2"));
        horizontalLayout_4 = new QHBoxLayout(page_2);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(5, 2, 5, 2);
        backButton = new QToolButton(page_2);
        backButton->setObjectName(QStringLiteral("backButton"));
        QIcon icon8;
        icon8.addFile(QStringLiteral(":/new/png/icons/gtk-go-back-ltr.png"), QSize(), QIcon::Normal, QIcon::Off);
        backButton->setIcon(icon8);
        backButton->setIconSize(QSize(32, 32));

        horizontalLayout_4->addWidget(backButton);

        label_2 = new QLabel(page_2);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_4->addWidget(label_2);

        RecFolder = new QLineEdit(page_2);
        RecFolder->setObjectName(QStringLiteral("RecFolder"));

        horizontalLayout_4->addWidget(RecFolder);

        toolButton = new QToolButton(page_2);
        toolButton->setObjectName(QStringLiteral("toolButton"));
        QIcon icon9;
        icon9.addFile(QStringLiteral(":/new/png/icons/gtk-open.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton->setIcon(icon9);
        toolButton->setIconSize(QSize(32, 32));

        horizontalLayout_4->addWidget(toolButton);

        codecBox = new QComboBox(page_2);
        codecBox->setObjectName(QStringLiteral("codecBox"));
        codecBox->setMinimumSize(QSize(110, 0));

        horizontalLayout_4->addWidget(codecBox);

        stackedWidget->addWidget(page_2);

        horizontalLayout_2->addWidget(stackedWidget);


        verticalLayout->addWidget(widget);

        widget_2 = new QWidget(PlaybackDialog);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        horizontalLayout = new QHBoxLayout(widget_2);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, 0, -1, -1);
        LeftStatus = new QLabel(widget_2);
        LeftStatus->setObjectName(QStringLiteral("LeftStatus"));

        horizontalLayout->addWidget(LeftStatus);

        RightStatus = new QLabel(widget_2);
        RightStatus->setObjectName(QStringLiteral("RightStatus"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(RightStatus->sizePolicy().hasHeightForWidth());
        RightStatus->setSizePolicy(sizePolicy1);
        RightStatus->setLayoutDirection(Qt::RightToLeft);
        RightStatus->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout->addWidget(RightStatus);


        verticalLayout->addWidget(widget_2);


        verticalLayout_2->addLayout(verticalLayout);

        QWidget::setTabOrder(playButton, stopButton);
        QWidget::setTabOrder(stopButton, horizontalSlider);
        QWidget::setTabOrder(horizontalSlider, ffwdButton);
        QWidget::setTabOrder(ffwdButton, recButton);
        QWidget::setTabOrder(recButton, RecSettings);
        QWidget::setTabOrder(RecSettings, fpsEdit);
        QWidget::setTabOrder(fpsEdit, rwdButton);
        QWidget::setTabOrder(rwdButton, backButton);
        QWidget::setTabOrder(backButton, RecFolder);
        QWidget::setTabOrder(RecFolder, toolButton);
        QWidget::setTabOrder(toolButton, codecBox);
        QWidget::setTabOrder(codecBox, recTimedButton);

        retranslateUi(PlaybackDialog);

        stackedWidget->setCurrentIndex(0);
        codecBox->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(PlaybackDialog);
    } // setupUi

    void retranslateUi(QDialog *PlaybackDialog)
    {
        PlaybackDialog->setWindowTitle(QApplication::translate("PlaybackDialog", "Playback", 0));
#ifndef QT_NO_TOOLTIP
        rwdButton->setToolTip(QApplication::translate("PlaybackDialog", "Slow down playback/acquisition", 0));
#endif // QT_NO_TOOLTIP
        rwdButton->setText(QApplication::translate("PlaybackDialog", "...", 0));
#ifndef QT_NO_TOOLTIP
        stopButton->setToolTip(QApplication::translate("PlaybackDialog", "Stop playback/acquisition", 0));
#endif // QT_NO_TOOLTIP
        stopButton->setText(QApplication::translate("PlaybackDialog", "...", 0));
        playButton->setText(QApplication::translate("PlaybackDialog", "...", 0));
#ifndef QT_NO_TOOLTIP
        ffwdButton->setToolTip(QApplication::translate("PlaybackDialog", "Speed up playback/acquisition", 0));
#endif // QT_NO_TOOLTIP
        ffwdButton->setText(QApplication::translate("PlaybackDialog", "...", 0));
        snapshotButton->setText(QApplication::translate("PlaybackDialog", "...", 0));
        recButton->setText(QApplication::translate("PlaybackDialog", "...", 0));
        recTimedButton->setText(QApplication::translate("PlaybackDialog", "...", 0));
        RecSettings->setText(QApplication::translate("PlaybackDialog", "...", 0));
        label->setText(QApplication::translate("PlaybackDialog", "Delay (ms)", 0));
#ifndef QT_NO_TOOLTIP
        fpsEdit->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        fpsEdit->setText(QApplication::translate("PlaybackDialog", "100", 0));
        backButton->setText(QApplication::translate("PlaybackDialog", "...", 0));
        label_2->setText(QApplication::translate("PlaybackDialog", "Rec Folder", 0));
        RecFolder->setText(QString());
        toolButton->setText(QApplication::translate("PlaybackDialog", "...", 0));
        codecBox->clear();
        codecBox->insertItems(0, QStringList()
         << QApplication::translate("PlaybackDialog", "HDF5", 0)
         << QApplication::translate("PlaybackDialog", "FMF", 0)
         << QApplication::translate("PlaybackDialog", "PNG", 0)
         << QApplication::translate("PlaybackDialog", "BMP", 0)
         << QApplication::translate("PlaybackDialog", "MSMPEG4V2", 0)
         << QApplication::translate("PlaybackDialog", "XVID", 0)
         << QApplication::translate("PlaybackDialog", "RAW", 0)
         << QApplication::translate("PlaybackDialog", "JPG", 0)
        );
        LeftStatus->setText(QString());
        RightStatus->setText(QApplication::translate("PlaybackDialog", "Framenumber", 0));
    } // retranslateUi

};

namespace Ui {
    class PlaybackDialog: public Ui_PlaybackDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLAYBACKDIALOG_H
