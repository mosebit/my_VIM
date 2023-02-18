#pragma once
#include<initializer_list>
#include<iostream>
#include<string>


class MyString
{
public:
	MyString();
	MyString(const char*);
	MyString(char);
	MyString(const MyString&);
	MyString(std::initializer_list<char>);
	MyString(std::string);
	MyString(int count, char char_arg);
	~MyString();

	void change_capacity(int new_str_size);	

	MyString& operator =(const MyString&);
	MyString& operator =(const char*);


	MyString operator +(const MyString&);
	MyString operator +(const char*);
	MyString operator +(std::string);

	void operator += (const char*);
	void operator += (const MyString&);

	char operator [](int);

	bool operator > (const MyString&);
	bool operator < (const MyString&);
	bool operator >= (const MyString&);
	bool operator <= (const MyString&);
	bool operator != (const MyString&);
	bool operator == (const MyString&);

	const char* c_str();
	const char* data();
	int length();
	int size();

	bool empty();
	int capacity();
	void shrink_to_fit();
	void clear();

	friend std::ostream& operator<< (std::ostream&, const MyString&);
	friend std::istream& operator>> (std::istream& in, MyString& res);

	void insert(int index, int count, char char_var);
	void insert(int index, MyString insert_str);
	void insert(int index, MyString insert_str, int count);
	void insert(int index, std::string insert_str);
	void insert(int index, std::string insert_str, int count);
	void insert(int index, const char*, int count);

	void erase(int index, int count);

	void append(const char* null_term_str);
	void append(int chars_count, char char_el);
	void append(const char* null_term_str, int index, int count);
	void append(std::string adding_str);
	void append(std::string adding_str, int index, int count);

	void replace(int index, int count, const char* repl_str);
	void replace(int index, int count, std::string repl_str);

	MyString substr(int index);
	MyString substr(int index, int count);

	int find(const char* searching_str);
	int find(const char* searching_str, int index);
	int find(std::string searching_str);
	int	find(std::string searching_str, int index);

private:
	char* str;
	int size_num;
	int capacity_num;
};

//MyString::MyString()
//{
//}
//
//MyString::~MyString()
//{
//}