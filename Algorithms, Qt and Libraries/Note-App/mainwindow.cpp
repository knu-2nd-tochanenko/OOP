#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "utilityclass.h"
#include "archivednotes.h"
#include "singlenoteview.h"
#include "singlenote.h"
#include "notedialog.h"
#include "exportdialog.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>
#include <QTime>
#include <QDate>
#include <QFileDialog>
#include <QMenu>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);



    // Right button for list
    ui->list_tags->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(ui->list_tags, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));

    ui->listWidget_notes->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listWidget_notes, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showListMenu(QPoint)));

    connect(ui->listWidget_notes, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(editNote()));
    connect(ui->list_tags, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(editTagItem()));

    ui->list_tags->addItem("uncategorized");
    ui->list_tags->addItem("work");
    ui->list_tags->addItem("personal");
    this->tags.push_back("work");
    this->tags.push_back("personal");

    if (!readJSON("backup.json")) {
        // If the is no safe file
        this->maxID = 0;
        qDebug() << "There is no file\n";
    }

    currentTheme = 0;
    runInterface();
}

MainWindow::~MainWindow() {
    if (!writeJSON("backup.json")) {
        qDebug()<<"Backup was corrupted!\n";
    }
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
        if (!hasSame) {
            ui->list_tags->addItem(tagText);
            this->tags.push_back(tagText);
        }
        else {
            // If tag is already available
            UtilityClass::error("There is already a \"" + tagText + "\" tag!");
        }
    }
}

void MainWindow::on_button_newTag_clicked() {
    addTagFunction();
}
/*
void MainWindow::on_button_export_clicked() {
    // Choose backup folder
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "/home",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    dir += "backup.txt";

    QFile backupFile(dir);
    backupFile.open(QFile::WriteOnly | QFile::Text);
    QTextStream out(&backupFile);

    // Write all content:
    out << "Main Notes:\n";
    int notesSize = this->notes.size();
    for (int i = 0; i < notesSize; i++) {
        out << this->notes[i]->getText() << "\n";
        out << "ID : " << this->notes[i]->getID() << "\n";
        out << "Tags : " << this->notes[i]->getTags() << "\n";
        out << "Edited on : " << this->notes[i]->getEditedTime().toString(Qt::TextDate)
            << " " << this->notes[i]->getEditedDate().toString(Qt::TextDate) << "\n";
        out << "Created on : " << this->notes[i]->getCreationTime().toString(Qt::TextDate)
            << " " << this->notes[i]->getCreationDate().toString(Qt::TextDate) << "\n";
        out << "----------------\n\n";
    }

    out << "Archived Notes:\n";
    int archiveSize = this->archive.size();
    for (int i = 0; i < archiveSize; i++) {
        out << this->archive[i]->getText() << "\n";
        out << "ID : " << this->archive[i]->getID() << "\n";
        out << "Tags : " << this->archive[i]->getTags() << "\n";
        out << "Edited on : " << this->archive[i]->getEditedTime().toString(Qt::TextDate)
            << " " << this->archive[i]->getEditedDate().toString(Qt::TextDate) << "\n";
        out << "Created on : " << this->archive[i]->getCreationTime().toString(Qt::TextDate)
            << " " << this->archive[i]->getCreationDate().toString(Qt::TextDate) << "\n";
        out << "----------------\n\n";
    }

    out << "All tags : uncategorized, work, personal";
    int tagsSize = this->tags.size();
    for (int i = 0; i < tagsSize; i++) {
        out << ", " << this->tags[i];
    }

    backupFile.flush();
    backupFile.close();
}
//*/
void MainWindow::on_button_newNote_clicked() {
    QTime creatingTime = QTime::currentTime();
    QDate creationDate = QDate::currentDate();
    QString text = "";
    QStringList noteTags;
    int ID = this->maxID + 1;
    this->maxID++;
    SingleNote *sn = new SingleNote(ID, creatingTime, creationDate, text, noteTags);
    noteDialog *nd = new noteDialog(this);
    nd->addNote(sn);
    nd->addtags(this->tags);
    nd->exec();
    if (sn->getText() == "") {
        // Don't need to do anything
    }
    else {
        this->notes.push_back(sn);
    }
    UtilityClass::debugNote(sn);
    runInterface();
}

