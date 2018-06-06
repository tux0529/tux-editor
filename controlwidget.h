#ifndef CONTROLWIDGET_H
#define CONTROLWIDGET_H

#include <QPushButton>
#include <QWidget>

class ControlWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ControlWidget(QWidget *parent = 0);

signals:
    void signalMinimize();
    void signalMaximize();
    void signalClose();

public slots:
private:
    QPushButton *m_pMinimizeButton;
    QPushButton *m_pMaximizeButton;
    QPushButton *m_pCloseButton;
};

#endif // CONTROLWIDGET_H
