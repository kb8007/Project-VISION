#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_Start_clicked();

    void on_Stop_clicked();

    void on_Stopplay_clicked();

    void on_Change_clicked();

    void on_save_clicked();

    void on_New_clicked();

    void on_Threshold_actionTriggered(int action);

    void on_Eraser_clicked();

    void on_Paint_b_clicked();

    void on_mode_point_clicked();

    void on_mode_line_clicked();

    void on_Thikness_actionTriggered(int action);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
