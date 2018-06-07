#ifndef DIRTRAVERSAL_H
#define DIRTRAVERSAL_H

#include <QDir>
#include <QFileInfo>
#include <QRegExp>

class DirTraversal
{
    static QRegExp s_regExp; //регулярное выражение для проверки соответствия имени файла формату имени лог-файлов

    DirTraversal(){}

public:
    static void traversal(const QString &path, QStringList &fileNameList); //обход выбраного каталога path и добавление в fileNameList всех найденых в каталоге и его подкаталогах лог-файлах

};

#endif // DIRTRAVERSAL_H
