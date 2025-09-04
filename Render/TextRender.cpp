#include "TextRender.hpp"
#include "Scene.hpp"

TextRender::TextRender(Scene* scene, QOpenGLContext* context)
  : GLObject::GLObject(scene, context)
{
  if (FT_Init_FreeType(&ft))
  {
    throw std::runtime_error("FreeType lib failed to init.\n");
  }
  QDir dir(QDir::currentPath());
  dir.cd("Misc/Fonts/static");
  std::string font_name = "Roboto-Black.ttf";

  if (!dir.exists(QString::fromStdString(font_name)))
  {
    throw std::runtime_error("No such font file " + font_name + " found.\n");
  }

  if (FT_New_Face(ft,
                  dir.absoluteFilePath(QString::fromStdString(font_name)).toStdString().c_str(),
                  0,
                  &face))
  {
    throw std::runtime_error("Failed to load font " + font_name + " .\n");
  }

  FT_Set_Pixel_Sizes(face, 0, 48);
}

void TextRender::init() {
    // Initialize buffers
    VAO.resize(1);
    VBO.resize(1);
    f->glGenVertexArrays(1, &VAO[0]);
    f->glGenBuffers(1, &VBO[0]);

    // Load shaders
    program.resize(1);
    program[0] = GLProgram(currentContext);
    program[0].createShaderFromFile("text.vert", "text.frag");

    // Load characters
    f->glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            continue;
        }

        GLuint texture;
        f->glGenTextures(1, &texture);
        f->glBindTexture(GL_TEXTURE_2D, texture);
        f->glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0, GL_RED, GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Characters.insert({(char)c, {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<unsigned int>(face->glyph->advance.x)
        }});
    }

    // Configure VAO/VBO
    f->glBindVertexArray(VAO[0]);
    f->glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    f->glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    f->glBindBuffer(GL_ARRAY_BUFFER, 0);
    f->glBindVertexArray(0);
}

void TextRender::draw() {

    f->glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    f->glUseProgram(program[0].getProgramId());
    f->glEnable(GL_BLEND);
    f->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    f->glDisable(GL_DEPTH_TEST);

    glm::mat4 proj = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
    f->glUniformMatrix4fv(f->glGetUniformLocation(program[0].getProgramId(), "m_proj"), 
                        1, GL_FALSE, glm::value_ptr(currentScene->getCamera()->getProjMatrix()/* proj */));
    glm::mat4 view(1.0f);
    f->glUniformMatrix4fv(f->glGetUniformLocation(program[0].getProgramId(), "m_view"), 
                        1, GL_FALSE, glm::value_ptr(currentScene->getCamera()->getViewMatrix()/* view */));

    for (Text& t : TextList) {
        f->glUniform3f(f->glGetUniformLocation(program[0].getProgramId(), "textColor"), 
                      t.color.x, t.color.y, t.color.z);

        float x = t.x;
        std::string::const_iterator c;
        for (c = t.text.begin(); c != t.text.end(); c++) {
            Character ch = Characters[*c];

            float xpos = x + ch.Bearing.x * t.scale;
            float ypos = t.y - (ch.Size.y - ch.Bearing.y) * t.scale;
            float w = ch.Size.x * t.scale;
            float h = ch.Size.y * t.scale;

            float vertices[6][4] = {
                {xpos,     ypos + h,   0.0f, 0.0f},
                {xpos,     ypos,       0.0f, 1.0f},
                {xpos + w, ypos,       1.0f, 1.0f},

                {xpos,     ypos + h,   0.0f, 0.0f},
                {xpos + w, ypos,       1.0f, 1.0f},
                {xpos + w, ypos + h,   1.0f, 0.0f}
            };

            f->glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            f->glBindVertexArray(VAO[0]);
            f->glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
            f->glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            f->glDrawArrays(GL_TRIANGLES, 0, 6);

            x += (ch.Advance >> 6) * t.scale;
        }
    }
    f->glBindVertexArray(0);
    f->glBindTexture(GL_TEXTURE_2D, 0);
    
}

