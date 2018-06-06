#-------------------------------------------------
#
# Project created by QtCreator 2017-11-27T21:25:52
#
# 图片打开保存  ok
# 查找  ok
# 文件列表 ok
# 添加选中查找  ok
# 替换    基本ok  完善对话框的样式
# 打印   ok
# 设置
# 自动换行   ok
# 字体
# 提取章节  提取小于40字符的段落
# 去掉非段落换行
# 加密
# 设置编码  tools/codecs/codecs.pro          ok
# 路径提示 tools/completer/completer.pro
# 正则表达式 tools/regexp/regexp.pro   完善查找替换
# 行号 widgets/codeeditor/codeeditor.pro    ok   完善点击行号选中行 参考 QtCreator
# 样式 widgets/stylesheet/stylesheet.pro
# 历史记录QUndoView  tools/undoframework/undoframework.pro
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

qtHaveModule(printsupport): QT += printsupport

DEFINES += DEBUGREPORTING

TARGET = TuxEditor
TEMPLATE = app

DEPENDPATH  += .  ./3rdparty
INCLUDEPATH += .  ./3rdparty

SOURCES += main.cpp \
    mainwindow.cpp \
    iconhelper.cpp \
    replacewidget.cpp \
    tuxtexteditor.cpp \
    common.cpp \
    filemanwidget.cpp \
    chapterwidget.cpp \
    chapterlistmodel.cpp \
    tuxwidget.cpp \
    settings.cpp

HEADERS  += \
    version.h \
    mainwindow.h \
    iconhelper.h \
    common.h \
    replacewidget.h \
    tuxtexteditor.h \
    filemanwidget.h \
    chapterwidget.h \
    chapterlistmodel.h \
    tuxwidget.h \
    settings.h

RESOURCES += \
    res/tuxeditor.qrc

win32 {
   RC_FILE = tuxeditor.rc
}
