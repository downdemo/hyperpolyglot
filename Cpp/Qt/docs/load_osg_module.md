## 利用 osgQt 的例子

* osgdlg.h

```cpp
#pragma once

#include <QApplication>
#include <QGridLayout>
#include <QTimer>
#include <iostream>
#include <osgDB/ReadFile>
#include <osgGA/MultiTouchTrackballManipulator>
#include <osgQt/GraphicsWindowQt>
#include <osgViewer/CompositeViewer>
#include <osgViewer/ViewerEventHandlers>

class ViewerWidget : public QWidget, public osgViewer::CompositeViewer {
 public:
  ViewerWidget(QWidget* parent = 0, Qt::WindowFlags f = 0,
               osgViewer::ViewerBase::ThreadingModel threadingModel =
                   osgViewer::CompositeViewer::SingleThreaded)
      : QWidget(parent, f) {
    setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    hide();
    setThreadingModel(threadingModel);
    osgViewer::GraphicsWindowEmbedded
        // disable the default setting of viewer.done() by pressing Escape.
        setKeyEventSetsDone(0);

    QWidget* widget1 = addViewWidget(createGraphicsWindow(0, 0, 100, 100),
                                     osgDB::readRefNodeFile("cow.osg"));
    // QWidget* widget2 = addViewWidget(createGraphicsWindow(0, 0, 100, 100),
    //                                  osgDB::readRefNodeFile("glider.osgt"));
    // QWidget* widget3 = addViewWidget(createGraphicsWindow(0, 0, 100, 100),
    //                                  osgDB::readRefNodeFile("axes.osgt"));
    // QWidget* widget4 = addViewWidget(createGraphicsWindow(0, 0, 100, 100),
    //                                  osgDB::readRefNodeFile("fountain.osgt"));

    QGridLayout* grid = new QGridLayout;
    grid->addWidget(widget1, 0, 0);
    // grid->addWidget(widget2, 0, 1);
    // grid->addWidget(widget3, 1, 0);
    // grid->addWidget(widget4, 1, 1);
    setLayout(grid);

    connect(&timer_, SIGNAL(timeout()), this, SLOT(update()));
    timer_.start(10);
  }

  QWidget* addViewWidget(osgQt::GraphicsWindowQt* gw,
                         osg::ref_ptr<osg::Node> scene) {
    osgViewer::View* view = new osgViewer::View;
    addView(view);

    osg::Camera* camera = view->getCamera();
    camera->setGraphicsContext(gw);

    const osg::GraphicsContext::Traits* traits = gw->getTraits();

    camera->setClearColor(osg::Vec4(0.2, 0.2, 0.6, 1.0));
    camera->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));

    // set the draw and read buffers up for a double buffered window with
    // rendering going to back buffer
    camera->setDrawBuffer(GL_BACK);
    camera->setReadBuffer(GL_BACK);

    camera->setProjectionMatrixAsPerspective(
        30.0f, static_cast<double>(traits->width) / traits->height, 1.0f,
        10000.0f);

    view->setSceneData(scene);
    view->addEventHandler(new osgViewer::StatsHandler);
    view->setCameraManipulator(new osgGA::MultiTouchTrackballManipulator);
    gw->setTouchEventsEnabled(true);
    return gw->getGLWidget();
  }

  osgQt::GraphicsWindowQt* createGraphicsWindow(int x, int y, int w, int h,
                                                const std::string& name = "",
                                                bool windowDecoration = false) {
    osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();
    osg::ref_ptr<osg::GraphicsContext::Traits> traits =
        new osg::GraphicsContext::Traits;
    traits->windowName = name;
    traits->windowDecoration = windowDecoration;
    traits->x = x;
    traits->y = y;
    traits->width = w;
    traits->height = h;
    traits->doubleBuffer = true;
    traits->alpha = ds->getMinimumNumAlphaBits();
    traits->stencil = ds->getMinimumNumStencilBits();
    traits->sampleBuffers = ds->getMultiSamples();
    traits->samples = ds->getNumMultiSamples();

    return new osgQt::GraphicsWindowQt(traits.get());
  }

  virtual void paintEvent(QPaintEvent* event) { frame(); }

 protected:
  QTimer timer_;
};
```

* 槽函数

