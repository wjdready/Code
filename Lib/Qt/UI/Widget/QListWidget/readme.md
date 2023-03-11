
# QListWidget

常用 API
```cpp
void addItem(const QString &label)  //添加一项标签

void addItem(QListWidgetItem *item) //添加一项item

void insertItem(int row, QListWidgetItem *item)  //在row行后添加一项item

QListWidgetItem * item(int row) //在row行的项

QListWidgetItem *takeItem(int row)  //删除第row行的项，注意qt不会给你释放内存，需要手动释放，只会从界面上删除该项

void clear() //清空界面

void setAlternatingRowColors(bool) //设置是否开启交替颜色显示
```

```cpp
void currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous) //当前项改变发出信号

void currentRowChanged(int currentRow) //当前行改变发出信号

void currentTextChanged(const QString ¤tText) //当前项改变发出信号

void itemClicked(QListWidgetItem *item)  //点击项时发出信号

void itemDoubleClicked(QListWidgetItem *item) //双击发出信号

void itemEntered(QListWidgetItem *item) //鼠标进入某项发出信号

void itemPressed(QListWidgetItem *item) //鼠标按住某项发出信号

void itemSelectionChanged() //改变选择项时发出信号
```

