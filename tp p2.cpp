#include <cctype>
#include <vector>
#include <iostream>
#include <string>
#include <numeric>
#include <math.h>
#include <map>
#include <future>
#include <thread>
using namespace std;
bool isDigit(char c)
{
	if ((int(c) > 47 && int(c) < 58))
	{
		return true;
	}
	return false;
}

int priority(char c)
{
	map<char, int>priorityMap
	{
		{'+',0},
		{'-',0},
		{'*',1},
		{'/',1},
		{'^',2},
		{'(',-1},
	};
	return priorityMap[c];
}

int getAction(string item)
{
	/*0-liczba(wprowadź w output)*/
	/*-1 inny znak matematyczny*/
	/*3-KONIEC NAWIASU(zdejmij ostatni znak ze stosu, włóż go na output, usuń '(' ze stosu*/

	if (isDigit(item[0]) == true)
	{
		return 0;

	}

	else if (item[0] == ')')
	{
		return 3;

	}
	else
	{
		return -1;
	}


}


vector<string>createInput(string formula)
{
	vector<string>input;
	bool stillDigit = false;
	string currentDigit;
	for (int i = 0; i < formula.length(); i++)
	{
		if (((isDigit(formula[i]) == true || formula[i] == '.') && stillDigit == true))
		{
			currentDigit += formula[i];
			if (i == formula.length() - 1)
			{
				input.push_back(currentDigit);

			}
		}
		if (((isDigit(formula[i]) == true) && stillDigit == false))
		{
			stillDigit = true;
			currentDigit = formula[i];
			if (i == formula.length() - 1)
			{
				input.push_back(currentDigit);

			}

		}
		if (((isDigit(formula[i]) == false && formula[i] != '.')))
		{
			if (stillDigit == true)
			{
				stillDigit = false;
				input.push_back(currentDigit);
			}
			input.push_back(string(1, formula[i]));
		}
	}
	return input;
}



vector <string> convertONP(vector<string>input, vector<string>stack, int size)
{
	vector<string>output;
	for (int i = 0; i < size; i++)
	{

		if (getAction(input[i]) == 0)
		{
			output.push_back(input[i]);

		}
		else if (getAction(input[i]) == -1)
		{
			if (priority(input[i][0]) == -1)
			{
				stack.push_back(input[i]);
			}
			else if (stack.size() > 0)
			{

				if (priority(stack[stack.size() - 1][0]) >= priority(input[i][0]))
				{

					output.push_back(stack[stack.size() - 1]);
					stack.pop_back();
					stack.push_back(input[i]);



				}
				else if (priority(stack[stack.size() - 1][0]) < priority(input[i][0]))
				{
					stack.push_back(input[i]);
				}

			}
			else if (stack.size() == 0)
			{
				stack.push_back(input[i]);

			}
		}
		else if (getAction(input[i]) == 3)
		{


			for (int j = stack.size() - 1; j > 0; j--)
			{
				if (stack[j][0] == '(')
				{
					stack.erase(stack.begin() + j);
					break;
				}

				string lastItem = stack.back();
				output.push_back(lastItem);
				stack.pop_back();


			}

		}



	}


	for (int i = stack.size() - 1; i >= 0; i--)
	{

		if (stack[i] == "(" || stack[i] == ")") continue;
		output.push_back(stack[i]);

	}


	return output;
}

float asyncAdd(float num1, float num2)
{


	return num1 + num2;

}
float asyncSubtract(float num1, float num2)
{


	return num1 - num2;

}
float asyncMultiple(float num1, float num2)
{


	return num1 * num2;

}
float asyncDivide(float num1, float num2)
{


	return num1 / num2;

}
float asyncExp(float num1, float num2)
{


	return pow(num1, num2);

}

float calculation(vector<string> stack, vector<string> output) {
	float outcome;
	float num1 = 0;
	float num2 = 0;
	for (int i = 0; i < output.size(); i++) {

		if (isDigit(output[i][0])) {
			stack.push_back(output[i]);
		}
		else {
			if (output[i][0] == '^') {

				num2 = stof(stack.back());
				stack.pop_back();
				num1 = stof(stack.back());
				stack.pop_back();
				future<float>result = async(launch::async, asyncExp, num1, num2);
				float asyncresult = result.get();
				stack.push_back(to_string(asyncresult));
			}

			else if (output[i][0] == '+') {

				num2 = stof(stack.back());
				stack.pop_back();
				num1 = stof(stack.back());
				stack.pop_back();
				future<float>result = async(launch::async, asyncAdd, num1, num2);
				float asyncresult = result.get();
				stack.push_back(to_string(asyncresult));
			}
			else if (output[i][0] == '-') {

				num2 = stof(stack.back());
				stack.pop_back();
				num1 = stof(stack.back());
				stack.pop_back();
				future<float>result = async(launch::async, asyncSubtract, num1, num2);
				float asyncresult = result.get();
				stack.push_back(to_string(asyncresult));
			}
			else if (output[i][0] == '*') {

				num2 = stof(stack.back());
				stack.pop_back();
				num1 = stof(stack.back());
				stack.pop_back();
				future<float>result = async(launch::async, asyncMultiple, num1, num2);
				float asyncresult = result.get();
				stack.push_back(to_string(asyncresult));
			}
			else if (output[i][0] == '/') {

				num2 = stof(stack.back());
				stack.pop_back();
				num1 = stof(stack.back());
				stack.pop_back();
				future<float>result = async(launch::async, asyncDivide, num1, num2);
				float asyncresult = result.get();
				stack.push_back(to_string(asyncresult));
			}



		}

	}

	return stof(stack.back());
}





int main()
{


	vector<string>stack;

	string formula;
	cout << "Podaj działanie:";
	cin >> formula;
	//cout << createInput(formula).size() << endl;
	vector<string>output = convertONP(createInput(formula), stack, createInput(formula).size());
	for (int i = 0; i < output.size(); i++) {
		cout << output[i];
	}
	cout << endl;

	future<float>result = async(launch::async, calculation, stack, output);
	float asyncresult = result.get();
	cout << "Wynik: " << asyncresult;








}