```cpp
void dlg::slot() {
  ViewerWidget* view =
      new ViewerWidget(this, Qt::Widget, osgViewer::ViewerBase::SingleThreaded);
  view->setGeometry(0, 0, width(), height());
  view->show();
}
```

## 读取 osg 文件并显示模型

* MyOsgModule.h

```cpp
#pragma once

#include <QApplication>
#include <QGridLayout>
#include <QTimer>
#include <iostream>
#include <osgDB/ReadFile>
#include <osgGA/MultiTouchTrackballManipulator>
#include <osgQt/GraphicsWindowQt>
#include <osgViewer/CompositeViewer>
#include <osgViewer/ViewerEventHandlers>

class ViewerWidget : public QWidget, public osgViewer::CompositeViewer {
 public:
  ViewerWidget(std::string path = "cow.osg", QWidget* parent = 0,
               Qt::WindowFlags f = 0,
               osgViewer::ViewerBase::ThreadingModel threadingModel =
                   osgViewer::CompositeViewer::SingleThreaded)
      : QWidget(parent, f) {
    setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    hide();
    setThreadingModel(threadingModel);
    osgViewer::GraphicsWindowEmbedded
        // disable the default setting of viewer.done() by pressing Escape.
        setKeyEventSetsDone(0);

    QWidget* widget1 = addViewWidget(createGraphicsWindow(0, 0, 100, 100),
                                     osgDB::readRefNodeFile(path));
    // QWidget* widget2 = addViewWidget(createGraphicsWindow(0, 0, 100, 100),
    //                                  osgDB::readRefNodeFile("glider.osgt"));
    // QWidget* widget3 = addViewWidget(createGraphicsWindow(0, 0, 100, 100),
    //                                  osgDB::readRefNodeFile("axes.osgt"));
    // QWidget* widget4 = addViewWidget(createGraphicsWindow(0, 0, 100, 100),
    //                                  osgDB::readRefNodeFile("fountain.osgt"));

    QGridLayout* grid = new QGridLayout;
    grid->addWidget(widget1, 0, 0);
    // grid->addWidget(widget2, 0, 1);
    // grid->addWidget(widget3, 1, 0);
    // grid->addWidget(widget4, 1, 1);
    setLayout(grid);

    connect(&timer_, SIGNAL(timeout()), this, SLOT(update()));
    timer_.start(10);
  }

  QWidget* addViewWidget(osgQt::GraphicsWindowQt* gw,
                         osg::ref_ptr<osg::Node> scene) {
    osgViewer::View* view = new osgViewer::View;
    addView(view);

    osg::Camera* camera = view->getCamera();
    camera->setGraphicsContext(gw);

    const osg::GraphicsContext::Traits* traits = gw->getTraits();

    camera->setClearColor(osg::Vec4(0.2, 0.2, 0.6, 1.0));
    camera->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));

    // set the draw and read buffers up for a double buffered window with
    // rendering going to back buffer
    camera->setDrawBuffer(GL_BACK);
    camera->setReadBuffer(GL_BACK);

    camera->setProjectionMatrixAsPerspective(
        30.0f,
        static_cast<double>(traits->width) /
            static_cast<double>(traits->height),
        1.0f, 10000.0f);

    view->setSceneData(scene);
    view->addEventHandler(new osgViewer::StatsHandler);
    view->setCameraManipulator(new osgGA::MultiTouchTrackballManipulator);
    gw->setTouchEventsEnabled(true);
    return gw->getGLWidget();
  }

  osgQt::GraphicsWindowQt* createGraphicsWindow(int x, int y, int w, int h,
                                                const std::string& name = "",
                                                bool windowDecoration = false) {
    osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();
    osg::ref_ptr<osg::GraphicsContext::Traits> traits =
        new osg::GraphicsContext::Traits;
    traits->windowName = name;
    traits->windowDecoration = windowDecoration;
    traits->x = x;
    traits->y = y;
    traits->width = w;
    traits->height = h;
    traits->doubleBuffer = true;
    traits->alpha = ds->getMinimumNumAlphaBits();
    traits->stencil = ds->getMinimumNumStencilBits();
    traits->sampleBuffers = ds->getMultiSamples();
    traits->samples = ds->getNumMultiSamples();

    return new osgQt::GraphicsWindowQt(traits.get());
  }

  virtual void paintEvent(QPaintEvent* /*event*/) { frame(); }

 protected:
  QTimer timer_;
};
```

