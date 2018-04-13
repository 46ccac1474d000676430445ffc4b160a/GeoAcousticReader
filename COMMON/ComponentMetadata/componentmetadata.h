#ifndef COMPONENTMETADATA_H
#define COMPONENTMETADATA_H

#include <QDateTime>
#include <QFile>
#include <QFileInfo>

#include <limits>
#include <cmath>

#include <QDebug>

class ComponentMetadata
{
    QDateTime m_startTime,
              m_endTime;

    QChar m_component;

    double m_frq;

    QString m_fileName;

    QString m_errString;

    ComponentMetadata(const QString &err);

public:
    ComponentMetadata();
    ComponentMetadata(const QDateTime &start, const QDateTime &end, const QChar &component, double frq, const QString &filename);
    ComponentMetadata(const ComponentMetadata &metadata);

    ComponentMetadata& operator =(const ComponentMetadata &val);

    friend bool operator <(const ComponentMetadata &val1, const ComponentMetadata &val2);

    QChar component() const;
    void setComponent(const QChar &component);

    QDateTime startTime() const;
    void setStartTime(const QDateTime &startTime);

    QDateTime endTime() const;
    void setEndTime(const QDateTime &endTime);

    QString fileName() const;
    void setFileName(const QString &fileName);

    double frq() const;
    void setFrq(double frq);

    bool isIncomplete();

    static ComponentMetadata getMetaData(const QString &filename);

    QString errorString() const;
};

#endif // COMPONENTMETADATA_H
