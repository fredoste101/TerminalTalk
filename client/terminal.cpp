#include "terminal.h"


using namespace std;

class Window
{
	private:
		WINDOW * win;
		int cursorY;
		int cursorX;
		int posY;
		int posX;
		int height;
		int width;
	public:
		Window(int height, int width, int row, int col);
		void addText(string text);
		void changePos(int row, int col);
		void setBorder();
		void refresh();
		void addChar(char c);
		void clear();
		void print(string text, int row, int col);
		void print(string text);
		void backSpace();
		void read(string& text);
		int getCursorY();
		int getCursorX();
		void setCursorY(int y);
		void setCursorX(int x);
		
};

ClientGUI::ClientGUI()
{
	int inputHeight = 10;
	term = new Terminal();
	term->refresh();
	input = new Window(inputHeight, term->getWidth(), (term->getHeight() - 10), 0);
	input->refresh();
	input->setBorder();
	
	output = new Window((term->getHeight() - inputHeight), term->getWidth(), 0, 0);

	output->refresh();
	input->refresh();
	term->refresh();
}

ClientGUI::~ClientGUI()
{
	delete term;
}

void ClientGUI::getInput(string & message)
{
	while(1)
	{
		char c = term->getInput();
		if(c == 10)
		{
			input->clear();
			
			//printSendMessage(inputMessage);
			//printReceivedMessage("From server!");
			
			//inputMessage.clear();
			return;
			
		}
		else
		{
			if(c == 8 || c == 127)
			{
				input->backSpace();
				message = message.substr(0, message.size() - 1);
				input->refresh();
				term->refresh();
			}
			else
			{
				message += c;
				input->addChar(c);
			}
		}
	}
}

void ClientGUI::printReceivedMessage(string message)
{
	int y, x;
	y = output->getCursorY();
	x = output->getCursorX();
	output->setCursorX(term->getWidth() - message.length());
	output->print(message);
	output->setCursorX(x);
	output->setCursorY(y + 1);
	output->refresh();
	
}

void ClientGUI::printSendMessage(string message)
{
	int y, x;
	y = output->getCursorY();
	x = output->getCursorX();
	output->setCursorX(0);
	output->print(message);
	output->setCursorX(x);
	output->setCursorY(y + 1);
	output->refresh();
}

void ClientGUI::refresh()
{
	term->refresh();
}


Terminal::Terminal()
{
	initscr();
	cbreak();
	noecho();
	win = stdscr;
	getmaxyx(win, height, width);
}

void Terminal::test()
{
	wprintw(win, "Hello\n\n");
	//getInput();
}

void Terminal::print(string text, int row, int col)
{
	mvprintw(row, col, text.c_str());
}

Terminal::~Terminal()
{
	endwin();
}

void Terminal::refresh()
{
	::refresh();
}

int Terminal::getHeight()
{
	return height;
}

int Terminal::getWidth()
{
	return width;
}

char Terminal::getInput()
{
	return getch();
}


Window::Window(int h, int w, int row, int col) //This is not generic
{
	win = newwin(height, width, row, col);
	height = h;
	width = w;
	posY = row;
	posX = col;
	cursorY = 1;
	cursorX = 1;
}

void Window::setBorder()	//This is not generic
{
	wborder(win, ' ', ' ', '_', ' ', '_', '_', ' ', ' ');
}

void Window::addText(string text)
{
	mvwprintw(win, 1, 1, text.c_str());
}

void Window::changePos(int row, int col)
{
	mvwin(win, row, col);	//this moves but old window still exists.
}

void Window::refresh()
{
	wrefresh(win);
}

void Window::addChar(char c)
{	
	mvwprintw(win, cursorY, cursorX, "%c", c);
	wrefresh(win);
	getyx(win, cursorY, cursorX);
}

void Window::print(string text)
{
	wmove(win, cursorY, cursorX);
	wprintw(win, text.c_str()); //Note the obvious flaw: %d in string!
}

void Window::print(string text, int row, int col)
{
	mvwprintw(win, row, col, text.c_str());
	wmove(win, row, col);
	cursorY = row++;
	cursorX = col;
}

void Window::read(string &text)
{

}

void Window::backSpace()
{
	mvwdelch(win, cursorY, cursorX);
	mvwprintw(win, cursorY, cursorX-1, " ");
	cursorX--;
	
	wrefresh(win);
}

int Window::getCursorY()
{
	return cursorY;
}

int Window::getCursorX()
{
	return cursorX;
}

void Window::setCursorY(int y)
{
	cursorY = y;
}

void Window::setCursorX(int x)
{
	cursorX = x;
}

void Window::clear()
{
	::werase(win);
	cursorX = 1;
	cursorY = 1;
	wmove(win, cursorY, cursorX);
	setBorder();
	refresh();
	
}
