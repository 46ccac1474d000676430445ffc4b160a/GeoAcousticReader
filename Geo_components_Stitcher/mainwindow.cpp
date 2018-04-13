#include "mainwindow.h"

void MainWindow::on_choosePathClicked()
{
    QString path = QFileDialog::getExistingDirectory(this, "Choose folder", m_pathEdit->text());

    if (!path.isEmpty())
    {
        m_pathEdit->setText(path);
    }
}

void MainWindow::on_checkPathClicked()
{
    setDisabled(true);

    QFileInfo fInfo(m_pathEdit->text());
    if (fInfo.exists() && fInfo.isDir())
    {
        QStringList pathList;

        QApplication::setOverrideCursor(Qt::WaitCursor);

        DirTraversal::traversal(m_pathEdit->text(), pathList);

        QApplication::restoreOverrideCursor();

        QList<ComponentMetadata> XmetadataList;
        QList<ComponentMetadata> YmetadataList;
        QList<ComponentMetadata> ZmetadataList;

        for (auto var : pathList)
        {
            ComponentMetadata temp = ComponentMetadata::getMetaData(var);

            if (!temp.isIncomplete())
            {
                int comp = temp.component().toLatin1();
                switch (comp)
                {
                    case 'X':
                    {
                        XmetadataList << temp;
                    } break;

                    case 'Y':
                    {
                        YmetadataList << temp;
                    } break;

                    case 'Z':
                    {
                        ZmetadataList << temp;
                    } break;
                }
            }
        }

        if (XmetadataList.isEmpty() && YmetadataList.isEmpty() && ZmetadataList.isEmpty())
        {
            QMessageBox::information(this, "Message", "Files not found.");
        }
        else
        {
            m_XCompList->setNewList(XmetadataList);
            m_YCompList->setNewList(YmetadataList);
            m_ZCompList->setNewList(ZmetadataList);
        }
    }
    else
    {
        QMessageBox::warning(this, "Message", "Can not open folder: " + m_pathEdit->text());
    }

    setEnabled(true);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_tabWidget(new QTabWidget()),
    m_pathEdit(new QLineEdit(QDir::homePath())),
    m_choosePathBut(new QToolButton()),
    m_checkPathBut(new QToolButton()),
    m_XCompList(new ComponentListWidget()),
    m_YCompList(new ComponentListWidget()),
    m_ZCompList(new ComponentListWidget()),
    m_openDirShortcut(new QShortcut(QKeySequence("Ctrl+o"), this)),
    m_updateListsShortcut(new QShortcut(QKeySequence(Qt::Key_F5), this))
{
    connect(m_choosePathBut, &QToolButton::clicked, this, &MainWindow::on_choosePathClicked);
    connect(m_checkPathBut, &QToolButton::clicked, this, &MainWindow::on_checkPathClicked);
    connect(m_openDirShortcut, &QShortcut::activated, this, &MainWindow::on_choosePathClicked);
    connect(m_updateListsShortcut, &QShortcut::activated, this, &MainWindow::on_checkPathClicked);

    m_choosePathBut->setIcon(style()->standardIcon(QStyle::SP_DirIcon));
    m_choosePathBut->setToolTip("<span>Choose path</span><br><strong>Ctrl+O</strong>");
    m_checkPathBut->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
    m_checkPathBut->setToolTip("<span>Update component lists</span><br><strong>F5</strong>");

    QMenu *fileMenu = new QMenu("&File", this);
    m_chooseDirAction = fileMenu->addAction("&Choose folder\tCtrl+O");
    m_dirUpdateAction = fileMenu->addAction("&Update folder\tF5");

    m_showFolderPanelAction = fileMenu->addAction("Show folder &panel");
    m_showFolderPanelAction->setCheckable(true);
    m_showFolderPanelAction->setChecked(true);

    fileMenu->addSeparator();

    m_quitAction = fileMenu->addAction("&Quit");

    connect(m_chooseDirAction, &QAction::triggered, this, &MainWindow::on_choosePathClicked);
    connect(m_dirUpdateAction, &QAction::triggered, this, &MainWindow::on_checkPathClicked);
    connect(m_showFolderPanelAction, &QAction::changed, [=](){
        if (m_showFolderPanelAction->isChecked())
        {
            m_checkPathBut->show();
            m_pathEdit->show();
            m_choosePathBut->show();
        }
        else
        {
            m_checkPathBut->hide();
            m_pathEdit->hide();
            m_choosePathBut->hide();
        }
    });
    connect(m_quitAction, &QAction::triggered, this, &MainWindow::close);

    menuBar()->addMenu(fileMenu);

    m_tabWidget->addTab(m_XCompList, "X-component");
    m_tabWidget->addTab(m_YCompList, "Y-component");
    m_tabWidget->addTab(m_ZCompList, "Z-component");

    QHBoxLayout *hPathLay = new QHBoxLayout();
    hPathLay->addWidget(m_checkPathBut);
    hPathLay->addWidget(m_pathEdit);
    hPathLay->addWidget(m_choosePathBut);

    QVBoxLayout *vlay = new QVBoxLayout();
    vlay->addLayout(hPathLay);
    vlay->addWidget(m_tabWidget);

    QWidget *wgt = new QWidget();
    wgt->setLayout(vlay);

    setCentralWidget(wgt);
}

MainWindow::~MainWindow()
{
}
