#include "tablemanager.h"

TableManager::TableManager(){
    left = new QTableWidget;
    left->setSelectionBehavior(QAbstractItemView::SelectRows);
    left->setColumnCount(4);
    left->setHorizontalHeaderLabels({"Фамилия",
                                     "Имя",
                                     "Отчество",
                                     "Тел.номер"});
    right = new QTableWidget;
    right->setColumnCount(4);
    right->setSelectionBehavior(QAbstractItemView::SelectRows);
    right->setHorizontalHeaderLabels({"Фамилия",
                                      "Имя",
                                      "Отчество",
                                      "Тел.номер"});
    compactData();
}

void TableManager::loadData(QVector<dataExel> send){
    data = send;
    setDataInTable(send);
}

void TableManager::compactData(){
    QHBoxLayout *hlay = new QHBoxLayout(this);
    QGroupBox *box = new QGroupBox("Исходные:");
    QVBoxLayout *vlay = new QVBoxLayout(box);
    QPushButton *addRow = new QPushButton("Добавить");
    connect(addRow,&QPushButton::clicked,this,&TableManager::addName);
    vlay->addWidget(addRow);
    vlay->addWidget(left);
    hlay->addWidget(box);

    box = new QGroupBox("Для выгрузки");
    vlay = new QVBoxLayout(box);
    QPushButton *removeRow = new QPushButton("Удалить");
    connect(removeRow,&QPushButton::clicked,this,&TableManager::removeName);
    vlay->addWidget(removeRow);
    vlay->addWidget(right);
    QPushButton *exportData = new QPushButton("Сформировать");
    connect(exportData,&QPushButton::clicked,this,&TableManager::uploadContacts);
    vlay->addWidget(exportData);
    hlay->addWidget(box);
}

void TableManager::setDataInTable(QVector<dataExel> send){
    /* формирование данных для таблицы выбора */
    QTableWidgetItem *item = nullptr;
    for(int r=0; r<send.size(); r++){
        left->insertRow(r);
        item = new QTableWidgetItem(send[r].FirstName); left->setItem(r,0,item);
        item = new QTableWidgetItem(send[r].SecondName); left->setItem(r,1,item);
        item = new QTableWidgetItem(send[r].ThirdName); left->setItem(r,2,item);
        item = new QTableWidgetItem(send[r].PhoneNumber); left->setItem(r,3,item);
    }
    left->resizeColumnsToContents();
}

void TableManager::uploadContacts(){
    QString nameFile = QFileDialog::getSaveFileName(nullptr,"Сохранение контактов:",QDir::homePath(),"vCard (*.vcf);;All Files (*)");
    if(!nameFile.isEmpty()){
        QFile fo(nameFile);
        fo.open((QIODevice::WriteOnly)| QIODevice::Text);
        fo.write(getDataContacts());
        fo.close();
    }
}

QByteArray TableManager::getDataContacts(){
    QVector<dataExel> gDVC = getDataVC();

    QString result;
    for(int i=0; i<gDVC.size(); i++){
        result += "BEGIN:VCARD\n"
                  "VERSION:3.0\n"
                  "N:" + gDVC[i].FirstName + ";" + gDVC[i].SecondName + ";" + gDVC[i].ThirdName + ";;\n" +
                "FN:" + gDVC[i].SecondName + " " + gDVC[i].ThirdName + " " + gDVC[i].FirstName + "\n" +
                "TEL;type=CELL;type=VOICE;type=pref:" + gDVC[i].PhoneNumber + "\n" +
                "REV:2012-" + QVariant(rand()%12).toString() + "-" + QVariant(rand()%25).toString() +
                "T" + QVariant(rand()%24).toString() + ":" + QVariant(rand()%60).toString() + ":" + QVariant(rand()%60).toString() + "Z\n" +
                "END:VCARD\n";
    }

    return result.toUtf8();
}

QVector<dataExel> TableManager::getDataVC(){
    QVector<dataExel> c;
    for(int r=0; r<right->rowCount(); r++){
        dataExel data;
        data.FirstName = right->item(r,0)->text();
        data.SecondName = right->item(r,1)->text();
        data.ThirdName = right->item(r,2)->text();
        data.PhoneNumber = right->item(r,3)->text();
        c.append(data);
    }
    return c;
}

void TableManager::addName(){
    QVector<QVector<QString>> names;
    /* сохранение во временный массив */
    for(int i=0; i<left->selectionModel()->selectedRows(0).count(); i++){
        int currow = left->selectionModel()->selectedRows(0)[i].row();
        QVector<QString> row;
        row = {left->item(currow,0)->text(),
               left->item(currow,1)->text(),
               left->item(currow,2)->text(),
               left->item(currow,3)->text()};
        names.append(row);
    }
    /* добавление в таблицу справа */
    for(int i=0; i<names.size(); i++){
        /* проверка в таблице на наличие строки */
        int contain = 0;
        for(int r=0; r<right->rowCount(); r++){
            if(names[i][0] == right->item(r,0)->text())
                contain++;
            if(names[i][1] == right->item(r,1)->text())
                contain++;
            if(names[i][2] == right->item(r,2)->text())
                contain++;
            if(names[i][3] == right->item(r,3)->text())
                contain++;
        }
        if(contain==0){
            /* добавление строки в таблицу */
            QTableWidgetItem *item;
            right->insertRow(right->rowCount());
            item = new QTableWidgetItem(names[i][0]);
            right->setItem(right->rowCount()-1,0,item);
            item = new QTableWidgetItem(names[i][1]);
            right->setItem(right->rowCount()-1,1,item);
            item = new QTableWidgetItem(names[i][2]);
            right->setItem(right->rowCount()-1,2,item);
            item = new QTableWidgetItem(names[i][3]);
            right->setItem(right->rowCount()-1,3,item);
        }
    }
}

void TableManager::removeName(){
    while(right->selectionModel()->selectedRows(0).count()>0)
        for(int i=0; i<right->selectionModel()->selectedRows(0).count(); i++){
            right->removeRow(right->selectionModel()->selectedRows(0)[i].row());
        }
}


