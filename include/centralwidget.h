#include <QPainter>
#include <QWidget>

/**
 * @file centralwidget.h
 * @author raik
 * @brief Central widget class for main ui
 *
 * @note This class was created to set background image that wont affect any children widgets of the main window.
 */

class CentralWidget : public QWidget {
public:
  explicit CentralWidget(QWidget *parent = nullptr) : QWidget(parent) {
  }

protected:
  void paintEvent(QPaintEvent * /*event*/) override {
    QPainter painter(this);
    QPixmap  pixmap(":/banner/main_menu.png");
    painter.drawPixmap(rect(), pixmap);
  }
};