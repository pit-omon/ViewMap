#include <QApplication>
#include "view.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  View view(QSize(1024, 1024));
  view.show();

  return a.exec();
}
