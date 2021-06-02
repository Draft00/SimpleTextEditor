#ifndef MY_STRING_H
#define MY_STRING_H

#include <string> //this is for std::string constructor
#include <initializer_list> // std::initializer_list


namespace STD
{
	class MyString
	{
	//ndr3w: usualy all private fields declraning at the bottom. Main reason - encapsulation. User are interested of a class interface or public methods at first.
	/*
	class MyString
	{
	public:
		more interesting for user
	protected:
		less interesting
	private:
		absolutely not interesting except high level of user curiosity
	*/

	public:
		static constexpr const size_t npos = -1;


	public:
		MyString();							// default constructor
		MyString(const MyString& other);			// copy constructor

		//ndr3w: use const for immutable vars
		MyString(const char* const cchar_array);				// char array constructor
		MyString(const std::string& std_string);				// std::string constructor. 
		
		MyString(const size_t count, const char c); //ndr3w: much better add an arg names in declarations! Names are comments for you code	(+) 
		MyString(const std::initializer_list<char>& list);
		MyString(const char* const cchar_array, const size_t count); 

	//ndr3w: too much public key words	(+)
		~MyString();

	//=
		MyString& operator = (const MyString& other);
		MyString& operator = (const char* cchar_array);
		MyString& operator = (const std::string& std_string); 
		MyString& operator = (char c); 
	// +=
		MyString& operator += (const MyString& other);
		MyString& operator += (const char* cchar_array); 
		MyString& operator += (const std::string& std_string);

	//+
		friend MyString operator+ (const MyString& left_other, const MyString& right_other); //friend???? friend.
		friend MyString operator+ (const MyString& other, const char* cchar_array);
		friend MyString operator+ (const MyString& left_other, const std::string& std_string);

		//ndr3w: alignment! (5?)
		friend bool operator == (const MyString& left, const MyString& right);
		friend bool operator != (const MyString& left, const MyString& right);
		friend bool operator <  (const MyString& left, const MyString& right);
		friend bool operator >  (const MyString& left, const MyString& right);
		friend bool operator <= (const MyString& left, const MyString& right);
		friend bool operator >= (const MyString& left, const MyString& right);

		//	Element access 
		const char& operator [] (size_t) const;
		char& operator [] (size_t);
		//In C++11 onwards, both functions are required to be the same.i.e.data is now required to be null - 
		//terminated.According to cppreference : "The returned array is null-terminated, that is, data() 
		//and c_str() perform the same function."
		const char* c_str() const;
		const char* data() const;

		size_t length() const; 
		size_t size() const; 
		bool empty() const;

		size_t capacity() const; // return the current amount of allocated memory for array
		void shrink_to_fit(size_t cap); // reduce the capacity to size 
		void clear(); 

		
		size_t find(const char* cchar_array, size_t pos) const;
		size_t find(const char* cchar_array) const;
		size_t find(const std::string& std_string, size_t pos) const;
		size_t find(const std::string& std_string) const;

		MyString& append(const char* cchar_array); 
		MyString& append(const char* cchar_array, size_t index, size_t count); 
		MyString& append(const std::string& std_string);
		MyString& append(const std::string& std_string, size_t index, size_t count);
		MyString& append(size_t count, char c);

		MyString& insert(size_t index, const char* cchar_array);
		MyString& insert(size_t index, const char* cchar_array, size_t count);
		MyString& insert(size_t index, size_t count, char c);
		MyString& insert(size_t index, const std::string& std_string);
		MyString& insert(size_t index, const std::string& std_string, size_t count);

		MyString& erase(size_t index, size_t count);

		MyString& replace(size_t index, size_t count, const char* cchar_array);
		MyString& replace(size_t index, size_t count, const std::string& std_string);

		MyString substr(size_t index, size_t count) const; 
		MyString substr(size_t index) const;

		//ok. I'll remember that for the future, but still (4?)
		//ndr3w: the _ (understroke) prefix is not recommend to use for identation of private fields or methods. Main reason - understroke is reserved for internal compiler usage. 
	protected:
		//private:
		void _dealloc_cstring(char*& buffer);
		void _setLength(const size_t len);
		void _clear_str(const size_t pos);
		void _setCapacity(const size_t cap);
		void _increaseCapacity(const size_t cap);
		void _decreaseCapacity(const size_t cap);
		char* _alloc_cstring(const size_t count) const;
		void _fill_str(char* cchar_array, const size_t len, size_t pos, char c) const;
		void _append(const char* cchar_array, size_t n);
		void _append(const char* cchar_array);
		size_t _strlen(const char* cchar_array);
		bool _find_compare(const char* cchar_array, size_t len, size_t pos) const;
		int _compare(const MyString& other) const; //strcoll - it should be lexicographically comparing. maybe i should use classic strcmp?
		char* _substr(const char* cchar_array, size_t pos, size_t len) const;
		char* _alloc_cstring(const size_t count, char c) const;
		size_t _find(const char* cchar_array, size_t pos, size_t len) const;
		void _insert(size_t pos, const char* cchar_array, size_t count);
		void _erase(size_t pos, size_t count);
		void _replace(size_t pos, size_t count, const char* cchar_array);

	protected:
	//private:
		//ndr3w: str_ prefix is useless and decrease the readability	(+) + align code with tabs	(2?)
		size_t _length		= 0;				// length
		size_t _capacity	= 0;				// capacity
		char*  _str			= nullptr;			// c-string

		//ndr3w: read about constexpr	(3?)
		static constexpr const size_t _increaseBy = 1;
	};

	//iostream
	std::ostream& operator<< (std::ostream& os, const STD::MyString& str);
	std::istream& operator>> (std::istream& is,  STD::MyString& str);


}
#endif // MY_STRING_H