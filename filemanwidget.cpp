#include "filemanwidget.h"
#include "iconhelper.h"
#include "settings.h"

#include <QBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QListView>
#include <QDir>
#include <QFileDialog>

#ifdef DEBUGREPORTING
#include <QDebug>
#endif

FileManWidget::FileManWidget(QWidget *parent) : QWidget(parent)
{
setupUi();
}

void FileManWidget::setCurrentDir(const QString &dir)
{
#ifdef DEBUGREPORTING
    qDebug() << "setCurrentDir:"<< dir ;
#endif
    QModelIndex index = m_model->index(dir);
    if (index.row() <1) {
        QString root = dir.left( dir.indexOf("/") + 1 );

        m_model->setRootPath(root );
        index = m_model->index(dir);
    }
    m_fileListView->setRootIndex(index);
}

void FileManWidget::setWallpaperDirPath()
{
    Settings::setWallpaperDirPath(m_dirLineEdit->text());
}

void FileManWidget::open(const QModelIndex &index)
{
#ifdef DEBUGREPORTING
    qDebug() << m_model->filePath(index) ;
#endif

    if(m_model->isDir(index)){
        m_dirLineEdit->setText(m_model->fileInfo(index).canonicalFilePath());
    }
    else {
        emit openFile(m_model->filePath(index));
    }
}

void FileManWidget::selectDir()
{
    QString fileSavePath = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 m_dirLineEdit->text().trimmed(),
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    if(fileSavePath.isEmpty()) {
        return;
    }
    fileSavePath.replace(QString("\\"), QString("/"));
    m_dirLineEdit->setText(fileSavePath);
}

void FileManWidget::selectDir(const QString &dir)
{
    m_dirLineEdit->setText(dir);
}

void FileManWidget::setupUi()
{
    this->setObjectName(QString::fromUtf8("fileManWidget"));

    QGridLayout *pfileManLayout = new QGridLayout(this);
    pfileManLayout->setContentsMargins(0,0,0,0);
    pfileManLayout->setSpacing(6);

    m_dirLineEdit = new QLineEdit(this);
    m_dirLineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    //m_dirLineEdit->setText( );

    QPushButton *pselectButton = new QPushButton(this);
    pselectButton->setFixedSize(32, 27);
    IconHelper::Instance()->SetIcon(pselectButton, QChar(0xf07c), 14);

    pfileManLayout->addWidget(m_dirLineEdit,0,0,1,1);
    pfileManLayout->addWidget(pselectButton,0,1,1,1);

    m_fileListView = new QListView(this);
    m_fileListView->setObjectName(QString::fromUtf8("fileListView"));
    m_model = new FSModel(this);
    m_model->setFilter(QDir::Files| QDir::Dirs );
    m_fileListView->setModel(m_model);

    pfileManLayout->addWidget(m_fileListView, 1, 0, 1, 2);

    connect(m_dirLineEdit, SIGNAL(textChanged(const QString &)),
                 this, SLOT(setCurrentDir(const QString &)));
    connect(m_dirLineEdit, SIGNAL(returnPressed()),
                 this, SLOT(setWallpaperDirPath()));
    connect(pselectButton, SIGNAL(clicked(bool)), this, SLOT(selectDir()));
    connect(m_fileListView, SIGNAL(doubleClicked(const QModelIndex&)),this, SLOT(open(const QModelIndex&)));

    //this->resize(300,600);
}


FSModel::FSModel(QObject *parent) : QFileSystemModel(parent)
{

}

QVariant FSModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    return QFileSystemModel::data( index, role);
}

void FSModel::sort(int column, Qt::SortOrder order)
{
    QFileSystemModel::sort( column, order);
}
