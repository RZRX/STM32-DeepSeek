#include "peripheral_view.h"
#include <QHeaderView>

PeripheralView::PeripheralView(QWidget *parent) 
    : QTreeWidget(parent) {
    
    setColumnCount(3);
    setHeaderLabels({"Register", "Address", "Value"});
    header()->setSectionResizeMode(QHeaderView::Interactive);
}

void PeripheralView::loadFromSVD(const QString &mcuName) {
    clear();
    m_regMap.clear();
    
    if(!m_parser.loadFromFile(":/svd/" + mcuName + ".svd"))
        return;
        
    createPeripheralTree(m_parser.peripherals());
}

void PeripheralView::createPeripheralTree(const QList<PeripheralInfo> &peripherals) {
    for(const auto &periph : peripherals) {
        QTreeWidgetItem *periphItem = new QTreeWidgetItem(this);
        periphItem->setText(0, periph.name);
        
        for(const auto &reg : periph.registers) {
            QTreeWidgetItem *regItem = new QTreeWidgetItem(periphItem);
            regItem->setText(0, reg.name);
            regItem->setText(1, QString("0x%1").arg(reg.address, 8, 16, QLatin1Char('0')));
            regItem->setText(2, "0x########");
            m_regMap[reg.address] = regItem;
        }
    }
    expandAll();
}

void PeripheralView::updateRegisterValue(uint32_t addr, uint32_t value) {
    if(m_regMap.contains(addr)) {
        QTreeWidgetItem *item = m_regMap[addr];
        item->setText(2, QString("0x%1").arg(value, 8, 16, QLatin1Char('0')));
    }
}
