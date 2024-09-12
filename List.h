#pragma once
#include <vector>
#include <algorithm>
#include <string>
#include <iostream>

namespace KDSim {

	class EasyHash {
	public:
		//初始容量
		static constexpr unsigned int s_initCapacity = 3;
		//哈希表初始化，
		EasyHash() : m_capacity(s_initCapacity) {
			m_storage.resize(1LL << m_capacity, 0 );//1LL << m_capacity
			current_max_index = 1LL << m_capacity;//1LL << m_capacity;
			current_index = 0;
			/*m_hash_index.reserve(vec_size);
			for (int temp = vec_size - 1; temp >= 0; temp--) {
				m_hash_index.push_back(temp);
			}*/
		}

		void insert(unsigned int& _index, unsigned int _value) {
			if (m_hash_index.size() == 0) {
				if (current_index == current_max_index) {
					reserve_();
				}
				_index = current_index;
				current_index++;
			}
			else {
				_index = m_hash_index.back();
				m_hash_index.pop_back();
			}
			
			m_storage[_index] = _value;
		}

		void erase(unsigned int _index) {
			m_hash_index.push_back(_index);
		}

		unsigned int& find(unsigned int _index) {
			return m_storage[_index];
			/*auto _pos = findFirstMatch(_key);
			if (!m_storage[_pos].first)
				throw ("Iterator is invalidated!");
			return m_storage[_pos].second;*/
		}

		void clear() {
			m_hash_index.clear();
			current_max_index = m_storage.size();
			current_index = 0;
			/*for (int temp =  - 1; temp >= 0; temp--) {
				m_hash_index.push_back(temp);
			}*/
		}

		void reserve_() {
			
			m_capacity+=2;
			current_max_index = 1LL << m_capacity;
			m_storage.resize(current_max_index);

			//m_storage.resize(m_storage.capacity(),0);

			/*m_hash_index.reserve(expand_size);
			m_hash_index.resize(m_hash_index.capacity(),2);*/
			/*std::iota(m_hash_index.begin(), m_hash_index.end(), 1LL << m_capacity - 1);*/
				
			
			
		}

		

		std::vector< unsigned int> m_storage;
		std::vector<unsigned int> m_hash_index;
		unsigned int current_max_index;
		unsigned int current_index;
		unsigned int m_capacity;
		//unsigned int m_size;
	};


	template<typename T>
	class List {
	public:
		
		friend class iterator;
		friend class const_iterator;

		static constexpr unsigned int s_initCapacity = 3;

		struct Node {
			unsigned int next;
			unsigned int pred;
			unsigned int rank;
			unsigned int hash_index;
		};

		class iterator {
		public:
			friend class List;

			iterator(unsigned int _rank, unsigned int _hash_index, List* _list) : m_rank_(_rank), m_hash_index_(_hash_index), m_list_(_list) {}

			iterator(const iterator& _other) = default;

			~iterator() = default;

			iterator& operator=(const iterator& _other) = default;

			T& operator*() {
				return m_list_->m_value_[m_list_->m_hash_.find(m_hash_index_)];
			}

			iterator& operator++() {
				unsigned int _next = m_list_->m_storage_[m_list_->m_hash_.find(m_hash_index_)].next;
				m_rank_ = m_list_->m_storage_[_next].rank;
				m_hash_index_ = m_list_->m_storage_[_next].hash_index;
				return *this;
			}

			iterator& operator--() {
				unsigned int _pred = m_list_->m_storage_[m_list_->m_hash_.find(m_hash_index_)].pred;
				m_rank_ = m_list_->m_storage_[_pred].rank;
				m_hash_index_ = m_list_->m_storage_[_pred].hash_index;
				return *this;
			}

			iterator operator+(unsigned int _offset) {
				iterator _tmp = *this;
				for (unsigned int i = 0; i < _offset; i++)
					++_tmp;
				return _tmp;
			}

			iterator operator-(unsigned int _offset) {
				iterator _tmp = *this;
				for (unsigned int i = 0; i < _offset; i++)
					--_tmp;
				return _tmp;
			}

			bool operator!=(const iterator& _other) const {
				return m_rank_ != _other.m_rank_;
			}

			bool operator==(const iterator& _other) const {
				return m_rank_ == _other.m_rank_;
			}

			

			

		private:
			unsigned int& getRank() {
				return m_rank_;
			}

			unsigned int& getHashIndex() {
				return m_hash_index_;
			}


			unsigned int m_rank_;
			unsigned int m_hash_index_;
			List* m_list_;
		};

		class const_iterator {
		
		public:
			friend class List;

			const_iterator(unsigned int _rank, unsigned int _hash_index, List* _list) : m_rank_(_rank), m_hash_index_(_hash_index),m_list_(_list) {}

			const_iterator(const const_iterator& _other) = default;

			const_iterator(iterator _other) : const_iterator(reinterpret_cast<const_iterator&>(_other)) {}

			~const_iterator() = default;

			const_iterator& operator=(const const_iterator& _other) = default;

