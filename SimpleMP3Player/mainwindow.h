#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "irrKlang.h"
#if defined(WIN32)
#include <windows.h>
#include <conio.h>
inline void sleepSomeTime() { Sleep(100); }
#else
#include "conio.h"
#endif
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    friend void *slider_thread(void* arg);

private slots:
    void on_playButton_clicked();
    void open();

    void on_horizontalSlider_sliderPressed();

    void on_horizontalSlider_sliderReleased();

    void on_horizontalSlider_sliderMoved(int position);

private:
    const char * currentFile;
    Ui::MainWindow *ui;
    irrklang::ISoundEngine* engine;
    QStringList fileName;
    irrklang::ISound* music = nullptr;
    bool paused = false;
    bool SliderThreadActive = false;
    long pos;
    pthread_t sliderThread;
    pthread_t clockThread;

};

#endif // MAINWINDOW_H
