#include "reader.h"

Reader::Reader(){
    QThread *thrd = new QThread;
    this->moveToThread(thrd);
    thrd->start();
}

void Reader::loadData(QString file){
    data.clear();
    /* получение доступа к файлу */
    QAxObject* excel = new QAxObject("Excel.Application", 0);
    QAxBase* workbooks = excel->querySubObject("Workbooks");
    QAxBase* workbook = workbooks->querySubObject( "Open(const QString&)",file);
    QAxObject* sheets = excel->querySubObject("Worksheets");
    //количество листов в книге
    int sheetsCount = sheets->dynamicCall("Count").toInt();
    //записываем название листов в массив
    QVector<QString> SheetNames;
    for (int i = 1; i <= sheetsCount; i++) {
        QAxObject* sheetsName = sheets->querySubObject("Item(const QVariant&)", QVariant(i));
        SheetNames<<sheetsName->dynamicCall("Name").toString().toLower();
    }
    //перебираем листы и заполняем необходимые массивы
    for (int i = 1; i <= sheetsCount; i++) {
        SetSheet = sheets->querySubObject( "Item(const QVariant&)", QVariant(i) );

        //получаем количество используемых строк
        QAxObject* usedRangeR = SetSheet->querySubObject("UsedRange");
        QAxObject* rows = usedRangeR->querySubObject("Rows");
        countRows = rows->property("Count").toInt();

        //получаем количество используемых столбцов
        QAxObject* usedRangeC = SetSheet->querySubObject("UsedRange");
        QAxObject* columns = usedRangeC->querySubObject("Columns");
        countCols = columns->property("Count").toInt();
    }
    /* чтение данных из таблицы */
    QAxObject* cell;
    for(int r=1; r<=countRows; r++){
        dataExel cur;
        cell = SetSheet->querySubObject("Cells(int,int)", r, 1);/* имена */
        auto split = cell->property("Value").toString().split(" ");
        cur.FirstName = split[0];
        cur.SecondName = split[1];
        cur.ThirdName = split[2];
        cur.PhoneNumber = SetSheet->querySubObject("Cells(int,int)", r, 2)->property("Value").toString();
        cur.formatNumber();
        data.push_back(cur);
    }
    /* закрытие потока чтения */
    workbook->dynamicCall("Close()");
    excel->dynamicCall("Quit()");
    /* отправка данных в таблицу управления */
    emit sendData(data);
}
