
#include "tuxwidget.h"
#include "iconhelper.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QEvent>
#include <QMouseEvent>
#include <QPainter>
#ifdef DEBUGREPORTING
#include <QDebug>
#endif

int TuxWidget::BORDER_WIDTH = 5;

TuxWidget::TuxWidget(QWidget *parent)
    : QWidget(parent),
      m_isMaximized(false),
      m_isMousePressed(false),
      m_bCloseEventAccepted(true),
      m_iconLabel(new QLabel(this)),
      m_titleLabel(new QLabel(this)),
      m_menuButton(new QPushButton(this)),
      m_minimizeButton(new QPushButton(this)),
      m_maximizeButton(new QPushButton(this)),
      m_closeButton(new QPushButton(this)),
      //m_titleWidget(new QWidget(this)),
      m_mainWidget(new QWidget(this))
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);

    //setAttribute(Qt::WA_OpaquePaintEvent);
    //setAttribute(Qt::WA_TranslucentBackground);
    // 关闭窗口时释放资源;
    //setAttribute(Qt::WA_DeleteOnClose);
    // 初始化界面;
    setupUi();
    installEventFilter(this);

    calcStrechRect();
}
/*
int TuxWidget::borderWidth() const
{
    //return BORDER_WIDTH;
}

void TuxWidget::setBorderWidth(int bw)
{
    //BORDER_WIDTH = bw;
}
*/
void TuxWidget::setMainLayout(QLayout *layout)
{
    m_mainWidget->setLayout(layout);
}

bool TuxWidget::isCloseEventAccepted()
{
    return this->m_bCloseEventAccepted;
}

void TuxWidget::setCloseEventAccepted(bool accepted)
{
    this->m_bCloseEventAccepted = accepted;
}

void TuxWidget::setupMenu(QMenu *menu)
{
    m_menuButton->show();
    m_menuButton->setMenu(menu);
}

bool TuxWidget::eventFilter(QObject *obj, QEvent *event)
{
    switch (event->type()) {
    case QEvent::MouseButtonDblClick:
    {
        QLabel *label = static_cast<QLabel*>(obj);
        if(label->objectName() == "titleLabel"){
            emit maximize();
            return true;
        }
    }
    case QEvent::MouseButtonPress:
    {
        QMouseEvent* e = static_cast<QMouseEvent*>(event);
        if (m_stretchArea != TuxCommon::NO_SELECT && e->button() == Qt::LeftButton){
            m_isMousePressed = true;
            m_dragPos = e->globalPos() - geometry().topLeft();
        }
        break;
    }
    case QEvent::MouseMove:
    {
        //qDebug()<< "Move:" << m_isMousePressed;
        QMouseEvent* e = static_cast<QMouseEvent*>(event);
        if (m_isMaximized){
            break;
        }
        if (m_isMousePressed){
            //qDebug()<< "Move:" << e->globalPos().x();
            updateWidget(e->globalPos());
        }
        break;
    }
    case QEvent::HoverMove:
    {
        if (m_isMaximized){
            break;
        }
        if (!m_isMousePressed){
            QHoverEvent* e = static_cast<QHoverEvent*>(event);
            //qDebug()<< "HoverMove:" << e->pos().x();
            m_stretchArea = getCurrentStrechArea(e->pos());
            updateCursor(m_stretchArea);
        }
        break;
    }
    case QEvent::MouseButtonRelease:
        m_isMousePressed = false;
        calcStrechRect();
        break;
    case QEvent::Leave:

        break;
    case QEvent::WindowTitleChange:
    {
        m_titleLabel->setText(windowTitle());
        break;
    }
    case QEvent::WindowIconChange:
        m_iconLabel->setPixmap(windowIcon().pixmap(m_iconLabel->size()));
        break;
    default:
        break;
    }
    return QObject::eventFilter(obj, event);
}
/*
void TuxWidget::addFixedWidget(QWidget *wg, TuxCommon::WidgetAlignment wa)
{
    m_widgetAlignHash.insert(wg, wa);
}
*/
void TuxWidget::onMaximized()
{
    if(m_isMaximized){
        setGeometry(m_windowGeometry);
        IconHelper::Instance()->SetIcon(m_maximizeButton, QChar(0xf2d0), 10);
        m_isMaximized=false;
    }
    else {
        m_windowGeometry = geometry();
        setGeometry(QRect( QPoint(0,0), QApplication::desktop()->availableGeometry().size()));
        IconHelper::Instance()->SetIcon(m_maximizeButton, QChar(0xf2d2), 10);
        m_isMaximized=true;
    }
}

