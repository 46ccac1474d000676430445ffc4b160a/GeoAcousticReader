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

//виджет, представлющий список найденых файлов для конкретной компоненты
class ComponentListWidget : public QWidget
{
    Q_OBJECT

    QListWidget *m_componentList; //виджет-список

    QPushButton *m_saveBut; //кнопка сохранения

    QRadioButton *m_toBinRadio, //выбор бинарного формата
                 *m_toTextRadio; //выбор текстового формата

    QGroupBox *m_radioBox;

public slots:
    void on_SaveButClicked(); //слот обработки события сохранения данных в файл

    void setNewList(const QList<ComponentMetadata> &componentsList); //установка нового списка
    void setNewList(const QStringList &fileNameList); //установка нового списка

    void addList(const QList<ComponentMetadata> &componentsList); //добавление элементов в список
    void addList(const QStringList &fileNameList); //добавление элементов в список

public:
    explicit ComponentListWidget(const QStringList &fileNameList = QStringList(), QWidget *parent = nullptr);


};

#endif // COMPONENTLISTWIDGET_H
