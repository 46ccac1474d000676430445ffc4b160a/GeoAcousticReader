#include "runner.h"
#include <cstring>

QString Runner::convertedFilesPath() const
{
    return m_convertedFilesPath;
}

qint64 Runner::getCommonSize() const
{
    qint64 size = 0;
    for (QString fName : m_fileNameList)
    {
        fName[fName.size()-1] = 'a';
        QFileInfo info(fName);
        if (info.exists()) size += info.size();
    }
    return size;
}

Runner::Runner(int argc, char *argv[], QObject *parent) :
    QObject(parent),
    m_convertedFilesPath(argv[2])
{
    for (int i = 3; i < argc; i++)
    {
        m_fileNameList << argv[i];
    }

    connect(this, &Runner::started, this, &Runner::run);
    connect(this, &Runner::finished, this, &Runner::deleteLater);

}

void Runner::run(QChar fileType)
{
    //передаёт строку с описанием проводимой процессом работы, датой и временем начала работы
    emit sendStringInfo(QString("Log\nFile type: %1\nStarted: %2.\n\n")
                        .arg(fileType.toUpper().toLatin1() == 'T' ? "text" : "binary")
                        .arg(QDateTime::currentDateTime().toString("dd-MM-yyyy, h:mm:ss")));

    QList<ComponentMetadata> metadataList;

    bool openFailed = false;
    for (QString fName : m_fileNameList)
    {
        ComponentMetadata metadata = ComponentMetadata::getMetaData(fName);
        if (!metadata.isIncomplete())
        {
            metadataList << metadata;
        }
        else
        {
            m_fileNameList.removeOne(fName);
            openFailed = true;
            emit sendStringInfo(QString(" --- Warning: %1.\n").arg(metadata.errorString()));
        }
    }
    if (openFailed) emit sendStringInfo("\n");

    qint64 commonSize = getCommonSize() / 2; //вычисление общего числа дискретных значениях во всех файлах
    qint64 percent = commonSize / 100;
    emit sizeCounted(commonSize);

    qint64 commonCounter = 0; //счётчик для подсчёта обработанных дискретных значений
    for (auto var : metadataList)
    {
        QFile fread(var.fileName());
        QFile fwrite;

        //составление имени файла для записи
        QString wrFileName = m_convertedFilesPath + QString("/%1 [%2] - [%3] - %4 items.%5")
                .arg(var.component())
                .arg(var.startTime().toString("dd-MM-yyyy, hh-mm-ss"))
                .arg(var.endTime().toString("dd-MM-yyyy, hh-mm-ss"))
                .arg(fread.size()/2)
                .arg(fileType.toUpper().toLatin1() == 'T' ? "txt" : "bin");

        QFileInfo fileInfo(wrFileName);
        if (fileInfo.exists()) //проверка имени файла на уникальность
        {
            //если файл с таким именем уже существует:

            //в имя файла добавляется индекс
            QString temp = m_convertedFilesPath + QString("/%1 [%2] - [%3] - %4 items (#).%5")
                    .arg(var.component())
                    .arg(var.startTime().toString("dd-MM-yyyy, hh-mm-ss"))
                    .arg(var.endTime().toString("dd-MM-yyyy, hh-mm-ss"))
                    .arg(fread.size()/2)
                    .arg(fileType.toUpper().toLatin1() == 'T' ? "txt" : "bin");
            temp.replace("#", "%1");

            //в имя файла с индексом, пока не будет найдено имя ещё не существующего файла
            int t_c = 1;
            do
            {
                fileInfo.setFile(temp.arg(++t_c));
            } while (fileInfo.exists());

            fwrite.setFileName(temp.arg(t_c));
        }
        else
        {
            //если файл с таким именем ещё не существует:
            fwrite.setFileName(wrFileName);
        }

        if (fread.open(QIODevice::ReadOnly))
        {
            if (fwrite.open(QIODevice::WriteOnly))
            {
                //передаёт строку с именем, компонентой и частотой фаила перед началом записи
                emit sendStringInfo(QString("File: \"%1\", comp: %2, frq: %3, ")
                                    .arg(fwrite.fileName())
                                    .arg(var.component())
                                    .arg(var.frq()));

                qint64 size = fread.size() / 2;

                //если производится запись в файл в текстовом формате:
                if (fileType.toUpper().toLatin1() == 'T')
                {
                    QDataStream readFileStream(&fread);
                    //установка порядка чтения байт
                    readFileStream.setByteOrder(QDataStream::LittleEndian);
                    for (int i = 0; i < size; i++)
                    {
                        short t;
                        readFileStream >> t; //чтение дискретног значения
                        float flo = t * 2.325439e-001;
                        fwrite.write(QString().setNum(flo, 'f', 6).append("\n").toUtf8());

                        commonCounter++; //инкремент счётчика для подсчёта обработанных дискретных значений
                        if (percent && commonCounter % percent == 0) emit curentElement(commonCounter);
                    }
                }
                //если производится запись в файл в бинарном формате:
                else if (fileType.toUpper().toLatin1() == 'B')
                {
                    QDataStream stFileStream(&fwrite);
                    //установка порядка чтения байт
                    stFileStream.setByteOrder(QDataStream::LittleEndian);
                    //установка точности записываемого числа
                    stFileStream.setFloatingPointPrecision(QDataStream::SinglePrecision);

                    QDataStream readFileStream(&fread);
                    readFileStream.setByteOrder(QDataStream::LittleEndian);
                    for (int i = 0; i < size; i++)
                    {
                        short t;
                        readFileStream >> t;
                        float flo = t * 2.325439e-001;
                        stFileStream << flo;

                        commonCounter++; //инкремент счётчика для подсчёта обработанных дискретных значений
                        //если счётчик посчёта кол-ва обработанных дискретных значений изменился на 1 процент
                        if (percent && commonCounter % percent == 0)
                            //то послать сигнал с числом (от общего числа) уже обработанных дискретных значений
                            emit curentElement(commonCounter);
                    }
                }

                //передаёт строку с числом записаных дискретных значений
                emit sendStringInfo(QString("items was writen: %1,  from: %2.\n").arg(size).arg(fread.fileName()));

                fwrite.close();
            }
            else
            {
                //передаёт строку об ошибке создания файла
                emit sendStringInfo(QString(" --- Warning: file \"%1\" not created, problem: %2\n").arg(fwrite.fileName()).arg(fwrite.errorString()));
            }
            fread.close();
        }
        else
        {
            //передаёт строку об ошибке открытия файла
            emit sendStringInfo(QString(" --- Warning: file \"%1\" not opened, problem :%2\n").arg(var.fileName()).arg(fread.errorString()));
        }
    }

    //передаёт строку о завершении обработки данных и временем завершения
    emit sendStringInfo(QString("\nEnd: %1.").arg(QDateTime::currentDateTime().toString("dd-MM-yyyy, h:mm:ss")));

    emit curentElement(commonSize);

    emit finished(); //сигнал о завершении работы потока
}
