#include "componentlistwidget.h"

void ComponentListWidget::on_SaveButClicked()
{
    QStringList params;

    if (m_toBinRadio->isChecked())
    {
        //BINARY
        params << QString("B");
    }
    else if (m_toTextRadio->isChecked())
    {
        //TEXT
        params << QString("T");
    }

    for (int i = 0; i < m_componentList->count(); i++)
    {
        if (m_componentList->item(i)->checkState() == Qt::Checked)
        {
            QString fName = static_cast<ExtListWidgetItem*>(m_componentList->item(i))->metadata().fileName();
            fName[fName.size()-1] = 'p';
            params << fName;
            qDebug() << fName;
        }
    }

    if (params.size() < 2)
    {
        QMessageBox::warning(this, "Warning", "File have not been selected!");
        return;
    }

    QString fileName = QFileDialog::getExistingDirectory(this, "Save in folder", QDir::homePath());

    if (fileName.isEmpty())
    {
        return;
    }

    params.insert(1, fileName);

#ifdef Q_OS_LINUX
    QString fileStitcherPath = "./Translator";
#endif
#ifdef Q_OS_WIN
    QString fileStitcherPath = "Translator.exe";
#endif

    if (QProcess::startDetached(fileStitcherPath, params))
    {
        qDebug() << "Translator started";
    }
    else
    {
        qDebug() << "Error! Translator not started";
        QMessageBox::warning(this, "Fatal!", QString("Process \"%1\" not started!").arg(fileStitcherPath));
    }
}

void ComponentListWidget::setNewList(const QList<ComponentMetadata> &componentsList)
{
    m_componentList->clear();

    addList(componentsList);
}

void ComponentListWidget::setNewList(const QStringList &fileNameList)
{
    m_componentList->clear();

    addList(fileNameList);
}

void ComponentListWidget::addList(const QList<ComponentMetadata> &componentsList)
{
    QList<ComponentMetadata> metadataList;

    for (int i = 0; i < m_componentList->count(); i++)
    {
        metadataList << static_cast<ExtListWidgetItem*>(m_componentList->item(i))->metadata();
    }

    m_componentList->clear();

    metadataList = metadataList + componentsList;

    std::sort(metadataList.begin(), metadataList.end());

    for (auto metadata : metadataList)
    {
        m_componentList->addItem(new ExtListWidgetItem(metadata));
    }
}

void ComponentListWidget::addList(const QStringList &fileNameList)
{
    QList<ComponentMetadata> metadataList;

    for (int i = 0; i < m_componentList->count(); i++)
    {
        metadataList << static_cast<ExtListWidgetItem*>(m_componentList->item(i))->metadata();
    }

    m_componentList->clear();

    for (const QString &fileName : fileNameList)
    {
        ComponentMetadata metadata = ComponentMetadata::getMetaData(fileName);
        if (!metadata.isIncomplete())
        {
            metadataList << metadata;
        }
    }

    std::sort(metadataList.begin(), metadataList.end());

    for (auto metadata : metadataList)
    {
        m_componentList->addItem(new ExtListWidgetItem(metadata));
    }
}

ComponentListWidget::ComponentListWidget(const QStringList &fileNameList, QWidget *parent) :
    QWidget(parent),
    m_componentList(new QListWidget()),
    m_saveBut(new QPushButton("&Save")),
    m_toBinRadio(new QRadioButton("&Binary")),
    m_toTextRadio(new QRadioButton("&Text")),
    m_radioBox(new QGroupBox("Save data"))
{
    m_toBinRadio->setChecked(true);
    m_saveBut->setMaximumWidth(120);

    m_saveBut->setToolTip("<span>Save to text/binary</span><br><strong>Alt+S</strong>");

    connect(m_saveBut, &QPushButton::clicked, this, &ComponentListWidget::on_SaveButClicked);

    QHBoxLayout *hRadioLay = new QHBoxLayout();
    hRadioLay->addWidget(m_toBinRadio);
    hRadioLay->addWidget(m_toTextRadio);

    m_radioBox->setLayout(hRadioLay);

    QHBoxLayout *hButLay = new QHBoxLayout();
    hButLay->addWidget(m_radioBox);
    hButLay->addWidget(m_saveBut);

    QVBoxLayout *vlay = new QVBoxLayout();
    vlay->addWidget(m_componentList);
    vlay->addLayout(hButLay);

    setLayout(vlay);

    if (!fileNameList.isEmpty()) setNewList(fileNameList);
}
