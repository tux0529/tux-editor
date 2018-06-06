#include "tuxtexteditor.h"

#include <QTextBlock>
#include <QPainter>
#ifdef DEBUGREPORTING
#include <QDebug>
#endif

#define LINENUMBERAREA_PADDING_RIGHT 5

TuxTextEditor::TuxTextEditor(QWidget *parent) : QPlainTextEdit(parent)
 {
     lineNumberArea = new LineNumberArea(this);
     lineNumberArea->setObjectName(QString::fromUtf8("lineNumberArea"));

     connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
     connect(this, SIGNAL(updateRequest(const QRect &, int)), this, SLOT(updateLineNumberArea(const QRect &, int)));
     connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

     updateLineNumberAreaWidth(0);
     highlightCurrentLine();
 }

 int TuxTextEditor::lineNumberAreaWidth()
 {
     int digits = 1;
     int max = qMax(1, blockCount());
     while (max >= 10) {
         max /= 10;
         ++digits;
     }

     int space = LINENUMBERAREA_PADDING_RIGHT*2 + lineNumberArea->fontMetrics().width(QLatin1Char('9')) * digits;

     return space;
 }

 void TuxTextEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
 {
     setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
 }

 void TuxTextEditor::updateLineNumberArea(const QRect &rect, int dy)
 {
     if (dy)
         lineNumberArea->scroll(0, dy);
     else
         lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

     if (rect.contains(viewport()->rect()))
         updateLineNumberAreaWidth(0);
 }

 void TuxTextEditor::resizeEvent(QResizeEvent *e)
 {
     QPlainTextEdit::resizeEvent(e);

     QRect cr = contentsRect();
     lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
 }

 void TuxTextEditor::highlightCurrentLine()
 {
     QList<QTextEdit::ExtraSelection> extraSelections;

     if (!isReadOnly()) {
         QTextEdit::ExtraSelection selection;

         QColor lineColor = QColor(Qt::gray).lighter(150);

         selection.format.setBackground(lineColor);
         selection.format.setProperty(QTextFormat::FullWidthSelection, true);
         selection.cursor = textCursor();
         selection.cursor.clearSelection();
         extraSelections.append(selection);
     }
     setExtraSelections(extraSelections);
 }

 void TuxTextEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
 {
     QPainter painter(lineNumberArea);
     painter.fillRect(event->rect(), Qt::lightGray);


     QTextBlock block = firstVisibleBlock();
     int blockNumber = block.blockNumber();
     int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
     int bottom = top + (int) blockBoundingRect(block).height();

     while (block.isValid() && top <= event->rect().bottom()) {
         if (block.isVisible() && bottom >= event->rect().top()) {
             QString number = QString::number(blockNumber + 1);
             //painter.setPen(Qt::black);
             painter.drawText(0, top, lineNumberArea->width()-LINENUMBERAREA_PADDING_RIGHT, fontMetrics().height(),
                              Qt::AlignRight|Qt::AlignVCenter, number);
         }

         block = block.next();
         top = bottom;
         bottom = top + (int) blockBoundingRect(block).height();
         ++blockNumber;
     }
 }
