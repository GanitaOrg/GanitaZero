// Schema class for GanitaZero

#include <string>
#include <vector>

class GanitaZeroSchema
{
private:
  std::vector<std::string> gzst;
  std::vector<std::string> gzstr;
public:
  GanitaZeroSchema(void);
  std::string returnSchema(int index);
};

