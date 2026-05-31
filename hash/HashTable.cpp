#include "HashTable.h"

HashTable::HashTable(size_t size) noexcept : table(size),_capacity(size),_filled(0) {}

HashTable::~HashTable() {}

size_t HashTable::hash_function(const KeyType& key) const {
	size_t hash = 0;
	for (auto i:key)
	{
		hash = hash*37 + static_cast<int>(i);
	}
	return hash%_capacity;	
}

void HashTable::insert(const KeyType& key,const ValueType& value) {
	ValueType tmp;
	if (find(key,tmp))
	{
		(*this)[key] = value;
		return;
	}
	if (getLoadFactor() > 0.75)
	{
		size_t cap = _capacity;
		_capacity *= 2;
		_filled = 0;
		std::vector<std::list<std::pair<KeyType, ValueType>>> newtable(_capacity);
		for (size_t i = 0;i<cap;i++)
		{
			for (auto uk = table[i].begin();uk!=table[i].end();uk++)
			{
				newtable[hash_function((*uk).first)].push_back(*uk);
				if (newtable[hash_function((*uk).first)].size()==1) _filled+=1;
			}
		}
		table = std::move(newtable);
	}
	table[hash_function(key)].push_back({key,value});
	if (table[hash_function(key)].size()==1) _filled++;
	if (getLoadFactor() > 0.75)
        {             
                size_t cap = _capacity;  
                _capacity *= 2;  
                _filled = 0;  
                std::vector<std::list<std::pair<KeyType, ValueType>>> newtable(_capacity);
                for (size_t i = 0;i<cap;i++)
                {           
                        for (auto uk = table[i].begin();uk!=table[i].end();uk++)
                        {   
                                newtable[hash_function((*uk).first)].push_back(*uk);
                                if (newtable[hash_function((*uk).first)].size()==1) _filled+=1;
                        }   
                }           
                table = std::move(newtable);
        } 	
}

bool HashTable::find(const KeyType& key,ValueType& value) const {
	const std::list<std::pair<KeyType,ValueType>>& tmp = table[hash_function(key)];
	for (auto i = tmp.begin();i!=tmp.end();i++)
	{
		if ((*i).first==key)
		{
			value = (*i).second;
			return true;
		}
	}
	return false;	
}

void HashTable::remove(const KeyType& key) {
	auto& tmp = table[hash_function(key)];
	for (auto i = tmp.begin();i!=tmp.end();i++)
        {
                if ((*i).first==key)
                {
                        tmp.erase(i);
			if (tmp.empty())
			{
				--_filled;
			}
			return;
                }
        }
}

ValueType& HashTable::operator[](const KeyType& key) {
	std::list<std::pair<KeyType,ValueType>>& tmp = table[hash_function(key)];
	for (auto i = tmp.begin();i!=tmp.end();i++)
	{
		if ((*i).first==key)
		{
			return (*i).second;
		}
	}
	insert(key,0);
	return (*this)[key];
}

double HashTable::getLoadFactor() {
	return static_cast<double>(_filled)/_capacity;
}
