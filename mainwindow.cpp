#include "mainwindow.h"
QMediaPlayer *player;
extern int slider_value;
bool judgeenterdelaytime = false,
     judgeplayover = false;
qint64 playtimer;
QTimer *playtime_timer;
extern bool m_isMoveing;
Volume_Slider *volume_slider;
QPushButton *volume;
QIcon icon_volume;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QFile file("./DATA");
    if(!file.open(QIODevice::ReadWrite|QIODevice::Text))
        QMessageBox::about(NULL,"ERROR","can't open the DATA file");
    QTextStream text(&file);
    QString LineStr;
    QTextStream write(&file);
    while(!text.atEnd())
        LineStr = text.readLine();
    QString dir_path = QCoreApplication::applicationDirPath();
    if(LineStr == ""){
        QString usage_needs = "file:///"+dir_path+"/USAGE%20NEEDS.pdf";
        QDesktopServices::openUrl(QUrl(usage_needs.toLatin1()));
        QMessageBox::about(NULL,"Notifation","Do not close the web page,and Configure it according to the content of the web page");
        write<<"[version] 1.1.0"<<endl;
    }
    file.close();
    int font_load_1 = QFontDatabase::addApplicationFont(":/fonts/Fonts/Merriweather-Black.ttf");
    QStringList fontlist_1(QFontDatabase::applicationFontFamilies(font_load_1));
    font.setFamily(fontlist_1.at(0));

    this->setContextMenuPolicy(Qt::NoContextMenu);
    this->setWindowTitle("X-PdfShow");
    QScreen *screen=QGuiApplication::primaryScreen ();
    QRect sc=screen->availableGeometry();
    screen_width = sc.width();
    screen_height = sc.height();

    this->setAttribute(Qt::WA_Hover,true);

    sizetimer = new QTimer;
    sizetimer->start(1000);
    connect(sizetimer,SIGNAL(timeout()),this,SLOT(get_size()));

    tToolBar = new QToolBar();
    this->addToolBar(Qt::TopToolBarArea,tToolBar);
    tToolBar->setMovable(false);
    tToolBar->setStyleSheet("border-bottom:1px solid black;background-color:rgb(222,220,221);");
    Title = new QLabel(tToolBar);
    Title->setText("X-PdfShow");
    Title->setFont(font);
    Title->setAlignment(Qt::AlignCenter);

    pToolBar = new QToolBar;
    this->addToolBar(Qt::LeftToolBarArea,pToolBar);
    pToolBar->setMovable(false);
    pToolBar->setStyleSheet("border-right:1px solid black;background-color:rgb(64,65,66);");

    github = new QPushButton(pToolBar);
    icon_github.addFile(":/icons/Icons/github-logo-24.png");
    github->setIcon(icon_github);
    github->setIconSize(QSize(32,32));
    github->setStyleSheet("border-radius:14px;");

    lock_playwidget_open = new QPushButton(pToolBar);
    icon_lock_playwidget_open.addFile(":/icons/Icons/lock-open-solid-24.png");
    lock_playwidget_open->setIcon(icon_lock_playwidget_open);
    lock_playwidget = new QPushButton(pToolBar);
    icon_lock_playwidget.addFile(":/icons/Icons/lock-solid-24.png");
    lock_playwidget->setIcon(icon_lock_playwidget);
    lock_playwidget_open->setIconSize(QSize(32,32));
    lock_playwidget->setIconSize(QSize(32,32));
    lock_playwidget_open->setStyleSheet("border-radius:14px;");
    lock_playwidget->setStyleSheet("border-radius:14px;");
    lock_playwidget->hide();
    lock_playwidget_open->setToolTip("you can lock the playwidget after click here");
    lock_playwidget->setToolTip("you can lock the playwidget after click here");
    connect(lock_playwidget_open,SIGNAL(clicked(bool)),this,SLOT(click_lock_playwidget_open()));
    connect(lock_playwidget,SIGNAL(clicked(bool)),this,SLOT(click_lock_playwidget()));
    connect(github,SIGNAL(clicked(bool)),this,SLOT(clickedgithub_button()));

    mathematics = new QPushButton(pToolBar);
    mathematics->setText("M\nA\nT\nH");
    mathematics->setCheckable(true);
    mathematics->setChecked(true);
    mathematics->setAutoExclusive(true);
    mathematics->setStyleSheet("QPushButton{background-color:rgb(225, 225, 225);border:2px groove gray;border-radius:10px;border-style: outset;font-weight:blog;}"
                               "QPushButton:hover{background-color:rgb(229, 241, 251); color: black;}"
                               "QPushButton:checked{background-color:blue}");
    mathematics->setFont(font);
    mathematics->setGeometry(0,0,30,90);

    computer_science = new QPushButton(pToolBar);
    computer_science->setText("C\nS");
    computer_science->setCheckable(true);
    computer_science->setAutoExclusive(true);
    computer_science->setStyleSheet("QPushButton{background-color:rgb(225, 225, 225);border:2px groove gray;border-radius:10px;border-style: outset;font-weight:blog;}"
                               "QPushButton:hover{background-color:rgb(229, 241, 251); color: black;}"
                               "QPushButton:checked{background-color:blue}");
    computer_science->setFont(font);
    computer_science->setGeometry(0,92,30,90);

    english = new QPushButton(pToolBar);
    english->setText("E\nN\nG");
    english->setCheckable(true);
    english->setAutoExclusive(true);
    english->setStyleSheet("QPushButton{background-color:rgb(225, 225, 225);border:2px groove gray;border-radius:10px;border-style: outset;font-weight:blog;}"
                               "QPushButton:hover{background-color:rgb(229, 241, 251); color: black;}"
                               "QPushButton:checked{background-color:blue}");
    english->setFont(font);
    english->setGeometry(0,184,30,90);

    music = new QPushButton(pToolBar);
    music->setText("M\nU\nS\nI\nC");
    music->setCheckable(true);
    music->setAutoExclusive(true);
    music->setStyleSheet("QPushButton{background-color:rgb(225, 225, 225);border:2px groove gray;border-radius:10px;border-style: outset;font-weight:blog;}"
                               "QPushButton:hover{background-color:rgb(229, 241, 251); color: black;}"
                               "QPushButton:checked{background-color:blue}");
    music->setFont(font);
    music->setGeometry(0,276,30,98);
    connect(music,SIGNAL(clicked(bool)),this,SLOT(music_clicked()));

    QPalette pal;
    pal.setColor(QPalette::Background,Qt::white);

    MathWidget = new QDockWidget(this);
    MathWidget->setAutoFillBackground(true);
    MathWidget->setPalette(pal);
    MathWidget->show();
    MathWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    addDockWidget(Qt::LeftDockWidgetArea,MathWidget);

    int font_load_2 = QFontDatabase::addApplicationFont(":/fonts/Fonts/CormorantGaramond-SemiBoldItalic.ttf");
    QStringList fontlist_2(QFontDatabase::applicationFontFamilies(font_load_2));
    font.setFamily(fontlist_2.at(0));
    MathWidget->setFont(font);

    QLabel *mathbarwidget_label = new QLabel("Mathematics", MathWidget);
    mathbarwidget_label->setStyleSheet("background-color:rgb(232,232,232);border:1px solid rgb(196,196,196);font:20px;");
    MathWidget->setTitleBarWidget(mathbarwidget_label);
    connect(mathematics,SIGNAL(clicked(bool)),this,SLOT(mathematics_clicked()));


    CsWidget = new QDockWidget(this);
    CsWidget->setAutoFillBackground(true);
    CsWidget->setPalette(pal);
    CsWidget->hide();
    CsWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    CsWidget->setFont(font);

    QLabel *csbarwidget_label = new QLabel("Computer Science", CsWidget);
    csbarwidget_label->setStyleSheet("background-color:rgb(232,232,232);border:1px solid rgb(196,196,196);font:20px;");
    CsWidget->setTitleBarWidget(csbarwidget_label);
    addDockWidget(Qt::LeftDockWidgetArea,CsWidget);


    MusicWidget = new QDockWidget(this);
    MusicWidget->setAutoFillBackground(true);
    MusicWidget->setPalette(pal);
    MusicWidget->hide();
    MusicWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    MusicWidget->setFont(font);
    QLabel *musicbarWidget_label = new QLabel("Music", MusicWidget);
    musicbarWidget_label->setStyleSheet("background-color:rgb(232,232,232);border:1px solid rgb(196,196,196);font:20px;");
    MusicWidget->setTitleBarWidget(musicbarWidget_label);
    addDockWidget(Qt::LeftDockWidgetArea,MusicWidget);


    EngWidget = new QDockWidget(this);
    EngWidget->setAutoFillBackground(true);
    EngWidget->setPalette(pal);
    EngWidget->setFont(font);
    QLabel *engbarwidget_label = new QLabel("English", EngWidget);
    engbarwidget_label->setStyleSheet("background-color:rgb(232,232,232);border:1px solid rgb(196,196,196);font:20px;");
    EngWidget->setTitleBarWidget(engbarwidget_label);
    connect(computer_science,SIGNAL(clicked(bool)),this,SLOT(computer_science_clicked()));
    EngWidget->hide();
    EngWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    addDockWidget(Qt::LeftDockWidgetArea,EngWidget);
    connect(english,SIGNAL(clicked(bool)),this,SLOT(english_clicked()));

    infshowWidget = new QDockWidget(this);
    infshowWidget->setAutoFillBackground(true);
    infshowWidget->setPalette(pal);
    setCentralWidget(infshowWidget);
    infshowWidget_label = new QLabel("",infshowWidget);
    infshowWidget_label->setStyleSheet("background-color:rgb(232,232,232);border:1px solid rgb(196,196,196);font:20px;");
    infshowWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    infshowWidget->setTitleBarWidget(infshowWidget_label);
    infshowWidget_label->setFont(font);
    addDockWidget(Qt::AllDockWidgetAreas,infshowWidget);

    loading_label = new QLabel(this);
    int font_load_3 = QFontDatabase::addApplicationFont(":/fonts/Fonts/CormorantGaramond-SemiBoldItalic.ttf");
    QStringList fontlist_3(QFontDatabase::applicationFontFamilies(font_load_3));
    font.setFamily(fontlist_3.at(0));
    font.setPointSize(17);

    flushfile = new QPushButton(this);
    icon_flush.addFile(":/icons/Icons/revision-regular-24.png");
    icon_loader.addFile(":/icons/Icons/loader-regular-24.png");
    flushfile->setIcon(icon_flush);
    flushfile->setStyleSheet("QPushButton{border-radius:14px;background-color:transparent;}");
    flushfile->setToolTip(QStringLiteral("You can click here to flush your file"));
    connect(flushfile,SIGNAL(clicked(bool)),this,SLOT(flushfilefunc()));

    websearch = new QLineEdit(infshowWidget);
    pdfinfo = new QWebEngineView();
    pdfinfo->setUrl(QUrl::fromUserInput("https://booxinx.github.io/wallpaper-updata/"));
    websearch->setText("https://booxinx.github.io/wallpaper-updata/");
    mathwidget_showfunc();
    pdfinfo->setContextMenuPolicy(Qt::NoContextMenu);
    computer_science_showfunc();
    websearch->setFocusPolicy(Qt::NoFocus);
    english_showfunc();
    music_showfunc();
    player = new QMediaPlayer;
    playwindow();
    windowflagsfunc();
    infshowWidget->setWidget(pdfinfo);
    search();
    back_startlocation = new QPushButton(infshowWidget);
    icon_back_startlocation.addFile(":/icons/Icons/chevrons-left-regular-24.png");
    back_startlocation->setIcon(icon_back_startlocation);
    back_startlocation->setIconSize(QSize(24,24));
    connect(back_startlocation,SIGNAL(clicked(bool)),this,SLOT(click_back_startlocation()));
}
MainWindow::~MainWindow(){
    delete pdfinfo;
}
void MainWindow::click_back_startlocation(){
    pdfinfo->setUrl(QUrl::fromUserInput("https://booxinx.github.io/wallpaper-updata/"));
    websearch->setText("https://booxinx.github.io/wallpaper-updata/");
}
void MainWindow::click_lock_playwidget_open(){

    lock_playwidget_open->hide();
    lock_playwidget->show();
    lock_playwidget_judge = false;
    lock->setDisabled(true);
    lock_open->setDisabled(true);
    lock->hide();
    lock_open->show();
    judgeenter = false;
    if(enter_continous_if_statement){
        enter_continous_if_statement = false;
        judgepositionbutton = true;
        judgeshowthevolume_adjustment = false;
        continous_clicked_volume_mute();
    }
    judgeclickedlock_positionbutton = true;
}
void MainWindow::click_lock_playwidget(){
    lock_playwidget_open->show();
    lock_playwidget->hide();
    lock_playwidget_judge = true;
    lock->setDisabled(false);
    lock_open->setDisabled(false);
    lock->hide();
    lock_open->show();
    judgeenter = true;
    judgeclickedlock_positionbutton = true;
}
bool MainWindow::WhetherProcessRunning(QString &processname){

    QProcess process;
    QString cmd = "tasklist";
    process.start(cmd);
    process.waitForFinished();
    QString str = process.readAllStandardOutput();
    if(str.contains(processname))
        return true;
    else
        return false;

}
void MainWindow::clickedgithub_button(){
    QString URL = "www.github.com";
    QString processname = "ipadress.exe";
    if(!WhetherProcessRunning(processname)){
        std::wstring operate = QString("runas").toStdWString();
        std::wstring path = QCoreApplication::applicationDirPath().toStdWString();

        std::wstring file = QString(QCoreApplication::applicationDirPath()+"/dist/ipadress/ipadress.exe").toStdWString();
        int ret = (int)ShellExecute(NULL,operate.c_str(),file.c_str(),NULL,path.c_str(),SW_SHOWNORMAL);
        Q_UNUSED(ret);
    }
    QDesktopServices::openUrl(QUrl(URL.toLatin1()));
}
void MainWindow::mouseMoveEvent(QMouseEvent *event){
    if(mousepress && isPresstToolbar){
        minimize->hide();
        zoomin->show();
        this->showNormal();
        move(event->globalPos().x()-m_point_x/1.5,event->globalPos().y()-m_point.y());
        this->resize(double(screen_width)/1.5,double(screen_height)/1.5);
    }
}

