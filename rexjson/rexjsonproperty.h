/*
 *  Sigmadrone
 *  Copyright (c) 2013-2015 The Sigmadrone Developers
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

#ifndef REXJSONPROPERTYEX_H_
#define REXJSONPROPERTYEX_H_

#include <assert.h>
#include <functional>
#include <memory>
#include "rexjson/rexjson++.h"

namespace rexjson {

/*
* Convert void* to
* rexjson::value 
*/
template<typename T>
rexjson::value property_get(void* ctx)
{
	return *static_cast<T*>(ctx);
}

/*
* Convert rexjson::value to plain type
*/
template<typename T>
void property_set(const rexjson::value& val, void* ctx)
{
	val.get(*static_cast<T*>(ctx));
}

class property;
using enumerate_children_callback = std::function<void(const std::string&, property&)>;

class iproperty {
public:
	virtual ~iproperty() { }
	virtual rexjson::value get() const = 0;
	virtual void set(const rexjson::value& val) = 0;
	virtual iproperty* clone() = 0;
	virtual void insert(const std::string& name, const property& v) = 0;
	virtual void enumerate(property& thisprop, const std::string& path, const enumerate_children_callback& callback) = 0;
	virtual property& navigate(const std::string& path) = 0;
	virtual property& operator[](const std::string& name) = 0;
};

class property_leaf : public iproperty {
public:
	property_leaf(void* ctx, const std::function<rexjson::value (void* ctx)>& getter, const std::function<void(const rexjson::value& value, void* ctx)>& setter)
		: ctx_(ctx), getter_(getter), setter_(setter)
	{
	}

	property_leaf(void* ctx, const std::function<const rexjson::value& (void* ctx)>& getter)
		: ctx_(ctx), getter_(getter)
	{
	}

	property_leaf(void* ctx, const std::function<void(const rexjson::value& value, void* ctx)>& setter)
		: ctx_(ctx), setter_(setter)
	{
	}

	virtual rexjson::value get() const override
	{
		if (!getter_) 
			throw std::runtime_error("No read operation defined for the property.");
		rexjson::value ret = getter_(ctx_);
		std::cout << "get(): " << ret.to_string() << std::endl;
		return ret;

	}

	virtual void set(const rexjson::value& value) override
	{
		if (!setter_) 
			throw std::runtime_error("No write operation defined for the property.");
		setter_(value, ctx_);
	}

	virtual iproperty* clone() override
	{
		return new property_leaf(ctx_, getter_, setter_);
	}

	virtual void insert(const std::string& name, const property& v) override
	{
		throw std::runtime_error("insert not implemented.");
	}

	virtual void enumerate(property& thisprop, const std::string& path, const enumerate_children_callback& callback) override
	{
		callback(path, thisprop);
	}

	virtual property& navigate(const std::string& path) override
	{
		throw std::runtime_error("navigate not implemented.");
	}

	virtual property& operator[](const std::string& name) override
	{
		throw std::runtime_error("operator[] not implemented.");
	}


	void* ctx_;
	std::function<rexjson::value (void* ctx)> getter_;
	std::function<void(const rexjson::value& value, void* ctx)> setter_;
};

class property_map;

class property {
public:
	property() = default;
	property(const property_map& map);
	property(void* ctx, const std::function<rexjson::value (void* ctx)>& getter, const std::function<void(const rexjson::value& value, void* ctx)>& setter)
		: property_object_(new property_leaf(ctx, getter, setter))
	{
	}

	property(void* ctx, const std::function<const rexjson::value& (void* ctx)>& getter)
		: property_object_(new property_leaf(ctx, getter))
	{
	}

	property(void* ctx, const std::function<void(const rexjson::value& value, void* ctx)>& setter)
		: property_object_(new property_leaf(ctx, setter))
	{
	}

	property(const property& prop) 
	{
		property_object_.reset(prop.property_object_->clone());
	}

	property& operator=(const property& prop)
	{
		property_object_.reset(prop.property_object_->clone());
		return *this;
	}

	rexjson::value get() const
	{
		return property_object_->get();
	}

	void set(const rexjson::value& value) 
	{
		property_object_->set(value);
	}


	void insert(const std::string& name, const property& v)
	{
		property_object_->insert(name, v);
	}

	void enumerate(property& thisprop, const std::string& path, const enumerate_children_callback& callback)
	{
		property_object_->enumerate(thisprop, path, callback);
	}

	property& navigate(const std::string& path)
	{
		return property_object_->navigate(path);
	}

	property& operator[](const std::string& name)
	{
		return property_object_->operator[](name);
	}

	value to_json()
	{
		rexjson::object ret;
		enumerate(*this, "", [&](const std::string& path, rexjson::property& prop)->void {
			ret[path] = prop.get();
		});
		return ret;
	}

protected:
	std::unique_ptr<iproperty> property_object_;
};

class property_map : public iproperty {
protected:
	std::map<std::string, property> map_;

public:
	property_map() = default;
	property_map(std::initializer_list<typename std::map<std::string, property>::value_type>lst) : map_(lst) {}
	property_map(const property_map& map) : map_(map.map_) {}
	virtual rexjson::value get() const override
	{
		throw std::runtime_error("get not implemented.");
	}

	virtual void set(const rexjson::value& value) override
	{
		throw std::runtime_error("set not implemented.");
	}

	virtual iproperty* clone() override
	{
		return new property_map(*this);
	}

	virtual void insert(const std::string& name, const property& v) override
	{
		map_[name] = property(v);
	}

	virtual void enumerate(property& thisprop, const std::string& path, const enumerate_children_callback& callback) override
	{
		for (auto &o : map_) {
			o.second.enumerate(o.second, path + "." + o.first, callback);
		}
	}

	virtual property& navigate(const std::string& path) override
	{
		std::string toc = path;
		std::size_t toc_pos = toc.find_first_of(".[", 1);
		if (toc_pos != std::string::npos)
			toc = toc.substr(0, toc_pos);
		else
			toc = toc.substr(0);
		if (toc.empty())
			throw (std::runtime_error("Invalid path"));
		std::string restpath = path.substr(toc.size());
		std::string trimmed = (toc.size() && toc.at(0) == '.') ? toc.substr(1) : toc;
		if (restpath.empty())
			return operator [](trimmed);
		else
			return (operator [](trimmed)).navigate(restpath);
	}

	virtual property& operator[](const std::string& name) override
	{
		std::map<std::string, property>::iterator it = map_.find(name);
		if (it == map_.end())
			throw std::runtime_error("Invalid property: " + name);
		return it->second;
	}	
};

inline property::property(const property_map& map)
{
	property_object_.reset(new property_map(map));
}	


}

#endif
