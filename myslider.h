#ifndef MYSLIDER_H
#define MYSLIDER_H

#include <QSlider>
#include <QObject>
#include <QMouseEvent>
#include <QDebug>
#include <QMediaPlayer>
#include <QTimer>
class MySlider : public QSlider
{

public:
    MySlider(QWidget *parent = nullptr);
    ~MySlider();
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
private:
    int currentX;
    double per;
};

#endif // MYSLIDER_H
