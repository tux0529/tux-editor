
#include "mainwindow.h"
#include "replacewidget.h"
#include "chapterwidget.h"
#include "filemanwidget.h"
#include "iconhelper.h"
#include "version.h"
#include "tuxtexteditor.h"
#include "common.h"
#include "settings.h"

#include <QSettings>
#include <QWidget>
#include <QTabWidget>
#include <QCloseEvent>
#include <QTextBlock>
#include <QVBoxLayout>
#include <QIcon>
#include <QToolButton>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QMenu>
#include <QPrintDialog>
#include <QPrinter>
#include <QLineEdit>
#include <QTextCodec>
#include <QClipboard>
#include <QProcess>
#include <QSqlQuery>

#ifdef DEBUGREPORTING
#include <QDebug>
#endif

MainWindow::MainWindow(QWidget *parent)
    :TuxWidget(parent),
      m_textEdit(new TuxTextEditor(this)),
      m_curFileId(-1),
      m_findCaseSensitively(false),
      m_isWordWraped(true)
{
    setupUi();
    initMenu();
    initFindBar();

    setWindowTitle(tr("%1 - A Simple Text Editor").arg(PRODUCT_NAME));
    setWindowIcon(QIcon(":/images/tuxeditor.png"));

    readSettings();

    setCurrentFile("");
    m_replaceWidget = new ReplaceWidget(m_textEdit);
    m_replaceWidget->hide();
    m_replaceWidget->setWindowModality(Qt::WindowModal  );

    m_textEdit->setFocus();
    TuxCommon::createConnection();
}

MainWindow::~MainWindow()
{

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        writeSettings();
        if(m_findWidget){
            m_findWidget->close();
        }
        if(m_replaceWidget){
            m_replaceWidget->close();
        }
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    if(m_findWidget){
        m_findWidget->move(width()-320, 64);
    }
    QWidget::resizeEvent(event);
    //
}
/*
void MainWindow::moveEvent(QMoveEvent *event)
{
    if(m_fileListWidget){
        m_fileListWidget->move(x()+this->width(), y());
    }
    QWidget::moveEvent(event);
}
*/
void MainWindow::newFile()
{
    if (maybeSave()) {
        m_textEdit->clear();
        setCurrentFile("");
    }
}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    open(fileName);
}

void MainWindow::open(const QString &fileName, int fileId)
{
    if (maybeSave()) {
        m_curFileId = fileId;
        if (fileName.endsWith(".jpg", Qt::CaseInsensitive)){
            loadJpgFile(fileName);
        }
        else if (!fileName.isEmpty())
            loadFile(fileName);
    }
}

void MainWindow::reopenWithGBK()
{
    reloadFile(TuxCommon::ANSI);
}

void MainWindow::reopenWithUTF8()
{
    reloadFile(TuxCommon::UTF8);
}

void MainWindow::reopenWithUTF16LE()
{
    reloadFile(TuxCommon::UTF16_LE);
}

void MainWindow::reopenWithUTF16BE()
{
    reloadFile(TuxCommon::UTF16_BE);
}

void MainWindow::content()
{
    QProcess::startDetached("ContentBrowser.exe", QStringList());
}

bool MainWindow::save()
{
    if (m_curFile.isEmpty()) {
        return saveAs();
    } else if(m_curFile.endsWith(".jpg", Qt::CaseInsensitive)) {
        return saveJpgFile(m_curFile);
    } else {
        return saveFile(m_curFile);
    }
}

bool MainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this);
    if (fileName.isEmpty())
        return false;

    if (fileName.endsWith(".jpg", Qt::CaseInsensitive)){
        if(m_curFile.endsWith(".jpg", Qt::CaseInsensitive)){
            return saveJpgFile(fileName);
        } else{
            fileName += ".txt";
        }
        //return;
    }
    return saveFile(fileName);
}

void MainWindow::slotSideWidget()
{
    QPushButton* pb=findChild<QPushButton*>(QString::fromUtf8("showFileListButton"));

    if(m_sideWidget->isVisible()){
        //m_fileListWidget->setParent(0);
        m_mainHLayout->removeWidget(m_sideWidget);
        m_sideWidget->hide();
        IconHelper::Instance()->SetIcon(pb, QChar(0xf101), 14);

    }else{
        m_sideWidget->show();
        m_mainHLayout->addWidget(m_sideWidget);
        IconHelper::Instance()->SetIcon(pb, QChar(0xf100), 14);

    }
}

