/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Mon 9. Apr 16:34:29 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QTextEdit>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout;
    QListView *rssList;
    QPushButton *addNode;
    QHBoxLayout *horizontalLayout;
    QListView *taxThemeList;
    QListView *taxGeoList;
    QGridLayout *gridLayout;
    QLabel *label;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *btnNodeNew;
    QLineEdit *nodeTitle;
    QTextEdit *nodeEditor;
    QLabel *label_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QGridLayout *gridLayout_2;
    QLabel *label_3;
    QListView *nodesList;
    QLabel *label_4;
    QListView *attachedRss;
    QPushButton *btnAttachedRemove;
    QPushButton *btnSync;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1024, 600);
        MainWindow->setMinimumSize(QSize(1024, 600));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 10, 1001, 521));
        horizontalLayout_3 = new QHBoxLayout(layoutWidget);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setSizeConstraint(QLayout::SetMaximumSize);
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        rssList = new QListView(layoutWidget);
        rssList->setObjectName(QString::fromUtf8("rssList"));

        verticalLayout->addWidget(rssList);

        addNode = new QPushButton(layoutWidget);
        addNode->setObjectName(QString::fromUtf8("addNode"));
        addNode->setEnabled(false);

        verticalLayout->addWidget(addNode);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        taxThemeList = new QListView(layoutWidget);
        taxThemeList->setObjectName(QString::fromUtf8("taxThemeList"));

        horizontalLayout->addWidget(taxThemeList);

        taxGeoList = new QListView(layoutWidget);
        taxGeoList->setObjectName(QString::fromUtf8("taxGeoList"));

        horizontalLayout->addWidget(taxGeoList);


        verticalLayout->addLayout(horizontalLayout);


        horizontalLayout_3->addLayout(verticalLayout);

        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        btnNodeNew = new QPushButton(layoutWidget);
        btnNodeNew->setObjectName(QString::fromUtf8("btnNodeNew"));
        btnNodeNew->setEnabled(false);

        horizontalLayout_2->addWidget(btnNodeNew);

        nodeTitle = new QLineEdit(layoutWidget);
        nodeTitle->setObjectName(QString::fromUtf8("nodeTitle"));
        nodeTitle->setEnabled(false);

        horizontalLayout_2->addWidget(nodeTitle);


        gridLayout->addLayout(horizontalLayout_2, 1, 0, 1, 1);

        nodeEditor = new QTextEdit(layoutWidget);
        nodeEditor->setObjectName(QString::fromUtf8("nodeEditor"));

        gridLayout->addWidget(nodeEditor, 2, 0, 1, 2);

        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 3, 0, 2, 1);

        pushButton_3 = new QPushButton(layoutWidget);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setEnabled(false);

        gridLayout->addWidget(pushButton_3, 3, 1, 1, 1);

        pushButton_4 = new QPushButton(layoutWidget);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setEnabled(false);

        gridLayout->addWidget(pushButton_4, 4, 1, 1, 1);


        horizontalLayout_3->addLayout(gridLayout);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_2->addWidget(label_3, 0, 0, 1, 1);

        nodesList = new QListView(layoutWidget);
        nodesList->setObjectName(QString::fromUtf8("nodesList"));

        gridLayout_2->addWidget(nodesList, 1, 0, 1, 2);

        label_4 = new QLabel(layoutWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_2->addWidget(label_4, 2, 0, 1, 1);

        attachedRss = new QListView(layoutWidget);
        attachedRss->setObjectName(QString::fromUtf8("attachedRss"));

        gridLayout_2->addWidget(attachedRss, 3, 0, 1, 2);

        btnAttachedRemove = new QPushButton(layoutWidget);
        btnAttachedRemove->setObjectName(QString::fromUtf8("btnAttachedRemove"));
        btnAttachedRemove->setEnabled(false);

        gridLayout_2->addWidget(btnAttachedRemove, 4, 1, 1, 1);

        btnSync = new QPushButton(layoutWidget);
        btnSync->setObjectName(QString::fromUtf8("btnSync"));
        btnSync->setMinimumSize(QSize(0, 50));

        gridLayout_2->addWidget(btnSync, 5, 0, 1, 2);


        horizontalLayout_3->addLayout(gridLayout_2);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1024, 21));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        addNode->setText(QApplication::translate("MainWindow", "Add Theme", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Theme", 0, QApplication::UnicodeUTF8));
        btnNodeNew->setText(QApplication::translate("MainWindow", "New", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "Files", 0, QApplication::UnicodeUTF8));
        pushButton_3->setText(QApplication::translate("MainWindow", "Save", 0, QApplication::UnicodeUTF8));
        pushButton_4->setText(QApplication::translate("MainWindow", "Add media", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWindow", "Themes", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MainWindow", "Attached News", 0, QApplication::UnicodeUTF8));
        btnAttachedRemove->setText(QApplication::translate("MainWindow", "Remove", 0, QApplication::UnicodeUTF8));
        btnSync->setText(QApplication::translate("MainWindow", "Sync", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
