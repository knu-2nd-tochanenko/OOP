#include "exportdialog.h"
#include "ui_exportdialog.h"

#include <QFileDialog>
#include <QDebug>
#include <QProcess>
#include <QStackedWidget>
#include <fstream>
#include <QMessageBox>
#include "utilityclass.h"

using std::ifstream;

ExportDialog::ExportDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportDialog) {
    ui->setupUi(this);
}

ExportDialog::~ExportDialog() {
    delete ui;
}

void ExportDialog::on_buttonCancel_clicked() {
    this->close();
}

void ExportDialog::on_buttonCreate_clicked() {
    QString ownerPassword, userPassword, file, path;
    ownerPassword = ui->ownerPassword->text();
    userPassword = ui->userPassword->text();
    file = ui->fileName->text();
    path = ui->path->text();
    if (file == "" || path == "") {
        UtilityClass::error("You need to fill all forms with *!");
        ui->path->setText("");
        ui->fileName->setText("");
        return;
    }
    if (((userPassword == ownerPassword) && userPassword != "") ||
            (userPassword == "" && ownerPassword != "") || (userPassword != "" && ownerPassword == "")) {
        UtilityClass::error("Both passwords must be filled and different!");
        ui->userPassword->setText("");
        ui->ownerPassword->setText("");
        return;
    }

    QString fullPath = path;
    fullPath += file + ".pdf";

    generatePDF(fullPath, ownerPassword, userPassword);
}

void ExportDialog::on_buttonPath_clicked() {
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "/home",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if (dir[dir.size() - 1] != '/')
        dir += "/";
    ui->path->setText(dir);
}

void ExportDialog::on_userPassword_textChanged() {
    update();
}

void ExportDialog::on_ownerPassword_textChanged() {
    update();
}

bool ExportDialog::checkPassword(const QString &password) const {
    for (int i = 0; i < password.size(); i++)
        if (!(
            ((password[i] >= 'a') && (password[i] <= 'z')) ||
            ((password[i] >= 'A') && (password[i] <= 'Z')) ||
            ((password[i] >= '0') && (password[i] <= '9')) ||
            (password[i] == '!') ||
            (password[i] == '@') ||
            (password[i] == '#') ||
            (password[i] == '$') ||
            (password[i] == '%') ||
            (password[i] == '&') ||
            (password[i] == '*') ||
            (password[i] == '(') ||
            (password[i] == ')') ))
            return false;
    return true;
}

bool ExportDialog::checkFileName(const QString &fileName) const {
    return true;
}

bool ExportDialog::checkPathName(const QString &pathName) const {
    return true;
}

bool ExportDialog::doesExist(const QString &fullPath) {
    return true;
}

void ExportDialog::generatePDF(const QString &fullPath, const QString &adminPassword, const QString &userPassword) {
    if (adminPassword != "" || userPassword != "") {
        QString command = "pdf/JsonToPdf.exe " + fullPath + " " + adminPassword + " " + userPassword;
        qDebug() << command;
        QProcess::execute(command);
    }
    else {
        //  Generate without password
        QString command = "pdf/JsonToPdf.exe " + fullPath;
        qDebug() << command;
        QProcess::execute(command);
    }
    this->close();
}

void ExportDialog::update() {
    QString ownerPassword, userPassword, file, path;
    QPalette *palette = new QPalette();
    ownerPassword = ui->ownerPassword->text();
    userPassword = ui->userPassword->text();
    file = ui->fileName->text();
    path = ui->path->text();

    //  Owner Password
    if (!checkPassword(ownerPassword)) {
        palette->setColor(QPalette::Text,Qt::red);
        ui->ownerPassword->setPalette(*palette);
    }
    else {
        palette->setColor(QPalette::Text,Qt::black);
        ui->ownerPassword->setPalette(*palette);
    }
    //  User Password
    if (!checkPassword(userPassword)) {
        palette->setColor(QPalette::Text,Qt::red);
        ui->userPassword->setPalette(*palette);
    }
    else {
        palette->setColor(QPalette::Text,Qt::black);
        ui->userPassword->setPalette(*palette);
    }
}
