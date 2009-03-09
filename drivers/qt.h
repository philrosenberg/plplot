/*

  This software is provided under the LGPL in March 2009 by the
  Cluster Science Centre
  QSAS team,
  Imperial College, London

  Copyright (C) 2009  Imperial College, London

  This is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Lesser Public License as published
  by the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

  This software is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  To received a copy of the GNU Library General Public License
  write to the Free Software Foundation, Inc., 
  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
  
    History:

  
  March 2009:  v1.00
  Initial release.
  
*/

#ifndef QT_H
#define QT_H

#include <iostream>
#include <QImage>
#include <QPainter>
#include <QLinkedList>
#include <QPrinter>
#include <QApplication>
#include <QWidget>
#include <QMouseEvent>
#include <QTabWidget>
#include <QMainWindow>

#include "plplotP.h"
#include "drivers.h"

#define DPI 72
#define QT_DEFAULT_X 842
#define QT_DEFAULT_Y 598

// Types of buffer elements for buffered widgets
typedef enum ElementType_
{
 LINE,
 POLYLINE,
 POLYGON,
 SET_WIDTH,
 SET_COLOUR,
 SET_DASHED,
 SET_SOLID,
 SET_SMOOTH
} ElementType;

// A buffer element in a buffered widget
class BufferElement
{
	public:
        
		ElementType Element;
    
		union DataType
		{
			struct LineStruct_
			{
				PLFLT x1;
				PLFLT x2;
				PLFLT y1;
				PLFLT y2;
			} LineStruct;
        
			struct PolylineStruct_
			{
				PLINT npts;
				PLFLT* x;
				PLFLT* y;
			} PolylineStruct;
        
			struct ColourStruct_
			{
				PLINT R, G, B;
			} ColourStruct;
        
			PLINT intParam;
			PLFLT fltParam;
		} Data;
};

// Basic class, offering the common interface to all Qt plplot devices
class QtPLDriver
{
	public:
		// Constructor, taking the device size as arguments
		QtPLDriver(PLINT i_iWidth=QT_DEFAULT_X, PLINT i_iHeight=QT_DEFAULT_Y);
		
		virtual ~QtPLDriver();
		
		// Draws a line from (x1, y1) to (x2, y2) in internal plplot coordinates
		virtual void drawLine(short x1, short y1, short x2, short y2);

		virtual void drawPolyline(short * x, short * y, PLINT npts);
			
		virtual void drawPolygon(short * x, short * y, PLINT npts);

		virtual void setColor(int r, int g, int b);

		virtual void setWidth(PLINT w);

		virtual void setDashed(PLINT nms, PLINT* mark, PLINT* space);

		// Set pen to draw solid strokes (called after drawing dashed strokes)
		virtual void setSolid();
		
		// Conversion factor from internal plplot coordinates to device coordinates
		double downscale;
		double m_dWidth, m_dHeight;
	protected:
		
		QPainter* m_painterP;
};

// This driver is used if the device may have to redraw (Widget), then the accumulated commands are re-issued, possibly corrected to fit a new device size
class QtPLBufferedDriver: public QtPLDriver
{
	public:
		QtPLBufferedDriver(PLINT i_iWidth=QT_DEFAULT_X, PLINT i_iHeight=QT_DEFAULT_Y);

		virtual ~QtPLBufferedDriver();
			
		// Accumulates a line command in the end of the buffer
		virtual void drawLine(short x1, short y1, short x2, short y2);
			
		virtual void drawPolyline(short * x, short * y, PLINT npts);
			
		virtual void drawPolygon(short * x, short * y, PLINT npts);

		virtual void setColor(int r, int g, int b);

		virtual void setWidth(PLINT r);

		virtual void setDashed(PLINT nms, PLINT* mark, PLINT* space);

		virtual void setSolid();
		
		// Actually plots on the device, using the p QPainter.
		// if p is null, then the defaut QPainter m_painterP is used.
		// x_fact, y_fact, x_offset and y_offset allow to center the plot
		// on the device
		// These 5 parameters are actually only used for widgets:
		// widgets plot on a pixmap via p, and as the aspect ratio of 
		// what's drawn is set constant yet taking as much space as possible,
		// offsets and factors have to be set
		virtual void doPlot(QPainter* p=NULL, double x_fact=1., double y_fact=1., double x_offset=0., double y_offset=0.);
		
	protected:
		
		// Gets the offsets and factors using the current size of the device
		virtual void getPlotParameters(double & io_dXFact, double & io_dYFact, double & io_dXOffset, double & io_dYOffset);
		
		// Empties the buffer
		void clearBuffer();
		
		// Here is the buffer
		QLinkedList<BufferElement> m_listBuffer;
};

#if defined (PLD_bmpqt) || defined(PLD_jpgqt) || defined (PLD_pngqt) || defined(PLD_ppmqt) || defined(PLD_tiffqt)
// Driver painting whatever raster format Qt can save
class QtRasterDevice: public QtPLDriver, public QImage
{
	public:
		QtRasterDevice(int i_iWidth=QT_DEFAULT_X,
			    int i_iHeight=QT_DEFAULT_Y);
		
		virtual ~QtRasterDevice(){}
		
		void definePlotName(const char* fileName, const char* format);
		
		void savePlot();
		
	protected:
		char format[5];
		QString fileName;
		
};
#endif

