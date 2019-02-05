#ifndef FILEPARSER_HPP
#define FILEPARSER_HPP


class FileParser
{
public:
    FileParser();

    bool load_file(const char* filename);

    void shell_mode();

private:
    void clear_line(std::string& line);

};

#endif // FILEPARSER_HPP
