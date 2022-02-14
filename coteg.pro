TEMPLATE = subdirs

SUBDIRS += coteg coteg/plugins/otrs coteg/plugins/reminder coteg/plugins/simplereactions coteg/libs/basicstruct
coteg.file = coteg/coteg.pro
otrs.file = coteg/plugins/otrs/otrs.pro
reminder.file = coteg/plugins/reminder/reminder.pro
simplereactions.file = coteg/plugins/simplereactions.pro

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/coteg/bin/release/ -lbasicstruct
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/coteg/bin/debug/ -lbasicstruct
else:unix: LIBS += -L$$PWD/coteg/bin/ -lbasicstruct

INCLUDEPATH += $$PWD/coteg/libs
DEPENDPATH += $$PWD/coteg/libs

