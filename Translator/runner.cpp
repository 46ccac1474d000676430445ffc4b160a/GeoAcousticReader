#include "runner.h"
#include <cstring>

QString Runner::convertedFilesPath() const
{
    return m_convertedFilesPath;
}

qint64 Runner::getCommonSize()
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
    emit sendStringInfo(QString("Log\nStarted: %1.\n\n").arg(QDateTime::currentDateTime().toString("dd-MM-yyyy, h:mm:ss")));

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
            openFailed = true;
            emit sendStringInfo(QString(" --- Warning: %1.\n").arg(metadata.errorString()));
        }
    }
    if (openFailed) emit sendStringInfo("\n");

    qint64 commonSize = getCommonSize() / 2;
    qint64 percent = commonSize / 100;
    emit sizeCounted(commonSize);

    qint64 commonCounter = 0;
    for (auto var : metadataList)
    {
        QFile fread(var.fileName());
        QFile fwrite;

        QString wrFileName = m_convertedFilesPath + QString("/[%1] - [%2].%3")
                .arg(var.startTime().toString("dd-MM-yyyy, hh-mm-ss"))
                .arg(var.endTime().toString("dd-MM-yyyy, hh-mm-ss"))
                .arg(fileType.toUpper().toLatin1() == 'T' ? "txt" : "bin");

        QFileInfo fileInfo(wrFileName);
        if (fileInfo.exists())
        {
            QString temp = m_convertedFilesPath + QString("/[%1] - [%2] (#).%3")
                    .arg(var.startTime().toString("dd-MM-yyyy, hh-mm-ss"))
                    .arg(var.endTime().toString("dd-MM-yyyy, hh-mm-ss"))
                    .arg(fileType.toUpper().toLatin1() == 'T' ? "txt" : "bin");
            temp.replace("#", "%1");

            int t_c = 0;
            do
            {
                fileInfo.setFile(temp.arg(++t_c));
            } while (fileInfo.exists());

            fwrite.setFileName(temp.arg(t_c));
        }
        else
        {
            fwrite.setFileName(wrFileName);
        }

        if (fread.open(QIODevice::ReadOnly))
        {
            if (fwrite.open(QIODevice::WriteOnly))
            {
                emit sendStringInfo(QString("File: \"%1\",  frq: %2,  ")
                                    .arg(fwrite.fileName())
                                    .arg(var.frq()));

                qint64 size = fread.size() / 2;

                if (fileType.toUpper().toLatin1() == 'T')
                {
                    for (int i = 0; i < size; i++)
                    {
                        QDataStream s(&fread);
                        s.setByteOrder(QDataStream::LittleEndian);
                        short t;
                        s >> t;
                        float flo = t * 2.325439e-001;
                        fwrite.write(QString().setNum(flo, 'f', 6).append("\n").toUtf8());

                        commonCounter++;
                        if (percent && commonCounter % percent == 0) emit curentElement(commonCounter);
                    }
                }
                else if (fileType.toUpper().toLatin1() == 'B')
                {
                    QDataStream stFileStream(&fwrite);
                    stFileStream.setFloatingPointPrecision(QDataStream::SinglePrecision);
                    for (int i = 0; i < size; i++)
                    {
                        QDataStream s(&fread);
                        s.setByteOrder(QDataStream::LittleEndian);
                        short t;
                        s >> t;
                        float flo = t * 2.325439e-001;
                        stFileStream << flo;

                        commonCounter++;
                        if (percent && commonCounter % percent == 0) emit curentElement(commonCounter);
                    }
                }

                emit sendStringInfo(QString("items was writen: %1,  from: %2.\n").arg(size).arg(fread.fileName()));

                fwrite.close();
            }
            else
            {
                emit sendStringInfo(QString(" --- Warning: file \"%1\" not created, problem: %2\n").arg(fwrite.fileName()).arg(fwrite.errorString()));
            }
            fread.close();
        }
        else
        {
            emit sendStringInfo(QString(" --- Warning: file \"%1\" not opened, problem :%2\n").arg(var.fileName()).arg(fread.errorString()));
        }
    }

    emit sendStringInfo(QString("\nEnd: %1.").arg(QDateTime::currentDateTime().toString("dd-MM-yyyy, h:mm:ss")));

    emit curentElement(commonSize);

    emit finished();
}
