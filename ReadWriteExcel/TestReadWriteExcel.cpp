#include "TestReadWriteExcel.h"
#include "ui_ReadWriteExcel.h"
#include "ReadWriteExcel.h"
#include <QAxObject>
#include <QDir>
#include <QFileDialog>

TestReadWriteExcel::TestReadWriteExcel(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ReadWriteExcel)
{
    ui->setupUi(this);
}

TestReadWriteExcel::~TestReadWriteExcel()
{
    delete ui;
}

void TestReadWriteExcel::on_btnReadExcel_clicked()
{
    QString qstrFilePath = QFileDialog::getOpenFileName(nullptr, QString::fromLocal8Bit("��ѡ��Ҫ���ļ�"));
    qstrFilePath = QDir::toNativeSeparators(qstrFilePath);
    ReadWriteExcel excel;
    excel.OpenExcel(qstrFilePath);
    excel.ReadRowData(2);

    /*
    QAxObject* excel = new QAxObject("Excel.Application");
    excel->setProperty("Visible", false);
    QAxObject* workbooks = excel->querySubObject("WorkBooks");
    workbooks->dynamicCall("Open (const QString&)", QString("c:\\����1.xls")); //filename
    QAxObject* workbook = excel->querySubObject("ActiveWorkBook");
    QAxObject* worksheets = workbook->querySubObject("WorkSheets");
    QAxObject* worksheet = workbook->querySubObject("Worksheets(int)", 1); //worksheet number
    QAxObject* usedrange = worksheet->querySubObject("UsedRange");
    QAxObject* rows = usedrange->querySubObject("Rows");
    QAxObject* columns = usedrange->querySubObject("Columns");
    int intRowStart = usedrange->property("Row").toInt();
    int intColStart = usedrange->property("Column").toInt();
    int intCols = columns->property("Count").toInt();
    int intRows = rows->property("Count").toInt();
    QAxObject * cell;
    for (int i = intRowStart; i < intRowStart + intRows; i++)
    {
        for (int j = intColStart; j < intColStart + intCols; j++)
        {
            cell = excel->querySubObject("Cells(Int, Int)", i, j );
            QVariant cellValue = cell->dynamicCall("value");

            cout << "Content " << cellValue.toString().toStdString() << endl; //value of the cell
        }
    }
    excel->setProperty("DisplayAlerts", 0);
    workbook->dynamicCall("Save(void)");
    workbook->dynamicCall("Close (Boolean)", false);
    excel->setProperty("DisplayAlerts",1);

    delete excel;*/
}

