#ifndef COMPONENTLISTWIDGET_H
#define COMPONENTLISTWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QRadioButton>
#include <QGroupBox>
#include <QMessageBox>
#include <QFileDialog>

#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QProcess>

#include "extlistwidgetitem.h"

class ComponentListWidget : public QWidget
{
    Q_OBJECT

    QListWidget *m_componentList;

    QPushButton *m_saveBut;

    QRadioButton *m_toBinRadio,
                 *m_toTextRadio;

    QGroupBox *m_radioBox;

public slots:
    void on_SaveButClicked();

    void setNewList(const QList<ComponentMetadata> &componentsList);
    void setNewList(const QStringList &fileNameList);

    void addList(const QList<ComponentMetadata> &componentsList);
    void addList(const QStringList &fileNameList);

public:
    explicit ComponentListWidget(const QStringList &fileNameList = QStringList(), QWidget *parent = nullptr);


};

#endif // COMPONENTLISTWIDGET_H
