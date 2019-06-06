#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
class QAction;
class QActionGroup;
class QLabel;
class QMenu;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    //void on_pushButton_clicked();

    void on_action_triggered();

    void on_action_12_triggered();

    //void on_action_count_triggered();

    void on_action_12_changed();

    void on_action_12_destroyed();

    void on_action_5_triggered();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_action_3_triggered();

    void on_action_15_triggered();

    void on_action_2_triggered();

    void on_action_16_triggered();

    void on_action_4_triggered();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_action_10_triggered();

    void on_action_7_triggered();

    void on_action_8_triggered();

    void on_action_6_triggered();

private:
    Ui::MainWindow *ui;
    QStandardItemModel *csvModel;
    int c;//значение приращения для розенблата . спинбокс
};




/*
private slots:
    void newFile();
    void open();
    void save();
    void print();
    void undo();
    void redo();
    void cut();
    void copy();
    void paste();
    void bold();
    void italic();
    void leftAlign();
    void rightAlign();
    void justify();
    void center();
    void setLineSpacing();
    void setParagraphSpacing();
    void about();
    void aboutQt();


private:
    void createActions();
    void createMenus();

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *formatMenu;
    QMenu *helpMenu;
    QActionGroup *alignmentGroup;
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *printAct;
    QAction *exitAct;
    QAction *undoAct;
    QAction *redoAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *boldAct;
    QAction *italicAct;
    QAction *leftAlignAct;
    QAction *rightAlignAct;
    QAction *justifyAct;
    QAction *centerAct;
    QAction *setLineSpacingAct;
    QAction *setParagraphSpacingAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QLabel *infoLabel;
}; */



#endif // MAINWINDOW_H
