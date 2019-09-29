#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <Windows.h>
using namespace std;
bool iszhixiashi = false;
string str;//地址信息
string zhixiashi[4] = { "北京","上海","重庆","天津" };
ofstream out("out.txt");

string GbkToUtf8(const char* src_str)
{
	int len = MultiByteToWideChar(CP_ACP, 0, src_str, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, src_str, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	string strTemp = str;
	if (wstr) delete[] wstr;
	if (str) delete[] str;
	return strTemp;
}
//UTF-8转换到GBK
string Utf8ToGbk(const char* src_str)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, src_str, -1, NULL, 0);
	wchar_t* wszGBK = new wchar_t[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, src_str, -1, wszGBK, len);
	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char* szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
	string strTemp(szGBK);
	if (wszGBK) delete[] wszGBK;
	if (szGBK) delete[] szGBK;
	return strTemp;
}
string helper(string key)
{
	string address;
	for (unsigned int i = 0; i < str.length(); i++) {
		if (str[i] == key[0] && str[i + 1] == key[1]) {//说明此时地址包括“省/市/县”这个字
			int index = i + 1;
			for (int i = 0; i <= index; i++) {
				address += str[i];
			}
			str.erase(0, index + 1);
			return address;
		}
	}
	return "";
}
class Info
{
private:
	string Name;
	string  PhoneNum;
	string province;
	string city;
	string county;
	string town;
	string road;
	string housenum;
	string detail;

public: string SetName(string str);
		string SetPhoneNum(string str);
		string GetName();
		string GetPhoneNum();
		string GetProvince();
		string GetCity();
		string GetCounty();
		string GetTown();
		string GetRoad();
		string GetHouseNum();
		string GetDetail();
		void isShenhui();
		void SetProvince();
		void SetCity();
		void SetCounty();
		void SetTown();
		void SetRoad();
		void SetHouseNum();
		void SetDetail();

};
void Info::SetCounty()
{
	bool flag = false;
	string key[2] = { "县","区" };
	string address;
	for (unsigned int i = 0; i < str.length(); i++) {
		if ((str[i] == key[0][0] && str[i + 1] == key[0][1]) || (str[i] == key[1][0] && str[i + 1] == key[1][1])) {//说明此时地址包括"县/区”这个字
			flag = true;
			int index = i + 1;
			for (int i = 0; i <= index; i++) {
				address += str[i];
			}

			str.erase(0, index + 1);
			break;
		}

	}if (flag) {
		this->county = address;
	}
	else {
		this->county = "";
	}

}
void Info::isShenhui()
{
	string province;
	province += str[0];
	province += str[1];
	province += str[2];
	province += str[3];
	//cout << province << endl;
	if (province == zhixiashi[0] || province == zhixiashi[1] || province == zhixiashi[2] || province == zhixiashi[3]) {
		iszhixiashi = true;
	}
}
void Info::SetProvince()
{
	if (iszhixiashi) {  //此时为直辖市
		bool flag = false;
		string key = "市";
		string province;
		province += str[0];
		province += str[1];
		province += str[2];
		province += str[3];
		this->province = province;
		this->province += key;
		for (unsigned int i = 0; i < str.length(); i++) {
			if (str[i] == key[0] && str[i + 1] == key[1]) {//说明此时地址包括“市”这个字
				str.erase(0, i + 2);
				flag = true;
				break;
			}
		}
		if (!flag) {
			str.erase(0, 4);
		}
	}
	else {
		string key = "省";
		string province;
		int f = 1;
		for (unsigned int i = 0; i < str.length(); i++) {
			if (str[i] == key[0] && str[i + 1] == key[1]) {//说明此时地址包括“省”这个字
				f = 0;
				int index = i + 1;
				for (int i = 0; i <= index; i++) {
					province += str[i];
				}
				this->province = province;
				str.erase(0, index + 1);
				break;
			}
		}
		if (f) {
			for (unsigned int i = 0; i < 4; i++)province += str[i];
			this->province = province;
			this->province += key;
			str.erase(0, 4);
		}
	}
}
void Info::SetTown()
{
	bool flag = false;
	string key[4] = { "镇","乡","街","道" };
	string address;
	for (unsigned int i = 0; i < str.length(); i++) {
		if ((str[i] == key[0][0] && str[i + 1] == key[0][1]) || (str[i] == key[1][0] && str[i + 1] == key[1][1])) {//说明此时地址包括"乡/镇”这个字
			flag = true;
			int index = i + 1;
			for (int i = 0; i <= index; i++) {
				address += str[i];
			}

			str.erase(0, index + 1);
			break;
		}
		else if (str[i] == key[2][0] && str[i + 1] == key[2][1]) {  //匹配街道
			if (str[i + 2] == key[3][0] && str[i + 3] == key[3][1]) {
				flag = true;
				int index = i + 3;
				for (int i = 0; i <= index; i++) {
					address += str[i];
				}

				str.erase(0, index + 1);
				break;
			}
		}
	}	if (flag) {
		this->town = address;
	}
	else {
		this->town = "";
	}
}
void Info::SetCity()
{
	string key = "市";
	if (iszhixiashi) {  //直辖市直接传值
		this->city = this->province;
		if (sizeof(this->city) < 6)this->city += key;
	}
	else {
		string city;
		int f = 1;
		for (unsigned int i = 0; i < str.length(); i++) {
			if (str[i] == key[0] && str[i + 1] == key[1]) {//说明此时地址包括“市”这个字
				f = 0;
				int index = i + 1;
				for (int i = 0; i <= index; i++) {
					city += str[i];
				}
				this->city = city;
				str.erase(0, index + 1);
				break;
			}
		}
		if (f) {
			for (unsigned int i = 0; i < 4; i++)
				city += str[i];
			this->city = city;
			this->city += key;
			str.erase(0, 4);
		}
	}
}
void Info::SetDetail()
{
	string det;
	for (unsigned int i = 0; i < str.length() - 1; i++) {
		det += str[i];
	}
	this->detail = det;
	;
}
void Info::SetRoad()
{
	string key = "路";
	this->road = helper(key);
}
void Info::SetHouseNum()
{
	string key = "号";
	this->housenum = helper(key);
}
string Info::GetProvince()
{
	return province;
}
string Info::GetCity()
{
	return city;
}
string Info::GetCounty()
{
	return county;
}
string Info::GetTown()
{
	return town;
}
string Info::GetRoad()
{
	return road;
}
string Info::GetHouseNum()
{
	return housenum;
}
string Info::GetDetail()
{

	return detail;
}
string Info::SetName(string str)
{
	string name;
	int count = 0;
	for (unsigned int i = 0; i < str.length() - 1; i++)
	{
		if (str[i] == ',') break;
		else {
			count++;
			name += str[i];
		}
	}
	this->Name = name;
	str.erase(0, count + 1);
	return str;
}
string Info::SetPhoneNum(string str)
{
	int count = 0;
	int index = 0;
	int flag = 0;//退出判断
	string phone;
	for (unsigned int i = 0; i < str.length(); i++)
	{
		if (str[i] >= '0' && str[i] <= '9' && str[i + 1] >= '0' && str[i + 1] <= '9')
		{
			count++;
			if (count == 1) {//记录第一个数字的下标
				index = i;
			}
			if (count >= 10) {   //判断出是电话，则从第一个数字开始遍历
				count = 11;
				/*提取出电话号码*/
				for (int j = index;; j++)
				{
					phone += str[j];
					count--;
					if (count == 0) break;
				}
				this->PhoneNum = phone;
				str.erase(index, 11);
			}
		}
		else
		{
			count = 0;
			index = 0;
		}
	}
	return str;
}
string Info::GetName()
{
	return this->Name;
}
string Info::GetPhoneNum()
{
	return this->PhoneNum;
}
int SetLevel()
{
	int x;
	x = str[0];
	x -= 48;
	if (x)
		return x;
	return 0;
}
Info disposeLevel1(Info info)
{
	info.SetDetail();  //一级地址只需要设置详细地址
	return info;
}
Info disposeLevel2(Info info)
{
	//二级地址需要设置道路信息，门牌号，详细信息
	info.SetRoad();
	info.SetHouseNum();
	info.SetDetail();
	return info;
}
Info dispose(Info info)
{
	int Level;//提取题目难度
	Level = SetLevel();
	str.erase(0, 2);//去除题目难度的字符串
	str = info.SetName(str);//提取name
	str = info.SetPhoneNum(str);//提取电话
	info.isShenhui();
	info.SetProvince();
	info.SetCity();
	info.SetCounty();
	info.SetTown();

	if (Level == 1) {
		info = disposeLevel1(info);
		//cout << info.detail<< endl;
	}
	else if (Level == 2) {
		info = disposeLevel2(info);
		//cout << info.detail << endl;
	}
	return info;
}
void output(Info info, int level) {

	out << "{" << '"' << "姓名" << '"' << ':' << '"' << info.GetName() << '"' << ',' << endl;
	out << '"' << "手机" << '"' << ':' << '"' << info.GetPhoneNum() << '"' << ',' << endl;
	out << '"' << "地址" << '"' << ':';
	out << '[' << endl;
	out << '"' << info.GetProvince() << '"' << ',' << endl;
	out << '"' << info.GetCity() << '"' << ',' << endl;
	out << '"' << info.GetCounty() << '"' << ',' << endl;
	out << '"' << info.GetTown() << '"' << ',' << endl;
	if (level == 1)
	{
		out << '"';
		out << info.GetDetail();
		out << '"' << endl;
		out << ']' << endl;

	}
	if (level == 2 || level == 3)
	{
		out << '"' << info.GetRoad() << '"' << ',' << endl;
		out << '"' << info.GetHouseNum() << '"' << ',' << endl;;
		out << '"' << info.GetDetail() << '"' << endl;
		out << ']' << endl;
	}
	out << "}," << endl;
}

int main(int argc, char *argv[])//
{
	int t = 0;
	string str1;
	ifstream in(argv[1]);
	const char *a;
	int level;

	out << "[" << endl;

	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}
	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}
	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}
	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}
	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}

	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}
	while (!in.eof())
	{
		getline(in, str); if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}
	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}
	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}    	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}    	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}    	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}
	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}	while (!in.eof())
	{
		getline(in, str);
		if (str[0] == 0)break;
		a = str.c_str();
		str = Utf8ToGbk(a);
		//cout<<str<<endl; 
		level = SetLevel();
		Info info;
		iszhixiashi = false;
		info = dispose(info);
		output(info, level);
		break;
	}
	in.close();
	out << "]" << endl;
	out.close();
	ifstream c("out.txt");//
	ofstream s(argv[2]);
	while (!c.eof()) {

		getline(c, str1);
		a = str1.c_str();
		str1 = GbkToUtf8(a);
		s << str1 << endl;
	}
	c.close();
	s.close();
	getchar();
	system("pause");
	return 0;

}

