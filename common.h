#ifndef COMMON_H
#define COMMON_H

#include <QObject>
#include <QString>
#include <QTextCodec>
#ifdef DEBUGREPORTING
#include <QDebug>
#endif

namespace TuxCommon {

enum Encode { ANSI = 1, UTF16_LE, UTF16_BE, UTF8, UTF8_NOBOM, OTHER };

enum StretchArea
{
    NO_SELECT = 0,              // 鼠标未进入下方矩形区域;
    LEFT_TOP_RECT,              // 鼠标在左上角区域;
    TOP_BORDER,                 // 鼠标在上边框区域;
    RIGHT_TOP_RECT,             // 鼠标在右上角区域;
    RIGHT_BORDER,               // 鼠标在右边框区域;
    RIGHT_BOTTOM_RECT,          // 鼠标在右下角区域;
    BOTTOM_BORDER,              // 鼠标在下边框区域;
    LEFT_BOTTOM_RECT,           // 鼠标在左下角区域;
    LEFT_BORDER,                // 鼠标在左边框区域;
    TITLE_BAR
};
enum WidgetAlignment
{
    TOP_LEFT,
    TOP,
    TOP_RIGHT,
    RIGHT,
    BOTTOM_RIGHT,
    BOTTOM,
    BOTTOM_LEFT,
    LEFT
};

QString GetCorrectUnicode(const QByteArray &ba);
Encode DetectEncodeWithoutBOM(const uchar *pText, int length);
Encode DetectEncode(const uchar *pText, int length);

bool createConnection();

QString getFilePath();

}

#endif // COMMON_H