void TuxWidget::paintEvent(QPaintEvent */*event*/)
{
    //event
    QPainter painter(this);

    painter.setPen(Qt::gray);
    painter.drawRect( 0, 0 , this->width()-1, this->height()-1);
}

void TuxWidget::setupUi()
{
    if(objectName().isEmpty())
        setObjectName(QString::fromUtf8("TuxWidget"));

    QVBoxLayout *pvLayout = new QVBoxLayout(this);
    pvLayout->setSpacing(0);
    pvLayout->setContentsMargins(5,1,5,5);
    pvLayout->setObjectName(QString::fromUtf8("pvLayout"));
    pvLayout->setSizeConstraint(QLayout::SetMaximumSize);

    QHBoxLayout *phLayout = new QHBoxLayout();
    phLayout->setSpacing(0);
    phLayout->setContentsMargins(0, 0, 0, 0);

    //m_titleWidget->setFixedHeight(30);

    m_iconLabel->setObjectName("iconLabel");
    m_iconLabel->setFixedSize(21, 21);
    m_iconLabel->setScaledContents(true);

    m_titleLabel->setObjectName("titleLabel");
    m_titleLabel->setMinimumSize(QSize(30, 30));
    m_titleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_titleLabel->installEventFilter(this);

    m_menuButton->setObjectName("menuButton");
    m_menuButton->setFixedSize(32, 27);
    m_menuButton->setFocusPolicy(Qt::NoFocus);
    m_menuButton->setFlat(true);
    m_menuButton->setToolTip(tr("Menu"));
    m_menuButton->setCursor(QCursor(Qt::ArrowCursor));
    m_menuButton->hide();

    m_minimizeButton->setObjectName("minimizeButton");
    m_minimizeButton->setFixedSize(32, 27);
    m_minimizeButton->setToolTip(tr("Minimize"));

    m_maximizeButton->setObjectName("maximizeButton");
    m_maximizeButton->setFixedSize(32, 27);
    m_maximizeButton->setToolTip(tr("Maximize"));

    m_closeButton->setObjectName("closeButton");
    m_closeButton->setFixedSize(32, 27);
    m_closeButton->setToolTip(tr("Close"));

    IconHelper::Instance()->SetIcon(m_closeButton, QChar(0xf00d), 12);
    IconHelper::Instance()->SetIcon(m_maximizeButton, QChar(0xf2d0), 10);
    IconHelper::Instance()->SetIcon(m_minimizeButton, QChar(0xf068), 10);
    IconHelper::Instance()->SetIcon(m_menuButton, QChar(0xf0ca), 10);

    connect(this, SIGNAL(maximize()), m_maximizeButton, SIGNAL(clicked()));
    connect(m_minimizeButton, SIGNAL(clicked()), this, SLOT(showMinimized()));
    connect(m_maximizeButton, SIGNAL(clicked()), this, SLOT(onMaximized()));
    connect(m_closeButton, SIGNAL(clicked()), this, SLOT(close()));

    phLayout->addWidget(m_iconLabel);
    phLayout->addWidget(m_titleLabel);
    phLayout->addWidget(m_menuButton);
    phLayout->addWidget(m_minimizeButton);
    phLayout->addWidget(m_maximizeButton);
    phLayout->addWidget(m_closeButton);

    pvLayout->addLayout(phLayout);

    m_mainWidget->setObjectName(QString::fromUtf8("mainWidget"));
    m_mainWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    pvLayout->addWidget(m_mainWidget);
    setLayout(pvLayout);

    setMinimumSize(200,200);
    resize(600,400);
}

void TuxWidget::calcStrechRect()
{
    // 四个角Rect;
    m_leftTopRect = QRect(0, 0, BORDER_WIDTH, BORDER_WIDTH);
    m_leftBottomRect = QRect(0, this->height() - BORDER_WIDTH, BORDER_WIDTH, BORDER_WIDTH);
    m_rightTopRect = QRect(this->width() - BORDER_WIDTH, 0, BORDER_WIDTH, BORDER_WIDTH);
    m_rightBottomRect = QRect(this->width() - BORDER_WIDTH, this->height() - BORDER_WIDTH, BORDER_WIDTH, BORDER_WIDTH);

    // 四条边Rect;
    m_topBorderRect = QRect(BORDER_WIDTH, 0, this->width() - BORDER_WIDTH * 2, BORDER_WIDTH);
    m_rightBorderRect = QRect(width() - BORDER_WIDTH, BORDER_WIDTH, BORDER_WIDTH,
                              height() - BORDER_WIDTH * 2);
    m_bottomBorderRect = QRect(BORDER_WIDTH, this->height() - BORDER_WIDTH, this->width() - BORDER_WIDTH * 2, BORDER_WIDTH);
    m_leftBorderRect = QRect(0, BORDER_WIDTH, BORDER_WIDTH, this->height() - BORDER_WIDTH * 2);
    //*/

    m_titleBarRect = QRect(BORDER_WIDTH, BORDER_WIDTH, this->width() - BORDER_WIDTH * 2, m_titleLabel->height()- BORDER_WIDTH);
}

