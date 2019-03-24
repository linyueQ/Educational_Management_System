#-------------------------------------------------
#
# Project created by QtCreator 2018-10-15T17:11:30
#
#-------------------------------------------------

QT       += core gui
QT+=sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Educational_Management_System_Final_Version
TEMPLATE = app


# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        admin.cpp \
    add_one.cpp \
    add_all.cpp \
    welcome.cpp \
    base.cpp \
    query_all.cpp \
    edit_user_dialog.cpp \
    update_progress_thread.cpp \
    import_data_thread.cpp \
    addallmitable.cpp \
    addonemitable.cpp \
    query_all_thread.cpp \
    read_user_csv_thread.cpp \
    login.cpp \
    add_course.cpp \
    add_schedule.cpp \
    make_query_gpa.cpp \
    query_delete_course.cpp \
    query_schedule.cpp \
    change_pw.cpp \
    querymi.cpp \
    edit_mi_dialog.cpp \
    query_allmi_thread.cpp \
    student_score.cpp \
    studentlist.cpp \
    teacher.cpp \
    teachersearch.cpp \
    inform.cpp \
    inform_table.cpp \
    informdetaildialog.cpp \
    query_allinform_thread.cpp \
    queryinform.cpp \
    student.cpp \
    student_schedule.cpp \
    student_query_score.cpp


HEADERS  += admin.h \
    add_one.h \
    add_all.h \
    welcome.h \
    base.h \
    query_all.h \
    edit_user_dialog.h \
    update_progress_thread.h \
    import_data_thread.h \
    addallmitable.h \
    addonemitable.h \
    query_all_thread.h \
    read_user_csv_thread.h \
    login.h \
    add_course.h \
    add_schedule.h \
    make_query_gpa.h \
    query_delete_course.h \
    query_schedule.h \
    change_pw.h \
    edit_mi_dialog.h \
    query_allmi_thread.h \
    querymi.h \
    student_score.h \
    studentlist.h \
    teacher.h \
    teachersearch.h \
    inform.h \
    inform_table.h \
    informdetaildialog.h \
    query_allinform_thread.h \
    queryinform.h \
    student.h \
    student_schedule.h \
    student_query_score.h

FORMS    += admin.ui \
    add_one.ui \
    add_all.ui \
    welcome.ui \
    query_all.ui \
    edit_user_dialog.ui \
    addallmitable.ui \
    addonemitable.ui \
    login.ui \
    add_course.ui \
    add_schedule.ui \
    make_query_gpa.ui \
    query_delete_course.ui \
    query_schedule.ui \
    change_pw.ui \
    edit_mi_dialog.ui \
    querymi.ui \
    student_score.ui \
    studentlist.ui \
    teacher.ui \
    teachersearch.ui \
    inform.ui \
    inform_table.ui \
    informdetaildialog.ui \
    queryinform.ui \
    student.ui \
    student_schedule.ui \
    student_query_score.ui
