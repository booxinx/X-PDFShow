#ifndef VOLUMESLIDER_H
#define VOLUMESLIDER_H

#include <QSlider>
#include <QObject>
#include <QMediaPlayer>
#include <QMouseEvent>
#include <QDebug>
class VolumeSlider : public QSlider
{
public:
    VolumeSlider(QWidget *parent = nullptr);
    ~VolumeSlider();
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
private:
    int currentX;
    double per;
};

#endif // VOLUMESLIDER_H
