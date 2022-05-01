#ifndef TABLEMANAGER_H
#define TABLEMANAGER_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QTableWidget>
#include <QBuffer>

#include "reader.h"

class TableManager : public QWidget{
    Q_OBJECT
private:
    QTableWidget *left = nullptr;
    QTableWidget *right = nullptr;
    QVector<dataExel> data;
public:
    TableManager();
    ~TableManager(){}
public slots:
    void loadData(QVector<dataExel> send);
private:
    void compactData();
    void setDataInTable(QVector<dataExel> send);
    void uploadContacts();
    QByteArray getDataContacts();
    QVector<dataExel> getDataVC();
private slots:
    void addName();
    void removeName();
};

#endif // TABLEMANAGER_H
