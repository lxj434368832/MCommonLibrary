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
    QString qstrFilePath = QFileDialog::getOpenFileName(nullptr, QString::fromLocal8Bit("请选择要打开文件"));
    qstrFilePath = QDir::toNativeSeparators(qstrFilePath);
    ReadWriteExcel excel;
    excel.OpenExcel(qstrFilePath);
    excel.ReadRowData(2);

    /*
    QAxObject* excel = new QAxObject("Excel.Application");
    excel->setProperty("Visible", false);
    QAxObject* workbooks = excel->querySubObject("WorkBooks");
    workbooks->dynamicCall("Open (const QString&)", QString("c:\\副本1.xls")); //filename
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
    //QAxObject是Qt提供的包装COM组件的类，通过COM操作Excel需要使用QAxObject类，使用此类还需要在pro文件增加“QT += axcontainer”

   // 2. 与excel com连接的方法
   // #include <QAxObject> //注意包含此头文件，同时在pro增加QT+= axcontainer
    QAxObject *excel = new QAxObject(this);//建立excel操作对象
    excel->setControl("Excel.Application");//连接Excel控件
    excel->dynamicCall("SetVisible (bool Visible)","false");//设置为不显示窗体
    excel->setProperty("DisplayAlerts", false);//不显示任何警告信息，如关闭时的是否保存提示
    excel->dynamicCall("Quit(void)");//关闭excel程序,操作完成后记着关闭，由于是隐藏的看不到，不关闭进程会有很多excel.exe
    //关闭excel程序之前记着先关闭.xls文件，具体见后续内容:workbook->dynamicCall("Close(Boolean)", false); //关闭文件

    /*3. Excel基本操作
    下面只介绍主要读写操作的方法，若需要修改单元格格式等操作，请看“Excel VBA参考手册.chm”或者其他类似资料
    手册分享地址： 链接: https://pan.baidu.com/s/1pLvC5nl 密码: dnik*/

    //3.1. excel文件操作
    //获取当前工作簿的集合
    QAxObject *workbooks = excel->querySubObject("WorkBooks");//获取工作簿(excel文件)集合

    //新建一个工作簿
    workbooks->dynamicCall("Add");//新建一个工作簿
    QAxObject *workbook = excel->querySubObject("ActiveWorkBook");//获取当前工作簿

    //打开一个已有的工作簿
    QString excel_file_path = "XXXX.xlsx";
    workbooks->dynamicCall("Open(const QString&)", excel_file_path);
    workbook = excel->querySubObject("ActiveWorkBook");
    //或？
   workbook = workbooks->querySubObject("Open(const QString&)", excel_file_path);   //需要当前文件被打开过才行

    //保存工作簿
    workbook->dynamicCall("Save()"); //保存文件
    workbook->dynamicCall("Close(Boolean)", false); //关闭文件

    // 另存为工作簿
    //用QDir::toNativeSeparators, 将路径中的"/"转换为"\",否则无法保存,/只是qt中可以识别
    workbook->dynamicCall("SaveAs(const QString&)", QDir::toNativeSeparators(excel_file_path));
    workbook->dynamicCall("Close (Boolean)", false); //关闭文件


   // 3.2. sheet工作表操作
   // 下面的代码用到的workbook都是上面工作簿操作后得到的，也就是对某一个工作簿(excel文件)进行操作

    //获取所有工作表
    QAxObject *worksheets = workbook->querySubObject("Sheets");

    //根据序号获取某个工作表，序号顺序就是excel打开后下方的排序
    QAxObject *worksheet = worksheets->querySubObject("Item(int)",1);

    //获取表中的行数列数
    QAxObject* usedrange = worksheet->querySubObject("UsedRange");//sheet范围
    int intRowStart = usedrange->property("Row").toInt();//起始行数
    int intColStart = usedrange->property("Column").toInt(); //起始列数
    QAxObject *rows, *columns;
    rows = usedrange->querySubObject("Rows");//行
    columns = usedrange->querySubObject("Columns");//列
    int intRow = rows->property("Count").toInt();//行数
    int intCol = columns->property("Count").toInt();//列数

    //3.3. 内容操作
    //数据内容操作-获取单元格-基于坐标
    int i,j;
    QAxObject* cell = worksheet->querySubObject("Cells(int, int)", i, j);

    //数据内容操作-获取单元格-基于行列名称
    cell = worksheet->querySubObject("Range(QVariant, QVariant)", "A1");
    //数据内容操作-读单元格内容
    QVariant cell_value = cell->property("Value");

    //数据内容操作-写单元格内容
    cell->setProperty("Value", "内容");

    //4. 其他

    //4.1. 大数据量读取
    //读取所有单元格内容-数据量大请使用此方式，只需要进行一次操作即可读取所有内容到内容，避免重复每个单元格进行QAxObject操作
    QVariant var;
    QAxObject*axusedRange = worksheet->querySubObject("UsedRange");//获取用户区域范围
    if(NULL == axusedRange || axusedRange->isNull()) {
//     return var;
    }
    var = axusedRange->dynamicCall("Value");//读取区域内所有值
    delete axusedRange;
    //此时结果以QVariant保存，需要自行转换成QList<QList<QVariant>>

    QList<QList<QVariant>> excel_list;  //每个QList<QVariant>就是一行数据
