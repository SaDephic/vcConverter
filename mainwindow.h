#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QAction>

#include "reader.h"
#include "tablemanager.h"

class MainWindow : public QMainWindow{
    Q_OBJECT
public: signals:
    void openSignal(QString file);
private:
    Reader *read = nullptr;
    TableManager *tman = nullptr;
public:
    MainWindow();
    ~MainWindow(){}
    void readStart();
};
#endif // MAINWINDOW_H
