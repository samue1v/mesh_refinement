#include "FileChooser.hpp"
#include <iostream>
#include "MainWidget.hpp"

FileChooser::FileChooser(MainWidget * parent, Qt::WindowFlags f) : QWidget(parent,f){

  mainScreen = parent;

  splitter = new QSplitter(this);

  
  treeView = new QTreeView;
  listView = new QListView;

  iconProvider = new QFileIconProvider();   //Is this automatically destroyed???
  

  dirModel = new QFileSystemModel(this);
  dirModel->setRootPath(QDir::currentPath());
  dirModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
  dirModel->setIconProvider(iconProvider);

  fileModel = new QFileSystemModel(this);
  fileModel->setRootPath(QDir::currentPath());
  fileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
  fileModel->setIconProvider(iconProvider);


  treeView->setModel(dirModel);
  treeView->setRootIndex(dirModel->index(QDir::currentPath()));

  connect(treeView, &QTreeView::clicked, this, &FileChooser::folderClicked);

  listView->setModel(fileModel);
  listView->setRootIndex(fileModel->index(QDir::currentPath()));

  connect(listView,&QListView::doubleClicked,this,&FileChooser::fileDoubleClicked);

  splitter->addWidget(treeView);
  splitter->addWidget(listView);

  splitter->setStretchFactor(0,1);
  splitter->setStretchFactor(1,2);
    splitter->setContentsMargins(5,5,5,5);

}

void FileChooser::folderClicked(const QModelIndex &index){
  QString sPath = dirModel->fileInfo(index).absoluteFilePath();
  listView->setRootIndex(fileModel->setRootPath(sPath));
}

void FileChooser::fileDoubleClicked(const QModelIndex &index){
  QString sPath = fileModel->fileInfo(index).absoluteFilePath();
  mainScreen->fileChosen(sPath);
}

FileChooser::~FileChooser(){
  delete treeView;
  delete listView;
  delete iconProvider;
}
