#include <QtGui>

#include "MainWidget.h"

MainWidget::MainWidget(QWidget *parent) : QWidget(parent) {
    setAttribute(Qt::WA_StaticContents);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setFocusPolicy(Qt::StrongFocus);

    bgColor = Qt::black;
    fgColor = Qt::gray;
    selColor = Qt::yellow;
    xsize = 10;
    ysize = 20;
    showGrid = true;
    xasc = 26;
    yasc = 16;
    lastx = 0;
    lasty = 0;

    background = QImage(xasc, yasc, QImage::Format_ARGB32);
    background.fill(qRgba(255, 255, 255, 255));
    foreground = QImage(xasc, yasc, QImage::Format_ARGB32);
    foreground.fill(qRgba(0, 0, 0, 255));
    QList<QChar> l;
    for (int i = 0; i < xasc; ++i) {
        l << QChar(' ');
    }
    for (int i = 0; i < yasc; ++i) {
        text << l;
    }
}

void MainWidget::setGrid(bool g) {
    if (g != showGrid) {
        showGrid = g;
        update();
        updateGeometry();
    }
}

void MainWidget::setRectWidth(int x) {
    if (x < 1)
        x = 1;

    if (x != xsize) {
        xsize = x;
        update();
        updateGeometry();
    }
}

void MainWidget::setRectHeight(int y) {
    if (y < 1)
        y = 1;

    if (y != ysize) {
        ysize = y;
        update();
        updateGeometry();
    }
}

QSize MainWidget::sizeHint() const {
    QSize size(xsize*xasc, ysize*yasc);
    if (showGrid)
        size += QSize(1, 1);
    return size;
}

void MainWidget::mousePressEvent(QMouseEvent *event) {
    int i = event->pos().x() / xsize;
    int j = event->pos().y() / ysize;
    if (j >= yasc || j < 0 || i >= xasc || i < 0) {
        QWidget::mousePressEvent(event);
        return;
    }
    int oldx = lastx;
    int oldy = lasty;
    lastx = i;
    lasty = j;
    update(pixelRect(oldx,oldy));
    if (event->button() == Qt::LeftButton) {
        setBGImagePixel(event->pos());
    } else if (event->button() == Qt::RightButton) {
        setFGImagePixel(event->pos());
    } else {
        QWidget::mousePressEvent(event);
    }
}

void MainWidget::mouseMoveEvent(QMouseEvent *event) {
    int i = event->pos().x() / xsize;
    int j = event->pos().y() / ysize;
    if (j >= yasc || j < 0 || i >= xasc || i < 0) {
        QWidget::mouseMoveEvent(event);
        return;
    }
    int oldx = lastx;
    int oldy = lasty;
    lastx = i;
    lasty = j;
    update(pixelRect(oldx,oldy));
    if (event->buttons() & Qt::LeftButton) {
        setBGImagePixel(event->pos());
    } else if (event->buttons() & Qt::RightButton) {
        setFGImagePixel(event->pos());
    } else {
        QWidget::mouseMoveEvent(event);
    }
}

void MainWidget::mouseReleaseEvent(QMouseEvent *event) {
    int i = event->pos().x() / xsize;
    int j = event->pos().y() / ysize;
    if (j >= yasc || j < 0 || i >= xasc || i < 0) {
        QWidget::mouseReleaseEvent(event);
        return;
    }
    lastx = i;
    lasty = j;
    update(pixelRect(i,j));
}

