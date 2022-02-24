#ifndef VOLUME_H
#define VOLUME_H

#include <QSlider>
#include <QObject>

class Volume : public QSlider
{
public:
    Volume(QWidget *parent=nullptr);
    ~Volume();
};

#endif // VOLUME_H