			const_iterator& operator=(iterator _other) {
				return this->operator=(reinterpret_cast<const_iterator&>(_other));
			}

			const T& operator*() {
				return m_list_->m_value_[m_list_->m_hash_.find(m_hash_index_)];
			}

			const_iterator& operator++() {
				unsigned int _next = m_list_->m_storage_[m_list_->m_hash_.find(m_hash_index_)].next;
				m_rank_ = m_list_->m_storage_[_next].rank;
				m_hash_index_ = m_list_->m_storage_[_next].hash_index;
				return *this;
			}

			const_iterator& operator--() {
				unsigned int _pred = m_list_->m_storage_[m_list_->m_hash_.find(m_rank_)].pred;
				m_rank_ = m_list_->m_storage_[_pred].rank;
				m_hash_index_ = m_list_->m_storage_[_pred].hash_index;
				return *this;
			}

			const_iterator operator+(unsigned int _offset) {
				const_iterator _tmp = *this;
				for (unsigned int i = 0; i < _offset; i++)
					++_tmp;
				return _tmp;
			}

			const_iterator operator-(unsigned int _offset) {
				const_iterator _tmp = *this;
				for (unsigned int i = 0; i < _offset; i++)
					--_tmp;
				return _tmp;
			}

			bool operator!=(const const_iterator& _other) const {
				return m_rank_ != _other.m_rank_;
			}

			bool operator==(const const_iterator& _other) const {
				return m_rank_ == _other.m_rank_;
			}

		private:
			unsigned int& getRank() {
				return m_rank_;
			}

			unsigned int& getHashIndex() {
				return m_hash_index_;
			}

			unsigned int m_rank_;
			unsigned int m_hash_index_;
			List* m_list_;
		};

		iterator begin() {
			return iterator(m_storage_[m_begin_].rank, m_storage_[m_begin_].hash_index, this);
		}

		iterator end() {
			return iterator(m_storage_[0].rank, m_storage_[0].hash_index, this);
		}

		const_iterator begin() const {
			return const_iterator(m_storage_[m_begin_].rank, m_storage_[m_begin_].hash_index, this);
		}

		const_iterator end() const {
			return const_iterator(m_storage_[0].rank, m_storage_[0].hash_index, this);
			
		}

		const_iterator cbegin() const {
			return const_iterator(m_storage_[m_begin_].rank, m_storage_[m_begin_].hash_index, this);
		}

		const_iterator cend() const {
			return const_iterator(m_storage_[0].rank, m_storage_[0].hash_index, this);
		}
		

		List() : m_begin_(0), m_rank_(1) {
			unsigned int begin_hash_index;
			//m_hash_.clear();
			m_hash_.insert(begin_hash_index, 0);
			if (begin_hash_index != 0) {
				std::cout << "begin_hash_index wrong init" << std::endl;
				exit(-1);
			}
			Node endNode{ 0, 0, 1,begin_hash_index };
			
			m_storage_.push_back(endNode);
			m_value_.push_back(T());

			m_storage_.resize(8);
			m_value_.resize(8);

			m_begin_ = 0;
			m_size_ = 1;
			
		}

		List(const List& _other) = default;

		List& operator=(const List& _other) = default;

		iterator insert(const_iterator _it, const T& _value) {
			reserve_();
			unsigned int _pos = m_hash_.find(_it.getHashIndex());
			Node& originNode = m_storage_[_pos];
			Node _new;
			_new.next = _pos;
			_new.pred = originNode.pred;
			unsigned int _newPos = m_size_++;
			m_value_[_newPos] = _value;
			m_rank_++;
			_new.rank = m_rank_;
			unsigned int hash_index_t;
			m_hash_.insert(hash_index_t, _newPos);
			_new.hash_index = hash_index_t;
			m_storage_[_newPos] = _new;
			if (_pos == m_begin_)
				m_begin_ = _newPos;
			Node& originPredNode = m_storage_[originNode.pred];
			originPredNode.next = _newPos;
			originNode.pred = _newPos;
			return iterator(m_rank_, hash_index_t,this);
		}

		iterator erase(const_iterator _it) {
			unsigned int _pos = m_hash_.find(_it.getHashIndex());
			if (_pos == 0)
				throw("Attempt to erase the end node!");
			Node& originNode = m_storage_[_pos];
			Node& originNextNode = m_storage_[originNode.next];
			if (_pos == m_begin_)
				m_begin_ = originNode.next;
			Node& originPredNode = m_storage_[originNode.pred];
			originPredNode.next = originNode.next;
			originNextNode.pred = originNode.pred;
			unsigned int _rank = originNextNode.rank;
			unsigned int _hash_index = originNextNode.hash_index;
			m_hash_.erase(originNode.hash_index);
			recycle_(_pos);
			m_size_--;
			reserve_();
			//std::cout << _rank<<":";
			return iterator(_rank, _hash_index, this);
		}

		void push_back(const T& _value) {
			insert(end(), _value);
		}

		void push_front(const T& _value) {
			insert(begin(), _value);
		}

