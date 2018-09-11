
#include <LibSimple/SimpleGL.h>
#include "QVolumeRenderer.h"

#include <LibSimple/SimpleVR.h>
#include <LibSimple/SimpleRayCastPass.h>

// https://www.khronos.org/opengl/wiki/GluPerspective_code
void glhFrustumf2(float *matrix, float left, float right, float bottom, float top,
                  float znear, float zfar)
{
    float temp = 2.0 * znear;
    float temp2 = right - left;
    float temp3 = top - bottom;
    float temp4 = zfar - znear;
    matrix[0] = temp / temp2; matrix[4] = 0.0;          matrix[8] = (right + left) / temp2;   matrix[12] = 0.0;
    matrix[1] = 0.0;          matrix[5] = temp / temp3; matrix[9] = (top + bottom) / temp3;   matrix[13] = 0.0;
    matrix[2] = 0.0;          matrix[6] = 0.0;          matrix[10] = (-zfar - znear) / temp4; matrix[14] = (-temp * zfar) / temp4;
    matrix[3] = 0.0;          matrix[7] = 0.0;          matrix[11] = -1.0;                    matrix[15] = 0.0;
}
void glhPerspectivef2(float *matrix, float fovyInDegrees, float aspectRatio,
                      float znear, float zfar)
{
	float Pi=3.14159265f;
    float ymax = znear * tanf(fovyInDegrees * Pi / 360.0);
    float xmax = ymax * aspectRatio;
    glhFrustumf2(matrix, -xmax, xmax, -ymax, ymax, znear, zfar);
}

namespace SimpleVR {

	QVolumeRenderer::QVolumeRenderer(const std::string& file)
		: QGLWidget()
		, input_file(file)
		, m_dvr(0x0)
		, m_pass(0x0)
		, m_rot_x(45)
		, m_rot_y(45)
		, m_distance(1500)
	{
		redisplay=false;
		m_timer.setInterval(10);
		m_timer.setSingleShot(true);
		connect(&m_timer, SIGNAL(timeout()), this, SLOT(lazyUpdate()));
	}

	QVolumeRenderer::~QVolumeRenderer()
	{
		if (m_dvr) delete m_dvr;
		if (m_pass) delete m_pass;
	}

	void QVolumeRenderer::initializeGL()
	{
		SimpleGL::init();
		glClearColor(1,0,0,1);

		// Clean stuff up if required
		if (m_dvr) delete m_dvr;
		m_dvr=0x0;
		if (m_pass) delete m_pass;
		m_pass=0x0;

		// Set Up Volume Renderer
		m_dvr=new VolumeRenderer;
		auto *pass=new SimpleVR::IsoSurface();
		pass->isoValue=128;
		pass->shaded=true;
		pass->samplesPerVoxel=1.0;
		m_pass=pass;
	// 	m_pass=new SimpleVR::DebugRaycastEnvironment(); //for debugging

		// Create textures etc.
		if (!m_dvr->init(input_file,0x0,m_pass))
		{
			std::cerr << "Failed to initialize volume rendering.\n";
			delete m_dvr;
			m_dvr=0x0;
		}

	}

	void QVolumeRenderer::resizeGL( int width, int height )
	{
		glViewport (0, 0, (GLsizei) width, (GLsizei) height); 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (m_dvr) m_dvr->resize(width,height);
	}

	void QVolumeRenderer::paintGL()
	{
		m_frameTime.restart();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Set up camera
		glMatrixMode(GL_PROJECTION);
		float p[16];
		glhPerspectivef2(p,45.0f,(float)width()/height(),.1f,10000);
		glLoadMatrixf(p);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(0,0,-m_distance);
		glRotatef(m_rot_x,1,0,0);
		glRotatef(m_rot_y,0,1,0);

		if (m_dvr) {
			// Get Volume size in mm and move volume center to origin
			auto &v=*m_dvr->getVolume()->getData();
			float volume_size_mm[]= {
					v.dim(0)*v.spacing(0),
					v.dim(1)*v.spacing(1),
					v.dim(2)*v.spacing(2)
				};
			glTranslatef(-.5f*volume_size_mm[0],-.5f*volume_size_mm[1],-.5f*volume_size_mm[2]);
			// Actual rendering
			m_dvr->render();
		}
	}

	void QVolumeRenderer::setIsoValue(double iso)
	{
		auto *pass=dynamic_cast<SimpleVR::IsoSurface*>(m_pass);
		if (pass) pass->isoValue=iso;
		postRedisplay();
	}

	void QVolumeRenderer::setShaded(bool shaded)
	{
		auto *pass=dynamic_cast<SimpleVR::IsoSurface*>(m_pass);
		if (pass) pass->shaded=shaded;
		postRedisplay();
	}

	void QVolumeRenderer::setSamplesPerVoxel(double spv)
	{
		auto *pass=dynamic_cast<SimpleVR::IsoSurface*>(m_pass);
		if (pass) pass->samplesPerVoxel=spv;
		postRedisplay();
	}

	void QVolumeRenderer::postRedisplay()
	{
		if (!redisplay)
		{
			redisplay=1;
			m_timer.start();
		}
	}

	void QVolumeRenderer::lazyUpdate()
	{
		if (redisplay)
		{
			if (m_frameTime.elapsed()<30)
			{
				m_timer.start();
				return;
			}
			redisplay=false;
			updateGL();
		}
		else
		{
			// idle...
			m_timer.start();
		}
	}

	void QVolumeRenderer::mousePressEvent(QMouseEvent *event)
	{
		int x=event->x();
		int y=event->y();
		m_mouseX=x;
		m_mouseY=y;
		m_mouseDown=1;
		postRedisplay();
	}

	void QVolumeRenderer::mouseReleaseEvent(QMouseEvent *event)
	{
		m_mouseDown=0;
	}

	void QVolumeRenderer::mouseMoveEvent(QMouseEvent *event)
	{
		int x=event->x();
		int y=event->y();
		int dx=m_mouseX-x;
		int dy=m_mouseY-y;
		m_mouseX=x;
		m_mouseY=y;
		if (m_mouseDown)
		{
			// drag
			m_rot_x+=dy;
			m_rot_y+=dx;
			postRedisplay();
		}
		else
		{
			// move
		}
	}

	void QVolumeRenderer::keyPressEvent(QKeyEvent *event)
	{
		unsigned char key=0;
		key=(unsigned char)event->text()[0].toLatin1();
	
		if (key=='r') // for example
		{
			m_distance=1500;
			m_rot_x=m_rot_y=45;
		}

		postRedisplay();
	}

	void QVolumeRenderer::keyReleaseEvent(QKeyEvent *event)
	{
		unsigned char key=0;
		key=(unsigned char)event->text()[0].toLatin1();

		postRedisplay();
	}

	void QVolumeRenderer::wheelEvent(QWheelEvent *event)
	{
		int dx=event->angleDelta().x();
		int dy=event->angleDelta().y();
		m_distance-=dy;
		postRedisplay();
	}

} // namspace SimpleVR
