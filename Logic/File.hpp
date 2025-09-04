#ifndef FILE_HPP
#define FILE_HPP

#include <string>
#include <vector>
#include <QDir>
#include <fstream>

class File{
  public:
  virtual std::string getAbsPath() = 0;
  virtual std::string getFileName() = 0;
  virtual std::string getExtensionName() = 0;
  virtual std::string getRoot() = 0;

};

class AbsFile : public File{
  public:
  AbsFile(std::string absPath = "");
  ~AbsFile() = default;

  std::string getAbsPath() override;
  std::string getFileName() override;
  std::string getExtensionName() override;
  std::string getRoot() override;



  protected:
  std::string absPath;
  std::string sPath;
  std::string fileName;
  std::string extName;
  std::string root;

};

#endif