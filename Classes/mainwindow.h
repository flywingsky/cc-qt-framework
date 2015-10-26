#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class AppDelegate;
namespace Ui {
class MainWindow;
}

class GLWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    GLWidget* getGLWidget();

    static MainWindow* instance(){ return s_instance; }

    Ui::MainWindow *ui;

protected:
    void closeEvent(QCloseEvent *);

    static MainWindow* s_instance;
};

#endif // MAINWINDOW_H
