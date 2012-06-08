#include <string>
#include <stack>
#include <fstream>
#include <iostream>
#include <sstream>
#include <limits>
#include <cassert>

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
		END_OF_FILE

	};

	static Statement* EndOfFile()
	{
		static Instruction empty;
		Statement* self = new Statement(END_OF_FILE, empty);
		return self;
	}

	Statement(Type, Instruction&);
	void set_next(Statement* statement);
	void set_next_on_failure(Statement* statement);
	Statement* execute();

	void bypass();

	inline Statement* get_next() { return next_; }
	inline Type get_type() { return type_; }
	inline const Instruction& get_instruction() const { return instruction_; }
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
		case ELSE:
			if (bypass_condition_)
			{
				bypass_condition_ = false;
				return next_on_failure_;
			}
			return next_;
		case END_OF_FILE:
			return NULL;
	}
	return NULL;
}


void Statement::bypass()
{
	bypass_condition_ = true;
	if (next_on_failure_ == NULL)
	{
		printf("pas de sortie : %s\n", instruction_.c_str());
	}
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
			Statement* temp = NULL;
			bool end_of_scope = false;
			bool skip = false;
			std::string line;
			next_statement:
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
					end_of_scope = true;
					temp = stack.top();
					stack.pop();

					goto next_statement;
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

				printf("current = %s, (end of scope = %d)\n", current->get_instruction().c_str(), (int) end_of_scope);
				if (end_of_scope)
				{
				puts("!!!!!");
					assert(temp != NULL);
					printf("* end of scope, temp is : %s\n", temp->get_instruction().c_str());
					temp->set_next_on_failure(current);
					if (temp->get_type() == Statement::WHILE)
						previous->set_next(temp);
					temp = NULL;
					end_of_scope = false;
				}

				previous = current;


			}
			if (temp != NULL)
			{
				Statement* end  = Statement::EndOfFile();
				temp->set_next_on_failure(end);
				if (temp->get_type() == Statement::WHILE)
					previous->set_next(temp);
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