void MainWindow::onExpanded(const QModelIndex &index){
    Q_UNUSED(index);
    mathematics_tree->resizeColumnToContents(0);
    computer_science_tree->resizeColumnToContents(0);
    english_tree->resizeColumnToContents(0);
    music_tree->resizeColumnToContents(0);
}
void MainWindow::mousePressEvent(QMouseEvent *event){

    mousepress = true;
    if(event->globalPos().y() >= this->y() && event->globalPos().y() <= this->y()+26 && event->button() == Qt::LeftButton)
        isPresstToolbar = true;
    if(event->button() == Qt::LeftButton && isPresstToolbar){
        m_point = event->globalPos()-frameGeometry().topLeft();
        if(this->rect().size() == deskRect.size())
            m_point_x = event->globalPos().x()-frameGeometry().topLeft().x()/1.5;
        else
            m_point_x = double((event->globalPos().x()-frameGeometry().topLeft().x())*1.5);
    }

}


void MainWindow::mouseReleaseEvent(QMouseEvent *event){
    Q_UNUSED(event);
    mousepress = false;
    isPresstToolbar = false;
}
void MainWindow::resizeEvent(QResizeEvent *event){
    judgeclickedvolumelogic = true;
    judgeshowthevolume_adjustment = false;
    judgeclickedlock_positionbutton = true;
    lock->hide();
    lock_open->show();
    judgeenter = true;
    Q_UNUSED(event);
    volume_slider->setRange(0,100);
    QRect rect = this->geometry();
    QBitmap bmp(this->rect().size());
    bmp.fill();
    QPainter p(&bmp);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);

    if(this->rect().size() == deskRect.size())
        p.drawRoundedRect(bmp.rect(),0,0);
    else
        p.drawRoundedRect(bmp.rect(),8,8);
    setMask(bmp);
    pToolBar->setFixedSize(30,rect.height()-26);
    tToolBar->setFixedSize(rect.width(),26);
    Title->setGeometry(0,0,this->width(),tToolBar->height());
    github->setGeometry(2,this->rect().height()-120,28,28);
    lock_playwidget_open->setGeometry(2,this->rect().height()-90,28,28);
    lock_playwidget->setGeometry(2,this->rect().height()-90,28,28);
    MathWidget->setFixedSize(240,rect.height()-26);
    MusicWidget->setFixedSize(240,rect.height()-26);
    CsWidget->setFixedSize(240,rect.height()-26);
    EngWidget->setFixedSize(240,rect.height()-26);
    websearch->setGeometry(30,0,this->rect().width()-60-MathWidget->rect().width(),infshowWidget_label->geometry().height()-1);
    loading_label->setGeometry(MathWidget->geometry().width(),MathWidget->geometry().y(),26,26);
    flushfile->setGeometry(MathWidget->geometry().width(),MathWidget->geometry().y(),28,28);
    playwidget->setGeometry(30,this->rect().height(),this->rect().width()-30,55);
    minimize->setGeometry(rect.width()-50,3,20,20);
    zoomout->setGeometry(rect.width()-75,3,20,20);
    closure->setGeometry(rect.width()-25,3,20,20);
    zoomin->setGeometry(rect.width()-50,3,20,20);
    back_startlocation->setGeometry(0,0,30,websearch->height());
    play->setGeometry(MathWidget->width()+95,(playwidget->rect().height())/2-20,40,40);
    pause->setGeometry(MathWidget->width()+95,(playwidget->rect().height())/2-20,40,40);
    next->setGeometry(MathWidget->width()+141,(playwidget->rect().height())/2-17,34,34);
    previous->setGeometry(MathWidget->width()+55,(playwidget->rect().height())/2-17,34,34);
    musictext->setGeometry(MathWidget->width()+200,2,(playwidget->rect().width())/2.2,(playwidget->rect().height())/2);
    Slider->setGeometry(MathWidget->width()+200,playwidget->rect().height()-musictext->rect().height()-2,double((infshowWidget->rect().width())*2/3),(playwidget->rect().height())/2);
    volume->setGeometry(Slider->x()+Slider->rect().width()+20,(playwidget->rect().height())/2-17,34,34);
    volume_adjustment->setFixedSize(38,140);
    lock_widget->move(this->rect().width()*0.95,this->rect().height()-22);
    lock->setGeometry((lock_widget->width()-lock->width())/2.0,2,24,24);
    lock_open->setGeometry((lock_widget->width()-lock->width())/2.0,2,24,24);
    volume_slider->setFixedSize(38,140);
}

