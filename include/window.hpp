#pragma once
#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QLabel>
#include <QMenuBar>
#include <QAction>
#include <QApplication>
#include <QScrollBar>
#include <QDateTime>

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(const QString &username, QWidget *parent = nullptr);

private slots:
    void onSendClicked();

private:
    QString username;
    QTextEdit *chatDisplay;
    QLineEdit *messageEdit;
};

class regwindow : public QWidget
{
    Q_OBJECT

public:
    explicit regwindow(QWidget *parent = nullptr);

private slots:
    void onRegisterClicked();

private:
    QLineEdit *usernameEdit;
};
#endif // WINDOW_H
