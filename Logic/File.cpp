#include "File.hpp"

AbsFile::AbsFile(std::string _absPath) : absPath(_absPath){
  if(absPath!=""){
    size_t namePos = absPath.find_last_of("/");
    size_t extPos = absPath.find_last_of(".");

    fileName = absPath.substr(namePos+1,extPos);
    extName = absPath.substr(extPos+1);
    root = absPath.substr(0,namePos);
  }
  else{
    fileName = "";
    extName = "";
    root = "";
  }
} 

std::string AbsFile::getAbsPath(){
  return absPath;
}

std::string AbsFile::getExtensionName(){
  return extName;
}

std::string AbsFile::getFileName(){
  return fileName;
}

std::string AbsFile::getRoot(){
  return root;
}