void MainWidget::keyPressEvent(QKeyEvent *event) {
    QChar c = event->text()[0];
    if (c.isPrint()) {
        text[lasty][lastx] = c;
        int oldx = lastx;
        int oldy = lasty;
        if (++lastx >= xasc) {
            lastx = 0;
            if (++lasty >= yasc) {
                lasty = 0;
            }
        }
        update(pixelRect(oldx, oldy).united(pixelRect(lastx,lasty)));
    } else if (event->key() == Qt::Key_Backspace) {
        int oldx = lastx;
        int oldy = lasty;
        if (--lastx < 0) {
            lastx = xasc-1;
            if (--lasty < 0) {
                lasty = yasc-1;
            }
        }
        text[lasty][lastx] = ' ';
        update(pixelRect(oldx, oldy).united(pixelRect(lastx,lasty)));
    } else if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        int oldx = lastx;
        int oldy = lasty;
        lastx = 0;
        if (++lasty >= yasc) {
            lasty = 0;
        }
        update(pixelRect(oldx, oldy).united(pixelRect(lastx,lasty)));
    } else if (event->key() == Qt::Key_Up) {
        int oldy = lasty;
        if (--lasty < 0) {
            lasty = oldy;
        }
        update(pixelRect(lastx, oldy).united(pixelRect(lastx,lasty)));
    } else if (event->key() == Qt::Key_Down) {
        int oldy = lasty;
        if (++lasty >= yasc) {
            lasty = oldy;
        }
        update(pixelRect(lastx, oldy).united(pixelRect(lastx,lasty)));
    } else if (event->key() == Qt::Key_Left) {
        int oldx = lastx;
        if (--lastx < 0) {
            lastx = oldx;
        }
        update(pixelRect(oldx, lasty).united(pixelRect(lastx,lasty)));
    } else if (event->key() == Qt::Key_Right) {
        int oldx = lastx;
        if (++lastx >= xasc) {
            lastx = oldx;
        }
        update(pixelRect(oldx, lasty).united(pixelRect(lastx,lasty)));
    } else {
        QWidget::keyPressEvent(event);
    }
}

void MainWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    if (showGrid) {
        painter.setPen(palette().windowText().color());
        for (int i = 0; i <= xasc; ++i)
            painter.drawLine(xsize * i, 0,
                                 xsize * i, ysize * yasc);
        for (int j = 0; j <= yasc; ++j)
            painter.drawLine(0, ysize * j,
                                 xsize * xasc, ysize * j);
    }

    for (int i = 0; i < xasc; ++i) {
        for (int j = 0; j < yasc; ++j) {
            QRect rect = pixelRect(i, j);
            if (!event->region().intersect(rect).isEmpty()) {
                painter.fillRect(rect, QColor::fromRgba(background.pixel(i, j)));
                painter.setPen(QColor::fromRgba(foreground.pixel(i,j)));
                painter.drawText(rect.translated(1,0), Qt::AlignCenter, text.at(j).at(i));
            }
        }
    }
    if (hasFocus()) {
        painter.setPen(selColor);
        painter.drawRect(pixelRect(lastx,lasty).adjusted(0,0,-1,-1));
    }
}

void MainWidget::setBGImagePixel(const QPoint &pos) {
    int i = pos.x() / xsize;
    int j = pos.y() / ysize;

    if (background.rect().contains(i, j)) {
        background.setPixel(i, j, bgColor.rgba());
        update(pixelRect(i, j));
    }
}

void MainWidget::setFGImagePixel(const QPoint &pos) {
    int i = pos.x() / xsize;
    int j = pos.y() / ysize;

    if (foreground.rect().contains(i, j)) {
        foreground.setPixel(i, j, fgColor.rgba());
        update(pixelRect(i, j));
    }
}

QRect MainWidget::pixelRect(int i, int j) const {
    if (showGrid) {
        return QRect(xsize * i + 1, ysize * j + 1, xsize - 1, ysize - 1);
    } else {
        return QRect(xsize * i, ysize * j, xsize, ysize);
    }
}

void MainWidget::addRows(int place, int n) {
    if (n <= 0)
        return;
    if (place < 0)
        place = 0;
    if (place > yasc)
        place = yasc;

    QList<QList<QRgb> > bg, fg;
    for (int j = 0; j < yasc; ++j) {
        QList<QRgb> a, b;
        for (int i = 0; i < xasc; ++i) {
            a << background.pixel(i,j);
            b << foreground.pixel(i,j);
        }
        bg << a;
        fg << b;
    }
    QList<QChar> textl;
    QList<QRgb> bgRow, fgRow;
    for (int i = 0; i < xasc; ++i) {
        textl << QChar(' ');
        bgRow << bgColor.rgba();
        fgRow << fgColor.rgba();
    }
    for (int i = 0; i < n; ++i) {
        text.insert(place, textl);
        bg.insert(place, bgRow);
        fg.insert(place, fgRow);
    }
    QImage newbg(xasc, yasc+n, QImage::Format_ARGB32);
    QImage newfg(xasc, yasc+n, QImage::Format_ARGB32);
    for (int j = 0; j < yasc+n; ++j) {
        for (int i = 0; i < xasc; ++i) {
            newbg.setPixel(i, j, bg[j][i]);
            newfg.setPixel(i, j, fg[j][i]);
        }
    }
    background = newbg;
    foreground = newfg;
    yasc += n;
    update();
    updateGeometry();
}

