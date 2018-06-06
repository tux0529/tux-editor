#ifndef CHAPTERLISTMODEL_H
#define CHAPTERLISTMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QStringList>

class TuxTextEditor;

class ChapterListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    ChapterListModel(TuxTextEditor *editor, QObject *parent = 0);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void parseChapter();

signals:

public slots:

protected:

private:
    TuxTextEditor *m_textEdit;
    QList< QPair<int, QString> > chapterTitles;
    int fileCount;

};

#endif // CHAPTERLISTMODEL_H
