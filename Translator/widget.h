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

    QString m_convertedFilesPath; //имя папки, в кототорую будут сохраняться файлы

    QTextEdit *m_outputConsole; //консоль вывода лога

    QProgressBar *m_progressBar; //отображение прогресса

    Runner *m_runner; //обЪект для работы в потоке обработки данных

    QToolButton *m_saveBut; //кнопка сохранения лога в файл

    bool m_logSaved; //флаг, сигнализирует о том, сохранён ли файл

    void run(QChar c); //метод запуска процесса обработки, принимает формат записи файлов ('B', 'T')

private slots:
    void getStringInfo(const QString &info); //выводит строку в консоль логов
    void on_runnerFinished(); //обрабатывает событие завершения работы потока обработки данных

protected:
    void closeEvent(QCloseEvent *event); //переопределённый метод обработки события закрытия окна

public:
    Widget(int argc, char *argv[], QWidget *parent = 0); //конструктор, принимает аргументы командной строки
    ~Widget();

public slots:
    void saveLog(); //слот обработки сигнала о сохранении лог-файла

signals:
    void started(QChar); //сигнал о начале обработки данных

};

#endif // WIDGET_H
