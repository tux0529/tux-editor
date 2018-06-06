#include "replacewidget.h"
#include "tuxtexteditor.h"
#include "iconhelper.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>



#ifdef DEBUGREPORTING
#include <QDebug>
#endif

ReplaceWidget::ReplaceWidget(TuxTextEditor *ptextEdit, QWidget *parent) :
    TuxWidget(parent),
    m_textEdit(ptextEdit),
    m_findCaseSensitively(false)
{
    setupUi();

}

void ReplaceWidget::setFindText(const QString &s)
{
    m_findLineEdit->setText(s);
}

void ReplaceWidget::slotReplace()
{
    QTextCursor textCur = m_textEdit->textCursor();
    QString selectedString = m_findCaseSensitively? textCur.selectedText() : textCur.selectedText().toUpper();
    QString findString = m_findCaseSensitively? m_findLineEdit->text() : m_findLineEdit->text().toUpper();
    if( findString == selectedString){
        textCur.insertText(m_replaceLineEdit->text());
        slotFindNext();
    }else if(slotFindNext()){
        textCur.insertText(m_replaceLineEdit->text());
        slotFindNext();
    }
}

void ReplaceWidget::slotReplaceAll()
{
    QTextCursor textCur = m_textEdit->textCursor();
    textCur.setPosition(0);
    m_textEdit->setTextCursor(textCur);

    while(slotFindNext()){
        QTextCursor tc = m_textEdit->textCursor();
        tc.insertText(m_replaceLineEdit->text());
    }
}

bool ReplaceWidget::slotFindNext(const QString &s)
{
    if (!s.isEmpty()){
        QTextCursor tc = m_textEdit->textCursor();
        tc.setPosition(0);
        m_textEdit->setTextCursor(tc);
    }
    bool found = false;
    if (!m_findLineEdit->text().isEmpty ()) {
        if (m_findCaseSensitively)
            found = m_textEdit->find (m_findLineEdit->text(),QTextDocument::FindCaseSensitively);
        else {
            found = m_textEdit->find (m_findLineEdit->text());
        }
    }
    return found;
}

