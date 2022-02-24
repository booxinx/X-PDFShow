#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QResizeEvent>
#include <QDockWidget>
#include <QScrollArea>
#include <QToolBar>
#include <QPointF>
#include <QPainter>
#include <QFont>
#include <QFontDatabase>
#include <QScreen>
#include <QApplication>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QDebug>
#include <QPalette>
#include <QRgb>
#include <QLabel>
//#include "mupdf/fitz.h"
//#include "mupdf/pdf.h"
//#include "mupdf/mupdf-qt.h"
#include <QImage>
#include <QPixmap>
#include <QLineF>
#include <QHeaderView>
#include <QScrollBar>
#include <QStringList>
#include <QDir>
#include <QFileInfoList>
#include <QString>
#include <QFileInfo>
#include <string>
#include <QByteArray>
#include <QtWebEngineWidgets/QWebEngineView>
#include <QWebEngineView>
#include <QtMultimedia/QMediaPlayer>
#include <QLineEdit>
#include <QStackedWidget>
//#include <QHostAddress>
//#include <QNetworkInterface>
#include <QIcon>
#include <QMovie>
#include <QTimer>
#include <QApplication>
#include <QDesktopWidget>
#include <QBitmap>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QSlider>
#include <QDesktopServices>
#include "myslider.h"
#include <QProcess>
#include <Windows.h>
#include <shellapi.h>
#include "volume_slider.h"
#include <QMessageBox>
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    bool event(QEvent *event);
public slots:
    void resizeEvent(QResizeEvent *event);
    void mathematics_clicked();
    void computer_science_clicked();
    void english_clicked();
    void music_clicked();
    void pdfshow();
    void updatepage();
    void onExpanded(const QModelIndex& index);
    QFileInfoList allfile(QTreeWidgetItem *root,QString path);
    void OnlineTreeViewDoubleClick(QTreeWidgetItem *item,int count);
    void mathwidget_showfunc();
    void computer_science_showfunc();
    void english_showfunc();
    void music_showfunc();
    void webpdfshow();
    void obtainip();
    void mediaplay();
    void search();
    void obtaintextsearch();
    void audioclickplay();
    void audioclickpause();
    void flushfilefunc();
    void removeitem(QTreeWidgetItem *item);
    void funcfinished();
    void windowflagsfunc();
    void mouseDoubleClickEvent(QMouseEvent *event);
    void closure_clicked();
    void minimize_clicked();
    void zoomout_clicked();
    void zoomin_clicked();
    void get_size();
    void playwindow();
    void playshow();
    void Playhide();
    void playhide_hide();
    void clickednext();
    void clickedprevious();
    void getduration(qint64 playtime);
    void playtime_timerfunc();
    void delayplaytime();
    void clickednextdelay();
    void clickedpreviousdelay();
    void ValueChange();
    void ValueChangetimeover();
    void clickedvolume();
    void volume_adjustment_delay();
    void continous_clicked_volume_mute();
    void clickedlock_openbutton();
    void clickedlock_button();
    void clickedgithub_button();
    bool WhetherProcessRunning(QString &processname);
    void Volume();
    void click_lock_playwidget_open();
    void click_lock_playwidget();
    void click_back_startlocation();
private slots:
    void onstateChanged(QMediaPlayer::State state);
private:
    QPushButton *mathematics,
                *computer_science,
                *english,
                *music,
                *mediaclickplay_button,
                *mediaclickpause_button,
                *flushfile,
                *minimize,
                *zoomout,
                *zoomin,
                *closure,
                *play,
                *pause,
                *next,
                *previous,
                *lock,
                *lock_open,
                *github,
                *lock_playwidget_open,
                *lock_playwidget,
                *back_startlocation;

    QFont font;
    QDockWidget *MathWidget,
                *AudioWidget,
                *infshowWidget,
                *CsWidget,
                *EngWidget,
                *MusicWidget;
    QTreeWidget *computer_science_tree,
                *mathematics_tree,
                *english_tree,
                *music_tree;
    QTreeWidgetItem *Composition_principle,
                    *Data_structure,
                    *Computer_network,
                    *Operating_system,
                    *Advanced_mathematics,
                    *Linear_algebra,
                    *Probability_theory,
                    *indefinite_integral,
                    *Computer_ebook,
                    *music_audio,
                    *english_listening,
                    *Self_Added_Files_Math,
                    *Self_Added_Files_Computer,
                    *Self_Added_Files_English,
                    *Self_Added_Files_Music;
    QLabel *showpdflabel,
           *infshowWidget_label,
           *loading_label,
           *musictext,
           *Title;
//    fz_context *ctx;
//    fz_document *doc;
//    fz_pixmap *pix;
    QToolBar *pToolBar,
             *tToolBar;
    QScrollArea *scrollArea_pdf_w;
    QPixmap newpdf;
    QString global_strpath,
            pathtopdfjs,
            file,
            website,
            rootpath_Advanced_mathematics,
            rootpath_music_audio,
            rootpath_english_listening,
            rootpath_Computer_ebook,
            rootpath_Linear_algebra,
            rootpath_Probability_theory,
            root_Computer_network,
            root_Operating_system,
            root_Data_structure,
            root_Composition_principle,
            rootpath_Self_Added_Files_Math,
            rootpath_Self_Added_Files_Computer,
            rootpath_Self_Added_Files_English,
            rootpath_Self_Added_Files_Music;
    QStringList mp3file;
    QImage newimage;
    QWebEngineView *pdfinfo;
    QLineEdit *websearch;
    QStackedWidget *audioplaycont,
                   *playwidget,
                   *volume_adjustment,
                   *lock_widget;
    QIcon icon_play,
          icon_pause,
          icon_flush,
          icon_loader,
          icon_next,
          icon_previous,
          icon_lock,
          icon_lock_open,
          icon_github,
          icon_lock_playwidget_open,
          icon_lock_playwidget,
          icon_back_startlocation;
    QMovie *movie;
    QTimer *time,
           *sizetimer,
           *mousepresstimer,
           *playshowtimer,
           *playtransitiontimer,
           *playhidetimer,
           *hidewidgettimer,
           *delayplaytimer,
           *coiledclicked,
           *valuechangetime,
           *volume_adjustment_delaytimer,
           *continuous_clicked_volume_mute_timer;
    QPoint m_point;
    float screen_x,
          screen_y,
          scale,
          zoom,
          rotate;
    int screen_height,
        screen_width,
        global_mp3totalid,
        global_strpath_single_quoteid,
        page_number,
        page_count,
        width_image,height_image,
        global_mp3totalid_i = 0;
    double m_point_x;
    QRect getsize,
          titlesize,
          deskRect,
          screenRect;
    QDesktopWidget *desktopwidget;
    bool mousepress = false,
         judgeenter = true,
         judgeleave = true,
         judgeenter_again = true,
         judgeleave_again = true,
         judgefuncenter = true,
         isPresstToolbar,
         judgeenterplayif = true,
         judgeplaying = false,
         judgecoiledclicked = false,
         judgedoubleclickeditem,
         judgeplaywidgetvisible,
         judgeclickedvolumelogic = true,
         judgecontinous_clicked_v_m = false,
         judgeshowthevolume_adjustment = false,
         judgepositionbutton = true,
         judgeclickedlock_positionbutton = true,
         lock_playwidget_judge = true,
         enter_continous_if_statement = false;
    MySlider *Slider;
};
#endif // MAINWINDOW_H