void MainWindow::showContextMenu(const QPoint &pos) {
    QPoint globalPos = ui->list_tags->mapToGlobal(pos);

    QMenu myMenu;
    myMenu.addAction("New tag", this, SLOT(addTagItem()));
    myMenu.addAction("Edit", this, SLOT(editTagItem()));
    if (!this->filter.contains(ui->list_tags->selectedItems()[0]->text()))
        myMenu.addAction("Add to filter", this, SLOT(addTagToFilter()));
    else
        myMenu.addAction("Remove from filter", this, SLOT(removeTagFromFilter()));
    myMenu.addAction("Delete",  this, SLOT(deleteTagItem()));

    myMenu.exec(globalPos);
}

void MainWindow::showListMenu(const QPoint &pos) {
    QPoint globalPos = ui->listWidget_notes->mapToGlobal(pos);

    QMenu myMenu;
    myMenu.addAction("Edit Note", this, SLOT(editNote()));
    myMenu.addAction("Move to archive", this, SLOT(moveToArchive()));
    myMenu.addAction("Delete Note",  this, SLOT(deleteNote()));

    myMenu.exec(globalPos);
}

void MainWindow::addTagItem() {
    addTagFunction();
}

void MainWindow::editTagItem() {
    int listSize = ui->list_tags->selectedItems().size();
    for (int k = 0; k < listSize; ++k) {
        if (ui->list_tags->selectedItems()[k]->text() == "uncategorized"
                || ui->list_tags->selectedItems()[k]->text() == "work"
                || ui->list_tags->selectedItems()[k]->text() == "personal") {
            UtilityClass::error("You can't edit \"" + ui->list_tags->selectedItems()[k]->text() + "\" tag!");
        }
        else {
            QString tagToEdit = ui->list_tags->selectedItems()[k]->text();
            int tagSize = tags.size();
            for (int i = 0; i < tagSize; i++)
                if (tags[i] == tagToEdit) {
                    bool ok;
                    QString tagText = QInputDialog::getText(
                                this,
                                tr("Edit Tag"),
                                tr("Tag name:"),
                                QLineEdit::Normal,
                                ui->list_tags->selectedItems()[k]->text(), &ok);
                    tagText = tagText.toLower();

                    if (ok && !tagText.isEmpty()) {
                        // Chek for similar tag
                        int numberOfTags = ui->list_tags->count();
                        bool hasSame = false;
                        for (int i = 0; i < numberOfTags; i++)
                            if (ui->list_tags->item(k)->text() == tagText)
                                hasSame = true;
                        if (!hasSame) {

                            int notesSize = this->notes.size();
                            for (int i = 0; i < notesSize; i++)
                                if (notes[i]->checkForTag(tagToEdit)) {
                                    notes[i]->deleteTag(tagToEdit);
                                    notes[i]->addTag(tagText);
                                }

                            for (int j = 0; j < tagSize; j++) {
                                if (tags[j] == tagToEdit) {
                                    tags[j] = tagText;
                                    ui->list_tags->selectedItems()[k]->setText(tagText);
                                }
                            }
                        }
                        else if (!ok) {
                            // If tag is already available
                            UtilityClass::error("There is already a \"" + tagText + "\" tag!");
                        }
                    }
                    break;
                }
        }
    }
    runInterface();
}

void MainWindow::addTagToFilter() {
    if (ui->list_tags->selectedItems()[0]->text() == "uncategorized" && filter.size() > 0)
        filter.clear();
    else {
        int listSize = ui->list_tags->selectedItems().size();
        for (int i = 0; i < listSize; ++i) {
            if (!filter.contains(ui->list_tags->selectedItems()[i]->text()))
                filter.push_back(ui->list_tags->selectedItems()[i]->text());
            else {
                UtilityClass::error("There is already \"" + ui->list_tags->selectedItems()[i]->text() + "\" tag!");
            }
        }
    }
    runInterface();
}

