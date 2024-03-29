#include <iostream>
#include "record.h"
#include "date_time.h"
#include "node.h"
#include "function.h"
#include "MD5.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <time.h>
using namespace std;
record records[100000];
int k = 0;//总的记录数
void judge_permission()
{
	string str, password_MD5ed, result;
	ifstream infile;
	infile.open("password.dat", ios::in);
	infile >> password_MD5ed;
	infile.close();
	do
	{
		cout << "请输入管理员密码:";
		cin >> str;
		MD5 md5(str);
		result = md5.md5();
		cout << "密码错误\n";
	} while (result != password_MD5ed);
	cls();
}
void passwd_root()//取名自Linux的更改密码操作
{
	cls();
	string str, password_MD5ed, result;
	ifstream infile;
	infile.open("password.dat", ios::in);
	infile >> password_MD5ed;
	infile.close();
	do
	{
		cout << "请输入原来的管理员密码:";
		cin >> str;
		MD5 md5(str);
		result = md5.md5();
		cout << "密码错误\n";
	} while (result != password_MD5ed);
	cls();
	string psd1, psd2;
	do
	{
		cls();
		cout << "请输入新的密码:";
		cin >> psd1;
		MD5 md5_1(psd1);
		psd1 = md5_1.md5();
		cout << "请再输一次:";
		cin >> psd2;
		MD5 md5_2(psd2);
		psd2 = md5_2.md5();
		if (psd1 != psd2)
		{
			cout << "两次输入的密码不一致\n";
			pause();
		}
	} while (psd1 != psd2);
	cls();
	cout << "确定要修改密码吗？(y/n)";
	string op;
	cin >> op;
	if (op[0] == 'y' || op[0] == 'Y')
	{
		ofstream outfile;
		outfile.open("password.dat", ios::out);
		outfile << psd1;
		outfile.close();
		cls();
		cout << "密码修改成功\n";
		pause();
	}
	else
	{
		cout << "密码未修改\n";
		pause();
	}
}
void edit_num(int n)
{
	string num_building, num_house;
	cout << "请输入新的楼号与房号:";
	cin >> num_building;
	while (!is_proper_num(num_building))
	{
		cout << "楼号输入有误，请重新输入:";
		cin.ignore(100, '\n');//清空cin流
		cin >> num_building;
	}
	cin >> num_house;
	while (!is_proper_num(num_house))
	{
		cout << "房号输入有误，请重新输入:";
		cin.ignore(100, '\n');//清空cin流
		cin >> num_house;
	}
	records[n].edit_num(to_int(num_building), to_int(num_house));
}
void edit_date(int n)
{
	string day_later;
	cout << "请输入新的预约时间:（输入一个小于10的数n，表示n天后）";
	cin >> day_later;
	while (!is_proper_num(day_later))
	{
		cout << "输入有误，请重新输入";
		cin.ignore(100, '\n');//清空cin流
		cin >> day_later;
	}
	if (to_int(day_later) < 10)
	{
		date date_tmp = today();
		for (int x = 0; x < to_int(day_later); x++)
		{
			date_tmp = date_tmp.tomorrow();
		}
		records[n].edit_date_reserve(date_tmp);
	}
	else
	{
		int year = to_int(day_later);
		string month, day;
		cin >> month;
		while (!is_proper_num(month))
		{
			cout << "月份输入有误，请重新输入:";
			cin.ignore(100, '\n');//清空cin流
			cin >> month;
		}
		cin >> day;
		while (!is_proper_num(day))
		{
			cout << "日期输入有误，请重新输入:";
			cin.ignore(100, '\n');//清空cin流
			cin >> day;
		}
		records[n].edit_date_reserve(date(year, to_int(month), to_int(day)));
	}
}
void edit_content(int n)
{
	string content;
	cout << "请输入维修内容： ";
	cin >> content;
	records[n].edit_content(content);
}
void edit_money(int n)
{
	string money_income, money_outcome;
	cout << "请输入收费和成本： ";
	cin >> money_income;
	while (!is_proper_num(money_income))
	{
		cout << "收费输入有误，请重新输入:";
		cin.ignore(100, '\n');//清空cin流
		cin >> money_income;
	}
	cin >> money_outcome;
	while (!is_proper_num(money_outcome))
	{
		cout << "成本输入有误，请重新输入:";
		cin.ignore(100, '\n');//清空cin流
		cin >> money_outcome;
	}
	records[n].edit_money(to_double(money_income), to_double(money_outcome));
}
void edit_people(int n)
{
	string people;
	cout << "请输入检修人： ";
	cin >> people;
	records[n].edit_people(people);
}
void edit_remark(int n)
{
	string remark;
	cout << "请输入备注： ";
	cin >> remark;
	records[n].edit_remark(remark);
}
void edit_all(int n)
{
	edit_num(n);
	edit_date(n);
	edit_content(n);
	edit_money(n);
	edit_people(n);
	edit_remark(n);
}
void read_from_file()
{
	k = 0;
	int num_building, num_house; //楼号，房号
	date date_reserve, date_fact; //预约时间，实际时间
	string content; //维修内容
	double money_income, money_outcome; //收费，成本
	string people; //检修人
	string remark; //备注	
	int done;
	ifstream infile;
	infile.open("database.dat", ios::in);
	while (infile >> num_building >> num_house >> date_reserve >> date_fact >> content >> money_income >> money_outcome >> people >> remark >> done)
	{
		records[k].set(num_building, num_house, date_reserve, date_fact, content, money_income, money_outcome, people, remark, done);
		k++;
	}
	infile.close();
}
bool cmp(record& a, record& b)
{
	return a < b;
}
void save_all()
{
	cout << "正在保存文件，请耐心等待\n";
	sort(records, records + k, cmp);
	ofstream outfile;
	outfile.open("database.dat", ios::out);
	for (int x = 0; x < k; x++)
	{
		outfile << records[x].show_in_line();
	}
	outfile.close();
	read_from_file();
}
void activity_edit(int n)
{
	string op;
	cls();
	records[n].show(n);
	cout << "\n请选择你要修改的项目\n";
	cout << "1.楼房号码\n2.预约时间\n3.维修内容\n4.收费与成本\n5.检修人\n6.备注\n7.全部\n8.退出\n";
	cout << "请一次性输入所有要修改的项目，如“135”\n";
	cin >> op;
	bool flag[7];
	for (int x = 0; x < 7; x++)
	{
		flag[x] = true;
	}
	for (int x = 0; x < op.length(); x++)
	{
		if (flag[op[x] - '0'])
		{
			cls();
			flag[op[x] - '0'] = false;
			records[n].show(n);
			switch (op[x])
			{
			case '1':edit_num(n); break;
			case '2':edit_date(n); break;
			case '3':edit_content(n); break;
			case '4':edit_money(n); break;
			case '5':edit_people(n); break;
			case '6':edit_remark(n); break;
			case '7':edit_all(n); break;
			}
		}
	}
	cls();
	records[n].show(n);
	save_all();
	cout << "操作成功完成\n";
	pause();
}
void activity_delete(int n)
{
	cout << "这个操作不可逆，确认删除这条记录吗？(y/n)\n";
	string yesno;
	cin >> yesno;
	if (yesno[0] != 'y')
	{
		cout << "操作已取消\n";
		pause();
	}
	else
	{
		for (int x = n; x < k - 1; x++)
		{
			records[x] = records[x + 1]; 
		}
		k--;
		save_all();
		cout << "操作已成功完成\n";
		pause();
	}
}
void activity_finish(int n)
{
	string day_later;
	date date_tmp = today();
	cout << "请输入时间:（输入一个小于10的数n，表示n天后）";
	cin >> day_later;
	while (!is_proper_num(day_later))
	{
		cout << "输入有误，请重新输入";
		cin.ignore(100, '\n');//清空cin流
		cin >> day_later;
	}
	if (to_int(day_later) < 10)
	{
		for (int x = 0; x < to_int(day_later); x++)
		{
			date_tmp = date_tmp.tomorrow();
		}
	}
	else
	{
		int year = to_int(day_later);
		string month, day;
		cin >> month;
		while (!is_proper_num(month))
		{
			cout << "月份输入有误，请重新输入:";
			cin.ignore(100, '\n');//清空cin流
			cin >> month;
		}
		cin >> day;
		while (!is_proper_num(day))
		{
			cout << "日期输入有误，请重新输入:";
			cin.ignore(100, '\n');//清空cin流
			cin >> day;
		}
		date_tmp = date(year, to_int(month), to_int(day));
	}
	string c;
	cout << "确定完成操作了？(y/n)  ";
	cin >> c;
	if (c[0] == 'y')
	{
		records[n].edit_date_fact(date_tmp);
		records[n].edit_done();
	}
	save_all();
	cls();
	records[n].show(n);
	cout << "\n操作已成功完成\n";
	pause();
}
int operate() //操作起来！
{
	int n;
	string n_to_judge;
	string t;
	cout << "请输入你要操作的ID：输入-1退出\n";
	cin >> n_to_judge;
	while (!is_proper_num(n_to_judge))
	{
		cout << "输入错误，请重新输入：";
		cin >> n_to_judge;
	}
	n = to_int(n_to_judge);
	if (n == -1)
	{
		return 0;
	}
	if (n >= k || n < 0)
	{
		cout << "输入错误\n";
		pause();
		return 1;
	}
	cout << "1.删除记录   2.修改记录  3.完工登记\n";
	cout << "请输入你要进行的操作： ";
	cin >> t;
	switch (t[0])
	{
	case '1':activity_delete(n); break;
	case '2':activity_edit(n); break;
	case '3':activity_finish(n); break;
	}
	return 0;
}
void new_record()
{
	cls();
	string num_building, num_house; //楼号，房号
	date date_reserve, date_fact; //预约时间，实际时间
	string content; //维修内容
	string money_income, money_outcome; //收费，成本
	string people; //检修人
	string remark; //备注	
	cout << "请输入楼号和房号\n";
	cin >> num_building;
	while (!is_proper_num(num_building))
	{
		cout << "楼号输入有误，请重新输入:";
		cin.ignore(100, '\n');//清空cin流
		cin >> num_building;
	}
	cin >> num_house;
	while (!is_proper_num(num_house))
	{
		cout << "房号输入有误，请重新输入:";
		cin.ignore(100, '\n');//清空cin流
		cin >> num_house;
	}
	string day_later;
	cout << "请输入预约时间:（输入一个小于10的数n，表示n天后）";
	cin >> day_later;
	while (!is_proper_num(day_later))
	{
		cout << "输入有误，请重新输入";
		cin.ignore(100, '\n');//清空cin流
		cin >> day_later;
	}
	if (to_int(day_later) < 10)
	{
		date date_tmp = today();
		for (int x = 0; x < to_int(day_later); x++)
		{
			date_tmp = date_tmp.tomorrow();
		}
		date_reserve = date_tmp;
	}
	else
	{
		int year = to_int(day_later);
		string month, day;
		cin >> month;
		while (!is_proper_num(month))
		{
			cout << "月份输入有误，请重新输入:";
			cin.ignore(100, '\n');//清空cin流
			cin >> month;
		}
		cin >> day;
		while (!is_proper_num(day))
		{
			cout << "日期输入有误，请重新输入:";
			cin.ignore(100, '\n');//清空cin流
			cin >> day;
		}
		date_reserve = date(year, to_int(month), to_int(day));
	}
	cout << "请输入维修内容\n";
	cin >> content;
	cout << "请输入收费与成本\n";
	cin >> money_income;
	while (!is_proper_num(money_income))
	{
		cout << "收费输入有误，请重新输入:";
		cin.ignore(100, '\n');//清空cin流
		cin >> money_income;
	}
	cin >> money_outcome;
	while (!is_proper_num(money_outcome))
	{
		cout << "成本输入有误，请重新输入:";
		cin.ignore(100, '\n');//清空cin流
		cin >> money_outcome;
	}
	cout << "请分配检修员:\n";
	cin >> people;
	cout << "请输入备注:\n";
	cin >> remark;
	records[k] = record(to_int(num_building), to_int(num_house), date_reserve, date_fact, content, to_double(money_income), to_double(money_outcome), people, remark);
	cls();
	cout << "保存成功!记录如下：\n";
	records[k].show();
	k++;
	save_all();
	read_from_file();
	pause();
}
void search_record()
{
	cls();
	string key;
	cout << "请输入你要查找的关键词，系统会自动进行全局匹配\n";
	cin >> key;
	cls();
	int n = 0;
	for (int x = 0; x < k; x++)
	{
		if (records[x].exist(key))
		{
			records[x].show(x);
			n++;
			if (!((n) % 2)) pause();
		}
	}
	if (n != 0)
		operate();
	else
	{
		cout << "没有符合要求的记录\n";
		pause();
	}
}
void find_all_record()
{
	cls();
	for (int x = 0; x < k; x++)
	{
		records[x].show(x);
		cout << '\n';
		if (!((x + 1) % 2)) pause();
	}
	if (k != 0)
		operate();
	else
	{
		cout << "没有符合要求的记录\n";
		pause();
	}
}
void fix_unfinished()
{
	cls();
	int n = 0;
	for (int x = 0; x < k; x++)
	{
		if (records[x].unfinished())
		{
			n++;
			records[x].show(x);
			cout << '\n';
			if (!(n % 2)) pause();
		}
	}
	if (n != 0)
		operate();
	else
	{
		cout << "没有符合要求的记录\n";
		pause();
	}
}
void fix_overdate()
{
	cls();
	int n = 0;
	for (int x = 0; x < k; x++)
	{
		if (records[x].unfinished() && records[x].overdate(today()))
		{
			records[x].show(x);
			cout << '\n';
			n++;
			if (!(n % 2)) pause();
		}
	}
	if (n != 0)
		operate();
	else
	{
		cout << "没有符合要求的记录\n";
		pause();
	}
}
void fix_two_days()
{
	cls();
	int n = 0;
	for (int x = 0; x < k; x++)
	{
		if (records[x].unfinished() && !records[x].overdate(today()) && records[x].overdate(today().tomorrow().tomorrow().tomorrow()))
		{
			n++;
			records[x].show(x);
			cout << '\n';
			if (!(n % 2)) pause();
		}
	}
	if (n != 0)
		operate();
	else
	{
		cout << "没有符合要求的记录\n";
		pause();
	}
}
void intime_service()
{
	cls();
	cout << "1.显示所有未完成的维修\n";
	cout << "2.显示超期未完成的维修\n";
	cout << "3.显示两天内需要完成的维修\n";
	cout << "请输入你要显示的内容\n";
	string t_todo;
	cin >> t_todo;
	char t = t_todo[0];
	switch (t)
	{
	case '1':fix_unfinished(); break;
	case '2':fix_overdate(); break;
	case '3':fix_two_days(); break;
	}
}
void statistic_income_date()
{
	cout << "请输入你要统计的日期，输入0统计今天：";
	date d;
	double total_income = 0, total_outcome = 0;
	int num = 0;
	string day_later;
	cin >> day_later;
	while (!is_proper_num(day_later))
	{
		cout << "输入有误，请重新输入";
		cin.ignore(100, '\n');//清空cin流
		cin >> day_later;
	}
	if (to_int(day_later) == 0)
		d = today();
	else
	{
		int year = to_int(day_later);
		string month, day;
		cin >> month;
		while (!is_proper_num(month))
		{
			cout << "月份输入有误，请重新输入:";
			cin.ignore(100, '\n');//清空cin流
			cin >> month;
		}
		cin >> day;
		while (!is_proper_num(day))
		{
			cout << "日期输入有误，请重新输入:";
			cin.ignore(100, '\n');//清空cin流
			cin >> day;
		}
		d = date(year, to_int(month), to_int(day));
	}
	for (int x = 0; x < k; x++)
	{
		if (records[x].day_fact_is(d) && !records[x].unfinished())
		{
			num++;
			total_income += records[x].get_income();
			total_outcome += records[x].get_outcome();
		}
	}
	cout << "\n在";
	d.show();
	cout << "，总共进行了" << num << "次维修，收到了" << total_income << "元，净盈利" << total_income - total_outcome << "元\n";
	pause();
}
void statistic_income_building()
{
	cout << "请输入你要统计的楼： ";
	string build_to_judge;
	double total_income = 0, total_outcome = 0;
	int num = 0;
	cin >> build_to_judge;
	while (!is_proper_num(build_to_judge))
	{
		cout << "输入错误，请重输:";
		cin >> build_to_judge;
	}
	int build = to_int(build_to_judge);
	for (int x = 0; x < k; x++)
	{
		if (records[x].num_building_is(build) && !records[x].unfinished())
		{
			num++;
			total_income += records[x].get_income();
			total_outcome += records[x].get_outcome();
		}
	}
	cout << "\n对这栋楼，总共进行了" << num << "次维修，收到了" << total_income << "元，净盈利" << total_income - total_outcome << "元\n";
	pause();
}
void statistic_income_people()
{
	cout << "请输入你要统计的检修员姓名： ";
	string name;
	double total_income = 0, total_outcome = 0;
	int num = 0;
	cin >> name;
	for (int x = 0; x < k; x++)
	{
		if (records[x].name_is(name) && !records[x].unfinished())
		{
			num++;
			total_income += records[x].get_income();
			total_outcome += records[x].get_outcome();
		}
	}
	cout << '\n' << name << "总共进行了" << num << "次维修，收到了" << total_income << "元，净盈利" << total_income - total_outcome << "元\n";
	pause();
}
void statistic_income()
{
	string n;
	cout << "1.按天统计\n";
	cout << "2.按楼统计\n";
	cout << "3.按检修员统计\n";
	cout << "请选择统计的维度： ";
	cin >> n;
	switch (n[0])
	{
	case '1': statistic_income_date(); break;
	case '2': statistic_income_building(); break;
	case '3': statistic_income_people(); break;
	}
}
void random_record()
{
	cls();
	for (int x = 0; x < 100; x++)
		srand(rand());
	Node* head = read_data_from_array(records, k);
	int t = int(float(rand()) / RAND_MAX * k);
	record random_record = get_node(head, t);
	random_record.show(t);
	operate();
}
int main()
{
	cout << "                                                                                          胡皓睿 201806061108\n";
	pause();
	judge_permission();
	read_from_file();
	cout << "欢迎使用物业维修管理系统\n";
	while (true)
	{
		string c;
		cout << "正在读取文件，请等待\n";
		read_from_file();
		cls();
		srand((int)time(0));
		now().show();
		cout << '\n';
		cout << "┌──────────────────┐\n";
		cout << "│ 1.新增记录       │\n";
		cout << "│ 2.搜索记录       │\n";
		cout << "│ 3.查看所有记录   │\n";
		cout << "│ 4.预约到期查询   │\n";
		cout << "│ 5.高级统计功能   │\n";
		cout << "│ 6.显示随机记录   │\n";
		cout << "│ 7.修改密码       │\n";
		cout << "│ 0.退出系统       │\n";
		cout << "└──────────────────┘\n";
		cout << "请输入你要使用的功能： ";
		cin >> c;
		switch (c[0] - '0') {
		case 1:new_record(); break;
		case 2:search_record(); break;
		case 3:find_all_record(); break;
		case 4:intime_service(); break;
		case 5:statistic_income(); break;
		case 6:random_record(); break;
		case 7:passwd_root(); break;
		case 0:save_all(); cout << "感谢使用，再见"; return 0; break;
		default:
			cout << "输入错误，请重新输入\n";
			pause();
		}
		cls();
	}
}