* 头文件

```cpp
#pragma once

#include <qwidget.h>

#include "MyOsgModule.h"
#include "qboxlayout.h"
#include "ui_MyWidget.h"

namespace Ui {
class MyWidget;
}

class MyWidget : public QWidget {
  Q_OBJECT
 public:
  explicit MyWidget(std::string path = "", QWidget* parent = Q_NULLPTR);
  ~MyWidget();

 private:
  Ui::MyWidget* ui;
  QVBoxLayout* layout;
  ViewerWidget* view;
};
```
* 源文件
```cpp
#include "MyWidget.h"

MyWidget::MyWidget(std::string path, QWidget* parent)
    : QWidget(parent), ui(new Ui::MyWidget) {
  ui->setupUi(this);
  view = new ViewerWidget(path, this, Qt::Widget,
                          osgViewer::ViewerBase::SingleThreaded);
  view->setGeometry(0, 0, width(), height());
  view->show();

  layout = new QVBoxLayout;
  layout->addWidget(view);
  setLayout(layout);
}

MyWidget::~MyWidget() { delete ui; }
```

## 重复载入模型文件

* 通过删除 tabWidget 的 tab 并创建新 tab 的方式防止多个模型重叠，下面是载入模型的槽函数

```cpp
void dlg::slot() {
  QString path = QFileDialog::getOpenFileName(
      this, tr("Open File"), "C:/OSG/Data", tr("OSG Model(*.osg*)"));
  if (!path.isEmpty()) {
    newPage = new MyWidget(path.toStdString());
    tabWidget->insertTab(0, newPage, tr(u8"osg模型"));
    tabWidget->removeTab(1);  // 删除原来的第二页
    tabWidget->setCurrentIndex(0);
  }
}
```

* 上述只适用于一个 tab 就是一个模型的情况，有时还包含其他控件，就不能直接这样删除。更恰当的方法是给 MyWidget 添加一个 changePath 函数

```cpp
MyWidget::MyWidget(std::string path, QWidget* parent)
    : QWidget(parent), ui(new Ui::MyWidget) {
  ui->setupUi(this);
  view = new ViewerWidget(path, this, Qt::Widget,
                          osgViewer::ViewerBase::SingleThreaded);
  view->setGeometry(0, 0, width(), height());
  view->show();

  layout = new QVBoxLayout;
  layout->addWidget(view);
  setLayout(layout);
}

void MyWidget::changePath(const std::string& filepath) {
  view->close();
  view = new ViewerWidget(filepath, this, Qt::Widget,
                          osgViewer::ViewerBase::SingleThreaded);
  view->setGeometry(0, 0, width(), height());
  view->show();
  layout->addWidget(view);
}
```

* 这样就不用删除整个 MyWidget，而只需要调用 changePath 删除 MyWidget 中的 OSG 模型部分

```cpp
void dlg::slot() {
  QString path = QFileDialog::getOpenFileName(
      this, tr("Open File"), "C:/OSG/Data", tr("OSG Model(*.osg*)"));
  if (!path.isEmpty()) {
    page->mywidget->changePath(path.toStdString());
  }
}
```

## 改变滚轮方向

* 默认情况下，鼠标滚轮往后滚动是放大。现在要改为，滚轮向前滚动放大。方法从 osgGA::OrbitManipulator 派生一个类 MyScroll，然后在 addViewWidget 函数中添加 view->setCameraManipulator(new MyScroll);

