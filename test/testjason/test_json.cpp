#include "json.hpp"
using json = nlohmann::json;
#include <iostream>
#include <vector>
#include <map>
#include <string>
using namespace std;
string func1()
{
    json js; // 直接序列化一个vector容器
    vector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(5);
    js["list"] = vec; // 直接序列化一个map容器
    map<int, string> m;
    m.insert({1, "黄山"});
    m.insert({2, "华山"});
    m.insert({3, "泰山"});
    js["path"] = m;
    string sendbuf = js.dump(); //将json数据对象转换为=》序列化json字符串
    return sendbuf;
    //cout << sendbuf << endl;
}
int main()
{
    string recvbuf = func1();
    json jsbuf = json::parse(recvbuf);
    cout << jsbuf["path"] << endl;
    cout << jsbuf["path"] << endl;
    cout << jsbuf["list"] << endl;
    cout << jsbuf["list"] << endl;
    return 0;
}