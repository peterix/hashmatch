#include <iostream>
#include <map>
#include <fstream>
#include <iterator>
#include <string>
#include <map>
#include <vector>
#include <sstream>

using namespace std;

typedef map<string, vector<string> > MatchMap;

vector<string> &split(const string &s, char delim, vector<string> &elems)
{
	stringstream ss(s);
	string item;
	while (getline(ss, item, delim))
	{
		elems.push_back(item);
	}
	return elems;
}

vector<string> split(const string &s, char delim)
{
	vector<string> elems;
	split(s, delim, elems);
	return elems;
}

std::string join( const std::vector<std::string>& elements, const char* const separator)
{
	switch (elements.size())
	{
		case 0:
			return "";
		case 1:
			return elements[0];
		default:
			std::ostringstream os;
			std::copy(elements.begin(), elements.end()-1, std::ostream_iterator<std::string>(os, separator));
			os << *elements.rbegin();
			return os.str();
	}
}

MatchMap fromFile(const char * filename)
{
	string line;
	ifstream infile(filename);
	MatchMap result;
	while (getline(infile, line))
	{
		auto vec = split(line, '\t');
		if(vec.size() != 2)
		{
			cerr << "Line doesn't match: " << line << " in file " << filename;
			throw 1;
		}
		auto & hash = vec[0];
		auto & file = vec[1];
		auto iter = result.find(hash);
		if(iter != result.end())
		{
			(*iter).second.push_back(file);
		}
		else
		{
			result[hash] = {file};
		}
	}
	return result;
}

void matchAndPrint(MatchMap left, MatchMap right)
{
	map<string, pair<vector<string>, vector<string>>> zipped;
	// left to right and mixed
	{
		auto iter = left.begin();
		while (iter != left.end())
		{
			auto & hash = iter->first;
			auto findRight = right.find(hash);
			if(findRight != right.end())
			{
				zipped[hash] = make_pair(iter->second, findRight->second);
			}
			else
			{
				zipped[hash] = make_pair(iter->second, vector<string>());
			}
			iter++;
		}
	}
	// only right to left
	{
		auto iter = right.begin();
		while (iter != right.end())
		{
			auto & hash = iter->first;
			if(!left.count(hash))
			{
				zipped[hash] = make_pair(vector<string>(), iter->second);
			}
			iter++;
		}
	}
	auto iter = zipped.begin();
	while (iter != zipped.end())
	{
		cout << join(iter->second.first, ",") << "\t" << iter->first << "\t" << join(iter->second.second, ",") << "\n";
		iter++;
	}
}

int main(int argc, char **argv)
{
	if(argc != 3)
		return 1;

	auto left = fromFile(argv[1]);
	auto right = fromFile(argv[2]);

	matchAndPrint(left, right);
	return 0;
}

