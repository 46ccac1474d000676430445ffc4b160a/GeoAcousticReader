#ifndef COMPONENTMETADATA_H
#define COMPONENTMETADATA_H

#include <QDateTime>
#include <QFile>
#include <QFileInfo>

#include <limits>
#include <cmath>

#include <QDebug>

//Класс, хранящий информация о бинарных файлах
class ComponentMetadata
{
    QDateTime m_startTime, //дата начала записи
              m_endTime; //дата окончания записи

    QChar m_component; //компонента, по которой записывался файл

    double m_frq; //частота дискретизации

    QString m_fileName; //полное имя файла

    QString m_errString; //строка описания ошибки

    ComponentMetadata(const QString &err); //конструктор, принимающий строку ошибки

public:
    ComponentMetadata();
    ComponentMetadata(const QDateTime &start, const QDateTime &end, const QChar &component, double frq, const QString &filename); //конструктор, инициализирующий все поля, кроме поля ошибки
    ComponentMetadata(const ComponentMetadata &metadata);

    ComponentMetadata& operator =(const ComponentMetadata &val);

    friend bool operator <(const ComponentMetadata &val1, const ComponentMetadata &val2);

//методы доступа
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

    bool isIncomplete(); //проверка на полноту хранящихся данных

    static ComponentMetadata getMetaData(const QString &filename); //генерирует обЪект по имени лог-файла

    QString errorString() const;
};

#endif // COMPONENTMETADATA_H
