#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "singlenote.h"
#include <QMainWindow>
#include <QString>
#include <QVector>
#include <QJsonObject>
#include <QMap>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void addTagFunction();
private slots:
    void on_button_newTag_clicked();
    void on_button_newNote_clicked();
    void on_button_toggleArchive_clicked();
    void on_button_backup_clicked();

    void showContextMenu(const QPoint&);
    void showTableMenu(const QPoint&);

    void deleteTagItem();
    void addTagItem();
    void editTagItem();
    void addTagToFilter();

    void editNote();
    void deleteNote();
    void moveToArchive();

private:
    Ui::MainWindow *ui;
    // Vector of notes
    QVector<SingleNote*> notes;
    QVector<SingleNote*> archive;
    QStringList tags;
    QStringList filter;

    int maxID;

    bool readJSON(QString filePath);
    bool writeJSON(QString filePath);

    bool checkYN();

    void debugNote(SingleNote *sn);

    void updateTable();

    bool moveToArchive(int ID);
    bool moveFromArchive(int ID);

    void runInterface();
};

#endif // MAINWINDOW_H