```cpp
class MyScroll : public osgGA::OrbitManipulator {
  bool handleMouseWheel(const osgGA::GUIEventAdapter& ea,
                        osgGA::GUIActionAdapter& us) {
    osgGA::GUIEventAdapter::ScrollingMotion sm = ea.getScrollingMotion();
    // handle centering
    if (_flags & SET_CENTER_ON_WHEEL_FORWARD_MOVEMENT) {
      if (((sm == osgGA::GUIEventAdapter::SCROLL_DOWN &&
            _wheelZoomFactor > 0.)) ||
          ((sm == osgGA::GUIEventAdapter::SCROLL_UP &&
            _wheelZoomFactor < 0.))) {
        if (getAnimationTime() <= 0.) {
          // center by mouse intersection (no animation)
          setCenterByMousePointerIntersection(ea, us);
        } else if (!isAnimating()) {
          // start new animation only if there is no animation in progress
          startAnimationByMousePointerIntersection(ea, us);
        }
      }
    }
    switch (sm) {
      case osgGA::GUIEventAdapter::SCROLL_DOWN: {  // mouse scroll up event
        // perform zoom
        zoomModel(_wheelZoomFactor, true);
        us.requestRedraw();
        us.requestContinuousUpdate(isAnimating() || _thrown);
        return true;
      }
      case osgGA::GUIEventAdapter::SCROLL_UP: {  // mouse scroll down event
        // perform zoom
        zoomModel(-_wheelZoomFactor, true);
        us.requestRedraw();
        us.requestContinuousUpdate(isAnimating() || _thrown);
        return true;
      }
      default:  // unhandled mouse scrolling motion
        return false;
    }
  }
};

QWidget* addViewWidget(osgQt::GraphicsWindowQt* gw,
                       osg::ref_ptr<osg::Node> scene) {
  osgViewer::View* view = new osgViewer::View;
  addView(view);
  osg::Camera* camera = view->getCamera();
  camera->setGraphicsContext(gw);
  const osg::GraphicsContext::Traits* traits = gw->getTraits();
  camera->setClearColor(osg::Vec4(0.2, 0.2, 0.6, 1.0));
  camera->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
  camera->setDrawBuffer(GL_BACK);
  camera->setReadBuffer(GL_BACK);
  camera->setProjectionMatrixAsPerspective(
      30.0f,
      static_cast<double>(traits->width) / static_cast<double>(traits->height),
      1.0f, 10000.0f);
  view->setSceneData(scene);
  view->addEventHandler(new osgViewer::StatsHandler);
  view->setCameraManipulator(new osgGA::MultiTouchTrackballManipulator);
  view->setCameraManipulator(new MyScroll);  // 添加这行
  gw->setTouchEventsEnabled(true);
  return gw->getGLWidget();
}
```

## 添加多个节点

* 原来的 MyOsgModule.h 中，只添加了一个 osg::Node

```cpp
QWidget* addViewWidget(osgQt::GraphicsWindowQt* gw,
                       osg::ref_ptr<osg::Node> scene) {
  osgViewer::View* view = new osgViewer::View;
  addView(view);
  view->setSceneData(scene);
}
```

* 其中 view 是要显示的视图，只需要一个。若要添加多个 osg::Node，应当创建一个 osg::Group，然后把所有的 osg::Node 加到 osg::Group 中，最后把 osg::Group 添加到 osgViewer::View 中

```cpp
QWidget* addViewWidget(osgQt::GraphicsWindowQt* gw,
                       osg::ref_ptr<osg::Node> scene1,
                       osg::ref_ptr<osg::Node> scene2,
                       osg::ref_ptr<osg::Node> scene3) {
  osgViewer::View* view = new osgViewer::View;
  addView(view);
  osg::ref_ptr<osg::Group> group = new osg::Group;
  group->addChild(scene1);
  group->addChild(scene2);
  group->addChild(scene3);
  view->setSceneData(group);
}
```

## 利用 Qt 对 OpenGL 的支持

* GLScene.h

```cpp
#pragma once

#include <QtWidgets/qgraphicsscene.h>

#include <QInputEvent>
#include <QTimerEvent>
#include <QtGui/QPainter>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsSceneEvent>

// #include "OsgLib.h"
#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

// #include "KeyBoardMap.h"

// static KeyBoardMap s_QtKeyboardMap;  // 按键映射

class GLScene : public QGraphicsScene {
 public:
  GLScene(QObject *parent = 0);
  ~GLScene(void);

 protected:
  // 绘制场景
  void drawBackground(QPainter *painter, const QRectF &rect);
  // 窗口更新函数：使用定时器，定时更新
  void timerEvent(QTimerEvent *);

 protected:
  // 鼠标事件
  void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
  void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
  void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent);
  void wheelEvent(QGraphicsSceneWheelEvent *wheelEvent);

  // 键盘按键
  void keyPressEvent(QKeyEvent *keyEvent);
  void keyReleaseEvent(QKeyEvent *keyEvent);

  int timer_id_;

 public:
  // 更新OSG窗口
  void resizeViewer();
  void setKeyboardModifiers(QInputEvent *event);  // 组合键
 public:
  osg::ref_ptr<osgViewer::Viewer> viewer_;
};
```

