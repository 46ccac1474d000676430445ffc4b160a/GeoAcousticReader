#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>

#include <QMainWindow>
#include <QTabWidget>
#include <QLineEdit>
#include <QToolButton>
#include <QStyle>
#include <QFileDialog>
#include <QMessageBox>
#include <QMenu>
#include <QAction>
#include <QMenuBar>

#include <QCursor>
#include <QShortcut>

#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QDir>

#include "ComponentListWidget/componentlistwidget.h"
#include "DirTraversal/dirtraversal.h"
#include "../COMMON/ComponentMetadata/componentmetadata.h"


#include <QDebug>

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QTabWidget *m_tabWidget;

    QLineEdit *m_pathEdit;

    QToolButton *m_choosePathBut,
                *m_checkPathBut;

    ComponentListWidget *m_XCompList,
                        *m_YCompList,
                        *m_ZCompList;

    QShortcut *m_openDirShortcut,
              *m_updateListsShortcut;

    QAction *m_chooseDirAction,
            *m_dirUpdateAction,
            *m_showFolderPanelAction,
            *m_quitAction;

public slots:
    void on_choosePathClicked();
    void on_checkPathClicked();

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
