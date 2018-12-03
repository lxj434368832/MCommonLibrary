#include "TestGlobal.h"
#include "Test.h"
#include "ui_Test.h"
//#include "MCSVParser.h"
#include "QDebug"

Q_LOGGING_CATEGORY(TESTLOG, "Test")

#define TEST_FILE_NAME "TestExcel.CSV"

Test::Test(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Test)
{
    ui->setupUi(this);
}

Test::~Test()
{
    delete ui;
}

void Test::on_btnExport_clicked()
{
    QList<QStringList> listRows;
    QStringList listCols;
    for(int iRow = 1; iRow < 10000; iRow ++)
    {
        listCols.clear();
        for(int iCol = 1; iCol < 20; iCol ++)
        {
            listCols.append(QString::number(iRow * iCol));
        }
        listRows.push_back(listCols);
    }
//    writeToSheet(TEST_FILE_NAME, listRows);
    qCDebug(TESTLOG)<< "export finished!\n";
}

void Test::on_btnImport_clicked()
{
    QList<QStringList> listRows;
//    readFromSheet(TEST_FILE_NAME, listRows);

    int iRowCount = listRows.size();
    for(int i = 0; i < iRowCount; i++)
        qDebug()<<listRows.at(i)<< '\n';
}
