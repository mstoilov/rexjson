/*
 *  Copyright (c) 2013-2023 Martin Stoilov
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Martin Stoilov <martin@sigmadrone.org>
 */

#ifndef REXJSON_HPP_
#define REXJSON_HPP_

#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <iostream>
#include <stdint.h>

namespace rexjson {

class value;
class input;
class output;

typedef std::map<std::string, value> object;

class array : public std::vector<value>
{
public:
	using base = std::vector<value>;
	using base::vector;
	base::reference operator[](base::size_type n)
	{
		if (n >= size()) {
			std::stringstream oss;
			oss << "array::operator[] called with invalid index: " << n << ", size is: " << size();
			throw std::range_error(oss.str());
		}
		return base::operator [](n);
	}
	base::const_reference operator[](base::size_type n) const
	{
		if (n >= size()) {
			std::stringstream oss;
			oss << "array::operator[] called with invalid index: " << n << ", size is: " << size();
			throw std::range_error(oss.str());
		}
		return base::operator [](n);
	}
};


enum value_type {null_type = 0, obj_type, array_type, str_type, bool_type, int_type, real_type};

class value {
	friend class input;
	friend class output;

public:
	static value null;

	value(); // creates null value
	value(const value& v);
	value(value&& v);
	value(const char* v);
	value(const std::string& v);
	value(bool v);
	value(int v);
	value(unsigned int v);
	value(long int v);
	value(unsigned long int v);
	value(long long int v);
	value(unsigned long long int v);
	value(double v);
	value(const object& v);
	value(const array& v);
	~value();

	/**
	 * Serialize the JSON tree to string.
	 *
	 * @param pretty Format the output string.
	 * @param nullprop If set to false properties with null values will be omitted
	 * @param tabsize if pretty is true the formatting will use indentation with the specified tabsize
	 * @param precision Specifies how many digits to use after the decimal dot
	 */
	std::string write(bool pretty = false, bool nullprop = true, size_t tabsize = 4, size_t precision = 4) const;

	/**
	 * Parse the string and return JSON value representation.
	 * If parsing fails this function will throw exception std::runtime_error
	 *
	 * @param str JSON string to be parsed
	 * @param maxlevels The maximum nested levels the parser can parse
	 */
	value& read(const std::string& str, size_t maxlevels = 32);
	value& read(const char* s, size_t n, size_t maxlevels = 32);

	value& push_back(const value& v = value::null);
	value& operator[](size_t i);
	value& operator[](const std::string& name);
	object& get_obj();
	const object& get_obj() const;
	array& get_array();
	const array& get_array() const;
	const std::string& get_str() const;
	bool is_null() const;
	bool get_bool() const;
	int get_int() const;
	int64_t get_int64() const;
	double get_real() const;
	value_type get_type() const;
	std::string get_typename() const;
	std::string to_string() const;
	static std::string get_typename(unsigned int type);

	template<typename T> void get(T& ret) const
	{
		ret = get_int();
	}


	void set(const object& v) {
		operator=(v);
	}

	template<typename T>
	typename std::decay<T>::type get_value() const
	{
		typename std::decay<T>::type ret{};
		get<T>(ret);
		return ret;
	}

	value& operator=(const value& v);
	value& operator=(value&& v);
	explicit operator float() const { return get_real(); }
	explicit operator double() const { return get_real(); }
	explicit operator int() const { return get_int(); }
	explicit operator unsigned int() const { return get_int(); }
	explicit operator int64_t() const { return get_int64(); }
	explicit operator bool() const { return get_bool(); }

	void check_type(value_type vtype) const;

protected:
	void destroy();

protected:
	value_type value_type_;
	union {
		bool v_bool_;
		double v_double_;
		int64_t v_int_;
		void* v_null_;
		std::string *v_string_;
		array *v_array_;
		object *v_object_;
	} store_;
};


inline std::ostream& operator<<(std::ostream& os, const rexjson::value& v)
{
	os << v.to_string();
	return os;
}

struct error : public std::runtime_error {
	error(size_t __offset, std::string __token)
			: std::runtime_error(std::string("Error: '" + __token + "' @ offset: " + std::to_string(__offset)))
			, offset(__offset)
			, token(__token)    {}

	size_t offset;
	std::string token;
};

class input {
public:
	input(std::istream& is) : is_(is), token_id_(0), offset_(0), levels_(0) {}
	void read_steam(value& v, size_t maxlevels = 32);

protected:
	void get_token();
	void next_token();
	void error_unexpectedtoken();
	void parse_token(int token);
	void parse_name(std::string &str);
	void parse_namevalue(object& parent);
	void parse_object(value& parent);
	void parse_array(value& parent);
	void parse_primitive(value& v);
	void parse_value(value& v);

protected:
	std::istream& is_;
	std::string token_;
	std::string loctok_;
	int token_id_;
	size_t offset_;
	size_t levels_;
};


template<> inline void value::get<int64_t>(int64_t& ret) const          { ret = get_int64(); }
template<> inline void value::get<bool>(bool& ret) const                { ret = get_bool(); }
template<> inline void value::get<double>(double& ret) const            { ret = get_real(); }
template<> inline void value::get<float>(float& ret) const              { ret = get_real(); }
template<> inline void value::get<std::string>(std::string& ret) const  { ret = get_str(); }


class output {
public:
	output(bool pretty = false, bool nullprop = true, size_t tabsize = 4, size_t precision = 3, const std::string& crlf = "\r\n");
	void write(const value& v, std::ostream& os);
	std::string write(const value& v);

protected:
	void write_stream(const value& v, std::ostream& os, size_t level);
	void write_stream_namevalue(const std::string& n, const value& v, std::ostream& os, size_t level);
	void write_stream_value(const value& v, std::ostream& os, size_t level);

protected:
	bool pretty_;
	bool nullprop_;
	size_t tabsize_;
	size_t precision_;
	std::string crlf_;
};

/**
 * Parse the string and return JSON value representation.
 * If parsing fails this function will throw std::runtine_error
 */
inline value read(const std::string& s)
{
	return value().read(s);
}

inline value read(char* s, size_t n)
{
	return value().read(s, n);
}

/**
 * Parse the string and return JSON value representation.
 * If parsing fails this function will return false
 */
inline bool read_no_throw(value& v, const std::string& str, size_t maxlevels = 32)
{
	try {
		std::stringstream oss(str);
		rexjson::input in(oss);
		in.read_steam(v, maxlevels);
	} catch (std::runtime_error &e) {
		return false;
	}
	return true;
}


inline std::string write(const value& v)
{
	return v.write(false);
}

inline std::string write_formatted(const value& v)
{
	return v.write(true);
}


} // namespace rexjson

#endif /* REXJSON_HPP_ */
