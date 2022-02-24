#include "volume_slider.h"
extern QPushButton *volume;
extern QIcon icon_volume;
Volume_Slider::Volume_Slider(QWidget *parent, uint16_t handleLen):QSlider(parent),m_handleLen(handleLen)
{
    setValue(sysVolume());
    connect(this,SIGNAL(valueChanged(int)),this,SLOT(setSysVolume(int)));
    pTimerListenVolume = new QTimer(this);
    pTimerListenVolume->start(100);
    connect(pTimerListenVolume,SIGNAL(timeout()),this,SLOT(pTimerListenVolume_timeover()));
}
Volume_Slider::~Volume_Slider(){
}
void Volume_Slider::pTimerListenVolume_timeover(){

    delete pTimerListenVolume;
    pTimerListenVolume = new QTimer(this);
    pTimerListenVolume->start(100);
    connect(pTimerListenVolume,SIGNAL(timeout()),this,SLOT(pTimerListenVolume_timeover()));
    int nSysVolume = sysVolume();
    if(this->value()!=nSysVolume)
        setValue(nSysVolume);
}
bool Volume_Slider::setSysVolume(int level){

    HRESULT hr;
    IMMDeviceEnumerator* pDeviceEnumerator=0;
    IMMDevice* pDevice=0;
    IAudioEndpointVolume* pAudioEndpointVolume=0;
    IAudioClient* pAudioClient=0;
    try{
        hr = CoCreateInstance(__uuidof(MMDeviceEnumerator),NULL,CLSCTX_ALL,__uuidof(IMMDeviceEnumerator),(void**)&pDeviceEnumerator);
        if(FAILED(hr)) throw "CoCreateInstance";
        hr = pDeviceEnumerator->GetDefaultAudioEndpoint(eRender,eMultimedia,&pDevice);
        if(FAILED(hr)) throw "GetDefaultAudioEndpoint";
        hr = pDevice->Activate(__uuidof(IAudioEndpointVolume),CLSCTX_ALL,NULL,(void**)&pAudioEndpointVolume);
        if(FAILED(hr)) throw "pDevice->Active";
        hr = pDevice->Activate(__uuidof(IAudioClient),CLSCTX_ALL,NULL,(void**)&pAudioClient);
        if(FAILED(hr)) throw "pDevice->Active";

        if(level==-2){
            hr = pAudioEndpointVolume->SetMute(FALSE,NULL);
            if(FAILED(hr)) throw "SetMute";
        }
        else if(level==-1){
            hr = pAudioEndpointVolume->SetMute(TRUE,NULL);
            if(FAILED(hr)) throw "SetMute";
        }
        else{

            float fVolume;
            fVolume = level/100.0f;
            hr = pAudioEndpointVolume->SetMasterVolumeLevelScalar(fVolume,&GUID_NULL);
            if(FAILED(hr)) throw "SetMasterVolumeLevelScalar";

            pAudioClient->Release();
            pAudioEndpointVolume->Release();
            pDevice->Release();
            pDeviceEnumerator->Release();

            return true;
        }
    }
    catch(...){
        if(pAudioClient) pAudioClient->Release();
        if(pAudioEndpointVolume) pAudioEndpointVolume->Release();
        if(pDevice) pDevice->Release();
        if(pDeviceEnumerator) pDeviceEnumerator->Release();
        throw;
    }
    return false;
}
int Volume_Slider::sysVolume(){
    HRESULT hr;
    IMMDeviceEnumerator* pDeviceEnumerator=0;
    IMMDevice* pDevice=0;
    IAudioEndpointVolume* pAudioEndpointVolume=0;
    IAudioClient* pAudioClient=0;
    try{

        hr = CoCreateInstance(__uuidof(MMDeviceEnumerator),NULL,CLSCTX_ALL,__uuidof(IMMDeviceEnumerator),(void**)&pDeviceEnumerator);
        if(FAILED(hr)) throw "CoCreateInstance";
        hr = pDeviceEnumerator->GetDefaultAudioEndpoint(eRender,eMultimedia,&pDevice);
        if(FAILED(hr)) throw "GetDefaultAudioEndpoint";
        hr = pDevice->Activate(__uuidof(IAudioEndpointVolume),CLSCTX_ALL,NULL,(void**)&pAudioEndpointVolume);
        if(FAILED(hr)) throw "pDevice->Active";
        hr = pDevice->Activate(__uuidof(IAudioClient),CLSCTX_ALL,NULL,(void**)&pAudioClient);
        if(FAILED(hr)) throw "pDevice->Active";
        float fVolume;
        hr = pAudioEndpointVolume->GetMasterVolumeLevelScalar(&fVolume);
        if(FAILED(hr)) throw "SetMasterVolumeLevelScalar";

        pAudioClient->Release();
        pAudioEndpointVolume->Release();
        pDevice->Release();
        pDeviceEnumerator->Release();

        int  intVolume = fVolume*100;
        if(fVolume>100)
            fVolume =100;
        if(intVolume == 0){
            icon_volume.addFile(":/icons/Icons/volume-mute-regular-36.png");
            volume->setIcon(icon_volume);
            volume->setIconSize(QSize(24,24));
        }
        else{
            icon_volume.addFile(":/icons/Icons/volume-full-regular-36.png");
            volume->setIcon(icon_volume);
            volume->setIconSize(QSize(24,24));
        }
        return intVolume;
    }
    catch(...){
        if(pAudioClient) pAudioClient->Release();
        if(pAudioEndpointVolume) pAudioEndpointVolume->Release();
        if(pDevice) pDevice->Release();
        if(pDeviceEnumerator) pDeviceEnumerator->Release();
        throw;
    }
}
void Volume_Slider::SetSliderValue(int val, bool bNotify){

    setSliderPosition(val);
    if(bNotify)
        emit sliderMoved(val);
}
void Volume_Slider::mousePressEvent(QMouseEvent *event){

    if(event->button()!=Qt::LeftButton)
        return;
    setSliderDown(true);
}
static bool bIsMove = false;
void Volume_Slider::mouseMoveEvent(QMouseEvent *event){

    bIsMove = true;
    int32_t pos = MousePositionToSliderVal(event->pos());
    if(pos!=sliderPosition()){
        setSliderPosition(pos);
        emit sliderMoved(pos);
    }
}
void Volume_Slider::mouseReleaseEvent(QMouseEvent *event){
    setSliderDown(false);
    if(!isEnabled() || event->button() !=Qt::LeftButton){
        bIsMove = false;
        return;
    }
    if(bIsMove){
        QSlider::mouseReleaseEvent(event);
        bIsMove = false;
    }
    else{
        int32_t pos = MousePositionToSliderVal(event->pos());
        if(pos!=sliderPosition()){

            setSliderPosition(pos);
            emit sliderMoved(pos);

        }
    }

}

int32_t Volume_Slider::MousePositionToSliderVal(const QPoint &pos){
    //setSliderPosition(sysVolume());
    int32_t duration = maximum()-minimum();
    int32_t sliderPos = 0;
    qreal mousePos = 0;
    uint16_t handleLen = 0;
    if(Qt::Vertical == orientation()){
        handleLen = m_handleLen ? m_handleLen:width()/2.0;
        mousePos = (qreal)(pos.y()-handleLen/2);
        if(mousePos<0)
            mousePos = 0;
        else if(mousePos>height()-handleLen)
            mousePos = (qreal)(height()-handleLen);
        sliderPos = maximum()-duration*(mousePos/(height()-handleLen));
    }else
    {
        handleLen = m_handleLen ? m_handleLen:height()/2.0;
        mousePos = (qreal)(pos.x()-handleLen/2);
        if(mousePos<0)
            mousePos=0;
        else if(mousePos>width() - handleLen)
            mousePos = (qreal)(width() - handleLen);
        sliderPos = minimum()+duration*(mousePos/(width()-handleLen));
    }
    return sliderPos;

}
