#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QInputDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // Right button for list
    ui->list_tags->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(ui->list_tags, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));

    // TODO : Add reading from JSON file
}

MainWindow::~MainWindow() {
    // TODO : Add writing to JSON file
    delete ui;
}

// Funciton to add new item
void MainWindow::addTagFunction() {
    bool ok;
    QString tagText = QInputDialog::getText(this, tr("Create New Tag"), tr("Tag name:"), QLineEdit::Normal, "default", &ok);
    tagText = tagText.toLower();
    if (ok && !tagText.isEmpty()) {
        // Chek for similar tag
        int numberOfTags = ui->list_tags->count();
        bool hasSame = false;
        for (int i = 0; i < numberOfTags; i++)
            if (ui->list_tags->item(i)->text() == tagText)
                hasSame = true;
        if (!hasSame)
            ui->list_tags->addItem(tagText);
        else {
            // If tag is already available
            QMessageBox messageBox;
            messageBox.critical(nullptr,"Error","There is already a \"" + tagText + "\" tag!");
            messageBox.setFixedSize(600,200);
        }
    }
}

void MainWindow::on_button_newTag_clicked() {
    addTagFunction();
}

void MainWindow::on_button_backup_clicked() {

}

void MainWindow::on_button_newNote_clicked() {

}

void MainWindow::on_button_openArchive_clicked() {

}

void MainWindow::showContextMenu(const QPoint &pos) {
    QPoint globalPos = ui->list_tags->mapToGlobal(pos);

    QMenu myMenu;
    myMenu.addAction("Insert", this, SLOT(addTagItem()));
    myMenu.addAction("Delete",  this, SLOT(deleteTagItem()));

    myMenu.exec(globalPos);
}

void MainWindow::addTagItem() {
    addTagFunction();
}

void MainWindow::deleteTagItem() {
    int listSize = ui->list_tags->selectedItems().size();
    for (int i = 0; i < listSize; ++i) {
        QListWidgetItem *item = ui->list_tags->takeItem(ui->list_tags->currentRow());
        // Unlick all notes
        delete item;
    }
}
