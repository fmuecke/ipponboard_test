TEMPLATE = app
CONFIG += console
TARGET = IpponboardTest
DESTDIR = bin

HEADERS = \
	TestJson.hpp \
	TestScore.hpp \
	TestFight.hpp \
    TestTournamentMode.hpp

SOURCES = \
    IpponboardTest.cpp \
    ../util/jsoncpp/json.cpp

OTHER_FILES += \
    TestData/TournamentModes-test.ini \
    TestData/utf8.json \
    TestData/utf8_with_bom.json

QMAKE_CXXFLAGS += -EHsc

MY_QTBINDIR=$$[QT_INSTALL_BINS]

# Copy required DLLs to output directory
CONFIG(debug, debug|release) {
	# Note: in Qt5 $$shell_quote and $$shell_path can be used instead of $$replace
    #QMAKE_POST_LINK += copy /Y $$shell_quote($$shell_path($$[QT_INSTALL_BINS]/QtCored4.dll)) bin
    QtCored4.commands = copy /Y $$replace(MY_QTBINDIR, /, \\)\\QtCored4.dll bin
    QtCored4.target = bin/QtCored4.dll
    QMAKE_EXTRA_TARGETS += QtCored4
    POST_TARGETDEPS += bin/QtCored4.dll
} else:CONFIG(release, debug|release) {
    #QMAKE_POST_LINK += copy /Y $$replace(MY_QTBINDIR, /, \\)\\QtCore4.dll bin
    QtCore4.commands = copy /Y $$replace(MY_QTBINDIR, /, \\)\\QtCore4.dll bin
    QtCore4.target = bin/QtCore4.dll
    QMAKE_EXTRA_TARGETS += QtCore4
    POST_TARGETDEPS += bin/QtCore4.dll
} else {
    error(Unknown set of dependencies.)
}

QMAKE_POST_LINK += "xcopy /Y /Q /I TestData bin\\TestData>nul"
