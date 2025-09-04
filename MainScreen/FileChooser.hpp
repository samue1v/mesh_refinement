#ifndef FILE_CHOOSER_H
#define FILE_CHOOSER_H

#include <QWidget>
#include <QDir>
#include <QFileSystemModel>
#include <QModelIndex>
#include <QTreeView>
#include <QListView>
#include <QSplitter>
#include <QTreeView>
#include <QFileIconProvider>
#include <QIcon>
#include <QString>

class MainWidget;

class FileChooser : public QWidget{
Q_OBJECT
public:
  FileChooser(MainWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
  ~FileChooser();

public slots:
  void folderClicked(const QModelIndex &index);
  void fileDoubleClicked(const QModelIndex & index);

private:
  QFileSystemModel * dirModel;
  QFileSystemModel * fileModel;
  QTreeView * treeView;
  QListView * listView;
  QSplitter * splitter;
  MainWidget * mainScreen;
  QFileIconProvider * iconProvider;



};

#endif