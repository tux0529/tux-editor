#ifndef CHAPTERWIDGET_H
#define CHAPTERWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QModelIndex>

class QListView;
class QComboBox;
class ChapterListModel;
class TuxTextEditor;

class ChapterWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChapterWidget(TuxTextEditor *editor,QWidget *parent = 0);

signals:

public slots:
    void itemDbclicked(const QModelIndex &);
    void parseChapter();

private:

    TuxTextEditor *m_textEdit;
    QListView *m_chapterListView;
    ChapterListModel *m_chapterListModel;
    QComboBox *m_prefixComboBox;
    QComboBox *m_suffixComboBox;

    void setupUi( );
};

#endif // CHAPTERWIDGET_H
