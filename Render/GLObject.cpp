#include "GLObject.hpp"
#include "Scene.hpp"

GLObject::GLObject(Scene * scene,QOpenGLContext * context){
  if(context != nullptr){
    currentContext = context;
    f = QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_3_3_Core>(context);
  }
  else{
    currentContext = nullptr;
    f = nullptr;
  }
  currentScene = scene;
  visibility = true;
  baseHandler = nullptr;

}

GLObject::GLObject(const GLObject & copyObj) 
: localPoints(copyObj.localPoints), 
  lines(copyObj.lines), 
  triangles(copyObj.triangles)

{
  sourceFilePointsSize = copyObj.sourceFilePointsSize;
  sourceFileLinesSize = copyObj.sourceFileLinesSize;
  sourceFileTrianglesSize = copyObj.sourceFileTrianglesSize;

  name = copyObj.name;
  
  program = copyObj.program;
  currentContext = copyObj.currentContext;
  f = QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_3_3_Core>(currentContext);

  currentFrame = 0;
  visibility = copyObj.visibility;
  currentScene = copyObj.currentScene;
}

GLObject & GLObject::operator=(const GLObject& copyObj){
  lines = copyObj.lines; 
  triangles = copyObj.triangles;
  //pointIndexes = copyObj.pointIndexes;
  sourceFilePointsSize = copyObj.sourceFilePointsSize;
  sourceFileLinesSize = copyObj.sourceFileLinesSize;
  sourceFileTrianglesSize = copyObj.sourceFileTrianglesSize;

  name = copyObj.name;
  
  program = copyObj.program;
  currentContext = copyObj.currentContext;
  if(f != nullptr){
    delete f;
  }
  f = QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_3_3_Core>(currentContext);

  currentFrame = 0;
  visibility = copyObj.visibility;
  currentScene = copyObj.currentScene;

  return *this;
}

GLObject::~GLObject(){
  delete baseHandler;
  //delete f;
}

void GLObject::init(){}

void GLObject::draw(){}


std::vector<Vertex> * GLObject::getLocalPoints(){
  return &localPoints;
}

std::vector<Triangle> * GLObject::getTriangles(){
  return &triangles;
}

std::vector<std::pair<uint,uint>> * GLObject::getLines(){
  return &lines;
}

void GLObject::addFrame(Frame f){
  frames.push_back(f);
}

std::vector<Frame> GLObject::getFrames(){
  return frames;
}

GLProgram & GLObject::getProgram(uint idx){
  return program[idx];
}

bool GLObject::nextFrame(){
  if(currentFrame == (int)frames.size()-1){
    return false;
  }
  currentFrame = std::min(currentFrame+1,(int)frames.size()-1);
  return true;

}

bool GLObject::previousFrame(){
  if(currentFrame == 0){
    return false;
  }
  currentFrame = std::max(currentFrame-1,0);
  return true;
}

Triangle GLObject::getCurrentTriangle(){
  return triangles[frames[currentFrame].TriangleEnd-1];
}

int GLObject::getCurrentFrame(){
  return currentFrame;
}

AbsHandler * GLObject::getBaseHandler(){
  return baseHandler;
}

void GLObject::setbaseHandler(AbsHandler * newHandler){
  baseHandler = newHandler;
}

int GLObject::pickTriangle(glm::vec3 p){
  glm::vec3 p0,p1,p2; 
  bool inside = false;
  for(int i = 0; i< currentFrame ;i++){
    //Vertex * v = triangles[i].getVertices();
    p0 = triangles[i].vertices[0]->position;
    p1 = triangles[i].vertices[1]->position;
    p2 = triangles[i].vertices[2]->position;
    //p0 = v[0].position;
    //p1 = v[1].position;
    //p2 = v[2].position;
    

    inside = Misc::Util::pointInsideTriangle(p,p0,p2,p1);
    if(inside){
      return i;
    }
  }
  return -1;
}

void GLObject::execHandlers(){
  if(baseHandler != nullptr){  baseHandler->handle(this);}

}

void GLObject::setVisible(bool vis){
  visibility = vis;
}

bool GLObject::isVisible(){
  return visibility;
}

void GLObject::restoreOriginal(){

  currentFrame = 0;

}

//get scene index from vertex v
int GLObject::getGlobalIndexFromVertex(Vertex v){
  int idx = currentScene->getIndexFromVertex(v);
  return idx;
}

////get Vertex at index in vertex idx
//Vertex GLObject::getGlobalVertexFromIndex(uint idx){
//  return currentScene->getVertexFromIndex(globalPointIndexes.at(idx));
//}

uint GLObject::addVertexToScene(Vertex v){
  uint idx = currentScene->addVertex(v);
  return idx;
}

std::vector<uint> * GLObject::getLocalIndexes(){
  return &localPointIndexes;
}

glm::vec3 GLObject::getCenter(){
  glm::vec3 res({0.f,0.f,0.f});
  glm::vec3 maxV = maxValAxis();
  glm::vec3 minV = minValAxis();
  glm::vec3 dif = (maxV-minV);
  res[0] = minV[0] + dif[0]/2.f;
  res[1] = minV[1] + dif[1]/2.f;
  return res;
}

glm::vec3 GLObject::maxValAxis(){
  float maxFloat = 2e22;
  glm::vec3 max({-maxFloat,-maxFloat,-maxFloat});
  for(Vertex v : localPoints){
    glm::vec3 p = v.position;
    for(int i = 0;i<3;i++){
      if(p[i] > max[i]){
        max[i] = p[i];
      }
    }
  }

  return max;
}

glm::vec3 GLObject::minValAxis(){
  float maxFloat = 2e22;
  glm::vec3 min({maxFloat,maxFloat,maxFloat});
  for(Vertex v : localPoints){
    glm::vec3 p = v.position;
    for(int i = 0;i<3;i++){
      if(p[i] < min[i]){
        min[i] = p[i];
      }
    }
  }

  return min;
}

Scene * GLObject::getScene(){
  return currentScene;
}

void GLObject::addNewVertex(glm::vec3 pos, glm::vec3 color,bool isActive){
  Vertex newVertex = {pos,color,isActive};
  localPoints.push_back(newVertex);
  uint newIdx = localPointIndexes.size();
  localPointIndexes.push_back(newIdx);
}

bool GLObject::isPointInside(glm::vec3 p){

  glm::vec3 global_min = minValAxis();
  glm::vec3 global_max = maxValAxis();

  glm::vec3 res = global_max - global_min;
  float dmax = std::max({res.x, res.y, res.z});
  std::vector<glm::vec3> limitPoints;
  glm::vec3 p_right({p.x+dmax+1,p.y,p.z});
  glm::vec3 p_down({p.x,p.y-dmax-1,p.z});
  glm::vec3 p_left({p.x-dmax-1,p.y,p.z});
  glm::vec3 p_up({p.x,p.y+dmax+1,p.z});

  limitPoints.push_back(p_right);
  limitPoints.push_back(p_down);
  limitPoints.push_back(p_left);
  limitPoints.push_back(p_up);

  for(glm::vec3 lp : limitPoints){
    int intersect = 0;
    for(int i = 0;i<sourceFileLinesSize;i++){
      std::pair<uint,uint> curEdge = lines.at(i);
      if(Misc::Util::doIntersect(p,lp,localPoints.at(curEdge.first).position,localPoints.at(curEdge.second).position)){
        intersect += 1;
        //break;
      }
    }
    if(intersect % 2 == 0) {
      return false;
    }
  }

  return true;

}
