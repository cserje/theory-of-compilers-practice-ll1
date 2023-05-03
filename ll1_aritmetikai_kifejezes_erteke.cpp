/*
E  -> TE'	:	E.v=T.v+E'.v
E' -> +TE' 	:	E'.v=T.v+E1'.v
E' -> -TE' 	: 	E'.v=-1*(T.v+E1'.v)
E' -> e 	: 	E'.v=0
T  -> FT' 	: 	T.v=T'.v*F.v
T' -> *FT' 	: 	T'.v=T1'.v*F.v
T' -> /FT' 	: 	T'.v=T1'.v*(1/F.v)
T' -> e 	: 	T'.v=1
F  -> N 	: 	F.v=N.v
F -> (E) 	: 	F.v=E.v
F -> sin(E) : 	F.v=sin(E.v)
F -> cos(E) : 	F.v=cos(E.v)
N  -> DN' 	: 	N.v=N'.v+D.v*N'.m*10
N' -> DN' 	: 	N'.m=N1'.m*10 és N'.v=N1'.v+D.v*N'.m  
N' -> e 	: 	N.v=0 és N.m=1
D -> 0 		:	D.v=0
...
D -> 9 		: 	D.v=9
*/

#include <iostream>
#include <string>
#include <cmath>

using namespace std;

class function{
public:
	virtual double calc(double a, double b)=0;
	virtual ~function();
};

class illegal_input{};

struct number{
	double value;
	double magnitude;
};

class parser{
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
    double D(){
    	double value;
    	if (LookAhead()>='0' && LookAhead()<='9'){
    		cout<<LookAhead();
    		value=LookAhead()-'0';
    		Match(LookAhead());
    	}
    	else
    		throw illegal_input();
    	return value;
    }
    number N_(){
    	double digitValue;
    	number num,returnValue;
    	if (LookAhead()>='0' && LookAhead()<='9'){
    		digitValue=D();
    		num=N_();
    		returnValue.magnitude=num.magnitude*10;
    		returnValue.value=digitValue*num.magnitude+num.value;
    	}
    	else
    	if (LookAhead()=='+' ||
    		LookAhead()=='-' ||
    		LookAhead()=='*' ||
    		LookAhead()=='/' ||
    		LookAhead()==')' ||
    		LookAhead()=='$'
    		)
    	{
    		// N'-> e
    		returnValue.value=0.0;
    		returnValue.magnitude=1.0;
		}
    	else
    		throw illegal_input();
    	return returnValue;
    }
	double N(){
		double digitValue,value;
		number num;
		if (LookAhead()>='0' && LookAhead()<='9'){
			digitValue=D();
			num=N_();
			// N-> DN'
    		value=digitValue*num.magnitude+num.value;
		}
		else
			throw illegal_input();
		return value;
    }
	double F();
	double T_(){
		double value;
		char c;
		if (LookAhead()=='*' || LookAhead()=='/'){
			c=LookAhead();
			Match(c);
			value=F();
			if (c=='/')
				value=1/value;
			cout<<c;
			value*=T_();
		}// T'->*F {print('*')} T' �s T'->/F {print('*')} T'
		else
		if (LookAhead()=='+' || LookAhead()=='-' ||
			LookAhead()==')' || LookAhead()=='$'){
			// T'-> e
			value=1.0;
		}
		else
			throw illegal_input();
		return value;
	}
	double T(){
		double value;
		if (LookAhead()>='0' && LookAhead()<='9'){
			value=F();
			value*=T_();
			// T->FT'
		}
		else
		if (LookAhead()=='('){
			value=F();
			value*=T_();
			// T->FT'
		}
		else if (LookAhead(4) == "sin(" || LookAhead(4) == "cos(")
		{
			value=F();
			value*=T_();
			// T->FT'
		}
		else
			throw illegal_input();
		return value;
	}
	double E_(){
		double value;
		char c;
		if (LookAhead()=='+' || LookAhead()=='-'){
			c=LookAhead();
			Match(c);
			value=T();
			cout<<c;
			value+=E_();
			if (c=='-')
				value*=-1.0;
		}// E'->+T {print('+')} E' �s E'->-T {print('+')} E'
		else
		if (LookAhead()==')' || LookAhead()=='$'){
			// E'-> e
			value=0.0;
		}
		else
			throw illegal_input();
		return value;
	}
	double E(){
		double value;
		if (LookAhead()>='0' && LookAhead()<='9'){
			value=T();
			value+=E_();
			// E->TE'
		}
		else
		if (LookAhead()=='('){
			value=T();
			value+=E_();
			// E->TE'
		}
		else if (LookAhead(4) == "sin(" || LookAhead(4) == "cos(")
		{
			value=T();
			value+=E_();
			// E->TE'
		}
		else
			throw illegal_input();
		return value;
	}
  public:
    void Parse(const string &s){
      input=s+'$';
      pos=0;
      cout<<"posfix form: ";
      double value=E();
      cout<<endl<<"value: "<<value;
      Match('$');
    }
};

double parser::F(){
	double value;
	if (LookAhead()>='0' && LookAhead()<='9'){
		value=N();
		// F->N
	}
	else
	if (LookAhead()=='('){
		Match('(');
		value=E();
		Match(')');
		// F->(E)
	}
	else if (LookAhead(4) == "sin(" || LookAhead(4) == "cos(")
	{
		string m = LookAhead(4);
		Match(m);
		double angle = E();
		double radians = angle * M_PI / 180.0;
		if (m == "sin("){
			value = sin(radians);
		}
		else {
			value = cos(radians);
		}
		Match(')');
		// F->(E)
	}
	else
		throw illegal_input();
	return value;
}


int main(){
  string s;
  cin>>s;
  try{
    parser().Parse(s);
    cout<<"\nAccepted.\n";
  }
  catch(illegal_input){
    cout<<"\nNot accepted.\n";;
  }
  return 0;
}

// ---------------- Accepted ----------------
/*
Input:
	sin(0)
Output:
	posfix form: 0
	value: 0
	Accepted.


Input:
	sin(30)+cos(60)
Output:
	posfix form: 3060+
	value: 1
	Accepted.

Input:
	10*(sin(90)+cos(90/(2+2)))
Output:
	posfix form: 10909022+/+*
	value: 19.2388
	Accepted.
*/

// ---------------- Not accepted ----------------

/*
Input:
	sin30
Output:
	posfix form: 
	Not accepted.


Input:
	sin(30+cos(60)
Output:
	posfix form: 3060+
	Not accepted.

Input:
	2*cos(90+)
Output:
	posfix form: 290
	Not accepted.
*/