void MainWindow::mathematics_clicked(){
    MathWidget->show();
    CsWidget->close();
    EngWidget->close();
    MusicWidget->close();
}
void MainWindow::computer_science_clicked(){
    CsWidget->show();
    MathWidget->close();
    EngWidget->close();
    MusicWidget->close();
}
void MainWindow::english_clicked(){
    EngWidget->show();
    CsWidget->close();
    MathWidget->close();
    MusicWidget->close();
}
void MainWindow::music_clicked(){
    EngWidget->close();
    CsWidget->close();
    MathWidget->close();
    MusicWidget->show();
}
QFileInfoList MainWindow::allfile(QTreeWidgetItem *root, QString path){
    QDir dir(path);
    QDir dir_file(path);
    dir_file.setFilter(QDir::Files|QDir::Hidden|QDir::NoSymLinks);
    dir_file.setSorting(QDir::Size|QDir::Reversed);
    font.setPointSize(10);
    QFileInfoList List_file = dir_file.entryInfoList();
    for(int i = 0;i<List_file.size();++i){
        QFileInfo fileinfo = List_file.at(i);
        QString childpath = fileinfo.fileName();
        QTreeWidgetItem *child = new QTreeWidgetItem(QStringList()<<childpath);
        if(childpath.contains(".pdf"))
            child->setFont(0,font);
        if(childpath.contains(".mp3"))
        {
            child->setFont(0,font);
            mp3file<<childpath;
        }

        root->addChild(child);
    }
    QFileInfoList file_list = dir.entryInfoList(QDir::Files|QDir::Hidden|QDir::NoSymLinks);
    QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs|QDir::NoDotAndDotDot);
    for(int i=0;i!=folder_list.size();i++){
        QString namepath = folder_list.at(i).absoluteFilePath();
        QFileInfo folderinfo = folder_list.at(i);
        QString childpath_1 = folderinfo.fileName();
        QTreeWidgetItem *childroot = new QTreeWidgetItem(QStringList()<<childpath_1);
        if(childpath_1.contains(".pdf"))
            childroot->setFont(0,font);
        if(childpath_1.contains(".mp3")){
            childroot->setFont(0,font);
            mp3file<<childpath_1;
        }
        root->addChild(childroot);
        QFileInfoList child_file_list = allfile(childroot,namepath);
        file_list.append(child_file_list);
        file_list.append(childpath_1);
    }
    return file_list;
}
void MainWindow::mathwidget_showfunc(){
    mathematics_tree = new QTreeWidget();
    mathematics_tree->setFont(font);
    connect(mathematics_tree, SIGNAL(expanded(QModelIndex)), this, SLOT(onExpanded(QModelIndex)));
    connect(mathematics_tree, SIGNAL(collapsed(QModelIndex)), this, SLOT(onExpanded(QModelIndex)));
    mathematics_tree->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    mathematics_tree->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mathematics_tree->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    mathematics_tree->header()->setMinimumSectionSize(300);
    mathematics_tree->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal{background-color:rgb(240,240,240);"
                                                           "margin:0px,0px,0px,0px;"
                                                           "padding-left:20px;"
                                                           "padding-right:20px;"
                                                           "padding-top:5px;"
                                                           "padding-bottom:5px;"
                                                           "height:17px;}"
                                                           "QScrollBar:handle:horizontal{background:rgb(169,169,169);}"
                                                           "QScrollBar:handle:horizontal:hover{background:rgb(133,133,133);}"
                                                           "QScrollBar::add-line:horizontal{background:rgb(240,240,240);width:20px;"
                                                           "subcontrol-position:right;"
                                                           "subcontrol-origin:margin;}"
                                                           "QScrollBar::sub-line:horizontal{background:rgb(240,240,240);width:20px;"
                                                           "subcontrol-position:left;"
                                                           "subcontrol-origin:margin;}"
                                                           "QScrollBar::left-arrow:horizontal{image:url(:/icons/Icons/left-arrow-solid-24.png);"
                                                           "width:10px;"
                                                           "height:10px;}"
                                                           "QScrollBar::right-arrow:horizontal{image:url(:/icons/Icons/right-arrow-solid-24.png);"
                                                           "width:10px;"
                                                           "height:10px;}"
                                                           "QScrollBar::add-page:horizontal,QScrollBar::sub-page:horizontal{background:none;}");
    mathematics_tree->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{background-color:rgb(240,240,240);"
                                                         "margin:0px,0px,0px,0px;"
                                                         "padding-left:5px;"
                                                         "padding-right:5px;"
                                                         "padding-top:20px;"
                                                         "padding-bottom:20px;"
                                                         "width:17px;}"
                                                         "QScrollBar:handle:vertical{background:rgb(169,169,169);}"
                                                         "QScrollBar:handle:vertical:hover{background:rgb(133,133,133);}"
                                                         "QScrollBar::add-line:vertical{background:rgb(240,240,240);height:20px;"
                                                         "subcontrol-position:bottom;"
                                                         "subcontrol-origin:margin;}"
                                                         "QScrollBar::sub-line:vertical{background:rgb(240,240,240);height:20px;"
                                                         "subcontrol-position:top;"
                                                         "subcontrol-origin:margin;}"
                                                         "QScrollBar::up-arrow:vertical{image:url(:/icons/Icons/up-arrow-solid-24.png);"
                                                         "width:10px;"
                                                         "height:10px;}"
                                                         "QScrollBar::dowm-arrow:vertical{image:url(:/icons/Icons/dowm-arrow-solid-24.png);"
                                                         "width:10px;"
                                                         "height:10px;}"
                                                         "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{background:none;}");
    rootpath_Advanced_mathematics = "./PdfJs/Pdf/Advanced mathematics";
    Advanced_mathematics = new QTreeWidgetItem(QStringList()<<rootpath_Advanced_mathematics);
    Advanced_mathematics->setText(0,"Advanced mathematics");
    mathematics_tree->addTopLevelItem(Advanced_mathematics);
    allfile(Advanced_mathematics,rootpath_Advanced_mathematics);

    rootpath_Linear_algebra = "./PdfJs/Pdf/Linear algebra";
    Linear_algebra = new QTreeWidgetItem(QStringList()<<rootpath_Linear_algebra);
    Linear_algebra->setText(0,"Linear algebra");
    mathematics_tree->addTopLevelItem(Linear_algebra);
    allfile(Linear_algebra,rootpath_Linear_algebra);

    rootpath_Probability_theory = "./PdfJs/Pdf/Probability theory";
    Probability_theory = new QTreeWidgetItem(QStringList()<<rootpath_Probability_theory);
    Probability_theory->setText(0,"Probability theory");
    mathematics_tree->addTopLevelItem(Probability_theory);
    allfile(Probability_theory,rootpath_Probability_theory);

    rootpath_Self_Added_Files_Math = "./PdfJs/Pdf/Self Added Files-Math";
    Self_Added_Files_Math = new QTreeWidgetItem(QStringList()<<rootpath_Self_Added_Files_Math);
    Self_Added_Files_Math->setText(0,"Self Added Files-Math");
    mathematics_tree->addTopLevelItem(Self_Added_Files_Math);
    allfile(Self_Added_Files_Math,rootpath_Self_Added_Files_Math);
    mathematics_tree->setStyleSheet("QTreeWidget::item{height:40px;}"
                                    "QTreeWidget::item:hover,QTreeWidget::branch:hover{background-color:rgb(204,232,255);}"
                                    "QTreeWidget::item:selected,QTreeWidget::branch:selected{background-color:rgb(217,217,217);border:none;}"
                                    "QTreeWidget{outline:none;show-decoration-selected:1;border:none;}"
                                    "QTreeView::branch:open:has-children {image: url(:/icons/Icons/chevron-down-regular-24.png);}"
                                    "QTreeView::branch:closed:has-children {image: url(:/icons/Icons/chevron-right-regular-24.png);}"
                                    "QTreeView::branch{image:none;}");
    MathWidget->setWidget(mathematics_tree);
    mathematics_tree->setHeaderHidden(true);

    connect(mathematics_tree,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(OnlineTreeViewDoubleClick(QTreeWidgetItem*,int)));
}
void MainWindow::computer_science_showfunc(){
    font.setPointSize(17);
    computer_science_tree = new QTreeWidget();
    CsWidget->setWidget(computer_science_tree);
    computer_science_tree->setFont(font);
    computer_science_tree->header()->setMinimumSectionSize(300);
    computer_science_tree->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal{background-color:rgb(240,240,240);"
                                                           "margin:0px,0px,0px,0px;"
                                                           "padding-left:20px;"
                                                           "padding-right:20px;"
                                                           "padding-top:5px;"
                                                           "padding-bottom:5px;"
                                                           "height:17px;}"
                                                           "QScrollBar:handle:horizontal{background:rgb(169,169,169);}"
                                                           "QScrollBar:handle:horizontal:hover{background:rgb(133,133,133);}"
                                                           "QScrollBar::add-line:horizontal{background:rgb(240,240,240);width:20px;"
                                                           "subcontrol-position:right;"
                                                           "subcontrol-origin:margin;}"
                                                           "QScrollBar::sub-line:horizontal{background:rgb(240,240,240);width:20px;"
                                                           "subcontrol-position:left;"
                                                           "subcontrol-origin:margin;}"
                                                           "QScrollBar::left-arrow:horizontal{image:url(:/icons/Icons/left-arrow-solid-24.png);"
                                                           "width:10px;"
                                                           "height:10px;}"
                                                           "QScrollBar::right-arrow:horizontal{image:url(:/icons/Icons/right-arrow-solid-24.png);"
                                                           "width:10px;"
                                                           "height:10px;}"
                                                           "QScrollBar::add-page:horizontal,QScrollBar::sub-page:horizontal{background:none;}");
    computer_science_tree->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{background-color:rgb(240,240,240);"
                                                         "margin:0px,0px,0px,0px;"
                                                         "padding-left:5px;"
                                                         "padding-right:5px;"
                                                         "padding-top:20px;"
                                                         "padding-bottom:20px;"
                                                         "width:17px;}"
                                                         "QScrollBar:handle:vertical{background:rgb(169,169,169);}"
                                                         "QScrollBar:handle:vertical:hover{background:rgb(133,133,133);}"
                                                         "QScrollBar::add-line:vertical{background:rgb(240,240,240);height:20px;"
                                                         "subcontrol-position:bottom;"
                                                         "subcontrol-origin:margin;}"
                                                         "QScrollBar::sub-line:vertical{background:rgb(240,240,240);height:20px;"
                                                         "subcontrol-position:top;"
                                                         "subcontrol-origin:margin;}"
                                                         "QScrollBar::up-arrow:vertical{image:url(:/icons/Icons/up-arrow-solid-24.png);"
                                                         "width:10px;"
                                                         "height:10px;}"
                                                         "QScrollBar::dowm-arrow:vertical{image:url(:/icons/Icons/dowm-arrow-solid-24.png);"
                                                         "width:10px;"
                                                         "height:10px;}"
                                                         "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{background:none;}");
    rootpath_Computer_ebook = "./PdfJs/Pdf/Computer ebook";
    Computer_ebook = new QTreeWidgetItem(QStringList()<<rootpath_Computer_ebook);
    Computer_ebook->setText(0,"Computer ebook");
    computer_science_tree->header()->setMinimumSectionSize(300);
    computer_science_tree->addTopLevelItem(Computer_ebook);
    allfile(Computer_ebook,rootpath_Computer_ebook);

    rootpath_Self_Added_Files_Computer = "./PdfJs/Pdf/Self Added Files-Computer";
    Self_Added_Files_Computer = new QTreeWidgetItem(QStringList()<<rootpath_Self_Added_Files_Computer);
    Self_Added_Files_Computer->setText(0,"Self Added Files-Computer");
    computer_science_tree->addTopLevelItem(Self_Added_Files_Computer);
    allfile(Self_Added_Files_Computer,rootpath_Self_Added_Files_Computer);

    root_Computer_network = "./PdfJs/Pdf/Computer network";
    Computer_network = new QTreeWidgetItem(QStringList()<<root_Computer_network);
    Computer_network->setText(0,"Computer network");
    computer_science_tree->addTopLevelItem(Computer_network);
    allfile(Computer_network,root_Computer_network);

    root_Operating_system = "./PdfJs/Pdf/Operating system";
    Operating_system = new QTreeWidgetItem(QStringList()<<root_Operating_system);
    Operating_system->setText(0,"Operating system");
    computer_science_tree->addTopLevelItem(Operating_system);
    allfile(Operating_system,root_Operating_system);

    root_Data_structure = "./PdfJs/Pdf/Data structure";
    Data_structure = new QTreeWidgetItem(QStringList()<<root_Data_structure);
    Data_structure->setText(0,"Data structure");
    computer_science_tree->addTopLevelItem(Data_structure);
    allfile(Data_structure,root_Data_structure);

    root_Composition_principle = "./PdfJs/Pdf/Composition principle";
    Composition_principle = new QTreeWidgetItem(QStringList()<<root_Composition_principle);
    Composition_principle->setText(0,"Composition principle");
    computer_science_tree->addTopLevelItem(Composition_principle);
    allfile(Composition_principle,root_Composition_principle);

    computer_science_tree->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    computer_science_tree->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    connect(computer_science_tree, SIGNAL(expanded(QModelIndex)), this, SLOT(onExpanded(QModelIndex)));
    connect(computer_science_tree, SIGNAL(collapsed(QModelIndex)), this, SLOT(onExpanded(QModelIndex)));
    computer_science_tree->setStyleSheet("QTreeWidget::item{height:40px;}"
                                         "QTreeWidget::item:hover,QTreeWidget::branch:hover{background-color:rgb(204,232,255);}"
                                         "QTreeWidget::item:selected,QTreeWidget::branch:selected{background-color:rgb(217,217,217);border:none;}"
                                         "QTreeWidget{outline:none;show-decoration-selected:1;border:none;}"
                                         "QTreeView::branch:open:has-children {image: url(:/icons/Icons/chevron-down-regular-24.png);}"
                                         "QTreeView::branch:closed:has-children {image: url(:/icons/Icons/chevron-right-regular-24.png);}"
                                         "QTreeView::branch{image:none;}");
    computer_science_tree->setHeaderHidden(true);

    connect(computer_science_tree,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(OnlineTreeViewDoubleClick(QTreeWidgetItem*,int)));
}
void MainWindow::windowflagsfunc(){
    minimize = new QPushButton(tToolBar);
    zoomout = new QPushButton(tToolBar);
    zoomin = new QPushButton(tToolBar);
    closure = new QPushButton(tToolBar);
    zoomin->hide();
    connect(minimize,SIGNAL(clicked(bool)),this,SLOT(minimize_clicked()));
    connect(zoomout,SIGNAL(clicked(bool)),this,SLOT(zoomout_clicked()));
    connect(zoomin,SIGNAL(clicked(bool)),this,SLOT(zoomin_clicked()));
    connect(closure,SIGNAL(clicked(bool)),this,SLOT(closure_clicked()));
    zoomout->setStyleSheet("QPushButton{background:rgb(252,188,70);border-radius:10px;border:none;}"
                           "QPushButton:hover{image:url(:/icons/Icons/minus-regular-24.png)}");
    minimize->setStyleSheet("QPushButton{background:rgb(0,177,81);border-radius:10px;border:none;}"
                            "QPushButton:hover{image:url(:/icons/Icons/windows-regular-24.png)}");
    closure->setStyleSheet("QPushButton{background:rgb(252,106,86);border-radius:10px;border:none;}"
                           "QPushButton:hover{image:url(:/icons/Icons/x-regular-24.png)}");
    zoomin->setStyleSheet("QPushButton{background:rgb(0,177,81);border-radius:10px;border:none;}"
                          "QPushButton:hover{image:url(:/icons/Icons/plus-regular-24.png)}");


}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event){
    if(event->globalPos().y() >= this->y() && event->globalPos().y() <= this->y()+26 && event->button() == Qt::LeftButton)
        isPresstToolbar = true;
    if(event->button()==Qt::LeftButton && isPresstToolbar){
        playwidget->setVisible(false);

        if(this->rect().size() == deskRect.size())
        {
            minimize->hide();
            zoomin->show();
            this->showNormal();
            this->resize(double(screen_width)/1.5,double(screen_height)/1.5);
            this->move((screen_width - this->width())/2,(screen_height - this->height())/2);
            playwidget->move(30,this->height());
            playwidget->setVisible(true);
        }
        else
        {
            minimize->show();
            zoomin->hide();
            this->showMaximized();
            this->resize(deskRect.width(),deskRect.height());
            this->move((getsize.width()-titlesize.width())/2,0);
            this->setWindowState(Qt::WindowMaximized);
            playwidget->move(30,this->height());
            playwidget->setVisible(true);
        }
    }
}
void MainWindow::closure_clicked(){
    this->close();
}
void MainWindow::minimize_clicked(){
    zoomin->show();
    minimize->hide();
    this->showNormal();
    this->resize(double(screen_width)/1.5,double(screen_height)/1.5);
    this->move((screen_width - this->width())/2,(screen_height - this->height())/2);
}
void MainWindow::zoomout_clicked(){
    this->showMinimized();
}
void MainWindow::get_size(){
    desktopwidget = QApplication::desktop();
    deskRect = desktopwidget->availableGeometry();
    screenRect = desktopwidget->screenGeometry();
    getsize = this->geometry();
    titlesize = this->frameGeometry();
    delete sizetimer;
}
void MainWindow::zoomin_clicked(){
    zoomin->hide();
    minimize->show();
    this->resize(deskRect.width(),deskRect.height());
    this->move((getsize.width()-titlesize.width())/2,0);
    this->setWindowState(Qt::WindowMaximized);
}
void MainWindow::english_showfunc(){
    font.setPointSize(17);
    english_tree = new QTreeWidget();
    EngWidget->setWidget(english_tree);
    english_tree->setFont(font);
    english_tree->setHeaderHidden(true);

    rootpath_english_listening = "./PdfJs/Pdf/English listening";
    english_listening = new QTreeWidgetItem(QStringList()<<rootpath_english_listening);
    english_listening->setText(0,"English Listening");
    english_tree->addTopLevelItem(english_listening);
    allfile(english_listening,rootpath_english_listening);

    rootpath_Self_Added_Files_English = "./PdfJs/Pdf/Self Added Files-English";
    Self_Added_Files_English = new QTreeWidgetItem(QStringList()<<rootpath_Self_Added_Files_English);
    Self_Added_Files_English->setText(0,"Self Added Files-English");
    english_tree->addTopLevelItem(Self_Added_Files_English);
    allfile(Self_Added_Files_English,rootpath_Self_Added_Files_English);

    english_tree->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal{background-color:rgb(240,240,240);"
                                                           "margin:0px,0px,0px,0px;"
                                                           "padding-left:20px;"
                                                           "padding-right:20px;"
                                                           "padding-top:5px;"
                                                           "padding-bottom:5px;"
                                                           "height:17px;}"
                                                           "QScrollBar:handle:horizontal{background:rgb(169,169,169);}"
                                                           "QScrollBar:handle:horizontal:hover{background:rgb(133,133,133);}"
                                                           "QScrollBar::add-line:horizontal{background:rgb(240,240,240);width:20px;"
                                                           "subcontrol-position:right;"
                                                           "subcontrol-origin:margin;}"
                                                           "QScrollBar::sub-line:horizontal{background:rgb(240,240,240);width:20px;"
                                                           "subcontrol-position:left;"
                                                           "subcontrol-origin:margin;}"
                                                           "QScrollBar::left-arrow:horizontal{image:url(:/icons/Icons/left-arrow-solid-24.png);"
                                                           "width:10px;"
                                                           "height:10px;}"
                                                           "QScrollBar::right-arrow:horizontal{image:url(:/icons/Icons/right-arrow-solid-24.png);"
                                                           "width:10px;"
                                                           "height:10px;}"
                                                           "QScrollBar::add-page:horizontal,QScrollBar::sub-page:horizontal{background:none;}");
    english_tree->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{background-color:rgb(240,240,240);"
                                                         "margin:0px,0px,0px,0px;"
                                                         "padding-left:5px;"
                                                         "padding-right:5px;"
                                                         "padding-top:20px;"
                                                         "padding-bottom:20px;"
                                                         "width:17px;}"
                                                         "QScrollBar:handle:vertical{background:rgb(169,169,169);}"
                                                         "QScrollBar:handle:vertical:hover{background:rgb(133,133,133);}"
                                                         "QScrollBar::add-line:vertical{background:rgb(240,240,240);height:20px;"
                                                         "subcontrol-position:bottom;"
                                                         "subcontrol-origin:margin;}"
                                                         "QScrollBar::sub-line:vertical{background:rgb(240,240,240);height:20px;"
                                                         "subcontrol-position:top;"
                                                         "subcontrol-origin:margin;}"
                                                         "QScrollBar::up-arrow:vertical{image:url(:/icons/Icons/up-arrow-solid-24.png);"
                                                         "width:10px;"
                                                         "height:10px;}"
                                                         "QScrollBar::dowm-arrow:vertical{image:url(:/icons/Icons/dowm-arrow-solid-24.png);"
                                                         "width:10px;"
                                                         "height:10px;}"
                                                         "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{background:none;}");
    english_tree->header()->setMinimumSectionSize(300);
    english_tree->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    english_tree->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    english_tree->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    connect(english_tree, SIGNAL(expanded(QModelIndex)), this, SLOT(onExpanded(QModelIndex)));
    connect(english_tree, SIGNAL(collapsed(QModelIndex)), this, SLOT(onExpanded(QModelIndex)));
    english_tree->setStyleSheet("QTreeWidget::item{height:40px;}"
                                "QTreeWidget::item:hover,QTreeWidget::branch:hover{background-color:rgb(204,232,255);}"
                                "QTreeWidget::item:selected,QTreeWidget::branch:selected{background-color:rgb(217,217,217);border:none;}"
                                "QTreeWidget{outline:none;show-decoration-selected:1;border:none;}"
                                "QTreeView::branch:open:has-children {image: url(:/icons/Icons/chevron-down-regular-24.png);}"
                                "QTreeView::branch:closed:has-children {image: url(:/icons/Icons/chevron-right-regular-24.png);}"
                                "QTreeView::branch{image:none;}");
    connect(english_tree,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(OnlineTreeViewDoubleClick(QTreeWidgetItem*,int)));
}