void MainWindow::initFindBar()
{
    m_findWidget = new QWidget(this);
    m_findWidget->setObjectName(QString::fromUtf8("findWidget"));
    QHBoxLayout *phLayout = new QHBoxLayout;
    phLayout->setContentsMargins(10,0,10,0);
    phLayout->setAlignment(Qt::AlignVCenter);

    QLineEdit *plineEdit = new QLineEdit(m_findWidget);
    plineEdit->setObjectName(QString::fromUtf8("findBarLineEdit"));
    plineEdit->setMinimumSize(QSize(21, 21));
    plineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    if(!m_findString.isEmpty()){
        plineEdit->setText(m_findString);
    }

    QLabel *resultLabel = new QLabel(m_findWidget);
    QPushButton *capsButton = new QPushButton(m_findWidget);
    QPushButton *preButton = new QPushButton(m_findWidget);
    QPushButton *nextButton = new QPushButton(m_findWidget);
    QPushButton *closeButton = new QPushButton(m_findWidget);

    capsButton->setObjectName(QString::fromUtf8("capsButton"));
    capsButton->setFixedSize(21, 21);
    capsButton->setCheckable(true);
    capsButton->setFocusPolicy(Qt::NoFocus);
    capsButton->setToolTip(tr("Match case"));
    resultLabel->setObjectName(QString::fromUtf8("resultLabel"));
    resultLabel->setFixedSize(60, 21);
    preButton->setFixedSize(21, 21);
    preButton->setFocusPolicy(Qt::NoFocus);
    preButton->setToolTip(tr("Find previous"));
    nextButton->setFixedSize(21, 21);
    nextButton->setFocusPolicy(Qt::NoFocus);
    nextButton->setToolTip(tr("Find next"));
    closeButton->setFixedSize(21, 21);
    closeButton->setFocusPolicy(Qt::NoFocus);

    IconHelper::Instance()->SetIcon(capsButton, QChar(0xf20d), 10);
    IconHelper::Instance()->SetIcon(preButton, QChar(0xf077), 10);
    IconHelper::Instance()->SetIcon(nextButton, QChar(0xf078), 10);
    IconHelper::Instance()->SetIcon(closeButton, QChar(0xf00d), 10);

    phLayout->addWidget(capsButton);
    phLayout->addWidget(plineEdit);
    phLayout->addWidget(resultLabel);
    phLayout->addWidget(preButton);
    phLayout->addWidget(nextButton);
    phLayout->addWidget(closeButton);


    //lowOrUpperBox = new QCheckBox(tr("区分大小写(C)"));

    m_findWidget->setLayout(phLayout);

    m_findWidget->resize(300,30);
    m_findWidget->hide();

    connect (plineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(slotFindNext(const QString &)));
    connect (preButton, SIGNAL(clicked(bool)), this, SLOT(slotFindPrev()));
    connect (nextButton, SIGNAL(clicked(bool)), this, SLOT(slotFindNext()));
    connect (closeButton, SIGNAL(clicked(bool)), m_findWidget, SLOT(hide()));

}

void MainWindow::slotFindBar()
{
    if(!m_findWidget){
        return;
    }
    QTextCursor tc = m_textEdit->textCursor();
    m_findWidget->show();
    m_findWidget->findChild<QLineEdit *>("findBarLineEdit")->setText( tc.selectedText() );
}

void MainWindow::slotFindNext(const QString &s)
{
    if (!s.isEmpty()){
        m_findString = s;
        QTextCursor tc = m_textEdit->textCursor();
        tc.setPosition(0);
        m_textEdit->setTextCursor(tc);
    }
    bool found;
    if (!m_findString.isEmpty ()) {
        if (m_findCaseSensitively)
            found = m_textEdit->find (m_findString,QTextDocument::FindCaseSensitively);
        else {
            found = m_textEdit->find (m_findString);
        }
    }
    else {
        return;
    }
    if (found) {
        m_findWidget->findChild<QLabel *>("resultLabel")->setText("");
    } else {
        m_findWidget->findChild<QLabel *>("resultLabel")->setText(tr("no more"));
    }

}

void MainWindow::slotFindPrev(const QString &s)
{
    if (!s.isEmpty()){
        m_findString = s;
        QTextCursor tc = m_textEdit->textCursor();
        tc.setPosition(0);
        m_textEdit->setTextCursor(tc);
    }
    bool found;
    if (!m_findString.isEmpty ()) {
        if (m_findCaseSensitively)
            found = m_textEdit->find (m_findString,QTextDocument::FindCaseSensitively|QTextDocument::FindBackward);
        else {
            found = m_textEdit->find (m_findString, QTextDocument::FindBackward);
        }
    }
    else {
        return;
    }
    if (found) {
        m_findWidget->findChild<QLabel *>("resultLabel")->setText("");
    } else {
        m_findWidget->findChild<QLabel *>("resultLabel")->setText(tr("no more"));
    }
}

