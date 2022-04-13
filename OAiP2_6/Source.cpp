#include <iostream>
#include <conio.h>
#include <iomanip>

using namespace std;

const double EPS = 0.000001;
const double DELTA = 0.000001;

double correctInputDouble();
double function(double x);
void stepOne(double a, double b, double h);
double secantFunction(double a, double b, int& k);
double vegstein(double x_1, double x_2, int& k);
bool isGoodStep(double a, double b, double h);

int main()
{
	int code;
	double a, b, h;
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
		stepOne(a, b, h);

		cout << "Do you want to continue?(Y/N)" << endl;
		code = _getch();
	} while (code == 'Y' || code == 'y');
	system("pause");
	return 0;
}

void stepOne(double a, double b, double h)
{
	double var;
	cout << "---------------------------------" << endl;
	for (double x = a; x <= b; x += h)
	{
		cout <<"|\t" << x;
		var = function(x);
		if (var > 0)cout << "\t|\t+\t|" << endl;
		else cout << "\t|\t-\t|"<<endl;
		cout << "---------------------------------" << endl;
		if (var * function(x + h) < 0)
		{
			cout << x << " " << x + h << endl;
			int k = 0;
			secantFunction(x, x + h, k);
			vegstein(x, x + h, k = 0);
		}
	}
}

double secantFunction(double x_p, double x, int& k)
{
	x = x_p - (function(x_p) * DELTA) / (function(x_p) - function(x_p - DELTA));
	if (abs(x - x_p) < EPS)
	{
		cout << setprecision(6) << "res: " << x << " k " << k << endl;
		return x;
	}
	return secantFunction(x, x, ++k);
}

double vegstein(double x_1, double x_2, int& k)
{
	double x = x_1 - (function(x_1) * (x_1 - x_2)) / (function(x_1) - function(x_2));
	if (abs(x - x_1) < EPS)
	{
		cout << "res: " << x << " k " << k << endl;
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
	return true;
}

double function(double x)
{
	return x - 5 * sin(x) * sin(x) - 5;
	//return x * x;
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