#include "widget.h"

void Widget::run(QChar c)
{
    QTimer *timer = new QTimer();
    timer->setSingleShot(true);
    timer->setInterval(777);

    connect(timer, &QTimer::timeout, timer, &QTimer::deleteLater);
    connect(timer, &QTimer::timeout, [=](){
        emit started(c);
    });

    timer->start();
}

void Widget::getStringInfo(const QString &info)
{
    m_outputConsole->setPlainText(m_outputConsole->toPlainText() + info);
    m_outputConsole->verticalScrollBar()->setValue(m_outputConsole->verticalScrollBar()->maximum());
}

void Widget::on_runnerFinished()
{
    QMessageBox::information(this, "Info", "Process finished!");
    m_saveBut->show();
}

void Widget::closeEvent(QCloseEvent *event)
{
    if (!m_logSaved)
    {
        int answer = QMessageBox::warning(this, "Confirm", "Are you sure you want to save log to file?", QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel);

        if (answer == QMessageBox::Yes)
        {
            saveLog();
        }
        else if (answer == QMessageBox::Cancel)
        {
            event->ignore();
            return;
        }
    }

    QWidget::closeEvent(event);
}

Widget::Widget(int argc, char *argv[], QWidget *parent) :
    QWidget(parent, Qt::Dialog),
    m_stitchedFilesPath(argc >= 2 ? argv[2] : nullptr),
    m_outputConsole(new QTextEdit()),
    m_progressBar(new QProgressBar()),
    m_saveBut(new QToolButton()),
    m_logSaved(false)
{
    if (argc < 4) throw std::runtime_error("Invalid argc value");

    thread()->setPriority(QThread::HighestPriority);

    setWindowTitle(argv[2]);

    m_outputConsole->setReadOnly(true);

    m_progressBar->setRange(0, 0);
    m_progressBar->setMinimumHeight(70);

    m_saveBut->hide();
    m_saveBut->setIcon(QIcon(QPixmap(":/save.png")));
    m_saveBut->setIconSize(QSize(70, 70));
    m_saveBut->setMinimumSize(70, 70);
    m_saveBut->setToolTip("Save log");

    QHBoxLayout *hlay = new QHBoxLayout();
    hlay->addWidget(m_progressBar);
    hlay->addWidget(m_saveBut);

    QVBoxLayout *vlay = new QVBoxLayout();
    vlay->addWidget(m_outputConsole);
    vlay->addLayout(hlay);

    setLayout(vlay);

    setMinimumSize(480, 200);

    QThread *t = new QThread();

    m_runner = new Runner(argc, argv);
    m_runner->setParent(0);
    m_runner->moveToThread(t);
    connect(m_runner, &Runner::finished, t, &QThread::quit, Qt::DirectConnection);
    connect(t, &QThread::finished, t, &QThread::quit, Qt::DirectConnection);

    t->start(QThread::LowestPriority);

    connect(m_runner, &Runner::sizeCounted, m_progressBar, &QProgressBar::setMaximum);
    connect(m_runner, &Runner::curentElement, m_progressBar, &QProgressBar::setValue);
    connect(m_runner, &Runner::sendStringInfo, this, &Widget::getStringInfo);
    connect(m_runner, &Runner::finished, this, &Widget::on_runnerFinished);

    connect(this, &Widget::started, m_runner, &Runner::started);

    connect(m_saveBut, &QToolButton::clicked, this, &Widget::saveLog);

    run(*argv[1]);
}

Widget::~Widget()
{
}

void Widget::saveLog()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save as...", m_stitchedFilesPath + QString("/%1.log").arg(QDateTime::currentDateTime().toString("dd-MM-yyyy, hh-mm-ss")));

    if (!fileName.isEmpty())
    {
        QFile f(fileName);
        if (f.open(QIODevice::WriteOnly))
        {
            f.write(m_outputConsole->toPlainText().toUtf8());

            f.close();

            m_logSaved = true;
        }
        else
        {
            QMessageBox::warning(this, "Error", QString("Can not create file\"%1\"").arg(fileName));
        }
    }
}


