
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "cinder/Camera.h"
#include "cinder/Matrix.h"

#include <ApplicationServices/ApplicationServices.h>

#define PI 3.14159265358979

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
  
  Vec2f angle;
  
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
  
  angle = Vec2f(PI, PI);
  
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
  
  
  // カメラ横振り
  difference.x = getMousePos().x - center.x;
  
  angle.x -= difference.x * 0.001;
  
  Quatf orientation = Quatf(Vec3f::yAxis(), angle.x);
  camera.setOrientation(orientation);
  
  
  // カメラ縦振り
  difference.y = center.y - getMousePos().y;
  
  angle.y -= difference.y * 0.001;
  
  orientation = Quatf(Vec3f::yAxis().cross(camera.getViewDirection()), angle.y);
  camera.setOrientation(orientation);
  
  
  // カメラを上むきにする
  camera.setWorldUp(Vec3f::yAxis());
  
  
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
  CGAssociateMouseAndMouseCursorPosition(0);
  CGWarpMouseCursorPosition(center);
  CGAssociateMouseAndMouseCursorPosition(1);
}

CINDER_APP_NATIVE(FPSCameraApp, RendererGl)