void MainWindow::music_showfunc(){
    font.setPointSize(17);
    music_tree = new QTreeWidget();
    MusicWidget->setWidget(music_tree);
    music_tree->setFont(font);
    music_tree->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal{background-color:rgb(240,240,240);"
                                                           "margin:0px,0px,0px,0px;"
                                                           "padding-left:20px;"
                                                           "padding-right:20px;"
                                                           "padding-top:5px;"
                                                           "padding-bottom:5px;"
                                                           "height:17px;}"
                                                           "QScrollBar:handle:horizontal{background:rgb(169,169,169);}"
                                                           "QScrollBar:handle:horizontal:hover{background:rgb(133,133,133);}"
                                                           "QScrollBar::add-line:horizontal{background:rgb(240,240,240);width:20px;"
                                                           "subcontrol-position:right;"
                                                           "subcontrol-origin:margin;}"
                                                           "QScrollBar::sub-line:horizontal{background:rgb(240,240,240);width:20px;"
                                                           "subcontrol-position:left;"
                                                           "subcontrol-origin:margin;}"
                                                           "QScrollBar::left-arrow:horizontal{image:url(:/icons/Icons/left-arrow-solid-24.png);"
                                                           "width:10px;"
                                                           "height:10px;}"
                                                           "QScrollBar::right-arrow:horizontal{image:url(:/icons/Icons/right-arrow-solid-24.png);"
                                                           "width:10px;"
                                                           "height:10px;}"
                                                           "QScrollBar::add-page:horizontal,QScrollBar::sub-page:horizontal{background:none;}");
    music_tree->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{background-color:rgb(240,240,240);"
                                                         "margin:0px,0px,0px,0px;"
                                                         "padding-left:5px;"
                                                         "padding-right:5px;"
                                                         "padding-top:20px;"
                                                         "padding-bottom:20px;"
                                                         "width:17px;}"
                                                         "QScrollBar:handle:vertical{background:rgb(169,169,169);}"
                                                         "QScrollBar:handle:vertical:hover{background:rgb(133,133,133);}"
                                                         "QScrollBar::add-line:vertical{background:rgb(240,240,240);height:20px;"
                                                         "subcontrol-position:bottom;"
                                                         "subcontrol-origin:margin;}"
                                                         "QScrollBar::sub-line:vertical{background:rgb(240,240,240);height:20px;"
                                                         "subcontrol-position:top;"
                                                         "subcontrol-origin:margin;}"
                                                         "QScrollBar::up-arrow:vertical{image:url(:/icons/Icons/up-arrow-solid-24.png);"
                                                         "width:10px;"
                                                         "height:10px;}"
                                                         "QScrollBar::dowm-arrow:vertical{image:url(:/icons/Icons/dowm-arrow-solid-24.png);"
                                                         "width:10px;"
                                                         "height:10px;}"
                                                         "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{background:none;}");
    rootpath_music_audio = "./PdfJs/Pdf/Music";
    music_audio = new QTreeWidgetItem(QStringList()<<rootpath_music_audio);
    music_audio->setText(0,"Music");
    music_tree->addTopLevelItem(music_audio);
    allfile(music_audio,rootpath_music_audio);

    rootpath_Self_Added_Files_Music = "./PdfJs/Pdf/Self Added Files-Music";
    Self_Added_Files_Music = new QTreeWidgetItem(QStringList()<<rootpath_Self_Added_Files_Music);
    Self_Added_Files_Music->setText(0,"Self Added Files-Music");
    music_tree->addTopLevelItem(Self_Added_Files_Music);
    allfile(Self_Added_Files_Music,rootpath_Self_Added_Files_Music);

    music_tree->header()->setMinimumSectionSize(300);
    music_tree->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    music_tree->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    music_tree->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    connect(music_tree, SIGNAL(expanded(QModelIndex)), this, SLOT(onExpanded(QModelIndex)));
    connect(music_tree, SIGNAL(collapsed(QModelIndex)), this, SLOT(onExpanded(QModelIndex)));
    music_tree->setStyleSheet("QTreeWidget::item{height:40px;}"
                                         "QTreeWidget::item:hover,QTreeWidget::branch:hover{background-color:rgb(204,232,255);}"
                                         "QTreeWidget::item:selected,QTreeWidget::branch:selected{background-color:rgb(217,217,217);border:none;}"
                                         "QTreeWidget{outline:none;show-decoration-selected:1;border:none;}"
                                         "QTreeView::branch:open:has-children {image: url(:/icons/Icons/chevron-down-regular-24.png);}"
                                         "QTreeView::branch:closed:has-children {image: url(:/icons/Icons/chevron-right-regular-24.png);}"
                                         "QTreeView::branch{image:none;}");
    music_tree->setHeaderHidden(true);
    connect(music_tree,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(OnlineTreeViewDoubleClick(QTreeWidgetItem*,int)));
}