//    QList<QVariant> rows = var.toList();
//    for(auto row : rows)
//    {
//        excel_list.append(row.toList());
//    }

   // 4.2. 大数据量写入

    //大数据以QList<QList<QVariant>>存储，与读取类似，此处需要先指定区域范围
    QAxObject *user_range = worksheet->querySubObject("Range(const QString&)","A1:D100");//范围
    //然后写入数据
    user_range->setProperty("Value", var);//需要将QList<QList<QVarient>>转换为QVarient

    //4.3. 范例：一个完整的打开-读取-关闭的操作
    /*QString excel_file_path = QDir::currentPath()+"/a.xlsx";
    excel_file_path = QDir::toNativeSeparators(excel_file_path);
    QAxObject *excel = new QAxObject(this);//建立excel操作对象
    excel->setControl("Excel.Application");//连接Excel控件
    excel->setProperty("Visible", true);//显示窗体看效果
    excel->setProperty("DisplayAlerts", true);//显示警告看效果
    QAxObject *workbooks = excel->querySubObject("WorkBooks");//获取工作簿(excel文件)集合
    workbooks->dynamicCall("Open(const QString&)", excel_file_path);
    QAxObject *workbook = excel->querySubObject("ActiveWorkBook");
    QAxObject *worksheet = workbook->querySubObject("WorkSheets(int)",1);
    QAxObject *usedRange = worksheet->querySubObject("UsedRange");
    QVariant var = usedRange->dynamicCall("Value");//这里是所有的内容
    workbook->dynamicCall( "Close(Boolean)", false );
    excel->dynamicCall( "Quit(void)" );
    delete excel;*/
   /* 注意：
    1、此范例为了看到效果吧窗口和警告设置为了显示，请自行改为false
    2、excel所有操作均是通过QAxObject 进行COM组件的操作，包括打开文件也是，所以路径必须传递完整路径，不能传递相对路径
    3、QAxObject自身会维护new出的空间，直接delete第一个QAxObject 即可
    4、所有内容保存在var的变量中
    5、上述操作均为判断返回值，若文件不存在后续内容会报错*/

    //5线程中的解决
    /*#include <windows.h>
   // 在线程开始的时候初始化COM库：
    HRESULT r = OleInitialize(0);
    if (r != S_OK && r != S_FALSE)
    {
        qWarning("Qt:初始化Ole失败（error %x）",(unsigned int)r);
    }
    //使用结束后需要释放掉：
    OleUninitialize();*/
}

