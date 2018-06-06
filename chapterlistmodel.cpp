#include "chapterlistmodel.h"
#include "tuxtexteditor.h"
#include <QApplication>
#include <QPalette>
#include <QTextDocument>
#include <QTextBlock>
#include <QBrush>
#include <QDir>



#ifdef DEBUGREPORTING
#include <QDebug>
#endif

ChapterListModel::ChapterListModel(TuxTextEditor *editor, QObject *parent)
    : QAbstractListModel(parent),
      m_textEdit(editor)
{
    chapterTitles = QList<QPair<int, QString> >();
}

int ChapterListModel::rowCount(const QModelIndex & /* parent */) const
{
    return chapterTitles.size();
}

QVariant ChapterListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= chapterTitles.size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole)
        return chapterTitles.at(index.row()).second;

    return QVariant();
}

void ChapterListModel::parseChapter()
{
    this->resetInternalData();
    chapterTitles = QList<QPair<int, QString> >();
    QStringList lines = m_textEdit->toPlainText().split(QRegExp("\n"));
    for(int i=0; i < lines.size(); ++i){
        if(!lines.at(i).isEmpty() && lines.at(i).length()<40){
            chapterTitles << QPair<int, QString>(i, lines.at(i));
        }
    }

}


