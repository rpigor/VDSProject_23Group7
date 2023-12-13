/*=============================================================================
    Written by Mohammad R Fadiheh (2017)
=============================================================================*/

#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<map>

struct node {
	std::string var_name;
	int low;
	int high;
};

typedef std::map<int, node> uniqueTable;

bool isEquivalent(const uniqueTable &BDD1, const uniqueTable &BDD2, int root1, int root2)
{
	if(BDD1.find(root1) == BDD1.end() || BDD2.find(root2) == BDD2.end())
		return false;
	if(root1 == 1 && root2 == 1)
		return true;
	if(root1 == 0 && root2 == 0)
		return true;
	if(root1 != root2 && ( (root1 == 0 || root1 == 1) || (root2 == 0 || root2 == 1) ) )
		return false;
	if(BDD1.at(root1).var_name != BDD2.at(root2).var_name)
		return false;
    return isEquivalent(BDD1, BDD2, BDD1.at(root1).low, BDD2.at(root2).low) and isEquivalent(BDD1, BDD2, BDD1.at(root1).high, BDD2.at(root2).high);
}

int main(int argc, char* argv[])
{

	/* Number of arguments validation */
	if (3 > argc)
	{
		std::cout << "Must specify a filename!" << std::endl;
		return -1;
	}
	
	uniqueTable BDD1, BDD2;

	std::string BDD1_file = argv[1];
	std::string BDD2_file = argv[2];

	std::ifstream BDD1_if(BDD1_file.c_str());
	std::ifstream BDD2_if(BDD2_file.c_str());

	if(!BDD1_if.is_open() || !BDD2_if.is_open())
	{
		std::cout << "invalid file!" << std::endl;
		return -1;
	}

	std::stringstream ss;

	std::string temp;
	std::string var_name;
	int id, top_var;

	while(!BDD1_if.eof())
	{
		node n;			
		temp.clear();
		std::getline(BDD1_if, temp,'\n');

		if(temp.find("Terminal Node: 1") != std::string::npos)
		{
			n.var_name = "";
			n.low = 1;
			n.high = 1;
			BDD1.insert(std::pair<int,node>(1,n));
		}
		else if(temp.find("Terminal Node: 0") != std::string::npos)
		{
			n.var_name = "";
			n.low = 0;
			n.high = 0;
			BDD1.insert(std::pair<int,node>(0,n));

		}
		else if(temp.find("Variable Node:") != std::string::npos)
		{
			ss.clear();
			ss.str(temp);
			ss>>temp>>temp>>id>>temp>>temp>>temp>>top_var>>temp>>temp>>temp>>n.var_name>>temp>>n.low>>temp>>n.high;
			BDD1.insert(std::pair<int,node>(id,n));
		}
	}

	while(!BDD2_if.eof())
	{
		node n;			
		temp.clear();
		std::getline(BDD2_if, temp,'\n');

		if(temp.find("Terminal Node: 1") != std::string::npos)
		{
			n.var_name = "";
			n.low = 1;
			n.high = 1;
			BDD2.insert(std::pair<int,node>(1,n));
		}
		else if(temp.find("Terminal Node: 0") != std::string::npos)
		{
			n.var_name = "";
			n.low = 0;
			n.high = 0;
			BDD2.insert(std::pair<int,node>(0,n));

		}
		else if(temp.find("Variable Node:") != std::string::npos)
		{
			ss.clear();
			ss.str(temp);
			ss>>temp>>temp>>id>>temp>>temp>>temp>>top_var>>temp>>temp>>temp>>n.var_name>>temp>>n.low>>temp>>n.high;
			BDD2.insert(std::pair<int,node>(id,n));
		}
	}

	if( isEquivalent(BDD1, BDD2, BDD1.rbegin()->first, BDD2.rbegin()->first) )
		std::cout<<"Equivalent!"<<std::endl;
	else
		std::cout<<"Not Equivalent!"<<std::endl;
	return 0;
}

