#ifndef WINDOW_H
#define WINDOW_H 1

#include <ncurses.h>
#include <string>

using namespace std;

class Terminal;
class Window;

class ClientGUI
{
	private:
		Terminal * term;
		Window * input;
		Window * output;
		string inputMessage;
		string outputMessage;
	public:
		ClientGUI();
		~ClientGUI();
		void test();
		void refresh();
		void getInput(string & message);
		void printReceivedMessage(string message);
		void printSendMessage(string message);
};

class Terminal
{
	private:
		WINDOW * win;
		int height;
		int width;
	public:
		Terminal();
		~Terminal();
		void test();
		void refresh();
		int getHeight();
		int getWidth();
		char getInput();
		void print(string text, int row, int col);
};

#endif
