#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    engine = irrklang::createIrrKlangDevice();
    if(!engine){
        ;
        //handle error here l8er
    }
    ui->playButton->setEnabled(false);
    connect(ui->actionOpen, SIGNAL(triggered(bool)), this, SLOT(open()));
    ui->horizontalSlider->setMinimum(0);
}

void *slider_thread(void* arg){
    MainWindow * mwPoint;
    mwPoint = (MainWindow*)arg;
    int s = 0;
    int m = 0;
    int h = 0;

    do{
        mwPoint->ui->horizontalSlider->setValue(mwPoint->music->getPlayPosition());
        h = (mwPoint->music->getPlayPosition() / 1000) / 60 /60;
        m = ((mwPoint->music->getPlayPosition() / 1000) / 60) % 60;
        s = (mwPoint->music->getPlayPosition() /1000) % 60;
        mwPoint->ui->clockSecond->display(s);
        mwPoint->ui->clockMinute->display(m);
        mwPoint->ui->clockHour->display(h);
        sleepSomeTime();

    }while(!mwPoint->music->isFinished());

    mwPoint->ui->horizontalSlider->setValue(mwPoint->music->getPlayLength());
    h = (mwPoint->music->getPlayLength() / 1000) / 60 /60;
    m = ((mwPoint->music->getPlayLength() / 1000) / 60) % 60;
    s = (mwPoint->music->getPlayLength() /1000) % 60;
    mwPoint->ui->clockSecond->display(s);
    mwPoint->ui->clockMinute->display(m);
    mwPoint->ui->clockHour->display(h);
    mwPoint->SliderThreadActive = false;
    mwPoint->music->setIsPaused(true);
    pthread_exit(NULL);
}


MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::on_playButton_clicked()
{
    if(!paused)//file is playing
    {
        paused = true;
        pos = music->getPlayPosition();
        music->setIsPaused(true);
    }
    else{//file is paused
        paused = false;
        music->setPlayPosition(pos - 10);
        music->setIsPaused(false);
    }

}

void MainWindow::open(){

    ui->horizontalSlider->setValue(0);
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(tr("Audio (*.mp3 *.wav *.ogg *.flac *.mod *.it *.s3d *.xm)"));
    dialog.setViewMode(QFileDialog::Detail);
    if(dialog.exec())
            fileName = dialog.selectedFiles();
    //std::cout << fileName.toVector()[0].toStdString()<< std::endl;
    if(music){
       music->drop();
    }

    music = engine->play3D(fileName.toVector()[0].toStdString().c_str(), irrklang::vec3df(0,0,0), false, false, true);
    ui->playButton->setEnabled(true);
    ui->horizontalSlider->setMaximum(music->getPlayLength());

    int ret;
    ret = pthread_create(&sliderThread, NULL, &slider_thread, (void*)this);
    if(ret != 0) {
       std::cerr << "error on slider thread creation" << std::endl;
       this->deleteLater();
    }
    SliderThreadActive = true;
   // ui->horizontalSlider->setValue();

   // ui->playButton->setText("Pause");
}

void MainWindow::on_horizontalSlider_sliderPressed()
{
    music->setIsPaused(true);
    if(SliderThreadActive){
        int ret = pthread_cancel(this->sliderThread);
        if(ret != 0) {
                    std::cerr << "error on slider pressed" << std::endl;
                    this->deleteLater();
         }
        SliderThreadActive = false;
    }




}

void MainWindow::on_horizontalSlider_sliderReleased()
{
    if(music->isFinished()){
        music->drop();
        music = engine->play3D(fileName.toVector()[0].toStdString().c_str(), irrklang::vec3df(0,0,0), false, false, true);
    }
    music->setPlayPosition(ui->horizontalSlider->value());
    music->setIsPaused(false);

    int ret = pthread_create(&sliderThread, NULL, &slider_thread, (void*)this);
    if(ret != 0) {
         std::cerr << "error on slider thread creation in released" << std::endl;
         this->deleteLater();
    }
    SliderThreadActive = true;

}

void MainWindow::on_horizontalSlider_sliderMoved(int position)
{

    int h = (position / 1000) / 60 /60;;
    int m = (position / 1000) / 60 % 60;
    int s = (position /1000) % 60;
    ui->clockSecond->display(s);
    ui->clockMinute->display(m);
    ui->clockHour->display(h);
}
