#ifndef TABLES_H
#define TABLES_H

#include <QWidget>
#include <QComboBox>

namespace Ui {
class Tables;
}

class Tables : public QWidget
{
    Q_OBJECT

public:
    explicit Tables(QWidget *parent = nullptr);
    ~Tables();

private:
    Ui::Tables *ui;

    void setComboBoxColor(QComboBox *comboBox, const QString &status);
};

#endif // TABLES_H