void MainWindow::OnlineTreeViewDoubleClick(QTreeWidgetItem *item,int count){
    if(judgeenterdelaytime && !judgeplayover){
        judgeenterdelaytime = false;
        delete playtime_timer;
    }

    Q_UNUSED(count);
    QStringList filepath;
    QTreeWidgetItem *itemfile = item;

    while(itemfile!=NULL){
        filepath<<itemfile->text(0);
        itemfile = itemfile->parent();
    }
    QString strpath;

    for(int i=(filepath.size()-1);i>=0;i--){
        strpath+=filepath.at(i);
        if(i!=0)
            strpath+='/';
    }

    global_strpath = strpath;
    if(strpath.contains(".pdf"))
        webpdfshow();
    if(filepath.at(0).contains(".mp3")){
        for(int i = 0;i<mp3file.size();i++){
            if(filepath.at(0) == mp3file.at(i)){
                judgedoubleclickeditem = true;
                global_mp3totalid = i;
                mediaplay();
                break;
            }
        }

    }




}
void MainWindow::webpdfshow(){
    pathtopdfjs = "http://localhost:8080//web/viewer.html";
    file = "http://localhost:8080//Pdf/"+global_strpath;
    pdfinfo->page()->load(QUrl::fromUserInput(pathtopdfjs+"?file="+file));
    websearch->setText(pathtopdfjs+"?file="+file);
}

