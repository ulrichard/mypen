

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

QT4_WRAP_UI(UI_SOURCES mainwindow.ui)
SET(loganalyzer_SRCS main.cpp mainwindow.cpp)
SET(loganalyzer_MOC_HDRS mainwindow.h ${UI_SOURCES})
QT4_WRAP_CPP(loganalyzer_SRCS ${loganalyzer_MOC_HDRS})

ADD_EXECUTABLE(loganalyzer
	${loganalyzer_SRCS}
)

TARGET_LINK_LIBRARIES(loganalyzer
	${Boost_SYSTEM_LIBRARY}
	${Boost_FILESYSTEM_LIBRARY}
#	${Boost_THREAD_LIBRARY}
	${QT_LIBRARIES}
)

SET_TARGET_PROPERTIES(loganalyzer
  PROPERTIES
    VERSION ${VERSION_SERIES}.${VERSION_MAJOR}.${VERSION_MINOR}
    DEBUG_POSTFIX "d"
  )


