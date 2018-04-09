#include "dirtraversal.h"

QRegExp DirTraversal::s_regExp = QRegExp("sig[0-9][0-9][0-9][0-9].anp", Qt::CaseInsensitive);

void DirTraversal::traversal(const QString &path, QStringList &fileNameList)
{
    QFileInfoList list(QDir(path).entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot));

    for (int i = 0; i < list.size(); i++)
    {
        if (list[i].isFile() && s_regExp.exactMatch(list[i].fileName()))
        {
            fileNameList << list[i].absoluteFilePath();
        }
        else if (list[i].isDir()) traversal(list[i].absoluteFilePath(), fileNameList);
    }
}
