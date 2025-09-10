#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H
#include <string>
#include <vector>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include "File.hpp"

//FILE HANDLER INTERFACE
class FileHandler{
public:
  virtual void readFile(std::string file = "") = 0;
  virtual void writeFile() = 0;
};


class GLSimpleMesh;
class Vertex;


class AbsFileHandler : public FileHandler{
public:
  AbsFileHandler();
  virtual ~AbsFileHandler();

  //Read sPath file and write to inStrings buffer
  void readFile(std::string file = "") override;
  //write outString buffer data to dPath file
  void writeFile() override;
  uint getNumStrings() const;
protected:
  void setPaths(std::string fileSrc);
  void clear();
protected:
  std::vector<std::string> strings;
  AbsFile file;
  std::string sPath;
  std::string dPath;

};

class GLFileHandler : public AbsFileHandler{
public:
  GLFileHandler();
  ~GLFileHandler() = default;
  virtual void parse(GLSimpleMesh * obj,int num);
};

class ObjFileHandler : public GLFileHandler{
public:
  ObjFileHandler();
  ~ObjFileHandler() = default;

  void readFile(std::string file = "") override;
  void parse(GLSimpleMesh * obj,int num);

private:
  void getL(GLSimpleMesh* obj,std::vector<std::pair<uint, uint>> *lines, std::string str, int mod);
  void getV(GLSimpleMesh* obj,std::vector<Vertex> *vec, std::string str);
}; 

//class JmeshFileAdapter : public GLFileHandler{
//public:
//  JmeshFileAdapter(GLFileHandler * handler, std::string sPath = "");
//  ~JmeshFileAdapter() = default;
//
//  void readFile() override;
//  void writeFile() override;
//private:
//  void addLinesToString(int stringIdx,int numVertex);
//private:
//  GLFileHandler * objHandler;
//
//};

#endif
