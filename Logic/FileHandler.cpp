#include "FileHandler.hpp"
#include "../Render/GLObject.hpp"

AbsFileHandler::AbsFileHandler() {}

AbsFileHandler::~AbsFileHandler() {}

void
AbsFileHandler::readFile(std::string fileSrc)
{
  clear();
  setPaths(fileSrc);
  std::string line = "";
  std::string result = "";
  std::fstream myObj;
  myObj.open(sPath, std::ios::in);
  if (myObj.is_open())
  {
    while (std::getline(myObj, line))
    {
      result.append(line);
    }
  }
}

void
AbsFileHandler::setPaths(std::string fileSrc)
{
  file = AbsFile(fileSrc);
  sPath = fileSrc;
  std::string destFolder = file.getRoot().append("/objFilesResult");
  QDir dir = QDir(QString::fromStdString(destFolder));
  if (!dir.exists())
  {
    dir.mkpath(".");
  }

  dPath = dir.path().toStdString().append("/").append("RES_").append(file.getFileName());
}

void
AbsFileHandler::clear()
{
  strings.clear();
  sPath = "";
  dPath = "";
}

void
AbsFileHandler::writeFile()
{

  std::string result = "";
  std::string line = "";
  std::fstream myObj;
  myObj.open(dPath, std::ios::out);
  bool isOpen = myObj.is_open();
  for (std::string s : strings)
  {
    myObj << s;
  }

  myObj.close();
}

uint
AbsFileHandler::getNumStrings() const
{
  return strings.size();
}

///////////////

GLFileHandler::GLFileHandler() {}

void
GLFileHandler::parse(GLSimpleMesh* obj, int num)
{
  // for(std::string s : strings){}
}

///////////////

ObjFileHandler::ObjFileHandler() {}

void
ObjFileHandler::readFile(std::string fileSrc)
{
  clear();
  setPaths(fileSrc);
  std::string result = "";
  std::string line = "";
  std::fstream myObj;
  myObj.open(sPath, std::ios::in);
  int objCounter = 0;
  if (myObj.is_open())
  {
    while (std::getline(myObj, line))
    {
      if (line[0] == 'o' && line[1] == ' ')
      {
        if (objCounter > 0)
        {
          strings.push_back(result);
          result = "";
        }
        objCounter++;
      }

      result.append(line).append("\n");
    }
    strings.push_back(result);
  }
}

void
ObjFileHandler::parse(GLSimpleMesh* obj, int stringNum)
{
  auto pointIndexes = obj->getIndexes();
  auto points = obj->getPoints();
  auto lines = obj->getLines();
  auto triangles = obj->getTriangles();

  std::string objString = strings[stringNum];
  std::istringstream f(objString);
  std::string line = "";
  bool line_seen = false;
  int first = 1;
  while (std::getline(f, line))
  {
    if (line[0] == 'o' && line[1] == ' ')
    {
      obj->name = line.substr(2, line.size() - 1);
    }
    else if (line[0] == 'v' && line[1] == ' ')
    {
      getV(obj, points, line.substr(2, line.size() - 1));
    }
    else if (line[0] == 'l' && line[1] == ' ')
    {
      if (!line_seen)
      {
        line_seen = true;
        std::string delimiter = " ";
        std::string r = line.substr(2);
        r = r.substr(0, r.find(delimiter));
        first = std::stoi(r);
      }

      getL(obj, lines, line.substr(2, line.size() - 1), first);
    }
  }

  pointIndexes->reserve(points->size());
  pointIndexes->resize(points->size());
  std::iota(pointIndexes->begin(), pointIndexes->end(), 0);
  obj->sourceFilePointsSize = points->size();
  obj->sourceFileLinesSize = lines->size();
  obj->sourceFileTrianglesSize = triangles->size();
  return;
}

void
ObjFileHandler::getL(GLSimpleMesh* obj,
                     std::vector<std::pair<uint, uint>>* lines,
                     std::string str,
                     int mod)
{
  std::pair<uint, uint> out;
  std::stringstream ss(str);
  std::string result = "";
  std::getline(ss, result, ' ');
  out.first = std::stoi(result, 0) - mod;
  std::getline(ss, result, ' ');
  out.second = std::stoi(result, 0) - mod;
  lines->push_back(out);
}

