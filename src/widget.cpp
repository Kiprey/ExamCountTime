#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    //读取设置
    QSettings Settings("CountTime.ini", QSettings::IniFormat);

    //读取有关字体设置
    QString FontName = Settings.value("FontName", QVariant("Arial")).toString();
    Settings.setValue("FontName", QVariant(FontName));
    int FontSize = Settings.value("FontSize", QVariant(30)).toInt();
    Settings.setValue("FontSize", QVariant(FontSize));

    //读取字体颜色
    int RedValue = Settings.value("RedValue", QVariant(0)).toInt();
    Settings.setValue("RedValue", QVariant(RedValue));
    int GreenValue = Settings.value("GreenValue", QVariant(0)).toInt();
    Settings.setValue("GreenValue", QVariant(GreenValue));
    int BlueValue = Settings.value("BlueValue", QVariant(0)).toInt();
    Settings.setValue("BlueValue", QVariant(BlueValue));
    int AlphaValue = Settings.value("AlphaValue", QVariant(255)).toInt();
    Settings.setValue("AlphaValue", QVariant(AlphaValue));

    //设置相关字体
    Font = new QFont();
    Font->setFamily(FontName);
    Font->setPixelSize(FontSize);

    //设置标签
    Label = new QLabel();
    Label->setFont(*Font);
    Label->setAlignment(Qt::AlignCenter);

    //给字体设置颜色
    QPalette Palette;
    Palette.setColor(QPalette::WindowText, QColor(RedValue, GreenValue, BlueValue,  AlphaValue));
    Label->setPalette(Palette);

    //设置窗口置底
    this->setWindowFlags(Qt::WindowStaysOnBottomHint);
    //设置隐藏任务栏
    this->setWindowFlags(Qt::Tool | Qt::X11BypassWindowManagerHint);
    //设置窗口透明
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setWindowFlag(Qt::FramelessWindowHint);

    //设置标签父类
    Label->setParent(this);

    //设置时间
    QDateTime EndTime = QDateTime(QDate(2019, 6, 7), QTime(9, 0, 0));
    QDateTime StartTime = QDateTime::currentDateTime();
    qint64 TotalMSeconds  = StartTime.msecsTo(EndTime);

    Days = int(TotalMSeconds / (24 * 60 * 60 * 1000));
    Hours = (TotalMSeconds % (24 * 60 * 60 * 1000)) / (60 * 60 * 1000);
    Minutes = TotalMSeconds % (60 * 60 * 1000) / (60 * 1000);
    Seconds = TotalMSeconds % (60 * 1000) / 1000;
    MSeconds = TotalMSeconds % 1000;

    //先将窗口移出屏幕
    this->move(-1000, -1000);


    //读取窗口位置
    GeometryX = -1;
    GeometryY = -1;
    RefreshLabelText();
    QRect ScreenGeometry = QGuiApplication::screens().first()->availableGeometry();
    QRect WidgetGeometry = this->geometry();

    GeometryX = Settings.value("GeometryX", QVariant(ScreenGeometry.width() - WidgetGeometry.width() - 50)).toInt();
    Settings.setValue("GeometryX", QVariant(GeometryX));

    GeometryY = Settings.value("GeometryY", QVariant(50)).toInt();
    Settings.setValue("GeometryY", QVariant(GeometryY));



    //创建并设置毫秒级精度的定时器
    Timer = new QTimer();
    Timer->setTimerType(Qt::PreciseTimer);
    this->connect(Timer, SIGNAL(timeout()), this, SLOT(RefreshLabelText()));
    Timer->start(10);
}

Widget::~Widget()
{
    delete ui; 
    delete Font;
    delete Label;
    delete Timer;
}

void Widget::RefreshLabelText()
{
    QString String = "高考倒计时\n%1d %2h %3m %4s %5";
    String = String.arg(Days).arg(Hours).arg(Minutes).arg(Seconds).arg(MSeconds / 10);
    Label->setText(String);
    //调整各组件大小
    Label->adjustSize();
    this->adjustSize();


    if ((MSeconds -= 10) < 0)
    {
        MSeconds += 1000;

        //每秒检测窗口是否超出屏幕
        QRect ScreenGeometry = QGuiApplication::screens().first()->availableGeometry();
        QRect WidgetGeometry = this->geometry();
        int TempGeometryX;
        int TempGeometryY;
        if (GeometryX < 0 || GeometryX > ScreenGeometry.width() - WidgetGeometry.width())
            TempGeometryX = ScreenGeometry.width() - WidgetGeometry.width() - 50;
        else
            TempGeometryX = GeometryX;

        if (GeometryY < 0 || GeometryY > ScreenGeometry.height())
            TempGeometryY = 50;
        else
            TempGeometryY = GeometryY;

        //设置窗口坐标
        this->move(TempGeometryX, TempGeometryY);

        if (--Seconds < 0)
        {
            Seconds += 60;
            if (--Minutes < 0)
            {
                Minutes += 60;
                if (--Hours < 0)
                {
                    Hours += 24;
                    if (--Days < 0)
                    {
                        Label->setText("时辰到！！！\n");
                        return;
                    }
                }
            }
        }
    }

}

