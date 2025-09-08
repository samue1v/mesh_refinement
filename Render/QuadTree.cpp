#include "QuadTree.hpp"
#include "Scene.hpp"

bool randomBool() {
    static auto gen = std::bind(std::uniform_int_distribution<>(0,1),std::default_random_engine());
    return gen();
}

Square::Square(){
  points.reserve(4);
  points.resize(4);
}
 
Square::Square(glm::vec3 tl,glm::vec3 br ){
  points.reserve(4);
  points.resize(4);
  points[PointLabel::TL] = Vertex(tl,red);
  points[PointLabel::BR] = Vertex(br,red);
}

void Square::computeSquarePoints(){
 
  glm::vec3 tl = points[PointLabel::TL].position;
  glm::vec3 br = points[PointLabel::BR].position;
  glm::vec3 tr = {br.x,tl.y,0};
  glm::vec3 bl = {tl.x,br.y,0};

  points[PointLabel::TR] = Vertex(tr,red);
  points[PointLabel::BL] = Vertex(bl,red);


}

glm::vec3 Square::getCenter(){
  glm::vec3 t = points.at(PointLabel::TL).position + 
                points.at(PointLabel::TR).position +
                points.at(PointLabel::BR).position +
                points.at(PointLabel::BL).position;
  
  return t/4.f;
}

Vertex Square::getPoint(int idx){
  return points[idx]; 
}

Node::Node(Node * _parent, Square * sq) : parent(_parent), square(sq){
  children.reserve(4);
  seq = -1;
}

Node::~Node()
{
  for (int i = 0; i < children.size(); ++i)
  {
    delete children[i];
  }
  delete square;
}

Node *Node::getChildAt(int index)
{
  return children[index];
}
NodeType Node::getStatus()
{
  return status;
}
Node *Node::getParent()
{
  return parent;
}

void Node::organizeChildren(){
  if(status!=NodeType::Mid){return;}
  std::swap(children[0],children[3]);
  std::swap(children[2],children[3]);
  for(int i = 0; i < 4; i++){
    children[i]->seq = i;
  }
}

bool Node::isPointInside(glm::vec3 p){

  glm::vec3 tl = square->getPoint(PointLabel::TL).position;
  glm::vec3 br = square->getPoint(PointLabel::BR).position;
  return (p.x >= tl.x && p.x <= br.x) && (p.y >= br.y && p.y <= tl.y);

}

Square *Node::getSquare()
{
  return square;
}

void Node::setParent(Node * _parent){
  parent = _parent;
}

void Node::setStatus(int _status){
  status = (NodeType)_status;
}

void Node::setSquare(Square * _square){
  square = _square;
}

void Node::setChild(Node * node){
  children.emplace_back(node);
}

int Node::getSeq(){
  return seq;
}

QuadTree::QuadTree(QOpenGLContext * _context,GLDrawable * _glObj,int _depth) : GLDrawable::GLDrawable(_glObj->getScene(),_context),glObj(_glObj), depth(_depth), rootNode(nullptr){

}

QuadTree::~QuadTree(){
  delete rootNode;
}

void QuadTree::getMaxDimensionAndCenter(float * dmax, glm::vec3 * center){
  glm::vec3 global_min = glObj->minValAxis();
  glm::vec3 global_max = glObj->maxValAxis();

  *center = glObj->getCenter();

  glm::vec3 res = global_max - global_min;
  *dmax = std::max({res.x, res.y, res.z});
}


void QuadTree::init(){
//TODO
  //PASSAR O PARSING PARA ESSA CLASSE E CONTINUAR O RESTO...(feito)
  //Colocar, no inicio de init, a chamada de execHandlers...(feito)
  Frame firstframe = Frame({0,localPoints.size(),0,lines.size(),0,0,glm::vec3(0,0,0),0});
  frames.push_back(firstframe);

  //pontos,linhas, triangulo e circulo...
  VAO.reserve(1);
  VAO.resize(1);


  VBO.reserve(1);
  VBO.resize(1);

  EBO.reserve(1);
  EBO.resize(1);

  program.reserve(1);
  program.resize(1);

  
  for(int i = 0; i < 1; i++){
    f->glGenVertexArrays(1,&VAO[i]);
    f->glGenBuffers(1,&VBO[i]);
    f->glGenBuffers(1,&EBO[i]);
    program[i] = GLProgram(currentContext);
  }

  //points,lines and triangles
  //program[0].createShaderFromFile("fill_vertex.vert","fill_frag.frag");
  program[0].createShaderFromFile("quad_vertex.vert","quad_frag.frag");
  //Points initialization, index 0

  f->glBindVertexArray(VAO[0]);
  
  f->glBindBuffer(GL_ARRAY_BUFFER,VBO[0]);
  f->glBufferData(GL_ARRAY_BUFFER,sizeof(Vertex) * localPoints.size(), localPoints.data(),GL_DYNAMIC_DRAW);




  f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
  f->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * localPointIndexes.size(),  localPointIndexes.data(),GL_DYNAMIC_DRAW);


  GLint point_position_attribute = f->glGetAttribLocation(program[0].getProgramId(), "position");
  GLint point_color_attribute = f->glGetAttribLocation(program[0].getProgramId(), "color_a");
  f->glVertexAttribPointer(point_position_attribute,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),0);
  f->glVertexAttribPointer(point_color_attribute,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void *)(sizeof(glm::vec3)));

  f->glEnableVertexAttribArray(point_position_attribute);
  f->glEnableVertexAttribArray(point_color_attribute);

  f->glBindBuffer(GL_ARRAY_BUFFER,0); //unbid current VBO
  f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0); //unbid current EBO
  f->glBindVertexArray(0); //unbind current VAO

}