void MainWindow::slotReplaceWidget()
{
    //m_replaceWidget->setWindowFlags( m_replaceWidget->windowFlags() | Qt::WindowStaysOnTopHint);
    QTextCursor textCur = m_textEdit->textCursor();
    QString st = textCur.selectedText();
    if (!st.isEmpty())
        m_replaceWidget->setFindText(st);

    m_replaceWidget->show();
}

void MainWindow::slotPrint()
{
    QPrinter printer;
    //创建一个QPrintDialog对象，参数为QPrinter对象
    QPrintDialog printDialog(&printer, this);
    //判断打印对话框显示后用户是否单击“打印”,打印--则相关打印属性将可以通过创建QPrintDialog
    //对象时,使用的QPrinter对象获得;单击取消，则不执行后续的打印操作
    if (printDialog.exec ())
    {
        //获得QTextEdit对象的文档
        QTextDocument *doc = m_textEdit->document ();
        //打印
        doc->print (&printer);
    }
}

void MainWindow::slotWordWrap()
{
    QAction *act = dynamic_cast<QAction *>(sender());

#ifdef DEBUGREPORTING
    if (act && act->isCheckable())
        qDebug() << act->isChecked();
#endif

    if (act )
        m_isWordWraped = act->isChecked();
    if (m_isWordWraped){
        m_textEdit->setWordWrapMode(QTextOption::WordWrap);
    }
    else
        m_textEdit->setWordWrapMode(QTextOption::NoWrap);

    //act->setChecked(isWordWraped);
}

void MainWindow::copyPlainText()
{

    QTextCursor textCur = m_textEdit->textCursor();
    QClipboard *clipboard = QApplication::clipboard();

    clipboard->setText(m_textEdit->toPlainText());
}

void MainWindow::about()
{
   QMessageBox::about(this, tr("About %1").arg(PRODUCT_NAME),
            tr("The <b>Application</b> example demonstrates how to "
               "write modern GUI applications using Qt, with a menu bar, "
               "toolbars, and a status bar."));
}

void MainWindow::documentWasModified()
{
    //setWindowModified(m_textEdit->document()->isModified());
    QString tt = windowTitle();
    int pos = tt.lastIndexOf(QString(" - %1").arg(PRODUCT_NAME));
    if ( tt.at(pos-1) == '*'){
        return;
    }
    tt.insert(pos,"*");
    setWindowTitle(tt);
}

void MainWindow::readSettings()
{
    QSettings settings("redtux.cn", PRODUCT_NAME);
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(400, 400)).toSize();
    resize(size);
    move(pos);
}

void MainWindow::writeSettings()
{
    QSettings settings("redtux.cn", PRODUCT_NAME);
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}

bool MainWindow::maybeSave()
{
    if (m_textEdit->document()->isModified()) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, PRODUCT_NAME,
                     tr("The document has been modified.\n"
                        "Do you want to save your changes?"),
                     QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

void MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);

    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, PRODUCT_NAME,
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }
    QApplication::setOverrideCursor(Qt::WaitCursor);

    /*
    QByteArray ba = file.readAll();
    QString text;
    text = QTextCodec::codecForName("UTF-16LE")->toUnicode(ba);
    m_textEdit->setPlainText(text);
    /*/

    qint64 maxSize = 160480;
    QByteArray ba = file.read(maxSize);
    file.reset();
    QTextStream in(&file);

    TuxCommon::Encode ec = TuxCommon::DetectEncode((uchar*)ba.constData(), ba.size());

    switch (ec) {
    case TuxCommon::UTF16_BE :
    case TuxCommon::UTF16_LE :
    case TuxCommon::UTF8 :
    case TuxCommon::UTF8_NOBOM:
        in.setCodec("UTF-8");
        break;
    case TuxCommon::OTHER :
    case TuxCommon::ANSI :
    default:
        in.setCodec("GB18030");
        break;
    }
    m_textEdit->setPlainText(in.readAll());
    QApplication::restoreOverrideCursor();
    file.close();
#ifdef DEBUGREPORTING
    qDebug() <<fileName << ":" << ec;
