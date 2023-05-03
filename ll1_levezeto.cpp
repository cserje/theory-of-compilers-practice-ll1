/*
 * E  -> TE'
 * E' -> +TE' | -TE' | e
 * T  -> FT'
 * T' -> *FT' | /FT' | e
 * F  -> N | (E) | sin(E) | cos(E)
 * N  -> DN'
 * N' -> DN' | e
 */

#include <iostream>
#include <string>

using namespace std;

class illegal_input
{
};

class parser
{
private:
	string input;
	unsigned int pos;

	char LookAhead()
	{
		return input[pos];
	}
	string LookAhead(int count)
	{
		return input.substr(pos, count);
	}
	void Match(const char c)
	{
		if (LookAhead() == c)
			pos++;
		else
			throw illegal_input();
	}
	void Match(const string s)
	{
		int count = s.length();
		if (LookAhead(count) == s)
			pos += count;
		else
			throw illegal_input();
	}
	void D()
	{
		if (LookAhead() >= '0' && LookAhead() <= '9')
		{
			cout << "D -> " << LookAhead() << endl;
			Match(LookAhead());
		}
		else
			throw illegal_input();
	}
	void N_()
	{
		if (LookAhead() >= '0' && LookAhead() <= '9')
		{
			cout << "N'-> DN'" << endl;
			D();
			N_();
		}
		else if (LookAhead() == '+' ||
				 LookAhead() == '-' ||
				 LookAhead() == '*' ||
				 LookAhead() == '/' ||
				 LookAhead() == ')' ||
				 LookAhead() == '$')
		{
			cout << "N'-> e" << endl;
		}
		else
			throw illegal_input();
	}
	void N()
	{
		if (LookAhead() >= '0' && LookAhead() <= '9')
		{
			cout << "N-> DN'" << endl;
			D();
			N_();
		}
		else
			throw illegal_input();
	}
	void F();
	void T_()
	{
		char c;
		if (LookAhead() == '*' || LookAhead() == '/')
		{
			c = LookAhead();
			cout << "T'-> " << c << "FT'" << endl;
			Match(c);
			F();
			T_();
		}
		else if (LookAhead() == '+' || LookAhead() == '-' ||
				 LookAhead() == ')' || LookAhead() == '$')
		{
			cout << "T' -> e" << endl;
		}
		else
			throw illegal_input();
	}
	void T()
	{
		if (LookAhead() >= '0' && LookAhead() <= '9')
		{
			cout << "T -> FT'" << endl;
			F();
			T_();
		}
		else if (LookAhead() == '(')
		{
			cout << "T -> FT'" << endl;
			F();
			T_();
		}
		else if (LookAhead(4) == "sin(" || LookAhead(4) == "cos(")
		{
			cout << "T -> FT'" << endl;
			F();
			T_();
		}
		else
			throw illegal_input();
	}
	void E_()
	{
		char c;
		if (LookAhead() == '+' || LookAhead() == '-')
		{
			c = LookAhead();
			cout << "E -> " << c << "TE'" << endl;
			Match(c);
			T();
			E_();
		}
		else if (LookAhead() == ')' || LookAhead() == '$')
		{
			cout << "E' -> e" << endl;
		}
		else
			throw illegal_input();
	}
	void E()
	{
		if (LookAhead() >= '0' && LookAhead() <= '9')
		{
			cout << "E -> TE'" << endl;
			T();
			E_();
		}
		else if (LookAhead() == '(')
		{
			cout << "E -> TE'" << endl;
			T();
			E_();
		}
		else if (LookAhead(4) == "sin(" || LookAhead(4) == "cos(")
		{
			cout << "E -> TE'" << endl;
			T();
			E_();
		}
		else
			throw illegal_input();
	}

public:
	void Parse(const string &s)
	{
		input = s + '$';
		pos = 0;
		E();
		Match('$');
	}
};