TuxCommon::StretchArea TuxWidget::getCurrentStrechArea(const QPoint &gMousePos)
{
    TuxCommon::StretchArea sPos;
    if (m_leftTopRect.contains(gMousePos))
    {
        sPos = TuxCommon::LEFT_TOP_RECT;
    }
    else if (m_rightTopRect.contains(gMousePos))
    {
        sPos = TuxCommon::RIGHT_TOP_RECT;
    }
    else if (m_rightBottomRect.contains(gMousePos))
    {
        sPos = TuxCommon::RIGHT_BOTTOM_RECT;
    }
    else if (m_leftBottomRect.contains(gMousePos))
    {
        sPos = TuxCommon::LEFT_BOTTOM_RECT;
    }
    else if (m_topBorderRect.contains(gMousePos))
    {
        sPos = TuxCommon::TOP_BORDER;
    }
    else if (m_rightBorderRect.contains(gMousePos))
    {
        sPos = TuxCommon::RIGHT_BORDER;
    }
    else if (m_bottomBorderRect.contains(gMousePos))
    {
        sPos = TuxCommon::BOTTOM_BORDER;
    }
    else if (m_leftBorderRect.contains(gMousePos))
    {
        sPos = TuxCommon::LEFT_BORDER;
    }
    else if (m_titleBarRect.contains(gMousePos))
    {
        sPos = TuxCommon::TITLE_BAR;
    }
    else
    {
        sPos = TuxCommon::NO_SELECT;
    }

    return sPos;

}

void TuxWidget::updateCursor(const TuxCommon::StretchArea sp)
{
    switch (sp)
    {
    case TuxCommon::NO_SELECT:
        setCursor(Qt::ArrowCursor);
        break;
    case TuxCommon::LEFT_TOP_RECT:
    case TuxCommon::RIGHT_BOTTOM_RECT:
        setCursor(Qt::SizeFDiagCursor);
        break;
    case TuxCommon::TOP_BORDER:
    case TuxCommon::BOTTOM_BORDER:
        setCursor(Qt::SizeVerCursor);
        break;
    case TuxCommon::RIGHT_TOP_RECT:
    case TuxCommon::LEFT_BOTTOM_RECT:
        setCursor(Qt::SizeBDiagCursor);
        break;
    case TuxCommon::LEFT_BORDER:
    case TuxCommon::RIGHT_BORDER:
        setCursor(Qt::SizeHorCursor);
        break;
    default:
        setCursor(Qt::ArrowCursor);
        break;
    }
}

void TuxWidget::updateWidget(const QPoint &gMousePos)
{

    if (m_stretchArea == TuxCommon::TITLE_BAR)
    {
        move(gMousePos - m_dragPos);
        return;
    }
    // 拉伸时要注意设置窗口最小值;
    QRect windowRect = this->geometry();

    if (m_stretchArea == TuxCommon::LEFT_BORDER)
    {
        windowRect.setLeft(gMousePos.x());
    }
    else if (m_stretchArea == TuxCommon::RIGHT_BORDER)
    {
        windowRect.setRight(gMousePos.x());
    }
    else if (m_stretchArea == TuxCommon::TOP_BORDER)
    {
        windowRect.setTop(gMousePos.y());
    }
    else if (m_stretchArea == TuxCommon::BOTTOM_BORDER)
    {
        windowRect.setBottom(gMousePos.y());
    }
    else if (m_stretchArea == TuxCommon::LEFT_TOP_RECT)
    {
        windowRect.setTopLeft(gMousePos );
    }
    else if (m_stretchArea == TuxCommon::RIGHT_TOP_RECT)
    {
        windowRect.setTopRight(gMousePos);
    }
    else if (m_stretchArea == TuxCommon::RIGHT_BOTTOM_RECT)
    {
        windowRect.setBottomRight(gMousePos);
    }
    else if (m_stretchArea == TuxCommon::LEFT_BOTTOM_RECT)
    {
        windowRect.setBottomLeft(gMousePos);
    }
    if (minimumWidth() > windowRect.width() || (minimumHeight() > windowRect.height())){
        return;
    }
    this->setGeometry(windowRect);
}