		void pop_back() {
			erase(end() - 1);
		}

		T& back() {
			return *(end() - 1);
		}

		T& front() {
			return *(begin());
		}

		void pop_front() {
			erase(begin());
		}

		void clear() {
			m_size_ = 1;
			unsigned int begin_hash_index;
			m_hash_.clear();
			m_hash_.insert(begin_hash_index, 0);
		}

		size_t size() const {
			return m_size_ - 1;
		}

		bool empty() {
			return size() <= 0;
		}

		T& read_chl(int index) {
			return *(begin() + index);
		}
		T& read_iter(iterator aim_item) {
			return *aim_item;
		}
		void update_chl(int index, T value) {
			*(begin() + index) = value;
		}

		void update_iter(iterator aim_item, T value) {
			*(aim_item) = value;
		}

		/*void delete_iter(iterator aim_item) {
			erase(aim_item);
		}*/

		int scan_chl() {
			int out = 0;
			/*while (!empty()) {
				out += pop_front().size();
			}*/
			for (auto it = begin(); it != end(); ++it) {
				out += (*it).size();
			}
				
			/*for (int i = 0; i < m_size_ - 1; i++) {
				out += read_chl(i).size();
			}*/
			return out;
		}

		void insert_chl(int index,T value) {
			insert(begin()+index, value);
		}

		void insert_iter(iterator aim_item, T value) {
			insert(aim_item, value);
		}

		//template<typename U>
		//static char* pack(const KDSim::List<U>& _vec, char* _dst) {
		//	char* _first = _dst;
		//	_dst = pack(_vec.m_storage_.size(), _dst);
		//	memcpy(_dst, &_vec.m_storage_[0], _vec.m_storage_.size() * sizeof(_vec.m_storage_[0]));
		//	_dst += _vec.m_storage_.size() * sizeof(_vec.m_storage_[0]);
		//	_dst = pack(_vec.m_value_, _dst);
		//	_dst = pack(_vec.m_hash_, _dst);
		//	_dst = pack(_vec.m_begin_, _dst);
		//	_dst = pack(_vec.m_rank_, _dst);
		//	_dst = pack(_vec.m_size_, _dst);
		//	int ret = _dst - _first;
		//	return _dst;
		//}

		//template<typename U>
		//static const char* unpack(KDSim::List<U>& _vec, const char* _src) {
		//	size_t vectorSize;
		//	_src = unpack(vectorSize, _src);
		//	_vec.m_storage_.resize(vectorSize);
		//	memcpy(&_vec.m_storage_[0], _src, _vec.m_storage_.size() * sizeof(_vec.m_storage_[0]));
		//	_src += _vec.m_storage_.size() * sizeof(_vec.m_storage_[0]);
		//	_src = unpack(_vec.m_value_, _src);
		//	//_src = unpack(_vec.m_hash_, _src);
		//	_src = unpack(_vec.m_begin_, _src);
		//	_src = unpack(_vec.m_rank_, _src);
		//	_src = unpack(_vec.m_size_, _src);
		//	return _src;
		//}


		int get_memory_fragment() {
			std::vector<int> memory_location;
			for (iterator it = begin(); it != end(); ++it) {
				int adress=(int)&(*it);
				memory_location.push_back(adress);
			} 

			if (memory_location.size() == 0) {
				return 0;
			}

			std::sort(memory_location.begin(), memory_location.end());

			int fragmentCount = 1;  // 至少存在一个区间
			int single_size = sizeof(T);
			//std::cout << single_size << std::endl;
			for (size_t i = 1; i < memory_location.size(); ++i) {
				if (memory_location[i] - memory_location[i - 1] > single_size) {
					fragmentCount++;
				}
			}
			return fragmentCount;

		}

		iterator get_iterator(int index) {
			return begin() + index;
		}

	private:
		void recycle_(unsigned int _offset) {
			if (_offset == m_size_ - 1)
				return;
			if (m_begin_ == m_size_ - 1)
				m_begin_ = _offset;
			std::swap(m_storage_[_offset], m_storage_[m_size_ - 1]);
			std::swap(m_value_[_offset], m_value_[m_size_ - 1]);
			m_storage_[m_storage_[_offset].next].pred = _offset;
			m_storage_[m_storage_[_offset].pred].next = _offset;
			m_hash_.find(m_storage_[_offset].hash_index) = _offset;
		}

		bool reserve_() {
			if (m_size_ >= m_storage_.size()) {
				m_storage_.resize(m_storage_.size() << 2);
				m_value_.resize(m_value_.size() << 2);
			}
			else if (m_size_ > 1 && m_size_ << 2 <= m_storage_.size()) {
				m_storage_.resize(m_storage_.size() >> 1);
				m_value_.resize(m_value_.size() >> 1);
			}
			else
				return false;
			return true;
		}
	public:
		std::vector<Node> m_storage_;
		std::vector<T> m_value_;
		EasyHash m_hash_;
		unsigned int m_begin_;
		unsigned int m_rank_;
		size_t m_size_;
	};

}