void MainWindow::removeTagFromFilter() {
    if (ui->list_tags->selectedItems()[0]->text() == "uncategorized" && filter.size() > 0)
        filter.clear();
    else {
        int listSize = ui->list_tags->selectedItems().size();
        for (int i = 0; i < listSize; ++i) {
            if (filter.contains(ui->list_tags->selectedItems()[i]->text()))
                for (int j = 0; j < filter.size(); j++)
                    if (filter[j] == ui->list_tags->selectedItems()[i]->text()) {
                        filter.erase(filter.begin() + j);
                        break;
                    }
                    else continue;
            else {
                UtilityClass::error("There is no \"" + ui->list_tags->selectedItems()[i]->text() + "\" tag in filter!");
            }
        }
    }
    runInterface();
}

void MainWindow::editNote() {
    int listSize = ui->listWidget_notes->selectedItems().size();
    for (int z = 0; z < listSize; z++) {

        int editID = -1;
        int listWidgetSize = ui->listWidget_notes->count();
        for (int i = 0; i < listWidgetSize; i++)
            if (ui->listWidget_notes->item(i) == ui->listWidget_notes->selectedItems()[z]) {
                editID = (qobject_cast<singleNoteView*>(ui->listWidget_notes->itemWidget(ui->listWidget_notes->item(ui->listWidget_notes->currentRow()))))->getID();
            }
        if (editID == -1) {
            UtilityClass::error("Oops! Something went wrong...");
        }

        int notesSize = notes.size();
        SingleNote *noteToEdit = new SingleNote();

        for (int i = 0; i < notesSize; i++)
            if (notes[i]->getID() == editID) {
                noteToEdit = notes[i];
                break;
            }
        QTime creationTime = noteToEdit->getCreationTime();
        QDate creationDate = noteToEdit->getCreationDate();

        QString text = noteToEdit->getText();

        QStringList noteTags = noteToEdit->getTagsList();
        int ID = noteToEdit->getID();
        SingleNote *sn = new SingleNote(ID, creationTime, creationDate, text, noteTags);
        noteDialog *nd = new noteDialog(this);
        nd->addNote(sn);
        nd->addtags(this->tags);
        nd->exec();
        sn = nd->getExecData();
        if (sn->getText() == "") {
            int notesSize = notes.size();
            for (int i = 0; i < notesSize; i++)
                if (this->notes[i]->getID() == sn->getID()) {
                    // Remove item
                    this->notes.erase(this->notes.begin() + i);
                    break;
                }
        }
        else {
            noteToEdit->clearTags();
            noteToEdit->setText(sn->getText());
            QStringList tagsListNote = sn->getTagsList();
            for (int i = 0; i < tagsListNote.size(); i++)
                if (!noteToEdit->checkForTag(tagsListNote[i]))
                    noteToEdit->addTag(tagsListNote[i]);
            noteToEdit->setEditedTime(sn->getEditedTime());
            noteToEdit->setEditedDate(sn->getEditedDate());
        }
        UtilityClass::debugNote(sn);
    }
    runInterface();
}

void MainWindow::deleteNote() {
    int listSize = ui->listWidget_notes->selectedItems().size();
    for (int z = 0; z < listSize; z++) {
        if (UtilityClass::checkYN("Do you want to delete this note?", "Delete note")) {
            int deleteID = -1;
            int listWidgetSize = ui->listWidget_notes->count();
            for (int i = 0; i < listWidgetSize; i++)
                if (ui->listWidget_notes->item(i) == ui->listWidget_notes->selectedItems()[z]) {
                    deleteID = (qobject_cast<singleNoteView*>(ui->listWidget_notes->itemWidget(ui->listWidget_notes->item(ui->listWidget_notes->currentRow()))))->getID();
                }

            int notesSize = notes.size();

            for (int i = 0; i < notesSize; i++)
                if (notes[i]->getID() == deleteID) {
                    notes.erase(notes.begin() + i);
                    break;
                }
            runInterface();
        }
    }
}

void MainWindow::moveToArchive() {
    int listSize = ui->listWidget_notes->selectedItems().size();
    for (int z = 0; z < listSize; z++) {
        if (UtilityClass::checkYN("Do you want to move note to archive?", "Move to archive")) {
            int archiveID = -1;
            int listWidgetSize = ui->listWidget_notes->count();
            for (int i = 0; i < listWidgetSize; i++)
                if (ui->listWidget_notes->item(i) == ui->listWidget_notes->selectedItems()[z]) {
                    archiveID = (qobject_cast<singleNoteView*>(ui->listWidget_notes->itemWidget(ui->listWidget_notes->item(ui->listWidget_notes->currentRow()))))->getID();
                }

            int notesSize = notes.size();

            for (int i = 0; i < notesSize; i++)
                if (notes[i]->getID() == archiveID) {
                    archive.push_back(notes[i]);
                    notes.erase(notes.begin() + i);
                    break;
                }
            runInterface();
        }
    }
}

