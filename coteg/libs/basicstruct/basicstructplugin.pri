win32:CONFIG(release, debug|release): LIBS += -L./../../bin -lbasicstruct
else:win32:CONFIG(debug, debug|release): LIBS += -L./../../bin/ -lbasicstruct
else:unix: LIBS += -L./../../bin/ -lbasicstruct

INCLUDEPATH += $$PWD/coteg/libs
DEPENDPATH += $$PWD/coteg/libs