#ifndef DIRTRAVERSAL_H
#define DIRTRAVERSAL_H

#include <QDir>
#include <QFileInfo>
#include <QRegExp>

class DirTraversal
{
    static QRegExp s_regExp;

    DirTraversal(){}

public:
    static void traversal(const QString &path, QStringList &fileNameList);

};

#endif // DIRTRAVERSAL_H
