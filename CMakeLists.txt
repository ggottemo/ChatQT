cmake_minimum_required(VERSION 3.14)

project(ChatQt2)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
if(WIN32)
  set(OPENSSL_ROOT_DIR "C:/Qt/Tools/OpenSSL/Win_x64")
  set(OPENSSL_LIBRARIES "${OPENSSL_ROOT_DIR}/lib/libssl.lib;${OPENSSL_ROOT_DIR}/lib/libcrypto.lib")
  include_directories(${OPENSSL_ROOT_DIR}/include)
  link_directories(${OPENSSL_ROOT_DIR}/lib)
endif()

find_package(Qt6 COMPONENTS Core Gui Widgets Network REQUIRED)


 if(WIN32)
  set(QT_TLS_PLUGIN_DIR "${_qt6Core_install_prefix}/plugins/tls")
  install(DIRECTORY "${QT_TLS_PLUGIN_DIR}" DESTINATION "${CMAKE_INSTALL_PREFIX}/plugins")
endif()

qt_add_resources(RESOURCES resources.qrc)
set(APP_ICON "${CMAKE_CURRENT_SOURCE_DIR}/resources.rc")
add_executable(${PROJECT_NAME}
   main.cpp
   mainwindow.cpp
   settingswindow.cpp
        settingswindow.h
          ChatModel.cpp ChatModel.h
        ChatDelegate.cpp ChatDelegate.h
        ${RESOURCES} ${APP_ICON})

target_link_libraries(${PROJECT_NAME} Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Network  ${OPENSSL_LIBRARIES})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "C:/Qt/6.5.1/mingw_64/bin/Qt6Core.dll"
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "C:/Qt/6.5.1/mingw_64/bin/Qt6Gui.dll"
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "C:/Qt/6.5.1/mingw_64/bin/Qt6Widgets.dll"
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "C:/Qt/6.5.1/mingw_64/bin/Qt6Network.dll"
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "C:/Qt/Tools/OpenSSL/Win_x64/bin/libssl-1_1-x64.dll"
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "C:/Qt/Tools/OpenSSL/Win_x64/bin/libcrypto-1_1-x64.dll"
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