void MainWidget::delRows(int place, int n) {
    if (n <= 0)
        return;
    if (place < 0)
        place = 0;
    if (place > yasc)
        place = yasc;
    if (n > yasc)
        n = yasc;

    QList<QList<QRgb> > bg, fg;
    for (int j = 0; j < yasc; ++j) {
        QList<QRgb> a, b;
        for (int i = 0; i < xasc; ++i) {
            a << background.pixel(i,j);
            b << foreground.pixel(i,j);
        }
        bg << a;
        fg << b;
    }
    for (int i = 0; i < n; ++i) {
        text.removeAt(place);
        bg.removeAt(place);
        fg.removeAt(place);
    }
    QImage newbg(xasc, yasc-n, QImage::Format_ARGB32);
    QImage newfg(xasc, yasc-n, QImage::Format_ARGB32);
    for (int j = 0; j < yasc-n; ++j) {
        for (int i = 0; i < xasc; ++i) {
            newbg.setPixel(i, j, bg[j][i]);
            newfg.setPixel(i, j, fg[j][i]);
        }
    }
    background = newbg;
    foreground = newfg;
    yasc -= n;
    update();
    updateGeometry();
}

void MainWidget::addColumns(int place, int n) {
    if (n <= 0)
        return;
    if (place < 0)
        place = 0;
    if (place > xasc)
        place = xasc;

    QList<QList<QRgb> > bg, fg;
    for (int j = 0; j < yasc; ++j) {
        QList<QRgb> a, b;
        for (int i = 0; i < xasc; ++i) {
            a << background.pixel(i,j);
            b << foreground.pixel(i,j);
        }
        bg << a;
        fg << b;
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < yasc; ++j) {
            text[j].insert(place, QChar(' '));
            bg[j].insert(place, bgColor.rgba());
            fg[j].insert(place, fgColor.rgba());
        }
    }
    QImage newbg(xasc+n, yasc, QImage::Format_ARGB32);
    QImage newfg(xasc+n, yasc, QImage::Format_ARGB32);
    for (int j = 0; j < yasc; ++j) {
        for (int i = 0; i < xasc+n; ++i) {
            newbg.setPixel(i, j, bg[j][i]);
            newfg.setPixel(i, j, fg[j][i]);
        }
    }
    background = newbg;
    foreground = newfg;
    xasc += n;
    update();
    updateGeometry();
}

void MainWidget::delColumns(int place, int n) {
    if (n <= 0)
        return;
    if (place < 0)
        place = 0;
    if (place > xasc)
        place = xasc;
    if (n > xasc)
        n = xasc;

    QList<QList<QRgb> > bg, fg;
    for (int j = 0; j < yasc; ++j) {
        QList<QRgb> a, b;
        for (int i = 0; i < xasc; ++i) {
            a << background.pixel(i,j);
            b << foreground.pixel(i,j);
        }
        bg << a;
        fg << b;
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < yasc; ++j) {
            text[j].removeAt(place);
            bg[j].removeAt(place);
            fg[j].removeAt(place);
        }
    }
    QImage newbg(xasc-n, yasc, QImage::Format_ARGB32);
    QImage newfg(xasc-n, yasc, QImage::Format_ARGB32);
    for (int j = 0; j < yasc; ++j) {
        for (int i = 0; i < xasc-n; ++i) {
            newbg.setPixel(i, j, bg[j][i]);
            newfg.setPixel(i, j, fg[j][i]);
        }
    }
    background = newbg;
    foreground = newfg;
    xasc -= n;
    update();
    updateGeometry();
}