void MainWindow::search(){
    websearch->setPlaceholderText(QStringLiteral("Physical path"));
    websearch->setStyleSheet("QLineEdit{background:white;"
                                        "border:2px solid rgb(141,141,141);}"
                             "QLineEdit:focus{border:2px solid rgb(0,112,203)}");
    connect(websearch,SIGNAL(returnPressed()),this,SLOT(obtaintextsearch()));
}

void MainWindow::obtaintextsearch(){
    website = websearch->text();
    websearch->setText(website);
    pdfinfo->setUrl(website);
}
void MainWindow::mediaplay(){
    if(judgecoiledclicked)
        delete coiledclicked;
    judgecoiledclicked = false;

    QString new_global_strpath;
    for(int i = 0;i<global_strpath.size();i++){
        if(global_strpath.at(i) == "/")
            global_strpath_single_quoteid = i;
    }
    for(int i = 0;i<=global_strpath_single_quoteid;i++)
        new_global_strpath += global_strpath.at(i);
    musictext->setText(mp3file.at(global_mp3totalid));
    musictext->setFont(font);
    player->setMedia(QUrl::fromLocalFile("http://localhost:8080//Pdf/"+new_global_strpath+mp3file.at(global_mp3totalid)));
    player->play();
}
void MainWindow::clickednext(){
    if(judgeplaying){
        global_mp3totalid++;
        if(global_mp3totalid == mp3file.size())
            global_mp3totalid = 0;
        if(judgecoiledclicked)
            delete coiledclicked;
        coiledclicked = new QTimer;
        coiledclicked->start(500);
        connect(coiledclicked,SIGNAL(timeout()),this,SLOT(clickednextdelay()));
        judgecoiledclicked = true;
    }
}
void MainWindow::clickedprevious(){
    if(judgeplaying){
        global_mp3totalid -= 1;
        if(global_mp3totalid < 0)
            global_mp3totalid = mp3file.size() - 1;
        if(judgecoiledclicked)
            delete coiledclicked;
        coiledclicked = new QTimer;
        coiledclicked->start(500);
        connect(coiledclicked,SIGNAL(timeout()),this,SLOT(clickedpreviousdelay()));
        judgecoiledclicked = true;
    }

}
void MainWindow::clickednextdelay(){
    if(judgeenterdelaytime && !judgeplayover)
        delete playtime_timer;
    qDebug()<<"321";
    QTreeWidgetItemIterator fetchall_childnode(music_tree);
    while(*fetchall_childnode){
        if((*fetchall_childnode)->text(0).contains(".mp3")){
            if((*fetchall_childnode)->text(0) == mp3file.at(global_mp3totalid) && global_mp3totalid_i == global_mp3totalid){
                qDebug()<<global_mp3totalid<<global_mp3totalid_i;
                QTreeWidgetItemIterator selected(music_tree);
                while(*selected){
                    (*selected)->setSelected(false);
                    ++selected;
                }
                (*fetchall_childnode)->setSelected(true);
                global_mp3totalid_i = 0;
                break;
            }
            ++global_mp3totalid_i;
        }

        ++fetchall_childnode;
    }

    mediaplay();
}
void MainWindow::clickedpreviousdelay(){
    if(judgeenterdelaytime && !judgeplayover)
        delete playtime_timer;
    QTreeWidgetItemIterator fetchall_childnode(music_tree);
    while(*fetchall_childnode){

        if((*fetchall_childnode)->text(0).contains(".mp3")){
            if((*fetchall_childnode)->text(0) == mp3file.at(global_mp3totalid) && global_mp3totalid_i == global_mp3totalid){
                qDebug()<<global_mp3totalid<<global_mp3totalid_i;
                QTreeWidgetItemIterator selected(music_tree);
                while(*selected){
                    (*selected)->setSelected(false);
                    ++selected;
                }
                (*fetchall_childnode)->setSelected(true);
                global_mp3totalid_i = 0;
                break;
            }
            ++global_mp3totalid_i;
        }

        ++fetchall_childnode;
    }
    mediaplay();
}
bool MainWindow::event(QEvent *event){


    if(event->type() == QEvent::HoverEnter || event->type() == QEvent::HoverLeave || event->type() == QEvent::HoverMove){
        QHoverEvent* pHoverEvent = static_cast<QHoverEvent *>(event);
        if(((pHoverEvent->pos().x() >= this->width()*0.8 &&
                pHoverEvent->pos().x() <= this->width() &&
                pHoverEvent->pos().y() >= this->rect().height()-55
                && pHoverEvent->pos().y() <= this->rect().height()) || lock_widget->underMouse()) && judgeenter && lock_playwidget_judge){
            playshowtimer = new QTimer(this);
            playshowtimer->start(500);
            judgeenter = false;
            QPropertyAnimation *pAnimation = new QPropertyAnimation(playwidget,"pos");
            QPropertyAnimation *pAnimation_lock_widget = new QPropertyAnimation(lock_widget,"pos");
            pAnimation_lock_widget->setDuration(500);
            pAnimation->setDuration(500);
            pAnimation->setKeyValueAt(0,QPoint(30,this->rect().height()));
            pAnimation_lock_widget->setKeyValueAt(0,QPoint(this->rect().width()*0.95,this->rect().height()-22));
            pAnimation->setKeyValueAt(1,QPoint(30,this->rect().height()-55));
            pAnimation_lock_widget->setKeyValueAt(1,QPoint(this->rect().width()*0.95,this->rect().height()-77));
            pAnimation->start(QAbstractAnimation::DeleteWhenStopped);
            pAnimation_lock_widget->start(QAbstractAnimation::DeleteWhenStopped);
            connect(playshowtimer,SIGNAL(timeout()),this,SLOT(playshow()));
            playwidget->setVisible(true);
        }
        else if((((pHoverEvent->pos().y() < this->rect().height()-55
                   || pHoverEvent->pos().y() > this->rect().height() || pHoverEvent->pos().x() < 30)
                  && !lock_widget->underMouse())
                 || judgeenter == false)
                && judgepositionbutton
                && judgeclickedlock_positionbutton){

            if(playwidget->y() == this->rect().height()-55 && judgeleave){
                if(judgefuncenter){
                    judgefuncenter = false;
                    playhidetimer = new QTimer(this);
                    playhidetimer->start(1000);
                }

                if(((pHoverEvent->pos().y() >= this->rect().height()-55
                     && pHoverEvent->pos().y() <= this->rect().height() && pHoverEvent->pos().x()>30)
                    ||lock_widget->underMouse()) && judgeenter_again){

                    judgeenter_again = false;
                    playhidetimer->start(1000);
                    playhidetimer->stop();

                }
                else if((((pHoverEvent->pos().y() < this->rect().height()-55 || pHoverEvent->pos().y() > this->rect().height()) || pHoverEvent->pos().x()<30)
                         && !lock_widget->underMouse()) && judgeenter_again == false){
                    judgeenter_again = true;
                    playhidetimer->start(1000);
                }
                connect(playhidetimer,SIGNAL(timeout()),this,SLOT(Playhide()));

            }

        }

        if(lock_open->underMouse()){
            icon_lock_open.addFile(":/icons/Icons/lock-open-regular-24.png");
            lock_open->setIcon(icon_lock_open);
        }
        else{
            icon_lock_open.addFile(":/icons/Icons/lock-open-solid-24.png");
            lock_open->setIcon(icon_lock_open);
        }
        if(lock->underMouse()){
            icon_lock.addFile(":/icons/Icons/lock-regular-24.png");
            lock->setIcon(icon_lock);
        }
        else{
            icon_lock.addFile(":/icons/Icons/lock-solid-24.png");
            lock->setIcon(icon_lock);
        }
        if(lock_playwidget_open->underMouse()){
            icon_lock_playwidget_open.addFile(":/icons/Icons/lock-open-regular-24.png");
            lock_playwidget_open->setIcon(icon_lock_playwidget_open);
        }
        else{
            icon_lock_playwidget_open.addFile(":/icons/Icons/lock-open-solid-24.png");
            lock_playwidget_open->setIcon(icon_lock_playwidget_open);
        }
        if(lock_playwidget->underMouse()){
            icon_lock_playwidget.addFile(":/icons/Icons/lock-regular-24.png");
            lock_playwidget->setIcon(icon_lock_playwidget);
        }
        else{
            icon_lock_playwidget.addFile(":/icons/Icons/lock-solid-24.png");
            lock_playwidget->setIcon(icon_lock_playwidget);
        }

    }
    return QWidget::event(event);
}

