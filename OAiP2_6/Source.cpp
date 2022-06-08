#include <iostream>
#include <conio.h>
#include <iomanip>

using namespace std;

const double EPS = 0.000001;
const double DELTA = 0.00001;
const int MAX_ITERATIONS = 1000;

struct Queue
{
	double x;
	double h;
	Queue* prev, * next;
};

bool isGoodStep(double a, double b, double h);
int numOfSymAfterComma(double num, int k);
double correctInputDouble();
double function(double x);
double secantFunction(double x_prev, int& k);
double vegstein(double x_1, double x_2, int& k);
void pushQueue(Queue*& begin, Queue*& end, double x, double h);
void popQueue(Queue*& begin, Queue*& end, double& x, double& h);
void deleteQueue(Queue*& begin, Queue*& end);
void stepOne(Queue*& begin, Queue*& end, double a, double b, double h);
void showInterval(Queue* begin, int i, int k);
void stepTwo(Queue*& begin, Queue*& end, int k);
void searchSpecial(Queue*& begin, Queue*& end, double a, double b, double h);

int main()
{
	char code;
	double a, b, h;
	int k = numOfSymAfterComma(EPS, 1);
	Queue* begin = nullptr, * end = nullptr;
	cout << "Function: x - 5(sinx)^2 - 5" << "\nEPS: " << EPS << endl;
	cout << "------------------------------------------" << endl;
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
			showInterval(begin, 1, k);
			stepTwo(begin, end, k);
		}
		else
		{
			cout << "There are no roots of function!" << endl;
		}
		cout << "Do you want to search special roots?(Y/N)" << endl;
		code = (char)_getch();
		if (code == 'Y' || code == 'y')
		{
			searchSpecial(begin, end, a, b, EPS / 10.);
			if (begin)
			{
				cout << "\nIntervals:" << endl;
				showInterval(begin, 1, k);
			}
			else
			{
				cout << "There are no special roots of function!" << endl;
			}
		}
		if (begin)deleteQueue(begin, end);
		cout << "Do you want to continue?(Y/N)" << endl;
		code = (char)_getch();
	} while (code == 'Y' || code == 'y');
	system("pause");
	return 0;
}

bool isGoodStep(double a, double b, double h)
{
	if (h > fabs(a - b))
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

void stepOne(Queue*& begin, Queue*& end, double a, double b, double h)
{
	double var;
	cout << "---------------------------------" << endl;
	for (double x = a; x < b + h / 4.; x += h)
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

void showInterval(Queue* begin, int i, int k)
{
	if (begin)
	{
		cout << i << "-> [" << setprecision(k) << begin->x << " ; " << begin->x + begin->h << "]" << endl;
		begin = begin->next;
		return showInterval(begin, i + 1, k);
	}
	cout << endl;
}

void stepTwo(Queue*& begin, Queue*& end, int j)
{
	double x, h, result;
	int i = 1, k = 0;
	while (begin)
	{
		popQueue(begin, end, x, h);
		cout << "-> " << i << endl;
		result = secantFunction(x, k = 0);
		cout << "Secant method\n" << setprecision(j) << "x: " << result << "\nNumber of iterations: " << k << endl;
		result = vegstein(x, x + h, k = 0);
		cout << "\nVegstein method\n" << setprecision(j) << "x: " << result << "\nNumber of iterations: " << k << endl;
		cout << endl;
		i++;
	}
}

double secantFunction(double x_prev, int& k)
{
	if (k > MAX_ITERATIONS)
	{
		cout << "Unable to find value with given precision!" << endl;
		return MAX_ITERATIONS;
	}
	double x;
	double y_prev = function(x_prev);
	x = x_prev - (y_prev * DELTA) / (y_prev - function(x_prev - DELTA));
	if (fabs(x - x_prev) < EPS)
	{
		return x;
	}
	return secantFunction(x, ++k);
}

double vegstein(double x_1, double x_2, int& k)
{
	if (k > MAX_ITERATIONS)
	{
		cout << "Unable to find value with given precision!" << endl;
		return MAX_ITERATIONS;
	}
	double y_2 = function(x_2);
	double y_1 = function(x_1);
	double x = x_2 - (y_2 * (x_2 - x_1)) / (y_2 - y_1);
	if (fabs(x_2 - x) < EPS)
	{
		return x;
	}
	return vegstein(x_2, x, ++k);
}

void searchSpecial(Queue*& begin, Queue*& end, double a, double b, double h)
{
	double var, x_a, x_h;
	bool flag = false;
	cout << "In process..." << endl;
	for (double x = a; x < b + h / 4.; x += h)
	{
		var = function(x);
		if (fabs(var) < EPS && !flag)
		{
			x_a = x;
			flag = true;
		}
		else if (fabs(var) > EPS && flag)
		{
			x_h = x - x_a;
			pushQueue(begin, end, x_a, x_h);
			flag = false;
		}
	}
}

double function(double x)
{
	return x - 5 * sin(x) * sin(x) - 5;
	//return x*x;
}

int numOfSymAfterComma(double num, int k)
{
	if (num >= 1. || num <= -1.)
	{
		return k;
	}
	return numOfSymAfterComma(num * 10, k + 1);
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
			cout << "Error, please write numbers!\n" << "Try again!" << endl;
		}
	}
	return a;
}