void MainWindow::on_actionOpen_Archive_triggered() {
    ArchivedNotes *an = new ArchivedNotes();
    an->setNotes(this->archive);
    an->exec();

    QVector<int> deletedNotes = an->getDeletedNotes();
    QVector<int> unarchivedNotes = an->getUnarchivedNotes();

    if (deletedNotes.size() > 0)
        for (int j = 0; j < deletedNotes.size(); j++)
            for (int i = 0; i < archive.size(); i++) {
                if (this->archive[i]->getID() == deletedNotes[j]) {
                    archive.erase(archive.begin() + i);
                    break;
                }
            }
    if (unarchivedNotes.size() > 0)
        for (int j = 0; j < unarchivedNotes.size(); j++)
            for (int i = 0; i < archive.size(); i++) {
                if (this->archive[i]->getID() == unarchivedNotes[j]) {
                    notes.push_back(archive[i]);
                    archive.erase(archive.begin() + i);
                    break;
                }
            }
    runInterface();
}

void MainWindow::on_actionExport_to_triggered() {
    this->writeJSON("backup.json");
    ExportDialog* exportDialog = new ExportDialog(this);
    exportDialog->exec();
    qDebug() << "Export...";
}

void MainWindow::on_actionStandard_triggered() {
    setTheme(0);
}

void MainWindow::on_actionDark_triggered() {
    setTheme(1);
}

void MainWindow::on_actionOrange_triggered() {
    setTheme(2);
}

bool MainWindow::readJSON(QString filePath) {
    bool correctJSON = true;
    QFile readNotesFile;
    readNotesFile.setFileName(filePath);
    if (!readNotesFile.open(QFile::ReadOnly | QFile::Text))
        return false;

    QString jsonData = readNotesFile.readAll();
    QJsonDocument document = QJsonDocument::fromJson(jsonData.toUtf8());
    QJsonObject jsonObject = document.object();

    // Get array of notes
    if (jsonObject.contains("notes") && jsonObject["notes"].isArray()) {
        QJsonArray jsonArray = jsonObject["notes"].toArray();
        int arraySize = jsonArray.count();
        for (int i = 0; i < arraySize; i++) {
            SingleNote *note = new SingleNote(jsonArray[i].toObject());
            this->notes.push_back(note);
        }
    }
    else {
        correctJSON = false;
    }

    // Get array of archived notes
    if (jsonObject.contains("archive") && jsonObject["archive"].isArray()) {
        QJsonArray jsonArray = jsonObject["archive"].toArray();
        int arraySize = jsonArray.count();
        for (int i = 0; i < arraySize; i++) {
            SingleNote *note = new SingleNote(jsonArray[i].toObject());
            this->archive.push_back(note);
        }
    }
    else {
        correctJSON = false;
    }


    // Get max ID
    if (jsonObject.contains("max_id") && jsonObject["max_id"].isDouble())
        this->maxID = jsonObject["max_id"].toInt();
    else {
        correctJSON = false;
    }

    // Get list of new tags
    if (jsonObject.contains("tags") && jsonObject["tags"].isArray()) {
        QJsonArray tagArray = jsonObject["tags"].toArray();
        int tagsSize = tagArray.size();
        for (int i = 0; i < tagsSize; i++) {
            this->tags.push_back(tagArray[i].toString());
            ui->list_tags->addItem(tagArray[i].toString());
        }
    }

    readNotesFile.close();
    return true;
}

