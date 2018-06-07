#ifndef STITCHER_H
#define STITCHER_H

#include <QObject>

#include <QFileInfo>
#include <QFile>
#include <QDataStream>

#include "../COMMON/ComponentMetadata/componentmetadata.h"

#include <QMessageBox>

//класс обработки и записи данных
class Runner : public QObject
{
    Q_OBJECT

    QString m_convertedFilesPath; //имя папки, в кототорую будут сохраняться файлы

    QStringList m_fileNameList; //список лог-файлов, по ним будут найдены и обработаны бинарные файлы

    qint64 getCommonSize() const; //размер всех бинарных файлов, которые будут обработаны

public:
    explicit Runner(int argc, char *argv[], QObject *parent = nullptr);

    QString convertedFilesPath() const;

public slots:
    void run(QChar fileType); //основная функция, выполняемая в потоке

signals:
    void started(QChar fileType); //сигнал о начале работы, передаёт флаг о формате записываемого файла

    void sizeCounted(qint64); //сигнал, передающий общее число дискретных значений, которые будут обработаны

    void curentElement(qint64); //сигнал, передающий число (от общего числа) уже обработанных дискретных значений

    void finished(); //сигнал о завершении работы потока

    void sendStringInfo(const QString &); //сигнал, передающий логи

};

#endif // STITCHER_H
