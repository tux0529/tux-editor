#include <QHBoxLayout>

#include "controlwidget.h"

ControlWidget::ControlWidget(QWidget *parent) : QWidget(parent)
{

    m_pMinimizeButton = new QPushButton(this);
    m_pMaximizeButton = new QPushButton(this);
    m_pCloseButton = new QPushButton(this);

    m_pMinimizeButton->setFixedSize(27, 22);
    m_pMaximizeButton->setFixedSize(27, 22);
    m_pCloseButton->setFixedSize(27, 22);

    m_pMinimizeButton->setObjectName("minimizeButton");
    m_pMaximizeButton->setObjectName("maximizeButton");
    m_pCloseButton->setObjectName("closeButton");

    m_pMinimizeButton->setToolTip("Minimize");
    m_pMaximizeButton->setToolTip("Maximize");
    m_pCloseButton->setToolTip("Close");
    QHBoxLayout *pLayout = new QHBoxLayout(this);
    pLayout->addWidget(m_pMinimizeButton);
    pLayout->addWidget(m_pMaximizeButton);
    pLayout->addWidget(m_pCloseButton);
    pLayout->setSpacing(0);
    pLayout->setContentsMargins(3, 0, 3, 0);
    setLayout(pLayout);

    connect(m_pMinimizeButton, SIGNAL(clicked(bool)), this, SIGNAL(signalMinimize()));
    connect(m_pMaximizeButton, SIGNAL(clicked(bool)), this, SIGNAL(signalMaximize()));
    connect(m_pCloseButton, SIGNAL(clicked(bool)), this, SIGNAL(signalClose()));

}
