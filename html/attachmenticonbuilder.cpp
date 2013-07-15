#include "attachmenticonbuilder.h"
#include "global.h"
#include <QIcon>
#include <QFileInfo>
#include <QFile>
#include <QFileIconProvider>
#include <QPainter>

extern Global global;

AttachmentIconBuilder::AttachmentIconBuilder(QObject *parent) :
    QObject(parent)
{
}



QString AttachmentIconBuilder::buildIcon(qint32 lid, QString fileName) {
    // First get the icon for this type of file
    QIcon icon;
    QFileInfo info(fileName);
    QFileIconProvider provider;
    icon = provider.icon(info);

    // Build a string name for the display
    QString displayName = info.fileName();

    // Setup the painter
    QPainter p;

    // Setup the font
    QFont font;
    font.setPointSize ( 8 );
    font.setFamily("Arial");
    QFontMetrics fm(font);
    int width =  fm.width(displayName);
    if (width < 40)  // steup a minimum width
        width = 40;
    width=width+50;  // Add 10 px for padding & 40 for the icon

    // Start drawing a new pixmap for  the image in the note
    QPoint textPoint(40,15);
    QPoint sizePoint(40,29);
    QPixmap pixmap(width,37);
    pixmap.fill();

    p.begin(&pixmap);
    p.setFont(font);
    p.drawPixmap(QPoint(3,3), icon.pixmap(QSize(30,40)));

    // Write out the attributes of the file
    p.drawText(textPoint, displayName);

    QString unit = QString(tr("Bytes"));
    qint64 size = QFileInfo(fileName).size();
    if (size > 1024) {
        size = size/1024;
        unit = QString(tr("KB"));
    }
    if (size > 1024) {
        size = size/1024;
        unit= QString("MB");
    }
    p.drawText(sizePoint, QString::number(size).trimmed() +" " +unit);
    p.drawRect(0,0,width-1,37-1);   // Draw a rectangle around the image.
    p.end();

    // Now that it is drawn, we write it out to a temporary file
    QString tmpFile = global.fileManager.getTmpDirPath(QString::number(lid) + QString("_icon.png"));
    pixmap.save(tmpFile, "png");
    return tmpFile;
}
