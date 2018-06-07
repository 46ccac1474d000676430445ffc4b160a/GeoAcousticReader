#include "componentmetadata.h"


ComponentMetadata ComponentMetadata::getMetaData(const QString &filename)
{
    QFile f(filename);
    if (f.open(QIODevice::ReadOnly))
    {
        QString fileContent(f.read( f.size() > 4096 ? 4096 : f.size() )); //считывание содержимого лог-файла
        f.close();


//  ***************************************
        qint64 fileANAsize = -1; //размер биарного файла

        QString fileANAname = filename;
        fileANAname[fileANAname.size()-1] = 'a'; //вычисление имени бинарного файла по имени его лог-файла

        QFileInfo fileANAinfo(fileANAname);
        if (fileANAinfo.exists())
        {
            fileANAsize = fileANAinfo.size();
        }
        else
        {
            QString errString = QString("Can't find *.ana file for \"%1\"").arg(filename);
            //qDebug() << errString;
            return ComponentMetadata(errString);
        }
//  ***************************************


        fileContent.remove(QChar::Space);
        fileContent.remove("\r");

        QChar component = QChar(fileContent[0]).toUpper(); //чтение компоненты из лог-файла
        if (component != 'X' && component != 'Y' && component != 'Z')
        {
            return ComponentMetadata();
        }

        QDateTime startTime, endTime;
        double frq = std::numeric_limits<double>::quiet_NaN();  //set frq to NaN

        QStringList strList = fileContent.split("\n"); //разбиение содержимого лог-файла на отдельные строки
        for (QString var : strList)
        {
            if ( var.startsWith("DATE", Qt::CaseInsensitive) )
            {
                //qDebug() << var.mid(4);
                startTime.setDate( QDate::fromString(var.mid(4), "dd-MM-yyyy") ); //считывание даты начала запаси бинарного файла
            }
            else if ( var.startsWith("START", Qt::CaseInsensitive) )
            {
                //qDebug() << var.mid(5);
                startTime.setTime( QTime::fromString(var.mid(5), "h:mm:ss") ); //считывание времени начала записи бинарного файла
            }
            else if ( var.startsWith("FRQ", Qt::CaseInsensitive) )
            {
                //qDebug() << var.mid(3);
                frq = var.mid(3).toDouble(); //считывание частоты дискретизации
            }
        }

        endTime = startTime.addSecs( (fileANAsize / 2) / frq ); //вычисление даты и времени окончания записи бинарного файла по его размеру и частоте дискретизации

        return ComponentMetadata(startTime, endTime, component, frq, fileANAname);
    }
    else
    {
        QString errString = QString("Can't open file \"%1\"").arg(filename);
        //qDebug() << errString;
        return ComponentMetadata(errString);
    }
}


QChar ComponentMetadata::component() const
{
    return m_component;
}

void ComponentMetadata::setComponent(const QChar &component)
{
    m_component = component;
}

QDateTime ComponentMetadata::startTime() const
{
    return m_startTime;
}

void ComponentMetadata::setStartTime(const QDateTime &startTime)
{
    m_startTime = startTime;
}

QDateTime ComponentMetadata::endTime() const
{
    return m_endTime;
}

void ComponentMetadata::setEndTime(const QDateTime &endTime)
{
    m_endTime = endTime;
}

QString ComponentMetadata::fileName() const
{
    return m_fileName;
}

void ComponentMetadata::setFileName(const QString &fileName)
{
    m_fileName = fileName;
}

double ComponentMetadata::frq() const
{
    return m_frq;
}

void ComponentMetadata::setFrq(double frq)
{
    m_frq = frq;
}

bool ComponentMetadata::isIncomplete() //проверка на полноту хранящихся данных
{
    return m_startTime.isNull() || m_endTime.isNull() || m_component.isNull() || m_fileName.isEmpty() || std::isnan(m_frq);
}

QString ComponentMetadata::errorString() const
{
    return m_errString;
}

ComponentMetadata::ComponentMetadata(const QString &err) :
    m_errString(err)
{
}

ComponentMetadata::ComponentMetadata()
{
}

ComponentMetadata::ComponentMetadata(const QDateTime &start, const QDateTime &end, const QChar &component, double frq, const QString &filename) :
    m_startTime(start),
    m_endTime(end),
    m_component(component),
    m_frq(frq),
    m_fileName(filename)
{
}

ComponentMetadata::ComponentMetadata(const ComponentMetadata &metadata) :
    m_startTime(metadata.m_startTime),
    m_endTime(metadata.m_endTime),
    m_component(metadata.m_component),
    m_frq(metadata.m_frq),
    m_fileName(metadata.m_fileName)
{
}

ComponentMetadata &ComponentMetadata::operator =(const ComponentMetadata &val)
{
    if (this != &val)
    {
#define copy_member(mem) mem = val.mem

        copy_member(m_startTime);
        copy_member(m_endTime);
        copy_member(m_component);
        copy_member(m_frq);
        copy_member(m_fileName);

#undef copy_member
    }

    return *this;
}

bool operator <(const ComponentMetadata &val1, const ComponentMetadata &val2)
{
    return val1.m_startTime < val2.m_startTime;
}
