#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_but_refresh_process_clicked();

    void on_but_refresh_inp_dev_clicked();

    void load_sys_info();

    void on_but_refresh_dev_info_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
