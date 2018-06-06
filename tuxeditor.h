#ifndef TUXEDITOR_H
#define TUXEDITOR_H

#include <QWidget>
#include <QPlainTextEdit>

class TuxEditor : public QWidget
{
    Q_OBJECT

public:
    TuxEditor(QWidget *parent = 0);
    ~TuxEditor();

private slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();
private:
    QPlainTextEdit *m_txtEdit;
    QString m_curFile;
    QByteArray m_byteArray;
    bool maybeSave();
    void setCurrentFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void loadFile(const QString &fileName);
};

#endif // TUXEDITOR_H
