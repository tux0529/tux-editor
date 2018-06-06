#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "tuxwidget.h"
#include <QWidget>

class QHBoxLayout;
class QTabWidget;
class TuxWidget;
class ReplaceWidget;
class TuxTextEditor;

class MainWindow : public TuxWidget
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void open(const QString &fileName, int fileId = -1);

protected:
    void closeEvent(QCloseEvent *event);
    void resizeEvent(QResizeEvent *event);
    //void moveEvent(QMoveEvent *event);

private slots:
    void newFile();
    void open();
    void reopenWithGBK();
    void reopenWithUTF8();
    void reopenWithUTF16LE();
    void reopenWithUTF16BE();
    void content();
    bool save();
    bool saveAs();
    void slotSideWidget();
    void slotFindBar();   //显示查找工具条
    void slotFindNext(const QString &s="");  //向后查找
    void slotFindPrev(const QString &s="");  //向前查找
    void slotReplaceWidget();
    void slotPrint();
    void slotWordWrap();
    void copyPlainText();
    void about();
    void documentWasModified();

private:
    void initFindBar();
    void setupUi();
    void initMenu();
    void readSettings();
    void writeSettings();
    bool maybeSave();
    void loadFile(const QString &fileName);
    //void loadFileWithUTF16LE(const QString &fileName);
    void reloadFile(const TuxCommon::Encode encode);
    void loadJpgFile(const QString &fileName);
    bool saveFile(const QString &fileName, const TuxCommon::Encode encode = TuxCommon::OTHER);
    bool saveJpgFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);

    QWidget *m_findWidget;
    QHBoxLayout *m_mainHLayout;
    ReplaceWidget *m_replaceWidget;
    QTabWidget *m_sideWidget;
    TuxTextEditor *m_textEdit;
    QString m_curFile;
    int m_curFileId;
    QString m_title;
    qint64 m_fileSize;
    QString m_findString;
    bool m_findCaseSensitively;
    bool m_isWordWraped;
};

#endif // MAINWINDOW_H
