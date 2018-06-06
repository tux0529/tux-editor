#ifndef FILEMANWIDGET_H
#define FILEMANWIDGET_H

#include <QWidget>
#include <QFileSystemModel>

class QLineEdit;
class QListView;
class QModelIndex;


class FSModel : public QFileSystemModel
{
    Q_OBJECT
public:
    FSModel(QObject *parent = 0);

    //int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void sort ( int column, Qt::SortOrder order = Qt::AscendingOrder ) ;
};



class FileManWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FileManWidget(QWidget *parent = 0);

signals:
    void openFile(const QString &fileName);

public slots:
    void setCurrentDir(const QString &dir);
    void setWallpaperDirPath();
    void open(const QModelIndex&);
    void selectDir();
    void selectDir(const QString &dir);
private:
    QLineEdit *m_dirLineEdit;
    QListView *m_fileListView;
    FSModel *m_model;

    void setupUi();

};
#endif // FILEMANWIDGET_H