* GLSCene.cpp

```cpp
#include "GLScene.h"

GLScene::GLScene(QObject *parent)
    : QGraphicsScene(parent), viewer_(NULL), timer_id_(0) {
  // OSG初始化
  viewer_ = new osgViewer::Viewer;
  // 设置窗口的嵌入大小
  viewer_->setUpViewerAsEmbeddedInWindow(0, 0, 800, 600);
  viewer_->setSceneData(osgDB::readNodeFile("cow.osg"));
  // 添加操作器
  viewer_->setCameraManipulator(new osgGA::TrackballManipulator);
  // 状态信息
  viewer_->addEventHandler(new osgViewer::StatsHandler);
  viewer_->realize();  // 实例化 GraphicsContext
  // 当定时器触发时，应用程序会发送一个 QTimerEvent
  // 如果参数为 0，那么定时器事件每次发生时没有窗口系统事件处理
  timer_id_ = startTimer(0);
}

GLScene::~GLScene(void) {}

// 绘制场景
void GLScene::drawBackground(QPainter *painter, const QRectF &rect) {
  painter->beginNativePainting();
  painter->setRenderHint(QPainter::Antialiasing);  // 抗锯齿
  viewer_->frame();
  // viewer_->getViewerBase()->frame();
  painter->endNativePainting();
}

// 窗口更新函数
void GLScene::timerEvent(QTimerEvent *) { this->update(); }

void GLScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) {
  QGraphicsScene::mousePressEvent(mouseEvent);

  int button = 0;
  switch (mouseEvent->button()) {
    case Qt::LeftButton:
      button = 1;
      break;
    case Qt::MidButton:
      button = 2;
      break;
    case Qt::RightButton:
      button = 3;
      break;
    case Qt::NoButton:
      button = 0;
      break;
    default:
      button = 0;
      break;
  }
  // _gw->getEventQueue()->mouseButtonRelease( event->x(), event->y(), button );
  (dynamic_cast<osgViewer::GraphicsWindow *>(
       viewer_->getCamera()->getGraphicsContext()))
      ->getEventQueue()
      ->mouseButtonPress(mouseEvent->scenePos().x(), mouseEvent->scenePos().y(),
                         button);
}

void GLScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) {
  QGraphicsScene::mouseReleaseEvent(mouseEvent);

  int button = 0;
  switch (mouseEvent->button()) {
    case Qt::LeftButton:
      button = 1;
      break;
    case Qt::MidButton:
      button = 2;
      break;
    case Qt::RightButton:
      button = 3;
      break;
    case Qt::NoButton:
      button = 0;
      break;
    default:
      button = 0;
      break;
  }
  (dynamic_cast<osgViewer::GraphicsWindow *>(
       viewer_->getCamera()->getGraphicsContext()))
      ->getEventQueue()
      ->mouseButtonRelease(mouseEvent->scenePos().x(),
                           mouseEvent->scenePos().y(), button);
}

void GLScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) {
  QGraphicsScene::mouseMoveEvent(mouseEvent);

  (dynamic_cast<osgViewer::GraphicsWindow *>(
       viewer_->getCamera()->getGraphicsContext()))
      ->getEventQueue()
      ->mouseMotion(mouseEvent->scenePos().x(), mouseEvent->scenePos().y());
}

void GLScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent) {
  QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
  int button = 0;
  switch (mouseEvent->button()) {
    case Qt::LeftButton:
      button = 1;
      break;
    case Qt::MidButton:
      button = 2;
      break;
    case Qt::RightButton:
      button = 3;
      break;
    case Qt::NoButton:
      button = 0;
      break;
    default:
      button = 0;
      break;
  }

  (dynamic_cast<osgViewer::GraphicsWindow *>(
       viewer_->getCamera()->getGraphicsContext()))
      ->getEventQueue()
      ->mouseDoubleButtonPress(mouseEvent->scenePos().x(),
                               mouseEvent->scenePos().y(), button);
}

void GLScene::wheelEvent(QGraphicsSceneWheelEvent *wheelEvent) {
  QGraphicsScene::wheelEvent(wheelEvent);

  (dynamic_cast<osgViewer::GraphicsWindow *>(
       viewer_->getCamera()->getGraphicsContext()))
      ->getEventQueue()
      ->mouseScroll(wheelEvent->orientation() == Qt::Vertical
                        ? (wheelEvent->delta() > 0
                               ? osgGA::GUIEventAdapter::SCROLL_UP
                               : osgGA::GUIEventAdapter::SCROLL_DOWN)
                        : (wheelEvent->delta() > 0
                               ? osgGA::GUIEventAdapter::SCROLL_LEFT
                               : osgGA::GUIEventAdapter::SCROLL_RIGHT));
}

void GLScene::keyPressEvent(QKeyEvent *keyEvent) {
  setKeyboardModifiers(keyEvent);
  //   int value = s_QtKeyboardMap.remapKey(keyEvent);
  //   (dynamic_cast<osgViewer::GraphicsWindow *>(
  //        viewer_->getCamera()->getGraphicsContext()))
  //       ->getEventQueue()
  //       ->keyPress(value);
  QGraphicsScene::keyPressEvent(keyEvent);
}

void GLScene::keyReleaseEvent(QKeyEvent *keyEvent) {
  QGraphicsScene::keyReleaseEvent(keyEvent);
  //   int value = s_QtKeyboardMap.remapKey(keyEvent);
  //   (dynamic_cast<osgViewer::GraphicsWindow *>(
  //        viewer_->getCamera()->getGraphicsContext()))
  //       ->getEventQueue()
  //       ->keyRelease(value);
  QGraphicsScene::keyPressEvent(keyEvent);
}

// OSG 场景窗口更新
void GLScene::resizeViewer() {
  // 更新OSG窗口大小
  (dynamic_cast<osgViewer::GraphicsWindow *>(
       viewer_->getCamera()->getGraphicsContext()))
      ->resized(this->sceneRect().x(), this->sceneRect().y(),
                this->sceneRect().width(), this->sceneRect().height());
  (dynamic_cast<osgViewer::GraphicsWindow *>(
       viewer_->getCamera()->getGraphicsContext()))
      ->getEventQueue()
      ->windowResize(this->sceneRect().x(), this->sceneRect().y(),
                     this->sceneRect().width(),
                     this->sceneRect().height());  // ?  更新状态 S 按键

  (dynamic_cast<osgViewer::GraphicsWindow *>(
       viewer_->getCamera()->getGraphicsContext()))
      ->requestRedraw();  // ?
}

// 组合键
void GLScene::setKeyboardModifiers(QInputEvent *event) {
  int modkey = event->modifiers() &
               (Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier);
  unsigned int mask = 0;
  if (modkey & Qt::ShiftModifier) mask |= osgGA::GUIEventAdapter::MODKEY_SHIFT;
  if (modkey & Qt::ControlModifier) mask |= osgGA::GUIEventAdapter::MODKEY_CTRL;
  if (modkey & Qt::AltModifier) mask |= osgGA::GUIEventAdapter::MODKEY_ALT;
  (dynamic_cast<osgViewer::GraphicsWindow *>(
       viewer_->getCamera()->getGraphicsContext()))
      ->getEventQueue()
      ->getCurrentEventState()
      ->setModKeyMask(mask);
}
```

