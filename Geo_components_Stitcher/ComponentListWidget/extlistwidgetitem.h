#ifndef EXTLISTWIDGETITEM_H
#define EXTLISTWIDGETITEM_H

#include <QListWidgetItem>

#include "../COMMON/ComponentMetadata/componentmetadata.h"

class ExtListWidgetItem : public QListWidgetItem
{
    ComponentMetadata m_metadata;

public:
    ExtListWidgetItem();
    ExtListWidgetItem(const QString &label);
    ExtListWidgetItem(const ComponentMetadata &metadata);

    ComponentMetadata metadata() const;
    void setMetadata(const ComponentMetadata &metadata);
};

#endif // EXTLISTWIDGETITEM_H
