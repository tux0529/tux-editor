/********************************************************************************
** Form generated from reading ui file 'untitledmi1736.ui'
**
** Created: Thu Jan 4 11:48:35 2018
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UNTITLEDMI1736_H
#define UNTITLEDMI1736_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *m_iconLabel;
    QLabel *m_titleLabel;
    QPushButton *m_miniButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QWidget *widget;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QString::fromUtf8("Form"));
        Form->resize(673, 560);
        verticalLayout_2 = new QVBoxLayout(Form);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetMaximumSize);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetFixedSize);
        m_iconLabel = new QLabel(Form);
        m_iconLabel->setObjectName(QString::fromUtf8("m_iconLabel"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(20);
        sizePolicy.setVerticalStretch(20);
        sizePolicy.setHeightForWidth(m_iconLabel->sizePolicy().hasHeightForWidth());
        m_iconLabel->setSizePolicy(sizePolicy);
        m_iconLabel->setMinimumSize(QSize(20, 20));
        m_iconLabel->setPixmap(QPixmap(QString::fromUtf8("tuxeditor.png")));
        m_iconLabel->setScaledContents(true);

        horizontalLayout->addWidget(m_iconLabel);

        m_titleLabel = new QLabel(Form);
        m_titleLabel->setObjectName(QString::fromUtf8("m_titleLabel"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(27);
        sizePolicy1.setHeightForWidth(m_titleLabel->sizePolicy().hasHeightForWidth());
        m_titleLabel->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(m_titleLabel);

        m_miniButton = new QPushButton(Form);
        m_miniButton->setObjectName(QString::fromUtf8("m_miniButton"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(27);
        sizePolicy2.setVerticalStretch(20);
        sizePolicy2.setHeightForWidth(m_miniButton->sizePolicy().hasHeightForWidth());
        m_miniButton->setSizePolicy(sizePolicy2);
        m_miniButton->setMinimumSize(QSize(27, 20));
        m_miniButton->setMaximumSize(QSize(27, 20));
        m_miniButton->setSizeIncrement(QSize(27, 20));
        m_miniButton->setBaseSize(QSize(27, 20));

        horizontalLayout->addWidget(m_miniButton);

        pushButton_2 = new QPushButton(Form);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setMaximumSize(QSize(27, 20));

        horizontalLayout->addWidget(pushButton_2);

        pushButton_3 = new QPushButton(Form);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setMaximumSize(QSize(27, 20));

        horizontalLayout->addWidget(pushButton_3);


        verticalLayout->addLayout(horizontalLayout);

        widget = new QWidget(Form);
        widget->setObjectName(QString::fromUtf8("widget"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy3);

        verticalLayout->addWidget(widget);

        verticalLayout->setStretch(1, 1);

        verticalLayout_2->addLayout(verticalLayout);

        QWidget::setTabOrder(pushButton_3, pushButton_2);
        QWidget::setTabOrder(pushButton_2, m_miniButton);

        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        m_iconLabel->setText(QString());
        m_titleLabel->setText(QApplication::translate("Form", "  Title", 0, QApplication::UnicodeUTF8));
        m_miniButton->setText(QApplication::translate("Form", "Mini", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("Form", "Maxi", 0, QApplication::UnicodeUTF8));
        pushButton_3->setText(QApplication::translate("Form", "CLS", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(Form);
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UNTITLEDMI1736_H
