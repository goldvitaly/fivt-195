#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>

template <typename ValueType>
class Filemanager
{
	private:
		std::vector<std::fstream*> files;
		int count;
		bool read_success_flag;
	public:
		bool read_success();
		
		Filemanager();
		int open_next();
		void write(int fileid, ValueType& value);
		ValueType read(int fileid);
		void reset(int fileid);
		void close(int fileid);
		bool alive() const;
};

template <typename ValueType>
bool Filemanager<ValueType>::read_success()
{
	return read_success_flag;
}

template <typename ValueType>
Filemanager<ValueType>::Filemanager()
{
	count = 0;
}

template <typename ValueType>
int Filemanager<ValueType>::open_next()
{
	std::stringstream makename;
	makename << "tempfile" << count << "";
	
	std::fstream* newstream = new std::fstream(makename.str().c_str(), std::fstream::out);
	newstream->close();//Dirty hack to create file
	newstream->open(makename.str().c_str(), std::fstream::out | std::fstream::in);
	files.push_back(newstream);
	
	return count++;
}

template <typename ValueType>
void Filemanager<ValueType>::write(int fileid, ValueType& value)
{
	*files[fileid] << value << " ";//Primitive types only ;(
}

template <typename ValueType>
ValueType Filemanager<ValueType>::read(int fileid)
{
	ValueType ret;//If no default constructor?
	*files[fileid] >> ret;
	read_success_flag = !files[fileid]->fail();
	return ret;
}

template <typename ValueType>
void Filemanager<ValueType>::reset(int fileid)
{
	files[fileid]->seekg(std::fstream::beg);
}

template <typename ValueType>
void Filemanager<ValueType>::close(int fileid)
{
	files[fileid]->close();
}

template <typename ValueType>
bool Filemanager<ValueType>::alive() const
{
	for(int i = 0; i < count; i++)
	{
		if(!files[i] || !files[i]->is_open() || files[i]->eof())
			return false;
	}
	return true;
}
