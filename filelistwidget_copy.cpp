#include "filelistwidget.h"
#include "filelistmodel.h"
#include "iconhelper.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QCloseEvent>
#include <QFileSystemModel>

#ifdef DEBUGREPORTING
#include <QDebug>
#endif


FileListWidget::FileListWidget( const QString &dir, QWidget *parent) : QTabWidget(parent),
  m_bCloseEventAccepted(true)
{
    setupUi(dir);
    setWindowTitle(tr("Explorer"));
    setCloseEventAccepted(false);
}

void FileListWidget::closeEvent(QCloseEvent *event)
{
    if(this->isCloseEventAccepted()){
        event->accept();
    }else {
        event->ignore();
        emit signalCloseEvent();
    }
}

void FileListWidget::selectDir()
{
    QString fileSavePath = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 m_dirLineEdit->text().trimmed(),
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    if(fileSavePath.isEmpty()) {
        return;
    }
    fileSavePath.replace(QString("\\"), QString("/"));
    selectDir(fileSavePath);
}

void FileListWidget::selectDir(const QString &dir)
{
    m_dirLineEdit->setText(dir);
}

void FileListWidget::itemDbclicked()
{

}

void FileListWidget::openFile(const QModelIndex &index)
{
    /*
    QFileInfo fInfo(m_fileListModel->data(index, Qt::UserRole).toString());

#ifdef DEBUGREPORTING
    qDebug() <<fInfo.absoluteFilePath() ;
#endif

    if(fInfo.isDir()){
        m_dirLineEdit->setText(fInfo.canonicalFilePath());
    }
    else if (fInfo.isFile()) {
        emit openFile(fInfo.absoluteFilePath());
    }
    */
#ifdef DEBUGREPORTING
    qDebug() <<model->filePath(index);
#endif

}

void FileListWidget::setupUi(const QString &dir)
{
    this->setObjectName(QString::fromUtf8("fileListWidget"));

    this->setTabPosition(QTabWidget::South);
    this->setTabShape(QTabWidget::Triangular);

    m_explorerTab = new QWidget(this);
    m_explorerTab->setObjectName(QString::fromUtf8("explorerTab"));

    QVBoxLayout *pfileListVLayout = new QVBoxLayout();
    pfileListVLayout->setContentsMargins(0,0,0,0);
    pfileListVLayout->setSpacing(6);
    pfileListVLayout->setObjectName(QString::fromUtf8("replaceVLayout"));

    QHBoxLayout *horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));


    m_dirLineEdit = new QLineEdit(this);
    m_dirLineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_dirLineEdit->setText( dir );

    QPushButton *selectButton = new QPushButton(this);
    selectButton->setFixedSize(32, 27);
    IconHelper::Instance()->SetIcon(selectButton, QChar(0xf07c), 14);

    horizontalLayout->addWidget(m_dirLineEdit);
    horizontalLayout->addWidget(selectButton);

    m_fileListView = new QListView(this);
    m_fileListView->setObjectName(QString::fromUtf8("fileListView"));
    m_fileListModel = new FileListModel(this);
    m_fileListModel->setDirPath(dir);
    m_fileListView->setModel(m_fileListModel);

    pfileListVLayout->addLayout(horizontalLayout);
    pfileListVLayout->addWidget(m_fileListView);

    m_explorerTab->setLayout(pfileListVLayout);

    this->addTab(m_explorerTab, QString("Explorer"));

    m_chapterTab = new QWidget(this);
    m_chapterTab->setObjectName(QString::fromUtf8("chapterTab"));

    QVBoxLayout *chapterLayout = new QVBoxLayout(m_chapterTab);
    chapterLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    QListView *m_chapterListView = new QListView(m_chapterTab);
    m_chapterListView->setObjectName(QString::fromUtf8("listView"));

    model = new QFileSystemModel(this);
    model->setRootPath("D:/ezviewer");
    model->setFilter(QDir::Files| QDir::Dirs );
    m_chapterListView->setModel(model);
    m_chapterListView->setRootIndex(model->index("D:/ezviewer"));

    chapterLayout->addWidget(m_chapterListView);

    this->addTab(m_chapterTab, QString("Chapter"));

    connect(m_dirLineEdit, SIGNAL(textChanged(const QString &)),
                 m_fileListModel, SLOT(setDirPath(const QString &)));
    connect(selectButton, SIGNAL(clicked(bool)), this, SLOT(selectDir()));
    connect(m_fileListView, SIGNAL(doubleClicked(const QModelIndex&)),this, SLOT(openFile(const QModelIndex&)));
    connect(m_chapterListView, SIGNAL(doubleClicked(const QModelIndex&)),this, SLOT(openFile(const QModelIndex&)));

    //this->resize(300,600);
}

void FileListWidget::setCloseEventAccepted(bool accepted)
{
    this->m_bCloseEventAccepted = accepted;
}

bool FileListWidget::isCloseEventAccepted()
{
    return this->m_bCloseEventAccepted;
}

/*********************************************************************************
** Form generated from reading ui file 'designertx6080.ui'
**
** Created: Wed Jan 24 19:28:09 2018
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************  /

#ifndef DESIGNERTX6080_H
#define DESIGNERTX6080_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListView>
#include <QtGui/QPushButton>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QPushButton *pushButton;
    QTabWidget *tabWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout_2;
    QListView *listView_2;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout;
    QListView *listView;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QString::fromUtf8("Form"));
        Form->resize(407, 804);
        verticalLayout_3 = new QVBoxLayout(Form);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(Form);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(label);

        pushButton = new QPushButton(Form);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        horizontalLayout->addWidget(pushButton);


        verticalLayout_3->addLayout(horizontalLayout);

        tabWidget = new QTabWidget(Form);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setTabPosition(QTabWidget::South);
        tabWidget->setTabShape(QTabWidget::Triangular);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        verticalLayout_2 = new QVBoxLayout(tab);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        listView_2 = new QListView(tab);
        listView_2->setObjectName(QString::fromUtf8("listView_2"));

        verticalLayout_2->addWidget(listView_2);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        verticalLayout = new QVBoxLayout(tab_2);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        listView = new QListView(tab_2);
        listView->setObjectName(QString::fromUtf8("listView"));

        verticalLayout->addWidget(listView);

        tabWidget->addTab(tab_2, QString());

        verticalLayout_3->addWidget(tabWidget);


        retranslateUi(Form);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QApplication::translate("Form", "Form", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Form", "TextLabel", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("Form", "PushButton", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("Form", "Tab 1", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("Form", "Tab 2", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(Form);
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // DESIGNERTX6080_H

 */




