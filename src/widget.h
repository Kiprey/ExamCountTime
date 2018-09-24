#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QFont>
#include <QDateTime>
#include <QTimer>
#include <QSettings>
#include <QScreen>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;

    QFont * Font;
    QLabel * Label;
    QTimer * Timer;

    int Days;
    int Hours;
    int Minutes;
    int Seconds;
    int MSeconds;

    int GeometryX;
    int GeometryY;

private slots:
    void RefreshLabelText();

};

#endif // WIDGET_H
