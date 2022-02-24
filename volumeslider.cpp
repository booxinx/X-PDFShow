#include "volumeslider.h"
int volume_value;
bool v_mousemove;
VolumeSlider::VolumeSlider(QWidget *parent):QSlider(parent)
{

}
VolumeSlider::~VolumeSlider(){

}
void VolumeSlider::mousePressEvent(QMouseEvent *event){
    currentX = event->pos().x();
    per = currentX*1.0/this->width();
    volume_value = per*(this->maximum() - this->minimum())+this->minimum();
    if(volume_value>maximum())
            volume_value=maximum();
    if(volume_value<minimum())
            volume_value=minimum();
    this->setValue(volume_value);
    qDebug()<<volume_value;
    v_mousemove = true;
    QSlider::mousePressEvent(event);
}
void VolumeSlider::mouseReleaseEvent(QMouseEvent *event){
    currentX = event->pos().x();
    per = currentX*1.0/this->width();
    volume_value = per*(this->maximum() - this->minimum())+this->minimum();
    if(volume_value>maximum())
            volume_value=maximum();
    if(volume_value<minimum())
            volume_value=minimum();
    v_mousemove = true;
    QSlider::mouseReleaseEvent(event);
}
void VolumeSlider::mouseMoveEvent(QMouseEvent *event){
    currentX = event->pos().x();
    per = currentX*1.0/this->width();
    volume_value = per*(this->maximum() - this->minimum())+this->minimum();
    if(volume_value>maximum())
            volume_value=maximum();
    if(volume_value<minimum())
            volume_value=minimum();
    if(v_mousemove){
        this->setValue(volume_value);
        v_mousemove = false;
    }
    v_mousemove = false;
    QSlider::mouseMoveEvent(event);
}