#endif

    setCurrentFile(fileName);
}
/*
void MainWindow::loadFileWithUTF16LE(const QString &fileName)
{

    QFile file(fileName);

    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, PRODUCT_NAME,
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }
    QApplication::setOverrideCursor(Qt::WaitCursor);

    QByteArray ba = file.readAll();
    QTextCodec::ConverterState state;
    QTextCodec *codec = QTextCodec::codecForName("UTF-16LE");
    QString text = codec->toUnicode( ba.constData(), ba.size(), &state);
    if (state.invalidChars > 0)
    {
        qDebug() <<fileName << "With Unknown Encode" ;
    }

    m_textEdit->setPlainText(text);

    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
}
*/
void MainWindow::reloadFile(const TuxCommon::Encode encode)
{
    if(m_curFile.isEmpty()&& m_curFile.endsWith(".jpg", Qt::CaseInsensitive)) return;
    QFile file(m_curFile);

    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, PRODUCT_NAME,
                             tr("Cannot read file %1:\n%2.")
                             .arg(m_curFile)
                             .arg(file.errorString()));
        return;
    }
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QTextStream in(&file);
    switch (encode) {
    case TuxCommon::UTF16_BE :
        in.setCodec("UTF-16BE");
        break;
    case TuxCommon::UTF16_LE :
        in.setCodec("UTF-16LE");
        break;
    case TuxCommon::UTF8 :
    case TuxCommon::UTF8_NOBOM:
        in.setCodec("UTF-8");
        break;
    case TuxCommon::OTHER :
    case TuxCommon::ANSI :
    default:
        in.setCodec("GB18030");
        break;
    }
    m_textEdit->setPlainText(in.readAll());
    QApplication::restoreOverrideCursor();
    file.close();

}

void MainWindow::loadJpgFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly )) {
        QMessageBox::warning(this, PRODUCT_NAME,
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    QDataStream in(&file);

    in.device()->seek(file.size()-8);
    in >> m_fileSize;

    in.device()->seek(m_fileSize);

    // 检查魔术数字
    quint32 magic;
    QString  content = "";
    in >> magic;
    if (magic != 0xBEEFCAFE) {
        m_fileSize = 0;
        m_title = "";
    } else{
        // 检查版本
        qint32 version;
        in >> version;
        if (version <= 110) {
            in.setVersion(QDataStream::Qt_4_4);
        }
        in >> m_title;
        in >> content;
    }

    file.close();

    QApplication::setOverrideCursor(Qt::WaitCursor);
    m_textEdit->setPlainText(content);
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
}

bool MainWindow::saveFile(const QString &fileName, const TuxCommon::Encode encode)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, PRODUCT_NAME,
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
    switch (encode) {
    case TuxCommon::UTF16_BE :
        out.setGenerateByteOrderMark(true);
        out.setCodec("UTF-16BE");
        break;
    case TuxCommon::UTF16_LE :
        out.setGenerateByteOrderMark(true);
        out.setCodec("UTF-16LE");
        break;
    case TuxCommon::UTF8 :
        out.setGenerateByteOrderMark(true);
        out.setCodec("UTF-8");
        break;
    case TuxCommon::UTF8_NOBOM:
        out.setGenerateByteOrderMark(false);
        out.setCodec("UTF-8");
        break;
    case TuxCommon::OTHER :
    case TuxCommon::ANSI :
    default:
        out.setCodec("GB18030");
        break;
    }
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << m_textEdit->toPlainText();
    QApplication::restoreOverrideCursor();
    file.close();

    setCurrentFile(fileName);
    return true;
}

