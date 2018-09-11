#ifndef Q_VOLUME_RENDERER
#define Q_VOLUME_RENDERER

#include <iostream>

#include <QtOpenGL/QGLWidget>

#include <QKeyEvent>
#include <QMouseEvent>
#include <QTimer>
#include <QElapsedTimer>

namespace SimpleVR {

	class VolumeRenderer; //< Pre-delaration of VolumeRenderer
	class RayCastPass;    //< Pre-delaration of RayCastPass interface (Iso-Surface, E/A, Maximum Intensity, DVR...)

	class QVolumeRenderer: public QGLWidget
	{
		Q_OBJECT
	public slots:
		void lazyUpdate();

	public:
		// Creates its own independent context
		QVolumeRenderer();

		// Shares context of parent
		QVolumeRenderer(const std::string& file);
		~QVolumeRenderer();

		void setIsoValue(double iso);
		void setShaded(bool shaded);
		void setSamplesPerVoxel(double spv);

		void postRedisplay();

	protected:
		
		bool			redisplay;   //< Lazy updating 
		QTimer			m_timer;     //< Single shot Timer for lazy ppdates (see also: postRedisplay() )
		QElapsedTimer	m_frameTime; //< Timing 30fps and lazy updating 

		std::string input_file; //< File path of a volume on disk. Stored here because NO OPENGL is allowed before context exists (and initializeGL ran)

		int m_mouseX;    //< Mouse: Location in pixels within window X
		int m_mouseY;    //< Mouse: Location in pixels within window Y
		int m_mouseDown; //< Mouse: 0: released; otherwise: last pressed button.

		float m_rot_x;    //< Camera: Rotation about X-axis
		float m_rot_y;    //< Camera: Rotation about Y-axis
		float m_distance; //< Camera: Distance to Origin

		VolumeRenderer    *m_dvr;  //< Actual Work Done Here
		RayCastPass       *m_pass; //< Actual Work Done Here


		// User interaction:

		virtual void mousePressEvent(QMouseEvent *event);
		virtual void mouseReleaseEvent(QMouseEvent *event);
		virtual void mouseMoveEvent(QMouseEvent *event);
		virtual void keyPressEvent(QKeyEvent *event);
		virtual void keyReleaseEvent(QKeyEvent *event);
		virtual void wheelEvent(QWheelEvent *event);

		virtual void initializeGL();
		virtual void resizeGL( int width, int height );
		virtual void paintGL();

	};

}

#endif // Q_VOLUME_RENDERER