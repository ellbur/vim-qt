#include "mainwindow.moc"

#include "../runtime/vim32x32.xpm"

MainWindow::MainWindow( gui_T* gui, QWidget *parent)
:QMainWindow(parent), m_keepTabbar(false)
{
	setWindowIcon(QPixmap(vim32x32));

	// Menu
	menutoolbar = addToolBar("Menu");
	menutoolbar->setObjectName("menu");
	menutoolbar->setAllowedAreas(Qt::BottomToolBarArea | Qt::TopToolBarArea);
	menu = new QMenuBar(menutoolbar);
	menutoolbar->addWidget(menu);

	// Tool bar
	toolbar = addToolBar("ToolBar");
	toolbar->setObjectName("toolbar");

	// Vim shell
	vimshell = new QVimShell( this );
	setCentralWidget(vimshell);
	vimshell->setFocus();

	// TabLine
	tabtoolbar = addToolBar("tabline");
	tabtoolbar->setObjectName("tabline");
	tabtoolbar->setAllowedAreas(Qt::LeftToolBarArea | Qt::TopToolBarArea);

	connect( tabtoolbar, SIGNAL(orientationChanged(Qt::Orientation)),
			this, SLOT(updateTabOrientation(Qt::Orientation)) );

	tabbar = new QTabBar(tabtoolbar);
	tabbar->setTabsClosable(true);
	tabbar->setExpanding(false);
	tabbar->setFocusPolicy(Qt::NoFocus);
	tabbar->setDrawBase(false);
	tabbar->addTab("VIM"); // One tab must always exist

	tabtoolbar->addWidget(tabbar);

	connect( tabbar, SIGNAL(tabCloseRequested(int)),
			this, SLOT(closeTab(int)));
	connect( tabbar, SIGNAL(currentChanged(int)),
			this, SLOT(switchTab(int)));

}

void MainWindow::updateTabOrientation(Qt::Orientation orientation)
{
	if ( orientation == Qt::Horizontal ) {
		tabbar->setShape( QTabBar::RoundedNorth );
	} else {
		tabbar->setShape( QTabBar::RoundedWest );
	}
}

bool MainWindow::restoreState(const QByteArray& state, int version)
{
	bool ret = QMainWindow::restoreState(state, version);
	if ( keepTabbar() ) {
		showTabline(true);
	}

	return ret;
}

QVimShell* MainWindow::vimShell()
{
	return this->vimshell;
}

QMenuBar* MainWindow::menuBar() const
{
	return menu;
}

QToolBar* MainWindow::toolBar() const
{
	return toolbar;
}

void MainWindow::closeEvent (QCloseEvent * event)
{
	vimshell->closeEvent(event);
}

void MainWindow::showTabline(bool show)
{
	// VIM never removes the second tab,
	// instead it hides the entire tab bar
	if ( keepTabbar() && !show ) {
		removeTabs(1);
	}

	if ( keepTabbar() ) {
		tabtoolbar->setVisible(true);
	} else {
		tabtoolbar->setVisible(show);
	}
}

void MainWindow::showToolbar(bool show)
{
	toolbar->setVisible(show);
}

void MainWindow::showMenu(bool show)
{
	menutoolbar->setVisible(show);
}

bool MainWindow::tablineVisible()
{
	return tabtoolbar->isVisible();
}

void MainWindow::setCurrentTab(int idx)
{
	tabbar->setCurrentIndex(idx);
}

void MainWindow::setTab( int idx, const QString& label)
{
	while ( tabbar->count() <= idx ) {
		tabbar->addTab("[No name]");
	}

	tabbar->setTabText(idx, label);
}


void MainWindow::removeTabs(int idx)
{
	int i;
	for ( i=idx; i<tabbar->count(); i++) {
		tabbar->removeTab(i);
	}
}

void MainWindow::switchTab(int idx)
{
	vimshell->switchTab(idx+1);
}

void MainWindow::closeTab(int idx)
{
	if ( keepTabbar() && tabbar->count() == 1 ) {
		vimshell->close();
	} else {
		vimshell->closeTab(idx+1);
	}
}

void MainWindow::setKeepTabbar(bool keep)
{
	m_keepTabbar = keep;
}

bool MainWindow::keepTabbar()
{
	return m_keepTabbar;
}