void
ObjFileHandler::getV(GLSimpleMesh* obj, std::vector<Vertex>* vec, std::string str)
{
  std::stringstream ss(str);
  std::string result = "";
  int pos = 0;
  glm::vec3 out;
  while (std::getline(ss, result, ' '))
  {
    out[pos] = (std::stof(result, 0));
    pos++;
  }
  out[2] = 0.f;
  vec->push_back(Vertex(out, glm::vec3(0, 0, 0),0));
}

// void ObjFileHandler::parseToText(GLObject * obj,uint stringNum){
//   auto pointIndexes = obj->getIndexes();
//   auto points = obj->getPoints();
//   auto lines = obj->getLines();
//   auto triangles = obj->getTriangles();
//
//   std::string result = "";
//
//   result.append("o " + obj->name + "\n");
//   for(auto p : *points){
//     std::string w = "v " + std::to_string(p.position.x) + " " + std::to_string(p.position.y) +
//     "\n";
//    result.append(w);
//   }
//
//   for(auto l : *lines){
//     std::string w = "l " + std::to_string(l.first+1) + " " + std::to_string(l.second+1) + "\n";
//     result.append(w);
//   }
//
//   outStrings[stringNum] = result;
//
// }

// JmeshFileAdapter::JmeshFileAdapter(GLFileHandler * handler,std::string _sPath ) :
// objHandler(handler), GLFileHandler(_sPath){
// }
//
// void JmeshFileAdapter::readFile(){
//   std::string line = "";
//   std::string result = "";
//   std::fstream myObjIN;
//   myObjIN.open(sPath, std::ios::in);
//   int infoShift = 0;
//   int objcounter = 0;
//   int vertexcounter = 0;
//   if (myObjIN.is_open())
//   {
//     while (std::getline(myObjIN, line)){
//       if(infoShift < 3){
//         ++infoShift;
//         continue;
//       }
//
//       if ((line[0] == '-' && line[1] == '1')){
//         if(strings.size()>0){
//           strings.push_back(result);
//           result = "";
//           addLinesToString(objcounter-1,vertexcounter);
//           vertexcounter = 0;
//         }
//         result.append("o ").append(std::to_string(objcounter)).append("\n");
//         objcounter++;
//       }
//       else{
//         std::string vertex = std::string("v ").append(result.substr(result.find_first_of("
//         ")+1)).append(std::string(" 0\n"));
//         //pointStrings.push_back(vertex);
//         result.append(vertex);
//         vertexcounter++;
//       }
//     }
//   }
//
//
//   writeFile();//write the new OBJ file
//   if(objHandler!=nullptr){
//     objHandler->readFile();
//   }
//   return;
// }
//
// void JmeshFileAdapter::addLinesToString(int stringIdx,int numVertex){
//   for(int i = 0; i < numVertex;i++){
//     std::pair<uint,uint> out;
//     out.first = i%numVertex;
//     out.second = (i+1)%numVertex;
//     std::string line = std::string("l ").append(std::to_string(out.first+1)).append("
//     ").append(std::to_string(out.second+1)).append("\n"); strings[stringIdx].append(line);
//   }
// }
//
//
//
// void JmeshFileAdapter::writeFile(){
//
//   //size_t fileRootPos = dPath.find_last_of("/");
//   //std::string fileRoot = dPath.substr(0,fileRootPos);
//   //std::string fileName = dPath.substr(fileRootPos+1);
//   //size_t extensionPos = fileName.find_last_of(".");
//   //std::string rawFileName = fileName.substr(0,extensionPos);
//   size_t extensionPos = sPath.find_last_of(".");
//   std::string objdPath = sPath.substr(0,extensionPos).append(".obj");
//
//   QFileInfo check_file(QString::fromStdString(objdPath));
//   if(check_file.exists() && check_file.isFile()){
//     QFile file(QString::fromStdString(objdPath));
//     file.remove();
//   }
//   std::fstream myObjOUT;
//   myObjOUT.open(objdPath, std::ios::out);
//   for(std::string s : strings){
//     myObjOUT << s;
//   }
//   myObjOUT.close();
// }