#if defined(PLD_svgqt) && QT_VERSION >= 0x040300
#include <QSvgGenerator>
// Driver painting on an SVG device
class QtSVGDevice: public QtPLDriver, public QSvgGenerator
{
	public:
		QtSVGDevice(int i_iWidth=QT_DEFAULT_X,
			    int i_iHeight=QT_DEFAULT_Y);

		virtual ~QtSVGDevice();

		void definePlotName(const char* fileName);

		void savePlot();
	protected:
};
#endif

#if defined (PLD_epsqt) || defined (PLD_pdfqt)
// Driver painting on an EPS or PDF device, uses QPrinter
// A (possibly dummy) QApplication must be declared before use
class QtEPSDevice: public QtPLDriver, public QPrinter
{
	public:
		QtEPSDevice();
		
		virtual ~QtEPSDevice();
	
		void definePlotName(const char* fileName, int ifeps);
	
		void savePlot();
			
	protected:

};
#endif

#if defined (PLD_qtwidget)
// This widget allows to use plplot as a plotting engine in a Qt Application
// The aspect ratio of the plotted data is constant, so gray strips are used
// to delimit the page when the widget aspect ratio is not the one of the plotted page
class QtPLWidget: public QWidget, public QtPLBufferedDriver
{
	Q_OBJECT

	public:
		// Parameters are the actual size of the page, NOT the size of the widget
		// Call QWidget::resize for that
		QtPLWidget(int i_iWidth=QT_DEFAULT_X, int i_iHeight=QT_DEFAULT_Y, QWidget* parent=0);
		
		virtual ~QtPLWidget();
		
		// This is an addition to plplot drawing routines.
		// Calling setSmooth(...) will (de)activate anti-aliasing in plotting
		// For example:
		// plotWidget->setSmooth(true);
		// plbox(...);
		// plotWidget->setSmooth(false);
		// plline(...)
		// will only smooth the axes and their labels.
		void setSmooth(bool);
		
		// Clears the widget
		void clearWidget();
	
	public slots:
		
		// This slot can be used to select an x/y range interactively
		// xi, yi, xf and yf are the initial and final points coordinates
		// in their respective windows, 0 if outside a window
		void captureMousePlotCoords(double* xi, double* yi, double* xf, double* yf);
		
		// This slot can be used to select an x/y range interactively
		// xi, yi, xf and yf are the initial and final points coordinates
		// in the whole page (normalized between 0 and 1)
		void captureMouseDeviceCoords(double* xi, double* yi, double* xf, double* yf);
		
	protected slots:
		
		void resizeEvent(QResizeEvent*);
		void paintEvent(QPaintEvent*);
		void mousePressEvent(QMouseEvent*);
		void mouseReleaseEvent(QMouseEvent*);
		void mouseMoveEvent(QMouseEvent*);
	
	protected:
		
		// Used to center the plot on the page
		void getPlotParameters(double & io_dXFact, double & io_dYFact, double & io_dXOffset, double & io_dYOffset);
		
		// Used for cursor tracking (capture[...] slots)
		struct
		{
			bool isTracking;
			int step; //1 for first part, 2 for second part
			double cursor_start_x, cursor_start_y;
			double cursor_end_x, cursor_end_y;
		} cursorParameters;
		
		double m_dAspectRatio;
		QPixmap * m_pixPixmap; // to avoid redrawing everything
		bool m_bAwaitingRedraw;
		int m_iOldSize;
};


// This widget wraps up various QtPLWidgets as tabs
// The stream plots on currentWidget, which should be changed by eop()
// Actually, setting currentWidget as NULL creates a new page when
// the first plot instructions are issued. So eop() only has to set it as NULL;
class QtPLTabWidget: public QTabWidget, public QtPLDriver
{
    public:
		QtPLTabWidget()
		{
			currentWidget=NULL;
		}
		
		virtual void drawLine(short x1, short y1, short x2, short y2)
		{
			if(currentWidget==NULL) newTab();
			currentWidget->drawLine(x1, y1, x2, y2);
		}

		virtual void drawPolyline(short * x, short * y, PLINT npts)
		{
			if(currentWidget==NULL) newTab();
			currentWidget->drawPolyline(x, y, npts);
		}

		virtual void drawPolygon(short * x, short * y, PLINT npts)
		{
			if(currentWidget==NULL) newTab();
			currentWidget->drawPolygon(x, y, npts);
		}

		virtual void setColor(int r, int g, int b)
		{
			if(currentWidget==NULL) newTab();
			currentWidget->setColor(r, g, b);
		}

		virtual void setWidth(PLINT w)
		{
			if(currentWidget==NULL) newTab();
			currentWidget->setWidth(w);
		}

		virtual void setDashed(PLINT nms, PLINT* mark, PLINT* space)
		{
			if(currentWidget==NULL) newTab();
			currentWidget->setDashed(nms, mark, space);
		}

		// Set pen to draw solid strokes (called after drawing dashed strokes)
		virtual void setSolid()
		{
			if(currentWidget==NULL) newTab();
			currentWidget->setSolid();
		}
		
		void setSmooth(bool b)
		{
			if(currentWidget==NULL) newTab();
			currentWidget->setSmooth(b);
		}
		
		virtual void savePlot(char* fileName){}
	
		QtPLWidget* currentWidget;
		
		protected:
			void newTab();
};

#endif

#endif