void TestReadWriteExcel::TutorialExcel()
{
    //1. Qt-QAxObject
    //QAxObject��Qt�ṩ�İ�װCOM������࣬ͨ��COM����Excel��Ҫʹ��QAxObject�࣬ʹ�ô��໹��Ҫ��pro�ļ����ӡ�QT += axcontainer��

   // 2. ��excel com���ӵķ���
   // #include <QAxObject> //ע�������ͷ�ļ���ͬʱ��pro����QT+= axcontainer
    QAxObject *excel = new QAxObject(this);//����excel��������
    excel->setControl("Excel.Application");//����Excel�ؼ�
    excel->dynamicCall("SetVisible (bool Visible)","false");//����Ϊ����ʾ����
    excel->setProperty("DisplayAlerts", false);//����ʾ�κξ�����Ϣ����ر�ʱ���Ƿ񱣴���ʾ
    excel->dynamicCall("Quit(void)");//�ر�excel����,������ɺ���Źرգ����������صĿ����������رս��̻��кܶ�excel.exe
    //�ر�excel����֮ǰ�����ȹر�.xls�ļ����������������:workbook->dynamicCall("Close(Boolean)", false); //�ر��ļ�

    /*3. Excel��������
    ����ֻ������Ҫ��д�����ķ���������Ҫ�޸ĵ�Ԫ���ʽ�Ȳ������뿴��Excel VBA�ο��ֲ�.chm������������������
    �ֲ�����ַ�� ����: https://pan.baidu.com/s/1pLvC5nl ����: dnik*/

    //3.1. excel�ļ�����
    //��ȡ��ǰ�������ļ���
    QAxObject *workbooks = excel->querySubObject("WorkBooks");//��ȡ������(excel�ļ�)����

    //�½�һ��������
    workbooks->dynamicCall("Add");//�½�һ��������
    QAxObject *workbook = excel->querySubObject("ActiveWorkBook");//��ȡ��ǰ������

    //��һ�����еĹ�����
    QString excel_file_path = "XXXX.xlsx";
    workbooks->dynamicCall("Open(const QString&)", excel_file_path);
    workbook = excel->querySubObject("ActiveWorkBook");
    //��
   workbook = workbooks->querySubObject("Open(const QString&)", excel_file_path);   //��Ҫ��ǰ�ļ����򿪹�����

    //���湤����
    workbook->dynamicCall("Save()"); //�����ļ�
    workbook->dynamicCall("Close(Boolean)", false); //�ر��ļ�

    // ���Ϊ������
    //��QDir::toNativeSeparators, ��·���е�"/"ת��Ϊ"\",�����޷�����,/ֻ��qt�п���ʶ��
    workbook->dynamicCall("SaveAs(const QString&)", QDir::toNativeSeparators(excel_file_path));
    workbook->dynamicCall("Close (Boolean)", false); //�ر��ļ�


   // 3.2. sheet���������
   // ����Ĵ����õ���workbook�������湤����������õ��ģ�Ҳ���Ƕ�ĳһ��������(excel�ļ�)���в���

    //��ȡ���й�����
    QAxObject *worksheets = workbook->querySubObject("Sheets");

    //������Ż�ȡĳ�����������˳�����excel�򿪺��·�������
    QAxObject *worksheet = worksheets->querySubObject("Item(int)",1);

    //��ȡ���е���������
    QAxObject* usedrange = worksheet->querySubObject("UsedRange");//sheet��Χ
    int intRowStart = usedrange->property("Row").toInt();//��ʼ����
    int intColStart = usedrange->property("Column").toInt(); //��ʼ����
    QAxObject *rows, *columns;
    rows = usedrange->querySubObject("Rows");//��
    columns = usedrange->querySubObject("Columns");//��
    int intRow = rows->property("Count").toInt();//����
    int intCol = columns->property("Count").toInt();//����

    //3.3. ���ݲ���
    //�������ݲ���-��ȡ��Ԫ��-��������
    int i,j;
    QAxObject* cell = worksheet->querySubObject("Cells(int, int)", i, j);

    //�������ݲ���-��ȡ��Ԫ��-������������
    cell = worksheet->querySubObject("Range(QVariant, QVariant)", "A1");
    //�������ݲ���-����Ԫ������
    QVariant cell_value = cell->property("Value");

    //�������ݲ���-д��Ԫ������
    cell->setProperty("Value", "����");

    //4. ����

    //4.1. ����������ȡ
    //��ȡ���е�Ԫ������-����������ʹ�ô˷�ʽ��ֻ��Ҫ����һ�β������ɶ�ȡ�������ݵ����ݣ������ظ�ÿ����Ԫ�����QAxObject����
    QVariant var;
    QAxObject*axusedRange = worksheet->querySubObject("UsedRange");//��ȡ�û�����Χ
    if(NULL == axusedRange || axusedRange->isNull()) {
//     return var;
    }
    var = axusedRange->dynamicCall("Value");//��ȡ����������ֵ
    delete axusedRange;
    //��ʱ�����QVariant���棬��Ҫ����ת����QList<QList<QVariant>>

    QList<QList<QVariant>> excel_list;  //ÿ��QList<QVariant>����һ������
//    QList<QVariant> rows = var.toList();
//    for(auto row : rows)
//    {
//        excel_list.append(row.toList());
//    }

   // 4.2. ��������д��

    //��������QList<QList<QVariant>>�洢�����ȡ���ƣ��˴���Ҫ��ָ������Χ
    QAxObject *user_range = worksheet->querySubObject("Range(const QString&)","A1:D100");//��Χ
    //Ȼ��д������
    user_range->setProperty("Value", var);//��Ҫ��QList<QList<QVarient>>ת��ΪQVarient

    //4.3. ������һ�������Ĵ�-��ȡ-�رյĲ���
    /*QString excel_file_path = QDir::currentPath()+"/a.xlsx";
    excel_file_path = QDir::toNativeSeparators(excel_file_path);
    QAxObject *excel = new QAxObject(this);//����excel��������
    excel->setControl("Excel.Application");//����Excel�ؼ�
    excel->setProperty("Visible", true);//��ʾ���忴Ч��
    excel->setProperty("DisplayAlerts", true);//��ʾ���濴Ч��
    QAxObject *workbooks = excel->querySubObject("WorkBooks");//��ȡ������(excel�ļ�)����
    workbooks->dynamicCall("Open(const QString&)", excel_file_path);
    QAxObject *workbook = excel->querySubObject("ActiveWorkBook");
    QAxObject *worksheet = workbook->querySubObject("WorkSheets(int)",1);
    QAxObject *usedRange = worksheet->querySubObject("UsedRange");
    QVariant var = usedRange->dynamicCall("Value");//���������е�����
    workbook->dynamicCall( "Close(Boolean)", false );
    excel->dynamicCall( "Quit(void)" );
    delete excel;*/
   /* ע�⣺
    1���˷���Ϊ�˿���Ч���ɴ��ں;�������Ϊ����ʾ�������и�Ϊfalse
    2��excel���в�������ͨ��QAxObject ����COM����Ĳ������������ļ�Ҳ�ǣ�����·�����봫������·�������ܴ������·��
    3��QAxObject�����ά��new���Ŀռ䣬ֱ��delete��һ��QAxObject ����
    4���������ݱ�����var�ı�����
    5������������Ϊ�жϷ���ֵ�����ļ������ں������ݻᱨ��*/

    //5�߳��еĽ��
    /*#include <windows.h>
   // ���߳̿�ʼ��ʱ���ʼ��COM�⣺
    HRESULT r = OleInitialize(0);
    if (r != S_OK && r != S_FALSE)
    {
        qWarning("Qt:��ʼ��Oleʧ�ܣ�error %x��",(unsigned int)r);
    }
    //ʹ�ý�������Ҫ�ͷŵ���
    OleUninitialize();*/
}