/**
void
TextRender::init()
{
  // ... (keep your existing buffer and program initialization) ...
  VAO.reserve(1);
  VAO.resize(1);

  VBO.reserve(1);
  VBO.resize(1);

  EBO.reserve(1);
  EBO.resize(1);

  program.reserve(1);
  program.resize(1);

  for (int i = 0; i < 1; i++)
  {
    f->glGenVertexArrays(1, &VAO[i]);
    f->glGenBuffers(1, &VBO[i]);
    f->glGenBuffers(1, &EBO[i]);
    program[i] = GLProgram(currentContext);
  }

  program[0].createShaderFromFile("text.vert", "text.frag");

  // Keep alignment at 1 for the entire font loading process
  f->glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  for (unsigned char c = 0; c < 128; c++)
  {
    if (FT_Load_Char(face, c, FT_LOAD_RENDER))
    {
      std::cerr << "Failed to load Glyph for character: " << c << std::endl;
      continue;
    }


    GLuint texture;
    f->glGenTextures(1, &texture);
    f->glBindTexture(GL_TEXTURE_2D, texture);
    f->glTexImage2D(GL_TEXTURE_2D,
                    0,
                    GL_RED,
                    face->glyph->bitmap.width,
                    face->glyph->bitmap.rows,
                    0,
                    GL_RED,
                    GL_UNSIGNED_BYTE,
                    face->glyph->bitmap.buffer);

    // Improved texture parameters
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    f->glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    Character character = { texture,
                            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                            static_cast<unsigned int>(face->glyph->advance.x) };
    Characters.insert(std::pair<GLchar, Character>(c, character));
  }

  // VAO/VBO setup with proper vertex attributes
  f->glBindVertexArray(VAO[0]);
  f->glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
  f->glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

  // Proper vertex attribute setup
  f->glEnableVertexAttribArray(0);
  f->glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);

  f->glBindBuffer(GL_ARRAY_BUFFER, 0);
  f->glBindVertexArray(0);

  // Don't reset unpack alignment here - keep it at 1
}
void TextRender::draw() {
    // 1. Basic OpenGL state validation
    if (!f) {
        std::cerr << "Error: QOpenGLFunctions not initialized" << std::endl;
        return;
    }

    // 2. Program validation
    if (program.empty() || !program[0].getProgramId()) {
        std::cerr << "Error: Shader program not initialized" << std::endl;
        return;
    }
    GLuint p0ID = program[0].getProgramId();

    // 3. Character validation
    if (Characters.empty() || Characters.find('A') == Characters.end()) {
        std::cerr << "Error: Character 'A' not loaded (total chars: " 
                 << Characters.size() << ")" << std::endl;
        return;
    }
    Character ch = Characters['A'];

    // 4. Texture validation
    if (ch.TextureID == 0 || !f->glIsTexture(ch.TextureID)) {
        std::cerr << "Error: Invalid texture ID" << std::endl;
        return;
    }

    // 5. VAO/VBO safety check
    if (VAO.empty() || VBO.empty() || !VAO[0] || !VBO[0]) {
        std::cerr << "Error: VAO/VBO not properly initialized" << std::endl;
        return;
    }

    // Setup render state
    f->glUseProgram(p0ID);
    f->glDisable(GL_DEPTH_TEST);
    f->glDisable(GL_CULL_FACE);
    f->glEnable(GL_BLEND);
    f->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set matrices
    glm::mat4 proj = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
    f->glUniformMatrix4fv(f->glGetUniformLocation(p0ID, "m_proj"), 
                        1, GL_FALSE, glm::value_ptr(proj));
    glm::mat4 view(1.0f);
    f->glUniformMatrix4fv(f->glGetUniformLocation(p0ID, "m_view"), 
                        1, GL_FALSE, glm::value_ptr(view));

    // Calculate proper character position
    float xpos = 100.0f + ch.Bearing.x;  // Adjust for bearing
    float ypos = 100.0f - (ch.Size.y - ch.Bearing.y); // Baseline adjustment

    // Update VBO for character 'A'
    float w = ch.Size.x;
    float h = ch.Size.y;
    float vertices[6][4] = {
        {xpos,     ypos + h,   0.0f, 0.0f},  // Top-left
        {xpos,     ypos,       0.0f, 1.0f},  // Bottom-left
        {xpos + w, ypos,       1.0f, 1.0f},  // Bottom-right

        {xpos,     ypos + h,   0.0f, 0.0f},  // Top-left
        {xpos + w, ypos,       1.0f, 1.0f},  // Bottom-right
        {xpos + w, ypos + h,   1.0f, 0.0f}   // Top-right
    };

    // Render the character
    f->glBindVertexArray(VAO[0]);
    f->glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    f->glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

    f->glActiveTexture(GL_TEXTURE0);
    f->glBindTexture(GL_TEXTURE_2D, ch.TextureID);
    f->glUniform1i(f->glGetUniformLocation(p0ID, "text"), 0);
    f->glUniform3f(f->glGetUniformLocation(p0ID, "textColor"), 1.0f, 1.0f, 1.0f);

    f->glDrawArrays(GL_TRIANGLES, 0, 6);

    // Check for errors
    GLenum err;
    while ((err = f->glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << err << std::endl;
    }
}
*/
/*
void TextRender::draw() {
    // Setup
    f->glUseProgram(program[0].getProgramId());
    f->glDisable(GL_DEPTH_TEST);
    f->glDisable(GL_CULL_FACE);
    f->glEnable(GL_BLEND);
    f->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Simple ortho projection
    glm::mat4 proj = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
    f->glUniformMatrix4fv(f->glGetUniformLocation(program[0].getProgramId(), "m_proj"), 
                        1, GL_FALSE, glm::value_ptr(proj));
    glm::mat4 view(1.0f);
    f->glUniformMatrix4fv(f->glGetUniformLocation(program[0].getProgramId(), "m_view"), 
                        1, GL_FALSE, glm::value_ptr(view));
    
    // Render just 'A' at fixed position
    Character ch = Characters['A'];
    float x = 100, y = 100;
    
    float vertices[6][4] = {
        {x, y+ch.Size.y, 0,0}, {x, y, 0,1}, {x+ch.Size.x, y, 1,1},
        {x, y+ch.Size.y, 0,0}, {x+ch.Size.x, y, 1,1}, {x+ch.Size.x, y+ch.Size.y, 1,0}
    };
    
    f->glBindVertexArray(VAO[0]);
    f->glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    f->glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    
    f->glActiveTexture(GL_TEXTURE0);
    f->glBindTexture(GL_TEXTURE_2D, ch.TextureID);
    f->glUniform1i(f->glGetUniformLocation(program[0].getProgramId(), "text"), 0);
    f->glUniform3f(f->glGetUniformLocation(program[0].getProgramId(), "textColor"), 1,1,1);
    
    f->glDrawArrays(GL_TRIANGLES, 0, 6);
    
    // Check for errors
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << err << std::endl;
    }
}*/
/*
void
TextRender::draw()
{
  uint p0ID = program[0].getProgramId();
  f->glUseProgram(p0ID);

  // Set matrices
  f->glUniformMatrix4fv(f->glGetUniformLocation(p0ID, "m_view"),
                        1,
                        GL_FALSE,
                        glm::value_ptr(currentScene->getCamera()->getViewMatrix()));
  f->glUniformMatrix4fv(f->glGetUniformLocation(p0ID, "m_proj"),
                        1,
                        GL_FALSE,
                        glm::value_ptr(currentScene->getCamera()->getProjMatrix()));

  // Critical state settings
  f->glDisable(GL_CULL_FACE); // Disable culling for text
  f->glEnable(GL_BLEND);
  f->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  f->glActiveTexture(GL_TEXTURE0);
  f->glBindVertexArray(VAO[0]);

  for (Text t : TextList)
  {
    f->glUniform3f(f->glGetUniformLocation(p0ID, "textColor"), t.color.x, t.color.y, t.color.z);

    // Iterate through all characters
    std::string::const_iterator c;
    for (c = t.text.begin(); c != t.text.end(); c++)
    {
      Character ch = Characters[*c];

      // Improved position calculation with bearing handling
      float xpos = t.x + ch.Bearing.x * t.scale;
      float ypos = t.y + (ch.Bearing.y - ch.Size.y) * t.scale; // Fixed Y positioning

      float w = ch.Size.x * t.scale;
      float h = ch.Size.y * t.scale;

      // Update VBO
      float vertices[6][4] = { { xpos, ypos + h, 0.0f, 0.0f },    { xpos, ypos, 0.0f, 1.0f },
                               { xpos + w, ypos, 1.0f, 1.0f },

                               { xpos, ypos + h, 0.0f, 0.0f },    { xpos + w, ypos, 1.0f, 1.0f },
                               { xpos + w, ypos + h, 1.0f, 0.0f } };

      f->glBindTexture(GL_TEXTURE_2D, ch.TextureID);
      f->glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
      f->glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
      f->glBindBuffer(GL_ARRAY_BUFFER, 0);


// In your draw() function, before glDrawArrays:
GLint boundTexture;
glGetIntegerv(GL_TEXTURE_BINDING_2D, &boundTexture);
std::cout << "Drawing char '" << *c << "' with texture ID: " << boundTexture 
          << " (expected: " << ch.TextureID << ")\n";

if (boundTexture != ch.TextureID) {
    std::cerr << "TEXTURE BINDING MISMATCH!\n";
    glBindTexture(GL_TEXTURE_2D, ch.TextureID); // Force correct binding
}

GLenum err;
while ((err = glGetError()) != GL_NO_ERROR) {
    std::cerr << "OpenGL error pre-draw: " << err << std::endl;
}
      f->glDrawArrays(GL_TRIANGLES, 0, 6);

      // Advance cursor (handle negative bearings)
      t.x += (ch.Advance >> 6) * t.scale;
    }
  }

  // Cleanup
  f->glBindVertexArray(0);
  f->glBindTexture(GL_TEXTURE_2D, 0);
  f->glDisable(GL_BLEND);
}
*/