* GLView.h

```cpp
#pragma once

#include <QtGui/QResizeEvent>
#include <QtOpenGL/QGLWidget>
#include <QtWidgets/QGraphicsView>

#include "GLScene.h"

class GLView : public QGraphicsView {
 public:
  GLView(QWidget *parent = 0);
  ~GLView(void);

 protected:
  // 更新视图的时候，更新场景
  void resizeEvent(QResizeEvent *event);

 public:
  QGLWidget *widget_;
  GLScene *scene_;
};
```

* GLView.cpp（视图窗口大小在此设置）

```cpp
#include <QtWidgets/QMessageBox>

#include "GLView.h"

GLView::GLView(QWidget *parent)
    : QGraphicsView(parent),
      widget_(new QGLWidget(QGLFormat(QGL::DoubleBuffer))),
      scene_(new GLScene(this)) {
  // 设置视口大小
  this->resize(800, 600);
  // 将 widget_ 设置为视口：为了使用 OpenGL 渲染
  // 要设置一个新的 QGLWidget作为 QGraphicsView 的视口
  this->setViewport(widget_);
  widget_->setMouseTracking(true);  // 鼠标追踪
  this->viewport()->setMinimumSize(1, 1);

  this->setScene(scene_);
  // 设置视图的最小尺寸
  this->setMinimumSize(1, 1);
  // 设置场景大小
  this->scene()->setSceneRect(0, 0, this->width(), this->height());
  // 设置视图的更新方式：整个视图更新
  this->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
}

GLView::~GLView(void) {}

// 更新视图的时候，更新场景
void GLView::resizeEvent(QResizeEvent *event) {
  // 更新场景大小
  scene_->setSceneRect(this->rect().left(), this->rect().right(),
                       this->rect().width(), this->rect().height());
  scene_->resizeViewer();
}
```

