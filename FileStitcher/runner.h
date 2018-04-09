#ifndef STITCHER_H
#define STITCHER_H

#include <QObject>

#include <QFileInfo>
#include <QFile>
#include <QDataStream>

#include "../COMMON/ComponentMetadata/componentmetadata.h"

#include <QMessageBox>

class Runner : public QObject
{
    Q_OBJECT

    QString m_stitchedFilesPath;

    QStringList m_fileNameList;

    qint64 getCommonSize();

public:
    explicit Runner(int argc, char *argv[], QObject *parent = nullptr);

    QString stitchedFilesPath() const;

public slots:
    void run(QChar fileType);

signals:
    void started(QChar c);

    void sizeCounted(qint64);

    void curentElement(qint64);

    void finished();

    void sendStringInfo(const QString &);

};

#endif // STITCHER_H
