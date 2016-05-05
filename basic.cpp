#include <iostream>
using namespace std;
#include <fstream>
#include "Basic.h"

data_node m_data[100000];

int readFromFile(char* filename)
{
	ifstream infile(filename);
	char buffer[256];
	int number;//total number
	if (!infile.is_open())
	{
		cout << "Error opening file" << endl;
		return 0;
	}
	number = 0;
	while (!infile.eof())
	{
		infile.getline(buffer, 256);
		//cout << buffer << endl;
		m_data[number] = spiltline(buffer);
		//outPutNode(m_data[number]);
		number++;
	}
	infile.close();
	return number;
}

data_node spiltline(string buffer)
{
	data_node Node;
	int length = buffer.length();
	string temp;
	int number;
	int i, start, n_item;
	start = 0;
	n_item = 1;
	for (i = 0; i<length; i++)
	{
		if (buffer[i] == ',')
		{
			temp = buffer.substr(start, (i - start));
			switch (n_item)
			{
			case 1:
				number = atoi(temp.c_str());
				Node.item1 = number;
				break;
			case 2:
				number = atoi(temp.c_str());
				Node.item2 = number;
				break;
			case 3:
				number = atoi(temp.c_str());
				Node.item3 = number;
				break;
			case 4:
				Node.item4 = temp;
				break;
			case 5:
				Node.item5 = temp[0];
				break;
			case 6:
				Node.item6 = temp;
				break;
			case 7:
				number = atoi(temp.c_str());
				Node.item7 = number;
				break;
			case 8:
				Node.item8 = temp;
				break;
			case 9:
				Node.item9 = temp;
				break;
			case 10:
				Node.item10 = temp;
				break;
			case 11:
				Node.item11 = temp;
				break;
				/*case 12:
				number = atoi(temp.c_str());
				Node.item12 = number;
				break;*/
			default:
				break;
			}
			start = i + 1;
			n_item++;
		}
	}
	temp = buffer.substr(start, (i - start));
	number = atoi(temp.c_str());
	Node.item12 = number;


	return Node;
}

void outPutNode(data_node Node)
{
	cout << Node.item1 << " ";
	cout << Node.item2 << " ";
	cout << Node.item3 << " ";
	cout << Node.item4 << " ";
	cout << Node.item5 << " ";
	cout << Node.item6 << " ";
	cout << Node.item7 << " ";
	cout << Node.item8 << " ";
	cout << Node.item9 << " ";
	cout << Node.item10 << " ";
	cout << Node.item11 << " ";
	cout << Node.item12 << endl;
}
