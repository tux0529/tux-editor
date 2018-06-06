
#include "common.h"
#include "settings.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QFile>
#include <QStringList>

namespace TuxCommon {

QString GetCorrectUnicode(const QByteArray &ba)
{
    QTextCodec::ConverterState state;
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QString text = codec->toUnicode( ba.constData(), ba.size(), &state);
    if (state.invalidChars > 0)
    {
        text = QTextCodec::codecForName( "GBK" )->toUnicode(ba);
    }

    return text;
}

Encode DetectEncodeWithoutBOM(const uchar *pText, int length)
{
    qint32 nBytes = 0;

    uchar chr;
    bool bAllAscii = true;
    for (int i = 0; i < length; ++i)
    {
        chr = *(pText + i);
        if ((chr & 0x80) != 0)
            bAllAscii = false;
        if (nBytes == 0)
        {
            if (chr >= 0x80)
            {
                if (chr >= 0xFC && chr <= 0xFD)
                    nBytes = 6;
                else if (chr >= 0xF8)
                    nBytes = 5;
                else if (chr >= 0xF0)
                    nBytes = 4;
                else if (chr >= 0xE0)
                    nBytes = 3;
                else if (chr >= 0xC0)
                    nBytes = 2;
                else
                {
                    return OTHER;
                }
                nBytes--;
            }
        }
        else
        {
            if ((chr & 0xC0) != 0x80)
            {
                return OTHER;
            }
            nBytes--;
        }
    }
    if (nBytes > 0)
    {
        return OTHER;
    }
    if (bAllAscii)
    {
        return ANSI;
    }
    return UTF8_NOBOM;
}


Encode DetectEncode(const uchar *pText, int length)
{
#ifdef DEBUGREPORTING
    uchar chr_dbg;
    for(int i = 0; i < 10; ++i){
        chr_dbg = *(pText + i);
        qDebug() << "char:" << QString::number(chr_dbg, 16);
    }
#endif
    uchar chr[3];
    for (int i = 0; i< 3; ++i){
        chr[i] = *(pText + i);
    }
    if (chr[0] == 0xFF && chr[1] == 0xFE)
    {
        return UTF16_LE;
    }
    else if (chr[0] == 0xFE && chr[1] == 0xFF)
    {
        return UTF16_BE;
    }
    else if (chr[0] == 0xEF && chr[1] == 0xBB && chr[2] == 0xBF)
    {
        return UTF8;
    }
    else
        return DetectEncodeWithoutBOM(pText , length);
}

bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(Settings::wallpaperDirPath() + "/files.dat");
    if (!db.open()) {
        qDebug() << "Unable to establish a database connection.\n";
        return false;
    }
    QSqlQuery query;
    query.exec("create table files (id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "title varchar(80), catalog varchar(20), author varchar(20), filename varchar(80))");
    return true;
}

/*
static bool isUtf8(const void* pBuffer, long size){
    bool bIsUTF8 = true;
    unsigned char* start = (unsigned char*)pBuffer;
    unsigned char* end = (unsigned char*)pBuffer + size;
    while (start < end){
        if (*start < 0x80){ // (10000000): 值小于0x80的为ASCII字符
            start++;
        }
        else if (*start < (0xC0)){ // (11000000): 值介于0x80与0xC0之间的为无效UTF-8字符
            bIsUTF8 = false;
            break;
        }
        else if (*start < (0xE0)){ // (11100000): 此范围内为2字节UTF-8字符
            if (start >= end - 1){
                break;
            }
            if ((start[1] & (0xC0)) != 0x80){
                bIsUTF8 = false;
                break;
            }
            start += 2;
        }
        else if (*start < (0xF0)) {// (11110000): 此范围内为3字节UTF-8字符
            if (start >= end - 2){
                break;
            }
            if ((start[1] & (0xC0)) != 0x80 || (start[2] & (0xC0)) != 0x80){
                bIsUTF8 = false;
                break;
            }
            start += 3;
        }
        else{
            bIsUTF8 = false;
            break;
        }
    }
    return bIsUTF8;
}
*/
}
