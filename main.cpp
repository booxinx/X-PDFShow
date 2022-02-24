#include "mainwindow.h"
#include <QApplication>
#include <QScreen>
#include <QDesktopWidget>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow window;
    QScreen *screen=QGuiApplication::primaryScreen ();
    QRect sc=screen->availableGeometry();
    int screen_width = sc.width();
    int screen_height = sc.height();
    window.setMouseTracking(true);
    window.setWindowState(Qt::WindowMaximized);
    window.setWindowFlags(Qt::Window|Qt::FramelessWindowHint |Qt::WindowSystemMenuHint|Qt::WindowMinimizeButtonHint|Qt::WindowMaximizeButtonHint);
    window.setMinimumSize(screen_width/double(1.5),screen_height/double(1.5));
    window.move ((QApplication::desktop()->width() - window.width())/2,(QApplication::desktop()->height() - window.height())/2);
    window.show();
    return a.exec();
}
