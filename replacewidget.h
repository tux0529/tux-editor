#ifndef REAPLACEWIDGET_H
#define REAPLACEWIDGET_H

#include "tuxwidget.h"
#include <QWidget>
#include <QLineEdit>

class TuxTextEditor;

class ReplaceWidget : public TuxWidget
{
    Q_OBJECT
public:
    explicit ReplaceWidget(TuxTextEditor *ptextEdit, QWidget *parent = 0);
    void setFindText(const QString &s);

signals:

public slots:
    void slotReplace();
    void slotReplaceAll();
    bool slotFindNext(const QString &s="");  //向后查找
private:

    TuxTextEditor *m_textEdit;
    QLineEdit *m_findLineEdit;
    QPushButton *findButton;
    QPushButton *findNextButton;
    QLineEdit *m_replaceLineEdit;
    QPushButton *replaceButton;
    QPushButton *replaceAllButton;
    QPushButton *capsButton;
    QPushButton *regxButton;
    QPushButton *replaceButton_2;
    QPushButton *exitButton;

    bool m_findCaseSensitively;

    void setupUi();

};

#endif // REAPLACEWIDGET_H