bool MainWindow::saveJpgFile(const QString &fileName)
{
    if(fileName != m_curFile && (!QFile::copy(m_curFile, fileName))){
        return false;
    }
    QFile file(fileName);

    if (m_fileSize){
        file.resize(m_fileSize);
        m_fileSize = 0;
    }
    if(m_textEdit->toPlainText().isEmpty()){
        setCurrentFile(fileName);
        return true;
    }
    else {
        m_fileSize = file.size();
    }

    if (!file.open(QFile::Append)) {
        QMessageBox::warning(this, PRODUCT_NAME,
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

#ifdef DEBUGREPORTING
    qDebug() <<m_fileSize ;
    qDebug() <<file.size() ;
#endif

    QDataStream out(&file);

    QApplication::setOverrideCursor(Qt::WaitCursor);

    out << (quint32)0xBEEFCAFE;
    out << (qint32)110;
    out.setVersion(QDataStream::Qt_4_4);

    QString catalog = "";
    QString author = "";


    QStringList tt = m_textEdit->document()->findBlockByLineNumber(0).text().simplified().split(QString(":"));
    if(tt.size() < 2) {
        QTextCursor tc = m_textEdit->textCursor();
        tc.setPosition(0);
        tc.insertText("title:");
    }

    for (int i = 0; i< 10; ++i){
        QStringList lineStrs = m_textEdit->document()->findBlockByLineNumber(i).text().simplified().split(QString(":"));
        if(lineStrs.at(0).simplified().isEmpty()){  //is a empty line
            i =10;
        }else if ("title" == lineStrs.at(0).simplified()){
            m_title = lineStrs.at(1).simplified() ;
        }
        else if ("catalog" == lineStrs.at(0).simplified()) {
            catalog = lineStrs.at(1).simplified();
        }
        else if ("author" == lineStrs.at(0).simplified()) {
            author = lineStrs.at(1).simplified();
        }
    }
    QSqlQuery query;
    query.prepare( "SELECT id FROM files WHERE filename=?");
    query.addBindValue(strippedName(fileName));
    query.exec();
    if (query.first()){
        m_curFileId = query.value(0).toInt();
    }
    //把文件信息存放到 sqlite
    QString sql;
    if (m_curFileId < 0) {
        sql = "INSERT INTO files (title, catalog, author, filename) VALUES (:title, :catalog, :author, :filename)";
    } else {
        sql = "UPDATE files SET title=:title, catalog=:catalog, author=:author, filename=:filename WHERE id="+ QString::number(m_curFileId, 10);
    }
    query.prepare(sql);
    query.bindValue(":title", m_title);
    query.bindValue(":author", author);
    query.bindValue(":catalog", catalog);
    query.bindValue(":filepath", strippedName(fileName));
    query.exec();

    //save text to the end of the jpg file
    out << m_title;
    out << m_textEdit->toPlainText();
    out << m_fileSize;
    QApplication::restoreOverrideCursor();

    file.close();

    setCurrentFile(fileName);
    return true;
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    m_curFile = fileName;
    m_textEdit->document()->setModified(false);
    setWindowModified(false);

    QString shownName;
    if (m_curFile.isEmpty())
        shownName = "untitled.txt";
    else if(m_curFile.endsWith(".jpg", Qt::CaseInsensitive)&& !m_title.isEmpty())
        shownName = m_title;
    else
        shownName = strippedName(m_curFile);

    setWindowTitle(tr("%1 - %2").arg(shownName).arg(PRODUCT_NAME));
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void MainWindow::setupUi()
{
    QVBoxLayout *pvLayout = new QVBoxLayout();
    pvLayout->setContentsMargins(0,0,0,0);
    pvLayout->setSpacing(0);
    pvLayout->setObjectName(QString::fromUtf8("widgetVLayout"));

    QHBoxLayout *toolBarHLayout = new QHBoxLayout();
    toolBarHLayout->setSpacing(0);
    toolBarHLayout->setContentsMargins(0, 6, 0, 6);

    QPushButton *pnewFileButton = new QPushButton(this);
    pnewFileButton->setObjectName(QString::fromUtf8("newFileButton"));
    pnewFileButton->setFixedSize(QSize(34, 34));
    pnewFileButton->setCursor(QCursor(Qt::ArrowCursor));
    pnewFileButton->setFocusPolicy(Qt::NoFocus);
    pnewFileButton->setToolTip(tr("New"));
    //pnewFileButton->setIconSize(QSize(18, 18));
    IconHelper::Instance()->SetIcon(pnewFileButton, QChar(0xf0f6), 13);

    toolBarHLayout->addWidget(pnewFileButton);

    QPushButton *pOpenFileButton = new QPushButton(this);
    pOpenFileButton->setObjectName(QString::fromUtf8("openFileButton"));
    pOpenFileButton->setFixedSize(QSize(34, 34));
    pOpenFileButton->setCursor(QCursor(Qt::ArrowCursor));
    pOpenFileButton->setFocusPolicy(Qt::NoFocus);
    pOpenFileButton->setToolTip(tr("Open..."));
    //nextToolButton->setFlat(true);
    IconHelper::Instance()->SetIcon(pOpenFileButton, QChar(0xf115), 14);

    toolBarHLayout->addWidget(pOpenFileButton);

    QPushButton *pSaveButton = new QPushButton(this);
    pSaveButton->setObjectName(QString::fromUtf8("saveButton"));
    pSaveButton->setFixedSize(QSize(34, 34));
    pSaveButton->setCursor(QCursor(Qt::ArrowCursor));
    pSaveButton->setFocusPolicy(Qt::NoFocus);
    pSaveButton->setToolTip(tr("Save"));
    IconHelper::Instance()->SetIcon(pSaveButton, QChar(0xf0c7), 14);

    toolBarHLayout->addWidget(pSaveButton);

    QFrame *pline1 = new QFrame(this);
    pline1->setFrameShape(QFrame::VLine);
    pline1->setFrameShadow(QFrame::Sunken);

    toolBarHLayout->addWidget(pline1);

    QPushButton *pCutButton = new QPushButton(this);
    pCutButton->setObjectName(QString::fromUtf8("cutButton"));
    pCutButton->setFixedSize(QSize(34, 34));
    pCutButton->setCursor(QCursor(Qt::ArrowCursor));
    pCutButton->setFocusPolicy(Qt::NoFocus);
    pCutButton->setToolTip(tr("Cut"));
    IconHelper::Instance()->SetIcon(pCutButton, QChar(0xf0c4), 14);
    toolBarHLayout->addWidget(pCutButton);

    QPushButton *pCopyButton = new QPushButton(this);
    pCopyButton->setObjectName(QString::fromUtf8("copyButton"));
    pCopyButton->setFixedSize(QSize(34, 34));
    pCopyButton->setCursor(QCursor(Qt::ArrowCursor));
    pCopyButton->setFocusPolicy(Qt::NoFocus);
    pCopyButton->setToolTip(tr("Copy"));
    IconHelper::Instance()->SetIcon(pCopyButton, QChar(0xf0c5), 13);
    toolBarHLayout->addWidget(pCopyButton);

    QPushButton *pPasteButton = new QPushButton(this);
    pPasteButton->setObjectName(QString::fromUtf8("pasteButton"));
    pPasteButton->setFixedSize(QSize(34, 34));
    pPasteButton->setCursor(QCursor(Qt::ArrowCursor));
    pPasteButton->setFocusPolicy(Qt::NoFocus);
    pPasteButton->setToolTip(tr("Paste"));
    IconHelper::Instance()->SetIcon(pPasteButton, QChar(0xf0ea), 13);
    toolBarHLayout->addWidget(pPasteButton);

    QFrame *pline2 = new QFrame(this);
    pline2->setFrameShape(QFrame::VLine);
    pline2->setFrameShadow(QFrame::Sunken);

    toolBarHLayout->addWidget(pline2);

    QPushButton *pUndoButton = new QPushButton(this);
    pUndoButton->setObjectName(QString::fromUtf8("undoButton"));
    pUndoButton->setFixedSize(QSize(34, 34));
    pUndoButton->setCursor(QCursor(Qt::ArrowCursor));
    pUndoButton->setFocusPolicy(Qt::NoFocus);
    pUndoButton->setToolTip(tr("Undo"));
    IconHelper::Instance()->SetIcon(pUndoButton, QChar(0xf0e2), 14);
    toolBarHLayout->addWidget(pUndoButton);

    QPushButton *pRedoButton = new QPushButton(this);
    pRedoButton->setObjectName(QString::fromUtf8("redoButton"));
    pRedoButton->setFixedSize(QSize(34, 34));
    pRedoButton->setCursor(QCursor(Qt::ArrowCursor));
    pRedoButton->setFocusPolicy(Qt::NoFocus);
    pRedoButton->setToolTip(tr("Redo"));
    IconHelper::Instance()->SetIcon(pRedoButton, QChar(0xf01e), 14);
    toolBarHLayout->addWidget(pRedoButton);

    QFrame *pline3 = new QFrame(this);
    pline3->setFrameShape(QFrame::VLine);
    pline3->setFrameShadow(QFrame::Sunken);

    toolBarHLayout->addWidget(pline3);

    QPushButton *pFindButton = new QPushButton(this);
    pFindButton->setObjectName(QString::fromUtf8("findButton"));
    pFindButton->setFixedSize(QSize(34, 34));
    pFindButton->setCursor(QCursor(Qt::ArrowCursor));
    pFindButton->setFocusPolicy(Qt::NoFocus);
    pFindButton->setToolTip(tr("Find"));
    IconHelper::Instance()->SetIcon(pFindButton, QChar(0xf002), 14);
    toolBarHLayout->addWidget(pFindButton);

    QPushButton *pReplaceButton = new QPushButton(this);
    pReplaceButton->setObjectName(QString::fromUtf8("replaceButton"));
    pReplaceButton->setFixedSize(QSize(34, 34));
    pReplaceButton->setCursor(QCursor(Qt::ArrowCursor));
    pReplaceButton->setFocusPolicy(Qt::NoFocus);
    pReplaceButton->setToolTip(tr("Replace"));
    IconHelper::Instance()->SetIcon(pReplaceButton, QChar(0xf25d), 14);
    toolBarHLayout->addWidget(pReplaceButton);

    QPushButton *pPrintButton = new QPushButton(this);
    pPrintButton->setObjectName(QString::fromUtf8("printButton"));
    pPrintButton->setFixedSize(QSize(34, 34));
    pPrintButton->setCursor(QCursor(Qt::ArrowCursor));
    pPrintButton->setFocusPolicy(Qt::NoFocus);
    pPrintButton->setToolTip(tr("Print"));
    IconHelper::Instance()->SetIcon(pPrintButton, QChar(0xf02f), 14);
    toolBarHLayout->addWidget(pPrintButton);

    toolBarHLayout->addStretch(1);

    QPushButton *pShowSideWidgetButton = new QPushButton(this);
    pShowSideWidgetButton->setObjectName(QString::fromUtf8("showFileListButton"));
    pShowSideWidgetButton->setFixedSize(QSize(34, 34));
    pShowSideWidgetButton->setCursor(QCursor(Qt::ArrowCursor));
    pShowSideWidgetButton->setFocusPolicy(Qt::NoFocus);
    pShowSideWidgetButton->setToolTip(tr("Show/Hide Explorer"));
    IconHelper::Instance()->SetIcon(pShowSideWidgetButton, QChar(0xf101), 14);
    toolBarHLayout->addWidget(pShowSideWidgetButton);

    pvLayout->addLayout(toolBarHLayout);

    m_mainHLayout = new QHBoxLayout();
    m_mainHLayout->setSpacing(2);

    m_textEdit->setObjectName(QString::fromUtf8("plainTextEdit"));
    m_textEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_sideWidget = new QTabWidget( this);
    m_sideWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    m_sideWidget->setMinimumSize(200,200);
    m_sideWidget->setTabPosition(QTabWidget::South);
    m_sideWidget->setTabShape(QTabWidget::Triangular);

    FileManWidget *pfileManWidget = new FileManWidget( this);
    m_sideWidget->addTab(pfileManWidget, QString("Explorer"));
    pfileManWidget->selectDir(Settings::wallpaperDirPath());
    connect(pfileManWidget, SIGNAL(openFile(const QString &)), this, SLOT(open(const QString &)));

    ChapterWidget *pchapterWidget = new ChapterWidget(m_textEdit, this);
    m_sideWidget->addTab(pchapterWidget, QString("Chapter"));

    m_mainHLayout->addWidget(m_textEdit);
    m_mainHLayout->addWidget(m_sideWidget);


    m_sideWidget->hide();

    pvLayout->addLayout(m_mainHLayout);

    setMainLayout(pvLayout);

    connect(pnewFileButton, SIGNAL(clicked()), this, SLOT(newFile()));
    connect(pOpenFileButton, SIGNAL(clicked()), this, SLOT(open()));
    connect(pSaveButton, SIGNAL(clicked()), this, SLOT(save()));
    connect(pCutButton, SIGNAL(clicked()), m_textEdit, SLOT(cut()));
    connect(pCopyButton, SIGNAL(clicked()), this, SLOT(copyPlainText()));
    connect(pPasteButton, SIGNAL(clicked()), m_textEdit, SLOT(paste()));
    connect(pUndoButton, SIGNAL(clicked()), m_textEdit, SLOT(undo()));
    connect(pRedoButton, SIGNAL(clicked()), m_textEdit, SLOT(redo()));
    connect(pFindButton, SIGNAL(clicked()), this, SLOT(slotFindBar()));
    connect(pReplaceButton, SIGNAL(clicked()), this, SLOT(slotReplaceWidget()));
    connect(pPrintButton, SIGNAL(clicked()), this, SLOT(slotPrint()));
    connect(pShowSideWidgetButton, SIGNAL(clicked()), this, SLOT(slotSideWidget()));

    connect(m_textEdit->document(), SIGNAL(contentsChanged()),
            this, SLOT(documentWasModified()));
    setMinimumSize(400,300);
}

void MainWindow::initMenu()
{
    QMenu *pMenu = new QMenu(this);

    QMenu *pFileMenu = pMenu->addMenu(tr("&File"));

    QAction *pNewAct = pFileMenu->addAction(tr("&New"));
    pNewAct->setShortcuts(QKeySequence::New);
    connect(pNewAct, SIGNAL(triggered()), this, SLOT(newFile()));

    QAction *pOpenAct = pFileMenu->addAction(tr("&Open.."));
    pOpenAct->setShortcuts(QKeySequence::Open);
    connect(pOpenAct, SIGNAL(triggered()), this, SLOT(open()));

    QAction *pSaveAct = pFileMenu->addAction(tr("&Save"));
    pSaveAct->setShortcuts(QKeySequence::Save);
    connect(pSaveAct, SIGNAL(triggered()), this, SLOT(save()));

    QAction *pSaveAsAct = pFileMenu->addAction(tr("Save &As..."));
    //pSaveAsAct->setShortcuts(QKeySequence::SaveAS);
    pSaveAsAct->setShortcut( QKeySequence(tr("Ctrl+Shift+S")));
    connect(pSaveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    pFileMenu->addSeparator();
    pFileMenu->addAction(tr("P&rint ..."), this, SLOT(slotPrint()), tr("Ctrl+P"));

    QMenu *pEditMenu = pMenu->addMenu(tr("&Edit"));
    QAction *pUndoAct = pEditMenu->addAction(tr("&Undo"));
    pUndoAct->setShortcuts(QKeySequence::Undo);
    connect(pUndoAct, SIGNAL(triggered()), m_textEdit, SLOT(undo()));

    QAction *pRedoAct = pEditMenu->addAction(tr("&Redo"));
    pRedoAct->setShortcuts(QKeySequence::Redo);
    connect(pRedoAct, SIGNAL(triggered()), m_textEdit, SLOT(redo()));

    pEditMenu->addSeparator();

    QAction *pCutAct = pEditMenu->addAction(tr("Cu&t"));
    pCutAct->setShortcuts(QKeySequence::Cut);
    pCutAct->setStatusTip(tr("Cut the current selection's contents to the "
                            "clipboard"));
    connect(pCutAct, SIGNAL(triggered()), m_textEdit, SLOT(cut()));

    QAction *pCopyAct = pEditMenu->addAction(tr("&Copy"));
    pCopyAct->setShortcuts(QKeySequence::Copy);
    pCopyAct->setStatusTip(tr("Copy the current selection's contents to the "
                             "clipboard"));
    connect(pCopyAct, SIGNAL(triggered()), this, SLOT(copyPlainText()));

    QAction *pPasteAct = pEditMenu->addAction(tr("&Paste"));
    pPasteAct->setShortcuts(QKeySequence::Paste);
    pPasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
    connect(pPasteAct, SIGNAL(triggered()), m_textEdit, SLOT(paste()));

    QMenu *pSearchMenu = pMenu->addMenu(tr("&Search"));

    QAction *pFindAct = pSearchMenu->addAction(tr("&Find ..."));
    pFindAct->setShortcuts(QKeySequence::Find);
    connect(pFindAct, SIGNAL(triggered()), this, SLOT(slotFindBar()));

    QAction *pFindNextAct = pSearchMenu->addAction(tr("Find &Next"));
    pFindNextAct->setShortcuts(QKeySequence::FindNext);
    connect(pFindNextAct, SIGNAL(triggered()), this, SLOT(slotFindNext()));

    QAction *pFindPrevAct = pSearchMenu->addAction(tr("Find &Previous"));
    pFindPrevAct->setShortcuts(QKeySequence::FindPrevious);
    connect(pFindPrevAct, SIGNAL(triggered()), this, SLOT(slotFindPrev()));

    QAction *pReplaceAct = pSearchMenu->addAction(tr("&Replace ..."));
    pReplaceAct->setShortcuts(QKeySequence::Replace);
    connect(pReplaceAct, SIGNAL(triggered()), this, SLOT(slotReplaceWidget()));

    QMenu *pViewMenu = pMenu->addMenu(tr("View"));

    QAction *pWordWrapAct = pViewMenu->addAction(tr("&Word Wrap"));
    pWordWrapAct->setCheckable(true);
    pWordWrapAct->setChecked(m_isWordWraped);
    slotWordWrap();
    connect(pWordWrapAct, SIGNAL(triggered()), this, SLOT(slotWordWrap()));

    pViewMenu->addSeparator();

    pViewMenu->addAction(tr("Reopen with GB18030"), this, SLOT(reopenWithGBK()) );
    pViewMenu->addAction(tr("Reopen with UTF-8"), this, SLOT(reopenWithUTF8()) );
    pViewMenu->addAction(tr("Reopen with UTF-16LE"), this, SLOT(reopenWithUTF16LE()) );
    pViewMenu->addAction(tr("Reopen with UTF-16BE"), this, SLOT(reopenWithUTF16BE()) );

    pViewMenu->addSeparator();

    QAction *pFilesListAct = pViewMenu->addAction(tr("&Explorer"));
    pFilesListAct->setShortcut( QKeySequence(tr("Shift+Alt+D")));
    connect(pFilesListAct, SIGNAL(triggered()), this, SLOT(slotSideWidget()));

    pMenu->addSeparator();

    pMenu->addAction(tr("&Preferences ..."), this, SLOT(slotSideWidget()), tr("Ctrl+,"));

    QAction *pContentAct = pMenu->addAction(tr("Content..."));
    //pSaveAsAct->setShortcuts(QKeySequence::SaveAS);
    pContentAct->setShortcut( QKeySequence(tr("Ctrl+Shift+D")));
    connect(pContentAct, SIGNAL(triggered()), this, SLOT(content()));

    QAction *pExitAct = pMenu->addAction(tr("&Exit"));
    pExitAct->setShortcuts(QKeySequence::Close);
    connect(pExitAct, SIGNAL(triggered()), this, SLOT(close()));

    setupMenu(pMenu);

}
