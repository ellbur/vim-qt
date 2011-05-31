#ifndef __QVIMSHELL__
#define __QVIMSHELL__

#include <Qt/QtGui>

extern "C" {
#include "vim.h"
}


typedef enum { CLEARALL, FILLRECT, DRAWSTRING, DRAWRECT, INVERTRECT, SCROLLRECT} PaintOperationType;
class PaintOperation {
public:
	PaintOperationType type;
	QRect rect;
	QColor color;
	// DRAWSTRING
	QFont font;
	QString str;
	bool undercurl;
	QColor curlcolor;
	// SCROLL
	QPoint pos;
};

class QVimShell: public QWidget
{
	Q_OBJECT
public:
	QVimShell(gui_T* gui, QWidget *parent=0);

	bool hasInput();
	static QIcon iconFromTheme(const QString&);
	static QIcon icon(const QString&);
	void loadColors(const QString&);
	static QColor color(const QString&);

	void queuePaintOp(PaintOperation);

	QColor background();
	int charWidth();

	QByteArray convertTo(const QString& s);
	QString convertFrom(const char *, int size=-1);

	void setEncodingUtf8(bool);
	static void setupColorMap();
	virtual QVariant inputMethodQuery(Qt::InputMethodQuery);

	/**
	 * Map a row/col coordinate to a point in widget coordinates
	 */
	static QPoint mapText(int row, int col) { return QPoint( gui.char_width*col, gui.char_height*row ); }


public slots:
	void setBackground(const QColor);
	void setCharWidth(int);

	void close();
	virtual void closeEvent(QCloseEvent *event);
	void forceInput();

	void switchTab(int idx);
	void closeTab(int idx);

protected:
	void flushPaintOps();

	void resizeEvent(QResizeEvent *);
	void keyPressEvent ( QKeyEvent *);
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void wheelEvent(QWheelEvent *event );
	virtual void inputMethodEvent(QInputMethodEvent *event);

	bool specialKey(int, char*, int*);
	virtual void paintEvent( QPaintEvent *);

	void drawString(const PaintOperation&, QPainter& );
	QFont fixPainterFont(const QFont &);

	int_u vimKeyboardModifiers(Qt::KeyboardModifiers);
	int_u vimMouseModifiers(Qt::KeyboardModifiers);

	void dragEnterEvent(QDragEnterEvent *);
	void dropEvent(QDropEvent *);

private:
	QColor m_background;
	int m_charWidth;
	gui_T *m_gui;
	QFont m_font;

	long m_blinkWaitTime, m_blinkOnTime, m_blinkOffTime;

	enum blink_state{BLINK_NONE, BLINK_ON, BLINK_OFF};
	blink_state blinkState;

	volatile bool m_input;
	QQueue<PaintOperation> paintOps;
	QPixmap canvas;

	bool m_encoding_utf8;
	QTime m_lastClick;
	int m_lastClickEvent;

	static QHash<QString, QColor> m_colorMap;
};

struct special_key
{
	int key_sym;
	char code0;
	char code1;
};

static const struct special_key special_keys[] =
{
	{Qt::Key_Up,	'k', 'u'},
	{Qt::Key_Down,	'k', 'd'},
	{Qt::Key_Left,	'k', 'l'},
    	{Qt::Key_Right,	'k', 'r'},

    {Qt::Key_F1,	'k', '1'},
    {Qt::Key_F2,	'k', '2'},
    {Qt::Key_F3,	'k', '3'},
    {Qt::Key_F4,	'k', '4'},
    {Qt::Key_F5,	'k', '5'},
    {Qt::Key_F6,	'k', '6'},
    {Qt::Key_F7,	'k', '7'},
    {Qt::Key_F8,	'k', '8'},
    {Qt::Key_F9,	'k', '9'},
    {Qt::Key_F10,	'k', ';'},
    {Qt::Key_F11,	'F', '1'},
    {Qt::Key_F12,	'F', '2'},
    {Qt::Key_F13,	'F', '3'},
    {Qt::Key_F14,	'F', '4'},
    {Qt::Key_Backspace,	'k', 'b'},

    {Qt::Key_Delete,	'k', 'D'},
    {Qt::Key_Insert,	'k', 'i'},
    {Qt::Key_Home,	'k', 'h'},
    {Qt::Key_End,	'@', '7'},
    {Qt::Key_PageUp,	'k', 'P'},
    {Qt::Key_PageDown,	'k', 'N'},

    {Qt::Key_Print,	'%', '9'},


    /* End of list marker: */
    {0, 0, 0}
};

#endif
