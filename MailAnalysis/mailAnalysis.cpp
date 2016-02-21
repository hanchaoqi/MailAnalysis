#include<string>
#include<vector>
#include<map>
#include<fstream>
#include<iostream>
using namespace std;

typedef vector<string>::const_iterator Line_iter;
class Message{
	Line_iter first;
	Line_iter last;
public:
	Message(Line_iter p1, Line_iter p2) :first(p1), last(p2){}
	Line_iter begin() const{ return first; }
	Line_iter end() const{ return last; }
};

typedef vector<Message>::const_iterator Mess_iter;
struct Mail_file{
	string name;
	vector<string> lines;
	vector<Message> m;

	Mail_file(const string& m);
	Mess_iter begin() const{ return m.begin(); }
	Mess_iter end() const{ return m.end(); }
};
int is_prefix(const string& s, const string& p);
bool find_from_addr(const Message* m, string& s);
string find_subject(const Message& m);
ostream& operator<<(ostream& os, const Message& m);
int main()
{
	Mail_file mfile("my-mail-file.txt");
	multimap<string, const Message*> sender;
	for (Mess_iter p = mfile.begin(); p != mfile.end(); ++p)
	{
		const Message& m = *p;
		string s;
		if (find_from_addr(&m, s))
		{
			sender.insert(make_pair(s, &m));
		}
			
	}
	typedef multimap<string, const Message*>::const_iterator MCI;
	pair<MCI, MCI> pp = sender.equal_range("John Doe <jdoe@machine.example>");
	
	for (MCI p = pp.first; p != pp.second; ++p)
	{
		cout << "hello" << endl;
		cout << find_subject(*p->second) << '\n';
	}
	system("pause");
	return 0;
}
Mail_file::Mail_file(const string& n)
{
	ifstream in(n.c_str());
	if (!in)
	{
		cerr << "No " << n << endl;
		exit(1);
	}
	string s;
	while (getline(in, s))
		lines.push_back(s);

	Line_iter first = lines.begin();
	for (Line_iter p = lines.begin(); p != lines.end(); ++p)
	{
		if (*p == "----")
		{
			m.push_back(Message(first, p));
			first = p + 1;
		}
	}
}
int is_prefix(const string& s, const string& p)
{
	int n = p.size();
	if (string(s, 0, n) == p)
		return n;
	return 0;
}
bool find_from_addr(const Message* m, string& s)
{
	for (Line_iter p = m->begin(); p != m->end();++p)
	if (int n = is_prefix(*p, "From: "))
	{
		s = string(*p, n);
		return true;
	}
	return false;
}
string find_subject(const Message& m)
{
	for (Line_iter p = m.begin(); p != m.end();++p)
	if (int n = is_prefix(*p, "Subject: "))
	{
		return string(*p, n);
	}
		
	return "";
}
ostream& operator<<(ostream& os, const Message& m)
{
	string s;
	for (Line_iter p = m.begin(); p != m.end(); ++p)
		s += *p + "\n";
	
	return os << s;
}