void MainWindow::playshow(){
    delete playshowtimer;
    playwidget->setGeometry(30,this->rect().height()-55,this->rect().width()-30,55);
    lock_widget->move(this->rect().width()*0.95,this->rect().height()-77);
    judgeleave = true;
}
void MainWindow::Playhide(){
    delete playhidetimer;
    judgefuncenter = true;
    judgeleave = false;
    QPropertyAnimation *pAnimation = new QPropertyAnimation(playwidget,"pos");
    pAnimation->setDuration(500);
    pAnimation->setKeyValueAt(0,QPoint(30,this->rect().height()-55));
    pAnimation->setKeyValueAt(1,QPoint(30,this->rect().height()));
    pAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    QPropertyAnimation *pAnimation_lock_widget = new QPropertyAnimation(lock_widget,"pos");
    pAnimation_lock_widget->setDuration(500);
    pAnimation_lock_widget->setKeyValueAt(0,QPoint(this->rect().width()*0.95,this->rect().height()-77));
    pAnimation_lock_widget->setKeyValueAt(1,QPoint(this->rect().width()*0.95,this->rect().height()-22));
    pAnimation_lock_widget->start(QAbstractAnimation::DeleteWhenStopped);

    hidewidgettimer = new QTimer(this);
    hidewidgettimer->start(500);
    connect(hidewidgettimer,SIGNAL(timeout()),this,SLOT(playhide_hide()));
}
void MainWindow::playhide_hide(){
    judgeenter = true;
    if(!lock_playwidget_judge)
        judgeenter = false;
    delete hidewidgettimer;
    playwidget->setGeometry(30,this->rect().height(),this->rect().width()-30,55);
    lock_widget->move(this->rect().width()*0.95,this->rect().height()-22);
    playwidget->setVisible(false);

}
void MainWindow::playwindow(){

    playwidget = new QStackedWidget(this);
    playwidget->setStyleSheet("background-color:rgb(50,49,49);border:2px groove rgb(73,73,73);border-radius:5px;");
    playwidget->setVisible(false);
    play = new QPushButton(playwidget);
    pause = new QPushButton(playwidget);
    play->setStyleSheet("QPushButton{border-radius:20px;border:none;}");
    pause->setStyleSheet("QPushButton{border-radius:20px;border:none;}");
    pause->hide();
    icon_play.addFile(":/icons/Icons/play-circle-regular-36.png");
    icon_pause.addFile(":/icons/Icons/pause-circle-regular-36.png");
    play->setIcon(icon_play);
    play->setIconSize(QSize(60,60));
    pause->setIcon(icon_pause);
    pause->setIconSize(QSize(60,60));
    next = new QPushButton(playwidget);
    previous = new QPushButton(playwidget);
    next->setStyleSheet("QPushButton{border-radius:17px;border:none;}");
    previous->setStyleSheet("QPushButton{border-radius:17px;border:none;}");
    icon_next.addFile(":/icons/Icons/skip-next-circle-regular-24.png");
    icon_previous.addFile(":/icons/Icons/skip-previous-circle-regular-24.png");
    next->setIcon(icon_next);
    previous->setIcon(icon_previous);
    next->setIconSize(QSize(34,34));
    previous->setIconSize(QSize(34,34));
    musictext = new QLabel(playwidget);
    musictext->setText("Music Name");
    musictext->setFont(font);
    musictext->setStyleSheet("background-color:rgb(50,49,49);border:none;color:white");
    Slider = new MySlider(playwidget);
    Slider->setEnabled(false);
    Slider->setOrientation(Qt::Horizontal);
    Slider->setStyleSheet("QSlider{background-color: rgb(50,49,49);"
                          "padding-top: 15px;"
                          "padding-bottom: 15px;"
                          "border-radius: 5px;"
                          "border:none;}"
                          "QSlider::add-page:horizontal{background-color: #FF7826;"
                          "height:5px;"
                          "border-radius: 2px;}"
                          "QSlider::sub-page:horizontal{background-color: #7A7B79;width:5px;"
                          "border-radius: 2px;}"
                          "QSlider::groove:horizontal{background:transparent;"
                          "height:6px;}"
                          "QSlider::handle:horizontal{height: 14px;width: 14px;"
                          "margin:-4px 0px -4px 0px;"
                          "border-radius: 7px;background: white;}");
    connect(play,SIGNAL(clicked(bool)),this,SLOT(audioclickplay()));
    connect(pause,SIGNAL(clicked(bool)),this,SLOT(audioclickpause()));
    connect(player,SIGNAL(durationChanged(qint64)),this,SLOT(getduration(qint64)));
    connect(player,&QMediaPlayer::stateChanged,this,&MainWindow::onstateChanged);
    connect(next,SIGNAL(clicked(bool)),this,SLOT(clickednext()));
    connect(previous,SIGNAL(clicked(bool)),this,SLOT(clickedprevious()));
    connect(Slider,SIGNAL(valueChanged(int)),this,SLOT(ValueChange()));
    volume = new QPushButton(playwidget);
    volume->setStyleSheet("border-radius:17px;border:none;background:white;");
    icon_volume.addFile(":/icons/Icons/volume-full-regular-36.png");
    volume->setIcon(icon_volume);
    volume->setIconSize(QSize(24,24));
    connect(volume,SIGNAL(clicked(bool)),this,SLOT(clickedvolume()));
    volume_adjustment = new QStackedWidget(this);
    volume_adjustment->setStyleSheet("background:rgb(40,40,40)");
    volume_adjustment->setVisible(false);
    playwidget->raise();
    lock_widget = new QStackedWidget(this);
    QPixmap lock_widget_Img;
    lock_widget_Img.load(":/icons/Icons/polygon-solid-72.png");
    lock_widget->resize(lock_widget_Img.size());
    lock_widget->setMask(lock_widget_Img.mask());
    lock_widget->setStyleSheet("background: transparent;background-image: url(:/icons/Icons/polygon-solid-72.png);");
    lock = new QPushButton(lock_widget);
    lock_open = new QPushButton(lock_widget);
    icon_lock.addFile(":/icons/Icons/lock-solid-24.png");
    lock->setIcon(icon_lock);
    icon_lock_open.addFile(":/icons/Icons/lock-open-solid-24.png");
    lock->hide();
    lock_open->setIcon(icon_lock_open);
    connect(lock,SIGNAL(clicked(bool)),this,SLOT(clickedlock_button()));
    connect(lock_open,SIGNAL(clicked(bool)),this,SLOT(clickedlock_openbutton()));
    Volume();

}
void MainWindow::Volume(){
    volume_slider = new Volume_Slider(volume_adjustment);
    volume_slider->setOrientation(Qt::Vertical);
    volume_slider->setStyleSheet("QSlider{background-color: rgb(50,49,49);"
                          "padding-left: 15px;"
                          "padding-right: 15px;"
                          "border-radius: 5px;"
                          "border:none;}"
                          "QSlider::add-page:vertical{background-color: #00A0E6;"
                          "width:5px;"
                          "border-radius: 2px;}"
                          "QSlider::sub-page:vertical{background-color: #7A7B79;height:5px;"
                          "border-radius: 2px;}"
                          "QSlider::groove:vertical{background:transparent;"
                          "width:6px;}"
                          "QSlider::handle:vertical{height: 14px;width: 14px;"
                          "margin:0px -4px 0px -4px;"
                          "border-radius: 7px;background: white;}");

}
void MainWindow::clickedlock_openbutton(){
    lock->show();
    lock_open->hide();
    judgeclickedlock_positionbutton = false;
}
void MainWindow::clickedlock_button(){
    lock->hide();
    lock_open->show();
    judgeclickedlock_positionbutton = true;
}
void MainWindow::clickedvolume(){
    judgepositionbutton = false;
    if(judgecontinous_clicked_v_m){
        delete continuous_clicked_volume_mute_timer;
        judgecontinous_clicked_v_m = false;
    }
    continuous_clicked_volume_mute_timer = new QTimer;
    continuous_clicked_volume_mute_timer->start(300);
    connect(continuous_clicked_volume_mute_timer,SIGNAL(timeout()),this,SLOT(continous_clicked_volume_mute()));
    judgecontinous_clicked_v_m = true;
}
void MainWindow::continous_clicked_volume_mute(){

    if(judgeclickedvolumelogic)
        judgeclickedvolumelogic = false;
    else
        judgeclickedvolumelogic = true;
    if(lock_playwidget_judge){
        delete continuous_clicked_volume_mute_timer;
        judgecontinous_clicked_v_m = false;
    }
    if(!judgeclickedvolumelogic && !judgeshowthevolume_adjustment){
        enter_continous_if_statement = true;
        judgeshowthevolume_adjustment = true;
        volume_adjustment_delaytimer = new QTimer;
        QPropertyAnimation *pAnimation = new QPropertyAnimation(volume_adjustment,"pos");
        pAnimation->setDuration(500);
        volume_adjustment_delaytimer->start(500);
        pAnimation->setKeyValueAt(0,QPoint(Slider->x()+Slider->rect().width()+48,this->rect().height()));
        pAnimation->setKeyValueAt(1,QPoint(Slider->x()+Slider->rect().width()+48,this->rect().height()-195));

        pAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        connect(volume_adjustment_delaytimer,SIGNAL(timeout()),this,SLOT(volume_adjustment_delay()));
        volume_adjustment->setVisible(true);
    }
    else{
        enter_continous_if_statement = false;
        judgepositionbutton = true;
        judgeshowthevolume_adjustment = false;
        volume_adjustment_delaytimer = new QTimer;
        QPropertyAnimation *pAnimation = new QPropertyAnimation(volume_adjustment,"pos");
        pAnimation->setDuration(500);
        volume_adjustment_delaytimer->start(500);
        pAnimation->setKeyValueAt(0,QPoint(Slider->x()+Slider->rect().width()+48,this->rect().height()-195));
        pAnimation->setKeyValueAt(1,QPoint(Slider->x()+Slider->rect().width()+48,this->rect().height()));
        pAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        connect(volume_adjustment_delaytimer,SIGNAL(timeout()),this,SLOT(volume_adjustment_delay()));
    }
}
void MainWindow::volume_adjustment_delay(){

    delete volume_adjustment_delaytimer;
    if(!judgeclickedvolumelogic){
        volume_adjustment->setGeometry(Slider->x()+Slider->rect().width()+48,this->height()-195,38,140);
    }
    else{

        volume_adjustment->setGeometry(Slider->x()+Slider->rect().width()+48,this->height(),38,140);
        volume_adjustment->setVisible(false);
    }
}
void MainWindow::getduration(qint64 playtime){
    Q_UNUSED(playtime);
    playtimer = player->duration();
    Slider->setRange(0,playtimer);
    valuechangetime = new QTimer;
    valuechangetime->start(500);
    connect(valuechangetime,SIGNAL(timeout()),this,SLOT(ValueChangetimeover()));
}
void MainWindow::ValueChange(){
    if(!m_isMoveing){
        delete valuechangetime;
        valuechangetime = new QTimer;
        valuechangetime->start(1000);
        connect(valuechangetime,SIGNAL(timeout()),this,SLOT(ValueChangetimeover()));
    }
}
void MainWindow::ValueChangetimeover(){
    if(!m_isMoveing)
        Slider->setValue(player->position());
}
void MainWindow::audioclickplay(){
    player->play();
}
void MainWindow::audioclickpause(){
    player->pause();
}
void MainWindow::onstateChanged(QMediaPlayer::State state){
    if(state == QMediaPlayer::PlayingState){
        pause->show();
        play->hide();
        judgeplaying = true;
        if(judgeenterplayif){
            judgeenterplayif = false;
            delayplaytimer = new QTimer;
            delayplaytimer->start(200);
            connect(delayplaytimer,SIGNAL(timeout()),this,SLOT(delayplaytime()));
        }
    }
    else if(state == QMediaPlayer::StoppedState){
        pause->hide();
        play->show();
        judgeplaying = false;
        judgeenterplayif = true;

        if(judgeplayover){
            judgeplayover = false;
            global_mp3totalid++;
            if(global_mp3totalid == mp3file.size()-1)
                global_mp3totalid = 0;
            QTreeWidgetItemIterator fetchall_childnode(music_tree);
            while(*fetchall_childnode){

                if((*fetchall_childnode)->text(0).contains(".mp3")){
                    if((*fetchall_childnode)->text(0) == mp3file.at(global_mp3totalid) && global_mp3totalid_i == global_mp3totalid){
                        qDebug()<<global_mp3totalid<<global_mp3totalid_i;
                        QTreeWidgetItemIterator selected(music_tree);
                        while(*selected){
                            (*selected)->setSelected(false);
                            ++selected;
                        }
                            (*fetchall_childnode)->setSelected(true);
                            global_mp3totalid_i = 0;
                            mediaplay();
                            break;
                        }
                            ++global_mp3totalid_i;
                 }
                        ++fetchall_childnode;
             }
         }
    }
    else{
        pause->hide();
        play->show();
    }
}
void MainWindow::delayplaytime(){
    delete delayplaytimer;
    if(playtimer!=0){
        judgeenterdelaytime = true;
        Slider->setEnabled(true);
        qDebug()<<playtimer;
        playtime_timer = new QTimer;
        playtime_timer->start(playtimer-1000);
        connect(playtime_timer,SIGNAL(timeout()),this,SLOT(playtime_timerfunc()));
    }
}
void MainWindow::playtime_timerfunc(){

    if(!m_isMoveing)
        delete playtime_timer;
    judgeenterdelaytime = false;
    judgeplayover = true;
}
void MainWindow::removeitem(QTreeWidgetItem *item){
    while(item->childCount()>0)
        item->removeChild(item->child(0));
}
void MainWindow::flushfilefunc(){
    time = new QTimer(this);
    time->start(1000);
    flushfile->hide();
    loading_label->setStyleSheet("background-color:transparent;border-radius:13px;");
    movie = new QMovie(":/icons/Icons/loading.gif");
    movie->setScaledSize(QSize(20,20));
    loading_label->setAlignment(Qt::AlignCenter);
    loading_label->setMovie(movie);
    loading_label->show();
    movie->start();
    removeitem(music_audio);
    removeitem(Advanced_mathematics);
    removeitem(Linear_algebra);
    removeitem(Probability_theory);
    removeitem(Self_Added_Files_Math);
    removeitem(Self_Added_Files_Computer);
    removeitem(Self_Added_Files_English);
    removeitem(Self_Added_Files_Music);
    removeitem(Computer_network);
    removeitem(Operating_system);
    removeitem(Data_structure);
    removeitem(Composition_principle);
    removeitem(Computer_ebook);
    removeitem(english_listening);
    allfile(Advanced_mathematics,rootpath_Advanced_mathematics);
    allfile(music_audio,rootpath_music_audio);
    allfile(english_listening,rootpath_english_listening);
    allfile(Computer_ebook,rootpath_Computer_ebook);
    allfile(Linear_algebra,rootpath_Linear_algebra);
    allfile(Probability_theory,rootpath_Probability_theory);
    allfile(Self_Added_Files_Math,rootpath_Self_Added_Files_Math);
    allfile(Self_Added_Files_Computer,rootpath_Self_Added_Files_Computer);
    allfile(Self_Added_Files_English,rootpath_Self_Added_Files_English);
    allfile(Self_Added_Files_Music,rootpath_Self_Added_Files_Music);
    allfile(Computer_network,root_Computer_network);
    allfile(Operating_system,root_Operating_system);
    allfile(Data_structure,root_Data_structure);
    allfile(Composition_principle,root_Composition_principle);
    connect(time,SIGNAL(timeout()),this,SLOT(funcfinished()));
}
void MainWindow::funcfinished(){
    delete time;
    loading_label->hide();
    flushfile->show();
    movie->stop();
}
void MainWindow::obtainip(){
//    foreach(QHostAddress ptr,QNetworkInterface::allAddresses()){
//        if(ptr.protocol() == QAbstractSocket::IPv4Protocol){
//            if(!ptr.isLoopback()){
//                ipv4=ptr.toString();
//                qDebug()<<"ipadress:"<<ptr.toString()<<endl;
//            }
//        }
//    }
}
void MainWindow::pdfshow(){
//    page_number = 0;
//    zoom =2;
//    rotate = 0;
//    ctx = fz_new_context(NULL,NULL,FZ_STORE_UNLIMITED);
//    if(!ctx){
//        qDebug()<<stderr<<"cannot create mupdf context";
//        return;
//    }
//    fz_try(ctx)
//        fz_register_document_handlers(ctx);
//    fz_catch(ctx)
//    {
//        qDebug()<<stderr<<"cannot register document handlers:"<<fz_caught_message(ctx);
//        fz_drop_context(ctx);
//        return;
//    }
//    fz_try(ctx)
//            doc = fz_open_document(ctx,strpath_to_char_point);
//    fz_catch(ctx){
//        qDebug()<<stderr<<"cannot open document:"<<fz_caught_message(ctx);
//        fz_drop_context(ctx);
//        return;
//    }
//    fz_try(ctx)
//        page_count = fz_count_pages(ctx,doc);
//    fz_catch(ctx){
//        qDebug()<<stderr<<"cannot count number of pages:"<<fz_caught_message(ctx);
//        fz_drop_document(ctx,doc);
//        fz_drop_context(ctx);
//    }
//    updatepage();
}
void MainWindow::updatepage(){

//    for(int i=0;i<page_count;i++){
//        fz_matrix ctm = fz_scale(zoom,zoom);
//        fz_pre_rotate(ctm,rotate);
//        fz_try(ctx)
//                pix = fz_new_pixmap_from_page_number(ctx,doc,i,ctm,fz_device_rgb(ctx),0);
//        fz_catch(ctx){
//            qDebug()<<stderr<<"cannot render page:%s\n"<<fz_caught_message(ctx);
//            fz_drop_document(ctx,doc);
//            fz_drop_context(ctx);
//            return;
//        }
//        qDebug()<<i;
//        unsigned char *samples = pix->samples;
//        width_image= fz_pixmap_width(ctx,pix);
//        height_image = fz_pixmap_height(ctx,pix);
//        QImage image(samples,width_image,height_image,pix->stride,QImage::Format_RGB888);

//        if(i==0)
//            newimage = QImage(width_image,height_image*page_count,QImage::Format_RGB888);
//        QPainter p(&newimage);
//        p.drawPixmap(0,i*height_image,width_image,height_image,QPixmap::fromImage(image));
//        if(i!=page_count-1)
//        {
//            QPen pen;
//            pen.setColor(qRgb(0,0,0));
//            pen.setWidth(1);
//            QLineF line(0,height_image*(i+1)-1,width_image,height_image*(i+1)-1);
//            p.drawLine(line);
//        }
//        if(showpdflabel!=NULL)
//            showpdflabel->clear();
//        newpdf = QPixmap::fromImage(newimage);
//        showpdflabel = new QLabel;
//        showpdflabel->setPixmap(newpdf);
//    }
//    scrollArea_pdf_w = new QScrollArea;
//    scrollArea_pdf_w->setWidget(showpdflabel);
//    scrollArea_pdf_w->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
//    scrollArea_pdf_w->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
//    scrollArea_pdf_w->setWidgetResizable(true);

//    infshowWidget->setWidget(scrollArea_pdf_w);
}
