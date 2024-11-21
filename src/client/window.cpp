#include "window.hpp"

MainWindow::MainWindow(const QString &username, QWidget *parent)
    : username(username)
{
    setWindowTitle("Онлайн чат");
    setFixedSize(900, 800);

    QMenuBar *menuBar = new QMenuBar(this);

    QMenu *fileMenu = new QMenu("Options", this);
    QAction *exitAction = new QAction("Exit", this);

    connect(exitAction, &QAction::triggered, qApp, &QApplication::quit);

    fileMenu->addAction(exitAction);
    menuBar->addMenu(fileMenu);

    chatDisplay = new QTextEdit(this);
    chatDisplay->setReadOnly(true);

    chatDisplay->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    messageEdit = new QLineEdit(this);
    messageEdit->setPlaceholderText("Введите сообщение");

    QPushButton *sendButton = new QPushButton("Отправить", this);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *inputLayout = new QHBoxLayout();

    mainLayout->setMenuBar(menuBar);
    mainLayout->addWidget(chatDisplay);
    inputLayout->addWidget(messageEdit);
    inputLayout->addWidget(sendButton);
    mainLayout->addLayout(inputLayout);

    setLayout(mainLayout);

    connect(sendButton, &QPushButton::clicked, this, &MainWindow::onSendClicked);
    connect(messageEdit, &QLineEdit::returnPressed, this, &MainWindow::onSendClicked);
}

void MainWindow::onSendClicked()
{
    QString message = messageEdit->text();

    if (message.isEmpty())
        return;

    QString currentTime = QDateTime::currentDateTime().toString("HH:mm");

    QString formattedMessage = "<span style='color: gray; font-size: 10pt; margin-left: 8px;'>[" +
                               currentTime + "]</span> " +
                               "<b>" + username + ":</b> " + message;

    chatDisplay->append(formattedMessage);
    messageEdit->clear();

    chatDisplay->verticalScrollBar()->setValue(chatDisplay->verticalScrollBar()->maximum());
}

regwindow::regwindow(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("Регистрация");
    setFixedSize(250, 100);

    QLabel *usernameLabel = new QLabel("Введите ваш ник", this);
    usernameEdit = new QLineEdit(this);

    QPushButton *registerButton = new QPushButton("OK", this);
    QPushButton *cancelButton = new QPushButton("Cancel", this);

    QVBoxLayout *layot = new QVBoxLayout(this);
    layot->addWidget(usernameLabel);
    layot->addWidget(usernameEdit);

    QHBoxLayout *buttonLayot = new QHBoxLayout();
    buttonLayot->addWidget(registerButton);
    buttonLayot->addWidget(cancelButton);

    layot->addLayout(buttonLayot);
    setLayout(layot);

    connect(registerButton, &QPushButton::clicked, this, &regwindow::onRegisterClicked);
    connect(cancelButton, &QPushButton::clicked, this, &regwindow::close);
    connect(usernameEdit, &QLineEdit::returnPressed, this, &regwindow::onRegisterClicked);
}

void regwindow::onRegisterClicked()
{
    QString username = usernameEdit->text();

    if (username.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, введите ваш ник");
        return;
    }

    MainWindow *mainWindow = new MainWindow(username);
    mainWindow->show();
    this->close();
}