/*
void
TextRender::init()
{ // TODO

  VAO.reserve(1);
  VAO.resize(1);

  VBO.reserve(1);
  VBO.resize(1);

  EBO.reserve(1);
  EBO.resize(1);

  program.reserve(1);
  program.resize(1);

  for (int i = 0; i < 1; i++)
  {
    f->glGenVertexArrays(1, &VAO[i]);
    f->glGenBuffers(1, &VBO[i]);
    f->glGenBuffers(1, &EBO[i]);
    program[i] = GLProgram(currentContext);
  }

  program[0].createShaderFromFile("text.vert", "text.frag");

  f->glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  int count = 0;
  for (unsigned char c = 0; c < 128; c++)
  {
    if (FT_Load_Char(face, c, FT_LOAD_RENDER))
    {
      throw std::runtime_error("Failed to load Glyph for letter \n");
      continue;
    }
    else
    {std::cout << "Char '" << (char)c << "' "
          << "W:" << face->glyph->bitmap.width << " "
          << "H:" << face->glyph->bitmap.rows << " "
          << "BearingX:" << face->glyph->bitmap_left << " "
          << "BearingY:" << face->glyph->bitmap_top << " "
          << "Advance:" << (face->glyph->advance.x >> 6) << "px\n";

    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RED,
                 face->glyph->bitmap.width,
                 face->glyph->bitmap.rows,
                 0,
                 GL_RED,
                 GL_UNSIGNED_BYTE,
                 face->glyph->bitmap.buffer);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    Character character = { texture,
                            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                            static_cast<unsigned int>(face->glyph->advance.x) };
    Characters.insert(std::pair<GLchar, Character>(c, character));
  }
  FT_Done_Face(face);
  FT_Done_FreeType(ft);

  // Configure VAO/VBO for texture quads
  f->glBindVertexArray(VAO[0]);
  f->glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
  f->glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

  GLint vertex_attribute = f->glGetAttribLocation(program[0].getProgramId(), "vertex");
  f->glVertexAttribPointer(vertex_attribute, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

  f->glEnableVertexAttribArray(vertex_attribute);

  f->glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  f->glBindBuffer(GL_ARRAY_BUFFER, 0);         // unbid current VBO
  f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // unbid current EBO
  f->glBindVertexArray(0);                     // unbind current VAO
}

void
TextRender::draw()
{
  uint p0ID = program[0].getProgramId();
  f->glUseProgram(p0ID);
  GLuint vmatrix = f->glGetUniformLocation(p0ID, "m_view");
  GLuint pmatrix = f->glGetUniformLocation(p0ID, "m_proj");
  f->glUniformMatrix4fv(
    vmatrix, 1, GL_FALSE, glm::value_ptr(currentScene->getCamera()->getViewMatrix()));
  f->glUniformMatrix4fv(
    pmatrix, 1, GL_FALSE, glm::value_ptr(currentScene->getCamera()->getProjMatrix()));

  f->glEnable(GL_CULL_FACE);
  f->glEnable(GL_BLEND);
  f->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  for (Text t : TextList)
  {
    f->glUniform3f(f->glGetUniformLocation(p0ID, "textColor"), t.color.x, t.color.y, t.color.z);
    f->glActiveTexture(GL_TEXTURE0);
    f->glBindVertexArray(VAO[0]);

    // iterate through all characters
    std::string::const_iterator c;
    for (c = t.text.begin(); c != t.text.end(); c++)
    {
      Character ch = Characters[*c];

      float xpos = t.x + ch.Bearing.x * t.scale;
      float ypos = t.y - (ch.Size.y - ch.Bearing.y) * t.scale;

      float w = ch.Size.x * t.scale;
      float h = ch.Size.y * t.scale;
      // update VBO for each character
      float vertices[6][4] = { { xpos, ypos + h, 0.0f, 0.0f },    { xpos, ypos, 0.0f, 1.0f },
                               { xpos + w, ypos, 1.0f, 1.0f },

                               { xpos, ypos + h, 0.0f, 0.0f },    { xpos + w, ypos, 1.0f, 1.0f },
                               { xpos + w, ypos + h, 1.0f, 0.0f } };
      // render glyph texture over quad
      f->glBindTexture(GL_TEXTURE_2D, ch.TextureID);
      // update content of VBO memory
      f->glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
      f->glBufferSubData(GL_ARRAY_BUFFER,
                         0,
                         sizeof(vertices),
                         vertices); // be sure to use glBufferSubData and not glBufferData

      f->glBindBuffer(GL_ARRAY_BUFFER, 0);
      // render quad
      f->glDrawArrays(GL_TRIANGLES, 0, 6);
      // now advance cursors for next glyph (note that advance is number of 1/64
      // pixels)
      t.x += (ch.Advance >> 6) * t.scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide
                                          // amount of 1/64th pixels by 64 to get amount of pixels))
    }
  }
  f->glBindVertexArray(0);
  f->glBindBuffer(GL_ARRAY_BUFFER, 0);
  f->glDisable(GL_CULL_FACE);
  f->glDisable(GL_BLEND);
}*/

void
TextRender::addText(std::string text, float x, float y, float scale, glm::vec3 color)
{
  TextList.push_back({ text, x, y, scale, color });
}

void
TextRender::restoreOriginal()
{
}

void TextRender::clearTextList(){
  TextList.clear();
}