void parser::F()
{
	if (LookAhead() >= '0' && LookAhead() <= '9')
	{
		cout << "F -> N" << endl;
		N();
	}
	else if (LookAhead() == '(')
	{
		Match('(');
		E();
		Match(')');
		cout << "F -> (E)" << endl;
	}
	else if (LookAhead(4) == "sin(" || LookAhead(4) == "cos(")
	{
		string m = LookAhead(4);
		Match(m);
		E();
		Match(')');
		cout << "F -> " << m << "E)" << endl;
	}
	else
		throw illegal_input();
}

int main()
{
	string s;
	cin >> s;
	try
	{
		parser().Parse(s);
		cout << "\nAccepted.\n";
	}
	catch (illegal_input)
	{
		cout << "\nNot accepted.\n";
		;
	}
	return 0;
}

// ---------------- Accepted ----------------
/*
Input:
	sin(0)
Output:
	E -> TE'
	T -> FT'
	E -> TE'
	T -> FT'
	F -> N
	N-> DN'
	D -> 0
	N'-> e
	T' -> e
	E' -> e
	F -> sin(E)
	T' -> e
	E' -> e
	Accepted.


Input:
	sin(30)+cos(60)
Output:
	E -> TE'
	T -> FT'
	E -> TE'
	T -> FT'
	F -> N
	N-> DN'
	D -> 3
	N'-> DN'
	D -> 0
	N'-> e
	T' -> e
	E' -> e
	F -> sin(E)
	T' -> e
	E ->+TE'
	T -> FT'
	E -> TE'
	T -> FT'
	F -> N
	N-> DN'
	D -> 6
	N'-> DN'
	D -> 0
	N'-> e
	T' -> e
	E' -> e
	F -> cos(E)
	T' -> e
	E' -> e
	Accepted.
 

Input:
	10*(sin(90)+cos(90/(2+2)))
Output:
	E -> TE'
	T -> FT'
	F -> N
	N-> DN'
	D -> 1
	N'-> DN'
	D -> 0
	N'-> e
	T'-> *FT'
	E -> TE'
	T -> FT'
	E -> TE'
	T -> FT'
	F -> N
	N-> DN'
	D -> 9
	N'-> DN'
	D -> 0
	N'-> e
	T' -> e
	E' -> e
	F -> sin(E)
	T' -> e
	E ->+TE'
	T -> FT'
	E -> TE'
	T -> FT'
	F -> N
	N-> DN'
	D -> 9
	N'-> DN'
	D -> 0
	N'-> e
	T'->/FT'
	E -> TE'
	T -> FT'
	F -> N
	N-> DN'
	D -> 2
	N'-> e
	T' -> e
	E ->+TE'
	T -> FT'
	F -> N
	N-> DN'
	D -> 2
	N'-> e
	T' -> e
	E' -> e
	F -> (E)
	T' -> e
	E' -> e
	F -> cos(E)
	T' -> e
	E' -> e
	F -> (E)
	T' -> e
	E' -> e
	Accepted.
*/
  

// ---------------- Not accepted ----------------

/*
Input:
	sin30
Output:
	Not accepted.


Input:
	sin(30+cos(60)
Output:
	E -> TE'
	T -> FT'
	E -> TE'
	T -> FT'
	F -> N
	N-> DN'
	D -> 3
	N'-> DN'
	D -> 0
	N'-> e
	T' -> e
	E ->+TE'
	T -> FT'
	E -> TE'
	T -> FT'
	F -> N
	N-> DN'
	D -> 6
	N'-> DN'
	D -> 0
	N'-> e
	T' -> e
	E' -> e
	F -> cos(E)
	T' -> e
	E' -> e
	Not accepted.


Input:
	2*cos(90+)
Output:
	E -> TE'
	T -> FT'
	F -> N
	N-> DN'
	D -> 2
	N'-> e
	T'->*FT'
	E -> TE'
	T -> FT'
	F -> N
	N-> DN'
	D -> 9
	N'-> DN'
	D -> 0
	N'-> e
	T' -> e
	E ->+TE'
	Not accepted.
*/
