#ifndef READER_H
#define READER_H

#include <QDebug>
#include <QThread>
#include <QFile>
#include <QFileDialog>
#include <QAxObject>
#include <QMetaType>

struct dataExel{
    QString FirstName;
    QString SecondName;
    QString ThirdName;
    QString PhoneNumber;
    void formatNumber(){
        if(PhoneNumber[0] == '7')
            PhoneNumber.push_front('+');
    }
};
Q_DECLARE_METATYPE(dataExel);

class Reader : public QObject{
    Q_OBJECT
public: signals:
    void sendData(QVector<dataExel> send);
private:
    QAxObject *SetSheet;
    int countRows;
    int countCols;
    QVector<dataExel> data;
public:
    Reader();
    ~Reader(){}
public slots:
    void loadData(QString file);
};

#endif // READER_H
