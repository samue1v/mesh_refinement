#ifndef TEXT_RENDER_HPP
#define TEXT_RENDER_HPP
#include <freetype2/ft2build.h>
#include <iostream>
#include <string>
#include <vector>
#include FT_FREETYPE_H
#include "GLObject.hpp"
#include <QDir>
#include <QString>
#include <exception>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <map>

class TextRender : public GLObject
{

public:
  TextRender(Scene* scene = 0, QOpenGLContext* context = 0);
  ~TextRender() = default;
  void init() override;
  void draw() override;
  void restoreOriginal() override;
  void addText(std::string text,
                  float x,
                  float y,
                  float scale,
                  glm::vec3 color);
  void clearTextList();

private:
  struct Character
  {
    unsigned int TextureID;
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    unsigned int Advance;
  };
  struct Text
  {
    std::string text;
    float x;
    float y;
    float scale;
    glm::vec3 color;

  };
  std::map<char, Character> Characters;
  std::vector<Text> TextList;
  FT_Library ft;
  FT_Face face;
};

#endif // TEXT_RENDER_HPP