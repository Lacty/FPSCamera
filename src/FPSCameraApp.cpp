
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "cinder/Camera.h"

#include <ApplicationServices/ApplicationServices.h>

using namespace ci;
using namespace ci::app;
using namespace std;

class FPSCameraApp : public AppNative {
private:
  Font font;
  
  CGSize  monitor;
  CGPoint center;
  
  Vec2i mouse_current;
  Vec2i mouse_last;
  
  CameraPersp camera;
  Vec3f eye;
  Vec3f target;
  
  Vec3f direction;
  
  Vec2f difference;
  
  float angle;
  
  void warpMousePos();
  
public:
  void setup() override;
  void prepareSettings(Settings* settings) override;
  
  void mouseMove(MouseEvent event) override;
  void mouseDown(MouseEvent event) override;
  void keyDown(KeyEvent event) override;
  
  void update() override;
  void draw() override;
};

void FPSCameraApp::setup() {
  font = Font(loadAsset("rounded-l-mplus-1c-light.ttf"), 40);
  
  // フルスクに設定
  setFullScreen(true);
  
  // モニターのサイズを取得
  monitor.width  = CGDisplayPixelsWide(0);
  monitor.height = CGDisplayPixelsHigh(0);
  
  // モニターの中点を割り出す
  center.x = monitor.width  / 2;
  center.y = monitor.height / 2;
  
  camera = CameraPersp(getWindowWidth(), getWindowHeight(), 60.0f, 0.5f, 600.0f);
  eye = Vec3f::zero();
  target = Vec3f(0, 0, 500.0f);
  direction = target - eye;
  camera.lookAt(eye, target);
  camera.setViewDirection(direction.normalized());
  camera.setWorldUp(Vec3f::yAxis());
  
  angle = 0;
  
  gl::enableAlphaBlending();
  glBlendFunc(GL_SRC0_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void FPSCameraApp::prepareSettings(Settings* settings) {
  settings->disableFrameRate();
}

void FPSCameraApp::mouseMove(MouseEvent event) {}

void FPSCameraApp::mouseDown(MouseEvent event) {}

void FPSCameraApp::keyDown(KeyEvent event) {
  quit();
}

void FPSCameraApp::update() {
  showCursor();
  // カーソルを非表示に設定
  //hideCursor();
  
  
  mouse_current = getMousePos();
  
  difference.x = mouse_current.x - mouse_last.x;
  
  angle += difference.x * 0.001;
  
  Quatf orientation = Quatf(Vec3f::yAxis(), angle);
  camera.setOrientation(orientation);
  
  mouse_last = mouse_current;
  
  
  // カーソルを移動
  warpMousePos();
}

void FPSCameraApp::draw() {
  gl::clear(Color(0.4, 0.4, 0.4));
  gl::setMatrices(camera);
  
  gl::pushModelView();
  gl::translate(Vec3f(0, 200, 500));
  gl::rotate(Vec3f(180, 180, 0));
  gl::drawStringCentered("FPSCamera", Vec2f::zero(), Color(1, 0.6, 0), font);
  gl::popModelView();
  
  gl::drawCube(Vec3f(0, 0, 300), Vec3f(50, 50, 50));
}


void FPSCameraApp::warpMousePos() {
  CGPoint pos;
  if (mouse_current.x >= monitor.width) {
    pos.x = 1;
    pos.y = mouse_current.y;
    mouse_last.x = pos.x;
    mouse_last.y = pos.y;
    CGWarpMouseCursorPosition(pos);
  }
  else
  if (mouse_current.x <= 0) {
    pos.x = monitor.width - 1;
    pos.y = mouse_current.y;
    mouse_last.x = pos.x;
    mouse_last.y = pos.y;
    CGWarpMouseCursorPosition(pos);
  }
}

CINDER_APP_NATIVE(FPSCameraApp, RendererGl)
