#ifndef TUXTEXTEDITOR_H
#define TUXTEXTEDITOR_H


#include <QPlainTextEdit>

class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;

class LineNumberArea;

class TuxTextEditor : public QPlainTextEdit
 {
     Q_OBJECT

 public:
     TuxTextEditor(QWidget *parent = 0);

     void lineNumberAreaPaintEvent(QPaintEvent *event);
     int lineNumberAreaWidth();

 protected:
     void resizeEvent(QResizeEvent *event);

 private slots:
     void updateLineNumberAreaWidth(int newBlockCount);
     void highlightCurrentLine();
     void updateLineNumberArea(const QRect &, int);

 private:
     QWidget *lineNumberArea;
 };

class LineNumberArea : public QWidget
 {
 public:
     LineNumberArea(TuxTextEditor *editor) : QWidget(editor) {
         textEditor = editor;
     }

     QSize sizeHint() const {
         return QSize(textEditor->lineNumberAreaWidth(), 0);
     }

 protected:
     void paintEvent(QPaintEvent *event) {
         textEditor->lineNumberAreaPaintEvent(event);
     }

 private:
     TuxTextEditor *textEditor;
 };

#endif // TUXTEXTEDITOR_H
