
class KeywordTable
{
public:
	enum Keyword
	{
		K_IF,
		K_WHILE,
		K_LOAD, 
		K_EXIT,
		K_END,
	};
	
	
}

class Token
{
...
private:
	struct KeywordJump
	{
		int line;
		KeywordTable::Keyword keyword;
	};
}

tokens = tokenization();
instructions = []
std::stack<int> last_;

add_instruction(tokens)
{
	int current = instructions.size() - 1;
	if (tokens[0].type() == Token::KEYWORD)
	{
		switch (tokens[0].get_keyword())
		{
			case KeywordTable::K_IF:
			case KeywordTable::K_WHILE:
				last_.push(current);
				break;
			
		
		
		
Instruction
	statement -> std::list<Token>
	while -> std::list<Token>, end
	if -> std::list<Token>, end
	
Eval
	current = postfix.pop()
	if current.type = keyword
		switch keyword
			case k_if
				if !eval(postfix)
					r
				

function 
	x += speed * frametime;
	y = sin(x % 2 * PI)
	