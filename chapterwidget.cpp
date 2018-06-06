#include "chapterwidget.h"
#include "chapterlistmodel.h"
#include "iconhelper.h"
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QListView>
#include <QComboBox>

#ifdef DEBUGREPORTING
#include <QDebug>
#endif


ChapterWidget::ChapterWidget(TuxTextEditor *editor, QWidget *parent) : QWidget(parent),
    m_textEdit(editor)
{
    setupUi();
}


void ChapterWidget::itemDbclicked(const QModelIndex&)
{

}

void ChapterWidget::parseChapter()
{
    QRegExp rx( "find" );
#ifdef DEBUGREPORTING
    qDebug() << rx<<m_prefixComboBox->currentText()<< m_suffixComboBox->currentText();
#endif
    m_chapterListModel->parseChapter();
}


void ChapterWidget::setupUi()
{
    this->setObjectName(QString::fromUtf8("chapterWidget"));

    QLabel *label;
    QLabel *label_2;
    QPushButton *pformatButton;

    QGridLayout *pchapterLayout = new QGridLayout();
    pchapterLayout->setContentsMargins(0,0,0,0);
    pchapterLayout->setSpacing(6);

    label = new QLabel(this);
    label->setText(QApplication::translate("chapterWidget", "\347\211\271\345\276\201\357\274\232", 0));

    pchapterLayout->addWidget(label, 0, 0, 1, 1);

    m_prefixComboBox = new QComboBox(this);
    m_prefixComboBox->setObjectName(QString::fromUtf8("prefixComboBox"));
    m_prefixComboBox->setEditable(true);
    m_prefixComboBox->clear();
    m_prefixComboBox->insertItems(0, QStringList()
                                  << QString()
                                  << QApplication::translate("chapterWidget", "\347\254\254", 0)
                                  << QApplication::translate("chapterWidget", "\343\200\220", 0)
                                  << QApplication::translate("chapterWidget", "\357\274\210", 0)
                                  << QApplication::translate("chapterWidget", "(", 0)
                                  << QApplication::translate("chapterWidget", "\343\200\212", 0)
                                 );

    pchapterLayout->addWidget(m_prefixComboBox, 0, 1, 1, 1);

    label_2 = new QLabel(this);
    label_2->setObjectName(QString::fromUtf8("label_2"));
    label_2->setText(QApplication::translate("chapterWidget", "\346\225\260\345\255\227", 0));

    pchapterLayout->addWidget(label_2, 0, 2, 1, 1);

    m_suffixComboBox = new QComboBox(this);
    m_suffixComboBox->setObjectName(QString::fromUtf8("suffixComboBox"));
    m_suffixComboBox->setEditable(true);
    m_suffixComboBox->clear();
    m_suffixComboBox->insertItems(0, QStringList()
                                  << QString()
                                  << QApplication::translate("chapterWidget", "\347\253\240", 0)
                                  << QApplication::translate("chapterWidget", "\347\257\207", 0)
                                  << QApplication::translate("chapterWidget", "\343\200\221", 0)
                                  << QApplication::translate("chapterWidget", "\357\274\211", 0)
                                  << QApplication::translate("chapterWidget", ")", 0)
                                  << QApplication::translate("chapterWidget", "\343\200\213", 0)
                                  );

    pchapterLayout->addWidget(m_suffixComboBox, 0, 3, 1, 1);

    QPushButton *m_parseChapterButton = new QPushButton(this);
    m_parseChapterButton->setObjectName(QString::fromUtf8("newFileButton"));
    m_parseChapterButton->setFixedSize(32, 27);
    m_parseChapterButton->setCursor(QCursor(Qt::ArrowCursor));
    m_parseChapterButton->setFocusPolicy(Qt::NoFocus);
    m_parseChapterButton->setToolTip(tr("Parse chapter title"));

    m_parseChapterButton->setText(QApplication::translate("chapterWidget", "\346\217\220\345\217\226", 0));

    pchapterLayout->addWidget(m_parseChapterButton, 0, 4, 1, 1);

    pformatButton = new QPushButton(this);
    pformatButton->setObjectName(QString::fromUtf8("formatButton"));
    pformatButton->setFixedSize(32, 27);
    pformatButton->setCursor(QCursor(Qt::ArrowCursor));
    pformatButton->setFocusPolicy(Qt::NoFocus);
    pformatButton->setToolTip(tr("Format chapter title"));
    pformatButton->setText(QApplication::translate("chapterWidget", "\346\240\274\345\274\217\345\214\226", 0));

    pchapterLayout->addWidget(pformatButton, 0, 5, 1, 1);

    QSpacerItem *horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    pchapterLayout->addItem(horizontalSpacer, 0, 6, 1, 1);

    m_chapterListView = new QListView(this);
    m_chapterListView->setObjectName(QString::fromUtf8("chapterListView"));

    m_chapterListModel = new ChapterListModel(m_textEdit,this);
    m_chapterListModel->parseChapter();
    //m_chapterListModel->setDirPath(dir);
    m_chapterListView->setModel(m_chapterListModel);

    pchapterLayout->addWidget(m_chapterListView, 1, 0, 1, 7);

    this->setLayout(pchapterLayout);
    connect(m_parseChapterButton, SIGNAL(clicked(bool)), this, SLOT(parseChapter()));
    connect(m_chapterListView, SIGNAL(doubleClicked(const QModelIndex&)),this, SLOT(itemDbclicked(const QModelIndex&)));

    //this->resize(300,600);
}

