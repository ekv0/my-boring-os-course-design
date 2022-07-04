#include "gui_common.h"

QIcon getIconFromPath(std::string path)
{
    QString qpath(QString::fromStdString(path));
    QFileInfo fileInfo(qpath);
    QFileIconProvider iconPrv;
    QPixmap pixmap = iconPrv.icon(fileInfo).pixmap(32,32);
    return QIcon(pixmap);
}
