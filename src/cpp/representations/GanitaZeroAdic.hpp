#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

class GanitaZeroAdic
{
private:
  std::string name;
  std::vector<std::string> type;
  std::vector<std::string> keys;
  std::vector<std::vector<std::string>> values;
public:
  GanitaZeroAdic(void);
  int readAdic(std::ifstream &gzt_file);
};

