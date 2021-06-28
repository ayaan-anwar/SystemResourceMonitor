#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QString>
#include <QDebug>
#include <QFile>
#include <QTextStream>

#include <string>
#include <vector>
#include <fstream>
#include <cstdio>
#include <windows.h>

#include "Path.h"
#include "Inp_Devices.h"

//extern "C" {
//    int system(const char *command);
//}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->bar_cpu_usage->setMinimum(0);
    ui->bar_cpu_usage->setMaximum(100);
    ui->bar_ram_load->setMinimum(0);
    ui->bar_ram_load->setMaximum(100);
    ui->bar_hdd_used->setMinimum(0);
    ui->bar_hdd_used->setMaximum(100);

    on_but_refresh_process_clicked();
    on_but_refresh_inp_dev_clicked();
    load_sys_info();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::load_sys_info()
{
    std::string sys_info_cmd = "cmd /c python " + get_path("system_information.py").toStdString();
    system(sys_info_cmd.c_str());

    std::vector<QString> details;
    std::ifstream info(get_path("sys_info.txt").toStdString());
    std::string tmp;
    while (getline(info, tmp, '\n'))
    {
        details.push_back(QString::fromStdString(tmp + "\n"));
    }

    QString basic_info = details[0] + details[1] + details[2] + details[3] + details[4];
    ui->disp_basic_info->setText(basic_info);

    int cpu_usage = stoi(details[5].toStdString());
    int ram_usage = stoi(details[6].toStdString());

    SYSTEM_INFO siSysInfo;
    GetSystemInfo(&siSysInfo);
    QString cpu_info = "";
    cpu_info.append("Processor Type: ");
    if (siSysInfo.wProcessorArchitecture == 9)
        cpu_info.append("x64\n");
    else
        cpu_info.append("x86\n");
    cpu_info.append("Number of logical cores: " + QString::number(siSysInfo.dwNumberOfProcessors) + "\n");
    cpu_info.append("Page Size: " + QString::number(siSysInfo.dwPageSize) + "\n");
    cpu_info.append("CPU Usage: " + QString::number(cpu_usage) + "% \n");
    ui->disp_cpu->setText(cpu_info);

    ui->bar_cpu_usage->setValue(cpu_usage);

    ui->disp_ram->setText("RAM Usage: " + QString::number(ram_usage) + "% \n");
    ui->bar_ram_load->setValue(ram_usage);

    QString hdd_info = "Drive Letter: C:\\ \n";
    hdd_info.append("Name: " + details[7]);

    double total = stod(details[9].toStdString());
    double used = stod(details[10].toStdString());
    double free = stod(details[11].toStdString());
    hdd_info.append("Formatting: " + details[8]);
    hdd_info.append("Total Space: " + QString::number(total) + " GB\n");
    hdd_info.append("Used Space: " + QString::number(used) + " GB\n");
    hdd_info.append("Free Space: " + QString::number(free) + " GB\n");
    ui->disp_hdd->setText(hdd_info);
    int hdd_used = (int)(100.0f * stod(details[10].toStdString()) / stod(details[9].toStdString()));
//    qDebug() << hdd_used;
    ui->bar_hdd_used->setValue(hdd_used);
}

void MainWindow::on_but_refresh_process_clicked()
{
    std::string load_process_cmd = "cmd /c python " + get_path("process_monitor.py").toStdString()
            + " -n 10 --descending -c name,cpu_usage,memory_usage,status,n_threads"; // > " + get_path("proc.txt");
    //qDebug() << QString::fromStdString(load_process_cmd);

    system(load_process_cmd.c_str());

    QFile proc(get_path("proc.txt"));
    qDebug() << "file loaded";
    QString process_list = "<center>";
    if (!proc.exists())   {
        qDebug() << "Unable to open file\n";
    }
    else   {
        proc.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&proc);
        process_list.append(ts.readAll());
        process_list.append("</center>");
//        process_list = ts.readAll();
    }

//    QFile proc("C:\\dev\\projects\\OS_Project\\System_Monitor\\proc.txt");
//    QTextStream in(&proc);
//    QString process_list = in.readAll();
//    std::ifstream proc(get_path("proc.txt"));
//    std::string line;
//    while (std::getline(proc, line))
//    {
//        process_list.append(QString::fromStdString(line + "\n"));
//    }

    ui->disp_process->setText(process_list);
}

void MainWindow::on_but_refresh_inp_dev_clicked()
{
    ui->disp_inp_dev->setText(get_inp_dev());
}

void MainWindow::on_but_refresh_dev_info_clicked()
{
    load_sys_info();
}
