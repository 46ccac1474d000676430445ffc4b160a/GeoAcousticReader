#include "extlistwidgetitem.h"

ComponentMetadata ExtListWidgetItem::metadata() const
{
    return m_metadata;
}

void ExtListWidgetItem::setMetadata(const ComponentMetadata &metadata)
{
    m_metadata = metadata;
}

ExtListWidgetItem::ExtListWidgetItem()
{
    setFlags(flags() | Qt::ItemIsUserCheckable);
    setCheckState(Qt::Unchecked);
}

ExtListWidgetItem::ExtListWidgetItem(const QString &label) :
    QListWidgetItem(label)
{
    setFlags(flags() | Qt::ItemIsUserCheckable);
    setCheckState(Qt::Unchecked);
}

ExtListWidgetItem::ExtListWidgetItem(const ComponentMetadata &metadata) :
    QListWidgetItem(QString(metadata.component()) + " " +
                    metadata.startTime().toString("dd-MM-yyyy, hh:mm:ss") + " -> " +
                    metadata.endTime().toString("dd-MM-yyyy, hh:mm:ss") + ", frq: " +
                    QString::number(metadata.frq()) + ", file: \"" +
                    metadata.fileName() + "\""
                    ),
    m_metadata(metadata)
{
    setFlags(flags() | Qt::ItemIsUserCheckable);
    setCheckState(Qt::Unchecked);

    setFont(QFont("Lucida Console", 9));
}
