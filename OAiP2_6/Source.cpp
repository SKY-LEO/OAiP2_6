#include <iostream>
#include <conio.h>
#include <iomanip>
#include <ctime>

using namespace std;

const double EPS = 0.000001;
const double DELTA = 0.00001;

struct Queue
{
	double x;
	double h;
	Queue* prev, * next;
};

double correctInputDouble();
double function(double x);
void stepOne(Queue*& begin, Queue*& end, double a, double b, double h);
double secantFunction(double x_prev, double x, int& k);
double vegstein(double x_1, double x_2, int& k);
bool isGoodStep(double a, double b, double h);
void pushQueue(Queue*& begin, Queue*& end, double x, double h);
void popQueue(Queue*& begin, Queue*& end, double& x, double& h);
void deleteQueue(Queue*& begin, Queue*& end);
void showInterval(Queue* begin, int i);
void stepTwo(Queue*& begin, Queue*& end);

int main()
{
	char code;
	double a, b, h;
	Queue* begin = nullptr, * end = nullptr;
	do
	{
		cout << "Enter a:" << endl;
		a = correctInputDouble();
		cout << "Enter b:" << endl;
		b = correctInputDouble();
		if (b < a)
		{
			cout << "b < a, swap them!" << endl;
			double temp = a;
			a = b;
			b = temp;
		}
		do
		{
			cout << "Enter h:" << endl;
			h = correctInputDouble();
		} while (!isGoodStep(a, b, h));
		stepOne(begin, end, a, b, h);
		if (begin)
		{
			cout << "\nIntervals:" << endl;
			showInterval(begin, 1);
			stepTwo(begin, end);
		}
		else
		{
			cout << "There are no roots of function!" << endl;
		}
		if (begin)deleteQueue(begin, end);
		cout << "Do you want to continue?(Y/N)" << endl;
		code = (char)_getch();
	} while (code == 'Y' || code == 'y');
	system("pause");
	return 0;
}

void stepTwo(Queue*& begin, Queue*& end)
{
	double x, h;
	int i = 1, k = 0;
	unsigned int start_time, end_time;
	while (begin)
	{
		popQueue(begin, end, x, h);
		cout << "-> " << i << endl;
		start_time = clock();
		secantFunction(x, x + h, k = 0);
		end_time = clock();
		cout << "Time: "  << (double)(end_time - start_time) / CLOCKS_PER_SEC << " s" << endl;
		start_time = clock();
		vegstein(x, x + h, k = 0);
		end_time = clock();
		cout << "Time: " <<(double)(end_time - start_time)/ CLOCKS_PER_SEC << " s" << endl;
		cout << endl;
		i++;
	}
}

void stepOne(Queue*& begin, Queue*& end, double a, double b, double h)
{
	double var;
	cout << "---------------------------------" << endl;
	for (double x = a; x <= b; x += h)
	{
		cout << "|\t" << x;
		var = function(x);
		if (var > 0)cout << "\t|\t+\t|" << endl;
		else cout << "\t|\t-\t|" << endl;
		cout << "---------------------------------" << endl;
		if (var * function(x + h) < 0)
		{
			pushQueue(begin, end, x, h);
		}
	}
}

void showInterval(Queue* begin, int i)
{
	if (begin)
	{
		cout << i << "-> [" << begin->x << " ; " << begin->x + begin->h << "]" << endl;
		begin = begin->next;
		return showInterval(begin, i + 1);
	}
	cout << endl;
}

double secantFunction(double x_prev, double x, int& k)
{
	x = x_prev - (function(x_prev) * DELTA) / (function(x_prev) - function(x_prev - DELTA));
	if (abs(x - x_prev) < EPS)
	{
		cout << setprecision(7) << "Result: " << x << "\nNumber of iterations: " << k << endl;
		return x;
	}
	return secantFunction(x, x, ++k);
}

double vegstein(double x_1, double x_2, int& k)
{
	double x = x_1 - (function(x_1) * (x_1 - x_2)) / (function(x_1) - function(x_2));
	if (abs(x - x_1) < EPS)
	{
		cout << setprecision(7) << "Result: " << x << "\nNumber of iterations: " << k << endl;
		return x;
	}
	return vegstein(x, x_1, ++k);
}

bool isGoodStep(double a, double b, double h)
{
	if (h > b - a)
	{
		cout << "h bigger than gap!" << endl;
		return false;
	}
	if (h < EPS)
	{
		cout << "h smaller than EPS!" << endl;
		return false;
	}
	return true;
}

double function(double x)
{
	return x - 5 * sin(x) * sin(x) - 5;
	//return x * x;
}

void pushQueue(Queue*& begin, Queue*& end, double x, double h)
{
	Queue* temp = new Queue;
	temp->x = x;
	temp->h = h;
	if (begin == nullptr)
	{
		temp->next = temp->prev = nullptr;
		begin = temp;
		end = temp;
	}
	else
	{
		temp->next = nullptr;//сзади нет
		temp->prev = end;//впереди есть
		end->next = temp;
		end = temp;
	}
}

void popQueue(Queue*& begin, Queue*& end, double& x, double& h)
{
	Queue* temp = begin;
	x = begin->x;
	h = begin->h;
	if (begin != end)
	{
		begin = begin->next;
		begin->prev = nullptr;
	}
	else
	{
		begin = end = nullptr;
	}
	delete temp;
}

void deleteQueue(Queue*& begin, Queue*& end)
{
	while (begin)
	{
		Queue* temp = begin;
		begin = begin->next;
		delete temp;
	}
	end = nullptr;
	cout << "\nQueue deleted successfully" << endl;
}

double correctInputDouble()
{
	double a;
	while (true)
	{
		cin >> a;
		if (cin.get() == '\n') {
			break;
		}
		else
		{
			cin.clear();
			cin.ignore(256, '\n');
			cout << "Error, please write Integer numbers!\n" << "Try again!" << endl;
		}
	}
	return a;
}