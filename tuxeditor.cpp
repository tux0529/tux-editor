#include <QVBoxLayout>
#include <QToolButton>
#include <QAction>
#include <QMessageBox>
#include <QDialog>
#include <QFileDialog>
#include <QDateTime>
#include <QTextStream>
#include <QApplication>

#include "tuxeditor.h"


TuxEditor::TuxEditor(QWidget *parent)
    : QWidget(parent),
    m_txtEdit(new QPlainTextEdit)
{
    QVBoxLayout *pVLayout = new QVBoxLayout(this);

    //QWidget *pToolWidget = new QWidget(this);

    QHBoxLayout *pToolLayout = new QHBoxLayout();

    QToolButton *pNewFileButton = new QToolButton(this);
    const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/images/res/new.png"));
    QAction *newAct = new QAction(newIcon, tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    pNewFileButton->setDefaultAction(newAct);
    connect(newAct, &QAction::triggered, this, &TuxEditor::newFile);


    QToolButton *pFileOpenButton = new QToolButton(this);
    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/res/open.png"));
    QAction *openAct = new QAction(openIcon, tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    pFileOpenButton->setDefaultAction(openAct);
    connect(openAct, &QAction::triggered, this, &TuxEditor::open);

    QToolButton *pFileSaveButton = new QToolButton(this);
    const QIcon saveIcon = QIcon::fromTheme("document-save", QIcon(":/images/res/save.png"));
    QAction *saveAct = new QAction(saveIcon, tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    pFileSaveButton->setDefaultAction(saveAct);
    connect(saveAct, &QAction::triggered, this, &TuxEditor::save);


    QToolButton *pCopyButton = new QToolButton(this);
    const QIcon copyIcon = QIcon::fromTheme("edit-copy", QIcon(":/images/copy.png"));
    QAction *copyAct = new QAction(copyIcon, tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                             "clipboard"));
    pCopyButton->setDefaultAction(copyAct);
    connect(copyAct, &QAction::triggered, m_txtEdit, &QPlainTextEdit::copy);

    pToolLayout->addWidget(pNewFileButton);
    pToolLayout->addWidget(pFileOpenButton);
    pToolLayout->addWidget(pFileSaveButton);
    pToolLayout->addWidget(pCopyButton);
    pToolLayout->addStretch();

    pVLayout->addLayout(pToolLayout);
    pVLayout->addWidget(m_txtEdit);
    pVLayout->setContentsMargins(1, 1, 1, 1);

    QFont f = m_txtEdit->font();
    f.setFamily("simsun");
    f.setPointSize(12);

    m_txtEdit->setFont(f);

}

void TuxEditor::newFile()
{
    if (maybeSave()) {
        m_txtEdit->clear();
        setCurrentFile(QString());
    }
}

void TuxEditor::open()
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this);
        if (!fileName.isEmpty())
            loadFile(fileName);
    }
}

bool TuxEditor::save()
{
    if (m_curFile.isEmpty()) {
        return saveAs();
    } else {
        return saveFile(m_curFile);
    }
}

bool TuxEditor::saveAs()
{
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    if (dialog.exec() != QDialog::Accepted)
        return false;
    return saveFile(dialog.selectedFiles().first());
}

bool TuxEditor::maybeSave()
{
    if (!m_txtEdit->document()->isModified())
        return true;
    const QMessageBox::StandardButton ret
        = QMessageBox::warning(this, tr("Application"),
                               tr("The document has been modified.\n"
                                  "Do you want to save your changes?"),
                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    switch (ret) {
    case QMessageBox::Save:
        return save();
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }
    return true;
}

void TuxEditor::loadFile(const QString &fileName)
{
    if (fileName.endsWith(".jpg",Qt::CaseInsensitive )) {

        QFile file(fileName);
        file.open(QIODevice::ReadOnly);
        QDataStream in(&file);

        qint64 fsize = file.size();
        in.device()->seek(fsize-16);

        // 检查魔术数字
        quint32 magic;
        in >> magic;
        m_byteArray.clear();

        if (magic != 0xE0D0E0D0) {
            in.device()->seek(0);
            m_byteArray = in.device()->readAll();
            if ( m_byteArray.isEmpty() ) {
                //statusBar()->showMessage(tr("File load fail"), 5000);
                m_txtEdit->setPlainText("Error!!!");
            }else {
                m_txtEdit->setPlainText("");
            }
            setCurrentFile(fileName);
            return ;
        }

        // 检查版本
        qint32 version;
        in >> version;
        if (version <= 100) {
            in.setVersion(QDataStream::Qt_4_5);
        }

        // 读取数据
        qint64 imagesize;
        in >> imagesize;

        in.device()->seek(0);
        m_byteArray = in.device()->read(imagesize);

        in.device()->seek(imagesize);

        QString str;
        in >> str;

        m_txtEdit->setPlainText(str);

    }
    else {
        QFile file(fileName);
        if (!file.open(QFile::ReadOnly | QFile::Text)) {
            QMessageBox::warning(this, tr("Application"),
                                 tr("Cannot read file %1:\n%2.")
                                 .arg(QDir::toNativeSeparators(fileName), file.errorString()));
            return;
        }

        QTextStream in(&file);
    #ifndef QT_NO_CURSOR
        QApplication::setOverrideCursor(Qt::WaitCursor);
    #endif
        m_txtEdit->setPlainText(in.readAll());
    #ifndef QT_NO_CURSOR
        QApplication::restoreOverrideCursor();
    #endif

    }

    setCurrentFile(fileName);
    //statusBar()->showMessage(tr("File loaded"), 2000);
}

bool TuxEditor::saveFile(const QString &fileName)
{
    if (fileName.endsWith(".jpg",Qt::CaseInsensitive )) {
        QString tmppath = QDir::tempPath();
        QDateTime time = QDateTime::currentDateTime();
        QString strtime = time.toString("yyyyMMddhhmmss");
        QFile::copy(m_curFile , tmppath+"/qedit_" + strtime+".jpg");
        QFile file(fileName);
        file.open(QIODevice::WriteOnly);
        QDataStream out(&file);
        out.device()->write(m_byteArray);
        qint64 fsize = file.size();

        out.setVersion(QDataStream::Qt_4_5);

        out << m_txtEdit->toPlainText();

        out << (quint32)0xE0D0E0D0;
        out << (qint32)100;
        out << fsize;

        file.close();
    }
    else{
        QFile file(fileName);
        if (!file.open(QFile::WriteOnly | QFile::Text)) {
            QMessageBox::warning(this, tr("Application"),
                                 tr("Cannot write file %1:\n%2.")
                                 .arg(QDir::toNativeSeparators(fileName),
                                      file.errorString()));
            return false;
        }

        QTextStream out(&file);
    #ifndef QT_NO_CURSOR
        QApplication::setOverrideCursor(Qt::WaitCursor);
    #endif
        out << m_txtEdit->toPlainText();
    #ifndef QT_NO_CURSOR
        QApplication::restoreOverrideCursor();
    #endif
    }

    setCurrentFile(fileName);
    //statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}

void TuxEditor::setCurrentFile(const QString &fileName)
{
    m_curFile = fileName;
    m_txtEdit->document()->setModified(false);
    setWindowModified(false);

    QString shownName = m_curFile;
    if (m_curFile.isEmpty())
        shownName = "untitled.txt";
    setWindowFilePath(shownName);
}

TuxEditor::~TuxEditor()
{

}
