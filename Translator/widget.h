#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QProgressBar>
#include <QTextEdit>
#include <QMessageBox>
#include <QToolButton>
#include <QFileDialog>
#include <QScrollBar>

#include <QVBoxLayout>

#include <QTimer>

#include <QThread>

#include <QCloseEvent>

#include "runner.h"

class Widget : public QWidget
{
    Q_OBJECT

    QString m_convertedFilesPath;

    QTextEdit *m_outputConsole;

    QProgressBar *m_progressBar;

    Runner *m_runner;

    QToolButton *m_saveBut;

    bool m_logSaved;

    void run(QChar c);

private slots:
    void getStringInfo(const QString &info);
    void on_runnerFinished();

protected:
    void closeEvent(QCloseEvent *event);

public:
    Widget(int argc, char *argv[], QWidget *parent = 0);
    ~Widget();

public slots:
    void saveLog();

signals:
    void started(QChar);

};

#endif // WIDGET_H
