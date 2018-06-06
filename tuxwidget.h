#ifndef TUXWIDGET_H
#define TUXWIDGET_H

#include "common.h"

#include <QWidget>
#include <QHash>

class QLabel;
class QPushButton;
class QMenu;

class TuxWidget : public QWidget
{
    Q_OBJECT
    /*
    Q_PROPERTY(int borderWidth READ borderWidth WRITE setBorderWidth)*/

public:
    TuxWidget(QWidget *parent = 0);
/*
    int borderWidth() const;
    void setBorderWidth(int bw);
*/
    void setMainLayout(QLayout *layout);
    bool isCloseEventAccepted();
    void setCloseEventAccepted(bool accepted);
    void setupMenu(QMenu *menu);
    bool eventFilter(QObject *obj, QEvent *event);
    /*void addFixedWidget(QWidget * wg, TuxCommon::WidgetAlignment wa); */
signals:
    void maximize();
public slots:

    void onMaximized();

protected:
    //void resizeEvent(QResizeEvent *event);
    //void mouseMoveEvent(QMouseEvent *event);
    //void mousePressEvent(QMouseEvent *event);
    //void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent */*event*/);

private:

    static int BORDER_WIDTH;

    bool m_isMaximized;
    bool m_isMousePressed;
    bool m_bCloseEventAccepted;
    QPoint m_dragPos ;
    TuxCommon::StretchArea m_stretchArea;

    QRect m_windowGeometry;
    QLabel *m_iconLabel;
    QLabel *m_titleLabel;
    QPushButton *m_menuButton;
    QPushButton *m_minimizeButton;
    QPushButton *m_maximizeButton;
    QPushButton *m_closeButton;
    //QWidget *m_titleWidget;
    QWidget *m_mainWidget;

    QRect m_leftTopRect;
    QRect m_leftBottomRect;
    QRect m_rightTopRect;
    QRect m_rightBottomRect;

    // 四条边Rect;
    QRect m_topBorderRect;
    QRect m_rightBorderRect;
    QRect m_bottomBorderRect;
    QRect m_leftBorderRect;

    QRect m_titleBarRect;

    /*QHash< QWidget *, TuxCommon::WidgetAlignment> m_widgetAlignHash;*/

    void setupUi();
    void calcStrechRect();
    TuxCommon::StretchArea getCurrentStrechArea(const QPoint &gMousePos);
    void updateCursor(const TuxCommon::StretchArea sp);
    void updateWidget(const QPoint &gMousePos);
};

#endif // TUXWIDGET_H



