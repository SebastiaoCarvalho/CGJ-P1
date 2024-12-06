#ifndef MGL_DRAWABLE_HPP
#define MGL_DRAWABLE_HPP


////////////////////////////////////////////////////////////////////// IDrawable

namespace mgl
{

class IDrawable;
    
class IDrawable {
 public:
  virtual void draw(void) = 0;
};

} // mgl

#endif /* MGL_DRAWABLE_HPP */