void QuadTree::draw(){
  
//std::cout<<"Drawing...\n";
  f->glPointSize(4);
  f->glLineWidth(1);
    
  //f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  //Ver isso!!

  //ViewMatrix set

  f->glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

  uint p0ID = program[0].getProgramId();
  f->glUseProgram(p0ID);
  GLuint vmatrix = f->glGetUniformLocation(p0ID, "m_view");
  GLuint pmatrix = f->glGetUniformLocation(p0ID, "m_proj");
  f->glUniformMatrix4fv(vmatrix, 1, GL_FALSE, glm::value_ptr(currentScene->getCamera()->getViewMatrix()));
  f->glUniformMatrix4fv(pmatrix, 1, GL_FALSE, glm::value_ptr(currentScene->getCamera()->getProjMatrix()));

  //POINTS

  f->glBindBuffer(GL_ARRAY_BUFFER,VBO[0]);


  f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO[0]);


  f->glBindVertexArray(VAO[0]);

  //f->glUseProgram(program[0].getProgramId());
  f->glUseProgram(program[0].getProgramId());
  //????
  f->glDrawElements(GL_QUADS,localPointIndexes.size(),GL_UNSIGNED_INT,localPointIndexes.data());


  f->glBindVertexArray(0);
  f->glBindBuffer(GL_ARRAY_BUFFER,0);
  f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);


 
}

void QuadTree::initQuadTree(){

  directions.insert(directions.end(),{3,-1,-1,1});
  directions.insert(directions.end(),{2,0,-1,-1});
  directions.insert(directions.end(),{-1,3,1,-1});
  directions.insert(directions.end(),{-1,-1,0,2});
  float dmax = 0.0;
  glm::vec3 center(0.0, 0.0, 0.0);
  getMaxDimensionAndCenter(&dmax, &center);
  float halfdMax = dmax / 2.0f;
  glm::vec3 tl = glm::vec3(center.x - halfdMax, center.y + halfdMax, 0);

  glm::vec3 br = glm::vec3(center.x + halfdMax, center.y - halfdMax, 0);

  Square *sq = new Square(tl, br);
  sq->computeSquarePoints();
  rootNode = new Node(nullptr, sq);
  makeQuadTree(rootNode, depth);
  applyRule21();
  updateLeaves();
  insertPoints();
  fillRenderData(rootNode);
}

void QuadTree::makeQuadTree(Node * child, int depth){
  Node *root = nullptr;
  root = child;
  if (depth > 1){

    bool s = classify(root);
    //if true, subdivide
    if (s){
      root->setStatus(NodeType::Mid);
      subDivide(root);
      root->organizeChildren();
      for(int i = 0; i < 4;i++){
        makeQuadTree(root->getChildAt(i), depth - 1);
      }
    }
    else{
      root->setStatus(NodeType::Leaf);
      leaves.push_back(root);
    }
  }
  else{
    root->setStatus(NodeType::Leaf);
    leaves.push_back(root);
  }
}

void QuadTree::subDivide(Node * node){
  Square *parentBox = node->getSquare();
  calcBox(node, parentBox->getPoint(PointLabel::TL).position, parentBox->getPoint(PointLabel::BR).position, 2);
}

void QuadTree::calcBox(Node *node, glm::vec3 TL, glm::vec3 BR, uint8_t depth){
  glm::vec3 new_TL, new_BR;

  if (depth == 0)
  {
    Square *sq = new Square(TL, BR);
    sq->computeSquarePoints();

    Node *child = new Node(node, sq);
    node->setChild(child);

    return;
  }

  divideBox(TL, BR, new_TL, new_BR, depth);
  calcBox(node, new_TL, BR, depth - 1);
  calcBox(node, TL, new_BR, depth - 1);
}

void QuadTree::divideBox(const glm::vec3 &TL, const glm::vec3 &BR, glm::vec3 &new_TL, glm::vec3 &new_BR, uint8_t depth){
  int coord = depth - 1;
  float l = TL[coord] - BR[coord];
  new_TL = TL;
  new_TL[coord] = TL[coord] - (l / 2.0);

  new_BR = BR;
  new_BR[coord] = BR[coord] + (l / 2.0);
}

void QuadTree::fillRenderDataRec(Node * root){
  
  localPoints.push_back(root->getSquare()->getPoint(PointLabel::TL));
  localPoints.push_back(root->getSquare()->getPoint(PointLabel::TR));
  localPoints.push_back(root->getSquare()->getPoint(PointLabel::BR));
  localPoints.push_back(root->getSquare()->getPoint(PointLabel::BL));

  if(root->getStatus() == NodeType::Leaf){
    return;
  }

  for(int i = 0; i < 4; i++){
    fillRenderDataRec(root->getChildAt(i));
  }

}