bool MainWindow::writeJSON(QString filePath) {
    QFile writeNotesFile;
    writeNotesFile.setFileName(filePath);
    if (!writeNotesFile.open(QFile::WriteOnly))
        return false;

    // TODO : Change ID's and maxID (or not)

    // Add notes
    QJsonArray notesArray;
    int notesSize = notes.size();
    QJsonObject singleNoteObject;
    for (int i = 0; i < notesSize; i++) {
        notes[i]->writeJSON(singleNoteObject);
        notesArray.push_back(singleNoteObject);
    }

    // Add archived notes
    QJsonArray archiveArray;
    int archiveSize = archive.size();
    QJsonObject archiveObject;
    for (int i = 0; i < archiveSize; i++) {
        archive[i]->writeJSON(archiveObject);
        archiveArray.push_back(archiveObject);
    }

    // Add tags
    QJsonObject finalObject;
    QJsonArray tagArray;
    int tagArraySize = this->tags.size();
    for (int i = 0; i < tagArraySize; i++) {
        if (this->tags[i] != "work" && this->tags[i] != "personal" && this->tags[i] != "uncategorized")
            tagArray.push_back(this->tags[i]);
    }

    finalObject["notes"] = notesArray;
    finalObject["max_id"] = this->maxID;
    finalObject["tags"] = tagArray;
    finalObject["archive"] = archiveArray;

    QJsonDocument document(finalObject);
    writeNotesFile.write(document.toJson());
    writeNotesFile.close();
    return true;
}

void MainWindow::updateList() {
    // TODO : Add sorting
    int notesSize = this->notes.size();
    int filterSize = filter.size();
    QVector<SingleNote*> noteList;

    if (filter.size() == 0) {
        noteList = this->notes;
    }
    else {
        bool hasAllTags;
        for (int i = 0; i < notesSize; i++) {
            hasAllTags = true;
            for (int j = 0; j < filterSize; j++)
                if (!notes[i]->checkForTag(filter[j]))
                    hasAllTags = false;
            if (hasAllTags)
                noteList.push_back(notes[i]);
        }
    }

    int noteListSize = noteList.size();
    ui->listWidget_notes->clear();
    for (int i = 0; i < noteListSize; i++) {
        // Add single widget
        QListWidgetItem *listWidgetItem = new QListWidgetItem(ui->listWidget_notes);
        ui->listWidget_notes->addItem(listWidgetItem);
        singleNoteView *snv = new singleNoteView;
        snv->setStyleSheet("singleNoteView {border-bottom: 1px solid #BDBDBD}");
        snv->setNote(noteList[i]);
        listWidgetItem->setSizeHint(QSize(snv->sizeHint().width(), 85));
        ui->listWidget_notes->setItemWidget(listWidgetItem, snv);
    }
}

void MainWindow::runInterface() {
    updateList();
    QString labelText = "Notes :";
    if (filter.size() != 0) {
        for (int i = 0; i < filter.size(); i++) {
            labelText += " " + filter[i];
        }
    }
    ui->label_notes->setText(labelText);
}

void MainWindow::setTheme(int number) {
    if (number == 0) {
        qApp->setStyleSheet(styleSheet());
        return;
    }

    QString fileName;
    if (number == 1)
        fileName = "themes/style.qss";
    if (number == 2)
        fileName = "themes/orange.qss";

    // Apply theme
    QFile f(fileName);
    if (!f.exists())
        printf("Unable to set stylesheet, file not found\n");
    else {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        qApp->setStyleSheet(ts.readAll());
    }
}

void MainWindow::deleteTagItem() {
    int listSize = ui->list_tags->selectedItems().size();
    for (int i = 0; i < listSize; ++i) {
        if (UtilityClass::checkYN("Do you want to delete this tag?", "Delete tag")) {
            if (ui->list_tags->selectedItems()[i]->text() == "uncategorized"
                    || ui->list_tags->selectedItems()[i]->text() == "work"
                    || ui->list_tags->selectedItems()[i]->text() == "personal") {
                UtilityClass::error("You can't delete \"" + ui->list_tags->selectedItems()[i]->text() + "\" tag!");
            }
            else {
                QListWidgetItem *item = ui->list_tags->takeItem(ui->list_tags->currentRow());
                int tagSize = tags.size();
                for (int j = 0; j < tagSize; j++)
                    if (tags[j] == item->text()) {
                        tags.erase(tags.begin() + j);
                        break;
                    }

                // Unlinck all notes from deleted tag
                int notesSize = notes.size();
                int archiveSize = archive.size();
                for (int j = 0; j < notesSize; j++)
                    notes[j]->deleteTag(item->text());
                for (int j = 0; j < archiveSize; j++)
                    archive[j]->deleteTag(item->text());

                delete item;
            }
        }
    }
    updateList();
}
