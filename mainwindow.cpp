#include "mainwindow.h"

MainWindow::MainWindow(){
    read = new Reader;
    connect(this,&MainWindow::openSignal,read,&Reader::loadData);
    tman = new TableManager;
    connect(read,&Reader::sendData,tman,&TableManager::loadData);

    QMenuBar *menubar = new QMenuBar;
    setMenuBar(menubar);

    QMenu *file = new QMenu("Файл");

    QAction *open = new QAction("Открыть");
    connect(open,&QAction::triggered,this,&MainWindow::readStart);
    file->addAction(open);

    menubar->addMenu(file);

    setCentralWidget(tman);
}

void MainWindow::readStart(){
    QString nameFile = QFileDialog::getOpenFileName(nullptr,"Выбор файла таблицы:",QDir::homePath(),"All Files (*.*)");
    if(!nameFile.isEmpty()){
        read->loadData(nameFile);
    }
}
