#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMap>
#include <QMainWindow>

class MainWidget;
class QScrollArea;
class Palette;
class BrushList;
class QUndoStack;
class NewDialog;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow();

private slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void about();
    void openRecentFile();
    void clearRecentFiles();
    bool exportAsHtml();
    bool exportAsTerminal();
    bool exportAsImage();
    void importImage();
    void setCleanState(bool b);

protected:
    void closeEvent(QCloseEvent* event);

private:
    void writeSettings();
    void readSettings();
    bool okToContinue();
    void updateRecentFiles();
    void setToolbarSize(const QSize& s);
    void setCurrentFile(const QString& fname);
    bool exportToTxt(const QString& fname);
    bool exportToTerminal(const QString& fname);
    bool exportToHtml(const QString& fname);
    bool exportToImg(const QString& fname, bool grid); // image type is inferred from filename
    bool importFromTxt(const QString& fname);
    bool importFromImg(const QString& fname, int maxWidth, bool smooth); // maxWidth is the maximum number of ascii cells; if smooth is true, use bilinear interpolation while scaling down
    int rgbToIrc(QRgb c);
    QRgb ircToRgb(int i);
    QString rgbToHtml(QRgb c);
    QString ircToTerminal(int i, bool bg);
    void swapIrcColor(int i, QRgb c);
    QString getName() const;

    QString curFile;
    struct Lab {
        double l, a, b;
    };
    Lab rgbToLab(const QRgb& c);
    QRgb closestColor(const QRgb& c, const QMap<int, Lab>& labColors);

    QMap<int, QRgb> colors;
    QSize toolbarSize;
    bool displayTitle;

    QScrollArea* scroll;
    MainWidget* mwidget;
    QDockWidget* dock_p;
    QDockWidget* dock_b;

    Palette* palette;
    BrushList* blist;

    QStringList recentFiles;
    static const unsigned short int maxRecentFiles = 5;
    QAction* recentFileActions[maxRecentFiles];
    QAction* recentFileSeparatorAction;
    QAction* clearRecentFilesAction;

    QUndoStack* undo;

    NewDialog* newDialog;

    QAction* newAction;
    QAction* openAction;
    QAction* saveAction;
    QAction* saveAsAction;
    QAction* exitAction;
    QAction* undoAction;
    QAction* redoAction;
    QAction* aboutAction;
    QAction* aboutQtAction;
    QAction* showGridAction;
    QAction* alternatePaintAction;
    QAction* exportAsHtmlAction;
    QAction* exportAsImgAction;
    QAction* exportAsTermAction;
    QAction* importImgAction;
    QAction* addRowAction;
    QAction* delRowAction;
    QAction* addColumnAction;
    QAction* delColumnAction;
    QMenu* fileMenu;
    QMenu* importMenu;
    QMenu* exportMenu;
    QMenu* editMenu;
    QMenu* toolsMenu;
    QMenu* rowMenu;
    QMenu* columnMenu;
    QMenu* helpMenu;
    QToolBar* fileToolbar;
};

#endif // MAINWINDOW_H
