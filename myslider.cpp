#include "myslider.h"
int slider_value;
extern QMediaPlayer *player;
extern bool judgeenterdelaytime;
extern qint64 playtimer;
extern bool judgeplayover;
extern QTimer *playtime_timer;
bool m_isMoveing = false;
bool m_isPress = false;
MySlider::MySlider(QWidget *parent):QSlider(parent)
{
}
MySlider::~MySlider(){
}
void MySlider::mousePressEvent(QMouseEvent *event){

    currentX = event->pos().x();
    per = currentX*1.0/this->width();
    slider_value = per*(this->maximum() - this->minimum())+this->minimum();
    if(slider_value>maximum())
            slider_value=maximum();
    if(slider_value<minimum())
            slider_value=minimum();
    m_isPress = true;
    if(judgeenterdelaytime){
        player->setPosition(slider_value);
        this->setValue(slider_value);
        playtime_timer->start(playtimer-slider_value);
    }
    QSlider::mousePressEvent(event);
}
void MySlider::mouseMoveEvent(QMouseEvent *event){

    currentX = event->pos().x();
    per = currentX*1.0/this->width();
    slider_value = per*(this->maximum()-this->minimum())+minimum();
    if(slider_value>maximum())
            slider_value=maximum();
    if(slider_value<minimum())
            slider_value=minimum();
    if(m_isPress && judgeenterdelaytime){
        m_isMoveing = true;
        playtime_timer->start(playtimer-slider_value);
    }

    QSlider::mouseMoveEvent(event);
}
void MySlider::mouseReleaseEvent(QMouseEvent *event){
    currentX = event->pos().x();
    per = currentX*1.0/this->width();
    slider_value = per*(this->maximum()-this->minimum())+minimum();
    if(slider_value>maximum())
            slider_value=maximum();
    if(slider_value<minimum())
            slider_value=minimum();

    if(m_isMoveing){
        this->setValue(slider_value);
        playtime_timer->start(playtimer-slider_value);
        m_isMoveing = false;
        player->setPosition(slider_value);

    }
    m_isPress = false;
    QSlider::mouseReleaseEvent(event);
}

