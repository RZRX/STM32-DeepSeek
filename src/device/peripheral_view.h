#pragma once
#include <QWidget>
#include <QTreeWidget>
#include "svd_parser.h"

class PeripheralView : public QTreeWidget {
    Q_OBJECT
public:
    explicit PeripheralView(QWidget *parent = nullptr);
    void loadFromSVD(const QString &mcuName);
    void updateRegisterValue(uint32_t addr, uint32_t value);

private:
    void createPeripheralTree(const QList<PeripheralInfo> &peripherals);
    
    QMap<uint32_t, QTreeWidgetItem*> m_regMap;
    SVDParser m_parser;
};
