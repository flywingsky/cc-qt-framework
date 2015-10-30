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
private slots:
    void on_actionCreateSprite2D_triggered();
    void on_actionCreateNode_triggered();
    void on_actionFileOpen_triggered();
    void on_actionFileSave_triggered();
    void on_actionFileClear_triggered();
    void on_actionCreateSprite3D_triggered();
};

#endif // MAINWINDOW_H
