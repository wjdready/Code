#include "thread.h"
#include <algorithm>
#include <stdio.h>
#include <vector>
#include <string>

using namespace std;

class Good
{
  private:
  string name;
  public:
    Good(const char *name);
    void sayHello();
    ~Good();
};

Good::Good(const char *name)
{
    this->name = name;
}

void Good::sayHello()
{
    string text = "Hello I am " + name + ", I am good\n";
    
    printf("%s\n", text.c_str());
    
    vector<int> obj;

    obj.push_back(1);
    obj.push_back(3);
    obj.push_back(0);

    sort(obj.begin(), obj.end()); // 从小到大

    printf("从小到大:\n");
    for (int i = 0; i < obj.size(); i++)
    {
        printf("%d,", obj[i]);
    }

    printf("\n");

    printf("从大到小:\n");
    reverse(obj.begin(), obj.end()); // 从大到小
    for (int i = 0; i < obj.size(); i++)
    {
        printf("%d,", obj[i]);
    }
    printf("\n");
}

Good::~Good()
{
}

extern "C"
{
    void demo(void)
    {
        printf("Hello Demo\n");
        Good g("Jim Weiss");
        g.sayHello();

        Good *pg = new Good("Zhang San");
        pg->sayHello();
        delete pg;
    }
}
