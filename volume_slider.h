#ifndef VOLUME_SLIDER_H
#define VOLUME_SLIDER_H
#include <QSlider>
#include <QMouseEvent>
#include <QDebug>
#include <QTimer>
#include <windows.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <Audioclient.h>
#include <stdio.h>
#include <QPushButton>
class Volume_Slider : public QSlider
{
    Q_OBJECT
public:
    Volume_Slider(QWidget *parent=nullptr,uint16_t handleLen = 0);
    ~Volume_Slider();
    virtual void SetSliderValue(int val,bool bNotify = false);
protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);

private:
    int32_t MousePositionToSliderVal(const QPoint & pos);
    uint16_t m_handleLen;
    QTimer *pTimerListenVolume;
private slots:
    bool setSysVolume(int level);
    int sysVolume();
    void pTimerListenVolume_timeover();
};

#endif // VOLUME_SLIDER_H