void QuadTree::fillRenderData(Node * node){
  fillRenderDataRec(node);
  //std::cout<<"ini\n";
  //for(Vertex v: localPoints){
  //  glm::vec3 v3 = v.position;
  //  std::cout << "(" << v3.x <<","<<v3.y<<")"<<"\n";
  //}
  localPointIndexes.reserve(localPoints.size());
  localPointIndexes.resize(localPoints.size());
  std::iota(localPointIndexes.begin(),localPointIndexes.end(),0);
}

bool QuadTree::classify(Node * node){
  float sqLen = node->getSquare()->getPoint(PointLabel::TR).position.x - node->getSquare()->getPoint(PointLabel::TL).position.x;
  std::vector<Vertex> * points = glObj->getLocalPoints();
  for(std::pair<uint,uint> e : *(glObj->getLines())){
    glm::vec3 middlePoint = Misc::Util::edgeMiddle(points->at(e.first).position,points->at(e.second).position);
    if(node->isPointInside(middlePoint)){
      if(sqLen > Misc::Util::edgeLength(points->at(e.first).position,points->at(e.second).position)){
        return true;
      }
    }
  }
  return false;
}

void QuadTree::insertPoints(){
  for(Node * l : leaves){
    glm::vec3 center = l->getSquare()->getCenter();
    if(this->glObj->isPointInside(center)){
      //glObj->addNewVertex(center,glm::vec3(189.f, 52.f, 235.f)/255.f);
      glObj->addNewVertex(center,glm::vec3(255.f, 0.f, 0.f)/255.f,1);
    }
  }
}

void QuadTree::applyRule21(){
  std::queue<Node *> q;
  for(Node * n : leaves){
    q.push(n);
  }
  std::vector<Node *> n_nodes;
  while(!q.empty()){
    Node * current_leaf = q.front();
    if(current_leaf->getStatus()!=NodeType::Leaf){
      q.pop();
      continue;
    }
    bool gotDivided = false;
    for(int d = 0; d<4;d++){
      Node * ng_n = getNeighbour(current_leaf,d);
      if(ng_n != nullptr){
        if(ng_n->getStatus() == NodeType::Leaf){
          n_nodes.push_back(ng_n);
        }
        else{
          gotDivided = gotDivided || shouldSubdivide(current_leaf,ng_n,d);
        }
      } 
    }
    if(gotDivided){
      current_leaf->setStatus(NodeType::Mid);
      subDivide(current_leaf);
      for(int i = 0; i < 4;++i){
        current_leaf->getChildAt(i)->setStatus(NodeType::Leaf);
        q.push(current_leaf->getChildAt(i));
      }
      current_leaf->organizeChildren();
      for(Node * n : n_nodes){
        q.push(n);
      }

    }
    n_nodes.clear();
    q.pop();
  }
  
}

bool QuadTree::shouldSubdivide(Node * base, Node * nb, int dir){
  int newDir = (dir+2)%4;
  int s1 = (newDir+2)%4;
  int s2 = (newDir+3)%4;
  if(nb->getChildAt(s1)->getStatus() == NodeType::Mid 
  || nb->getChildAt(s2)->getStatus() == NodeType::Mid){
    return true;
  }
  return false;

}

Node * QuadTree::getNeighbour(Node * root, int dir){
  bool common_parent = false;
  Node * current_node = root;
  Node * common_parent_node = nullptr;
  std::vector<int> path;
  while(!common_parent){
    if(current_node->getParent() == nullptr){//root node
      break;
    }
    int ngPos = directions[current_node->getSeq()][dir];
    if(ngPos == -1){
      path.push_back(current_node->getSeq());
      current_node = current_node->getParent();//pode ser o raiz, analisar!!!!
    }
    else{
      common_parent = true;
      common_parent_node = current_node->getParent()->getChildAt(ngPos);
    }
  }
  if(!common_parent){
    return nullptr;//caso em que o vizinho nao existe
  }
  current_node = common_parent_node;
  int newDir = (dir+2)%4;
  int i;
  for(i = path.size()-1;i>=0;--i){
    if(current_node->getStatus() == NodeType::Leaf){
      break;
    }
    current_node = current_node->getChildAt(directions[path.at(i)][newDir]);
  }
  return current_node;

}

void QuadTree::updateLeaves(){
  leaves.clear();
  updateLeavesRecursive(rootNode);
}

void QuadTree::updateLeavesRecursive(Node * root){
  if(root->getStatus() == NodeType::Leaf){
    leaves.push_back(root);
  }
  else{
    for(int i = 0;i<4;++i){
      updateLeavesRecursive(root->getChildAt(i));
    }
  }
}

glm::vec3 QuadTree::getRootCenter(){
  return rootNode->getSquare()->getCenter();
}

float QuadTree::getRootHeight(){
  return (rootNode->getSquare()->getPoint(0).position.y - rootNode->getSquare()->getPoint(1).position.y);
}
