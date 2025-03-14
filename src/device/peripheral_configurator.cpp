#include "peripheral_configurator.h"
#include <QGraphicsScene>

PeripheralConfigurator::PeripheralConfigurator(QWidget *parent)
    : QGraphicsView(parent), m_mcu(nullptr) {
    
    m_scene = new QGraphicsScene();
    setScene(m_scene);
    setRenderHint(QPainter::Antialiasing);
}

void PeripheralConfigurator::loadMCU(const QString &mcuName) {
    m_mcu = new MCUModel(mcuName);
    drawPinout();
}

void PeripheralConfigurator::drawPinout() {
    const QVector<PinInfo> &pins = m_mcu->pins();
    for (const PinInfo &pin : pins) {
        QGraphicsRectItem *rect = new QGraphicsRectItem(0, 0, 30, 100);
        rect->setPos(pin.position.x(), pin.position.y());
        rect->setToolTip(pin.name + "\n" + pin.functions.join("|"));
        m_scene->addItem(rect);
    }
}

void PeripheralConfigurator::mouseDoubleClickEvent(QMouseEvent *event) {
    QGraphicsItem *item = itemAt(event->pos());
    if (item) emit pinSelected(item->toolTip());
}
