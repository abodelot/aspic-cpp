#include <string>
#include <stack>
#include <fstream>
#include <iostream>
#include <sstream>
#include <limits>

std::string str_trim(const std::string& str)
{
	const char* WHITESPACES = " \t\n\r\0xb";
	std::string::size_type first = str.find_first_not_of(WHITESPACES);
	if (first != std::string::npos)
	{
		std::string::size_type last = str.find_last_not_of(WHITESPACES);
		return str.substr(first, last - first + 1);
	}
	return str;
}

typedef std::string Instruction;

class Statement
{
public:
	enum Type
	{
		EXPRESSION,
		CONDITION,
		WHILE,
		ELSE,
		DUMMY
	};

	Statement(Type, Instruction&);
	void set_next(Statement* statement);
	void set_next_on_failure(Statement* statement);
	Statement* execute();

	void bypass();

	inline Statement* get_next() { return next_; }
	inline Type get_type() { return type_; }

	inline bool is_condition() const { return type_ == CONDITION || type_ == WHILE || type_ == ELSE; }
private:
	Type type_;
	Instruction instruction_;

	Statement* next_;
	Statement* next_on_failure_;
	bool bypass_condition_;
};


Statement::Statement(Type type, Instruction& instruction)
{
	instruction_ = instruction;
	next_ = NULL;
 	next_on_failure_ = NULL;
	bypass_condition_ = false;
	type_ = type;
}


void Statement::set_next(Statement* statement)
{
	next_ = statement;
}

void Statement::set_next_on_failure(Statement* statement)
{
	next_on_failure_ = statement;
}

Statement* Statement::execute()
{
	switch (type_)
	{
		case EXPRESSION:
			std::cout << instruction_ << std::endl;
			return next_;
		case CONDITION:
		case WHILE:
			if (!bypass_condition_)
			{
				std::cout << "[condition] " << instruction_ << std::endl;
				int input = 0;
				std::cout << ">> ";
				std::cin >> input;
				std::cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );

				if (!input)
					return next_on_failure_;

				if (next_on_failure_->is_condition())
					next_on_failure_->bypass();

				return next_;
			}
			bypass_condition_ = false;
			return next_on_failure_;
		case DUMMY:
			return next_;
		case ELSE:
			if (bypass_condition_)
			{
				bypass_condition_ = false;
				return next_on_failure_;
			}
			return next_;
	}
	return NULL;
}


void Statement::bypass()
{
	bypass_condition_ = true;
	if (next_on_failure_->is_condition())
		next_on_failure_->bypass();
}





class Parser
{
public:
	Parser()
	{
		start_ = NULL;

	}

	void parse(const std::string& filename)
	{


		std::ifstream file(filename.c_str());
		if (file)
		{

			std::stack<Statement*> stack;
			Statement* previous = NULL;
			Statement* current = NULL;

			std::string line;
			while (getline(file, line))
			{
				current = NULL;
				line = str_trim(line);

				std::istringstream iss(line);
				std::string first;
				iss >> first;

				if (first == "if")
				{
					current = new Statement(Statement::CONDITION, line);
					stack.push(current);
				}
				else if (first == "elif")
				{
					current = new Statement(Statement::CONDITION, line);
					Statement* last = stack.top();
					stack.pop();
					last->set_next_on_failure(current);
					stack.push(current);
				}
				else if (first == "else")
				{
					current = new Statement(Statement::ELSE, line);
					Statement* last = stack.top();
					stack.pop();
					last->set_next_on_failure(current);
					stack.push(current);
				}
				else if (first == "end")
				{
					current = new Statement(Statement::DUMMY, line);
					Statement* last = stack.top();
					stack.pop();
					last->set_next_on_failure(current);
					if (last->get_type() == Statement::WHILE)
						previous->set_next(last);
				}
				else if (first == "while")
				{
					current = new Statement(Statement::WHILE, line);
					stack.push(current);
				}
				else
				{
					current = new Statement(Statement::EXPRESSION, line);
				}


				if (start_ == NULL)
					start_ = current;

				if (previous != NULL && previous->get_next() == NULL)
					previous->set_next(current);

				previous = current;
			}
		}
	}

	void run()
	{
		Statement* i = start_->execute();
		while (i != NULL)
		{
			i = i->execute();
		}
	}

private:
	Statement* start_;

};

int main(int argc, char* argv[])
{
	Parser p;
	p.parse(argv[1]);
	p.run();
}