void ReplaceWidget::setupUi()
{
    if (this->objectName().isEmpty())
        this->setObjectName(QString::fromUtf8("replaceWidget"));
    this->resize(354, 160);

    QVBoxLayout *pvLayout = new QVBoxLayout();
    pvLayout->setContentsMargins(10,10,10,10);
    pvLayout->setSpacing(6);
    pvLayout->setObjectName(QString::fromUtf8("replaceVLayout"));

    QHBoxLayout *horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    QLabel *findLabel = new QLabel(this);
    findLabel->setObjectName(QString::fromUtf8("findLabel"));
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(40);
    sizePolicy.setVerticalStretch(20);
    sizePolicy.setHeightForWidth(findLabel->sizePolicy().hasHeightForWidth());
    findLabel->setSizePolicy(sizePolicy);
    findLabel->setMinimumSize(QSize(50, 20));
    findLabel->setMaximumSize(QSize(50, 20));

    horizontalLayout->addWidget(findLabel);

    m_findLineEdit = new QLineEdit(this);
    m_findLineEdit->setObjectName(QString::fromUtf8("findLineEdit"));
    m_findLineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_findLineEdit->setMinimumSize(QSize(24, 24));

    horizontalLayout->addWidget(m_findLineEdit);

    findButton = new QPushButton(this);
    findButton->setObjectName(QString::fromUtf8("findButton"));
    findButton->setFixedSize(50,24);
    findButton->setFocusPolicy(Qt::TabFocus);

    horizontalLayout->addWidget(findButton);

    findNextButton = new QPushButton(this);
    findNextButton->setObjectName(QString::fromUtf8("findNextButton"));
    findNextButton->setFixedSize(80,24);
    findNextButton->setFocusPolicy(Qt::TabFocus);

    horizontalLayout->addWidget(findNextButton);

    pvLayout->addLayout(horizontalLayout);

    QHBoxLayout *horizontalLayout_2 = new QHBoxLayout();
    horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));

    QLabel *replaceLabel = new QLabel(this);
    replaceLabel->setObjectName(QString::fromUtf8("replaceLabel"));
    QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(replaceLabel->sizePolicy().hasHeightForWidth());
    replaceLabel->setSizePolicy(sizePolicy1);
    replaceLabel->setMinimumSize(QSize(50, 20));
    replaceLabel->setMaximumSize(QSize(50, 20));

    horizontalLayout_2->addWidget(replaceLabel);

    m_replaceLineEdit = new QLineEdit(this);
    m_replaceLineEdit->setObjectName(QString::fromUtf8("replaceLineEdit"));
    m_replaceLineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_replaceLineEdit->setMinimumSize(QSize(24, 24));

    horizontalLayout_2->addWidget(m_replaceLineEdit);

    replaceButton = new QPushButton(this);
    replaceButton->setObjectName(QString::fromUtf8("replaceButton"));
    replaceButton->setFixedSize(50,24);
    replaceButton->setFocusPolicy(Qt::TabFocus);

    horizontalLayout_2->addWidget(replaceButton);

    replaceAllButton = new QPushButton(this);
    replaceAllButton->setObjectName(QString::fromUtf8("replaceAllButton"));
    replaceAllButton->setFixedSize(80,24);
    replaceAllButton->setFocusPolicy(Qt::TabFocus);

    horizontalLayout_2->addWidget(replaceAllButton);

    pvLayout->addLayout(horizontalLayout_2);

    QHBoxLayout *horizontalLayout_3 = new QHBoxLayout();
    horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));

    QLabel *spaceLabel = new QLabel(this);
    spaceLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    spaceLabel->setFixedSize(QSize(50, 20));

    horizontalLayout_3->addWidget(spaceLabel);

    capsButton = new QPushButton(this);
    capsButton->setObjectName(QString::fromUtf8("capsButton"));
    capsButton->setFixedSize(21, 21);
    capsButton->setCheckable(true);
    capsButton->setFocusPolicy(Qt::NoFocus);
    capsButton->setToolTip(tr("Match case"));
    IconHelper::Instance()->SetIcon(capsButton, QChar(0xf20d), 10);

    horizontalLayout_3->addWidget(capsButton);

    regxButton = new QPushButton(this);
    regxButton->setObjectName(QString::fromUtf8("regxButton"));
    regxButton->setFixedSize(21, 21);
    regxButton->setMaximumSize(QSize(21, 21));
    regxButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    regxButton->setCheckable(true);
    regxButton->setFocusPolicy(Qt::NoFocus);
    regxButton->setToolTip(tr("Regular expression mode"));
    IconHelper::Instance()->SetIcon(regxButton, QChar(0xf2a3), 10);

    horizontalLayout_3->addWidget(regxButton);

    replaceButton_2 = new QPushButton(this);
    replaceButton_2->setObjectName(QString::fromUtf8("replaceButton_2"));
    replaceButton_2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    replaceButton_2->setFixedSize(21, 21);
    replaceButton_2->setMaximumSize(QSize(21, 21));
    replaceButton_2->setCheckable(true);
    replaceButton_2->setFocusPolicy(Qt::NoFocus);
    IconHelper::Instance()->SetIcon(replaceButton_2, QChar(0xf1e5), 10);

    horizontalLayout_3->addWidget(replaceButton_2);

    horizontalLayout_3->addStretch(1);

    exitButton = new QPushButton(this);
    exitButton->setObjectName(QString::fromUtf8("exitButton"));
    exitButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    exitButton->setFixedSize(50, 21);

    horizontalLayout_3->addWidget(exitButton);

    pvLayout->addLayout(horizontalLayout_3);

    setMainLayout(pvLayout);

    this->setWindowTitle(QApplication::translate("Replace", "Replace", Q_NULLPTR));
    findLabel->setText(QApplication::translate("Replace", "Find:", Q_NULLPTR));
    findButton->setText(QApplication::translate("Replace", "Find", Q_NULLPTR));
    findNextButton->setText(QApplication::translate("Replace", "Find Next", 0));
    replaceLabel->setText(QApplication::translate("Replace", "Replace:", 0));
    replaceButton->setText(QApplication::translate("Replace", "Replace", 0));
    replaceAllButton->setText(QApplication::translate("Replace", "Replace All", Q_NULLPTR));
    exitButton->setText(QApplication::translate("Replace", "Close", 0));

    connect (m_findLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(slotFindNext(const QString &)));
    connect (findButton, SIGNAL(clicked(bool)), this, SLOT(slotFindNext()));
    connect (replaceButton, SIGNAL(clicked(bool)), this, SLOT(slotReplace()));
    connect (replaceAllButton, SIGNAL(clicked(bool)), this, SLOT(slotReplaceAll()));
    connect (exitButton, SIGNAL(clicked(bool)), this, SLOT(close()));

}
