#include <QApplication>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QAction>

class MainWindow : public QMainWindow {
public:
    MainWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
        // Створення меню-бару
        QMenuBar *menuBar = new QMenuBar(this);

        // Додавання меню "Опції"
        QMenu *optionsMenu = new QMenu("Опції", this);
        QAction *actionOption1 = new QAction("Вибір 1", this);
        QAction *actionOption2 = new QAction("Вибір 2", this);
        optionsMenu->addAction(actionOption1);
        optionsMenu->addAction(actionOption2);
        menuBar->addMenu(optionsMenu);

        // Додавання меню "Графік"
        QMenu *graphMenu = new QMenu("Графік", this);
        QAction *actionGraph1 = new QAction("Метод 1", this);
        QAction *actionGraph2 = new QAction("Метод 2", this);
        graphMenu->addAction(actionGraph1);
        graphMenu->addAction(actionGraph2);
        menuBar->addMenu(graphMenu);

        // Встановлення меню-бару
        setMenuBar(menuBar);

        // Налаштування головного вікна
        setWindowTitle("Міні ядро");
        resize(800, 600);
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}