* 槽函数

```cpp
#include "GLView.h"

void test::slot1() {
  glview_ = new GLView(this);
  glview_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  glview_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  glview_->move(100, 0);
  glview_->show();
}
```

* 选择路径导入模型

```cpp
// GLScene.h 构造函数声明中改为
GLScene(QObject *parent = 0, std::string path = "cow.osg");

// GLScene.cpp 构造函数实现改为
GLScene::GLScene(QObject *parent, std::string path)
    : QGraphicsScene(parent), viewer_(NULL), timer_id_(0) {
  viewer_->setSceneData(osgDB::readNodeFile(path));
}

// GLView.h 构造函数声明中改为
GLView(QWidget *parent = 0, std::string path = "aa.osgb");

// GLView.cpp 构造函数实现改为
GLView::GLView(QWidget *parent, std::string path)
    : QGraphicsView(parent),
      widget_(new QGLWidget(QGLFormat(QGL::DoubleBuffer))),
      scene_(new GLScene(this, path)) {
  this->resize(1600, 950);  // 设置视口大小
}

// 槽函数
void test::slot1() {
  QString path = QFileDialog::getOpenFileName(
      this, tr("Open File"), "D:/TestData", tr("Osg Module(*.osg*)"));
  if (!path.isEmpty()) {
    glview_ = new GLView(this, path.toStdString());
    glview_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    glview_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    glview_->move(200, 50);
    glview_->show();
  }
}

// 保存操作
void test::slot2() {
  QString path = QFileDialog::getSaveFileName(
      this, tr("Save File"), "D:/TestData", tr("Osg Module(*.osg)"));
  if (!path.isEmpty()) {
    osg::ref_ptr<osg::Node> root = createSceneGraph();  // 实现如下
    osgDB::writeNodeFile(*(root.get()), path.toStdString());
  }
}

// 模型实现文件
#include <iostream>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Notify>
#include <osg/ref_ptr>
#include <osgDB/Registry>
#include <osgDB/WriteFile>

osg::ref_ptr<osg::Node> createSceneGraph() {
  // 创建一个用于保存几何信息的对象
  osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;

  // 创建四个顶点的数组
  osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array;
  geom->setVertexArray(v.get());
  v->push_back(osg::Vec3(-1.f, 0.f, -1.f));
  v->push_back(osg::Vec3(1.f, 0.f, -1.f));
  v->push_back(osg::Vec3(1.f, 0.f, 1.f));
  v->push_back(osg::Vec3(-1.f, 0.f, 1.f));

  // 创建四种颜色的数组
  osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array;
  geom->setColorArray(c.get());
  geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
  c->push_back(osg::Vec4(1.f, 0.f, 0.f, 1.f));
  c->push_back(osg::Vec4(0.f, 1.f, 0.f, 1.f));
  c->push_back(osg::Vec4(0.f, 0.f, 1.f, 1.f));
  c->push_back(osg::Vec4(1.f, 1.f, 1.f, 1.f));

  // 为唯一的法线创建一个数组
  osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array;
  geom->setNormalArray(n.get());
  geom->setNormalBinding(osg::Geometry::BIND_OVERALL);
  n->push_back(osg::Vec3(0.f, -1.f, 0.f));

  // 由保存的数据绘制四个顶点的多边形
  geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));

  // 向 Geode 类添加几何体（Drawable）并返回 Geode
  osg::ref_ptr<osg::Geode> geode = new osg::Geode;
  geode->addDrawable(geom.get());
  return geode.get();
}
```
