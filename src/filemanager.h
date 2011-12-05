#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>

template <typename Type>
class filemanager
{
	private:
		std::vector<std::fstream*> files;
		int count;
	public:
		bool read_success;
		
		filemanager();
		int open_next();
		void write(int fileid, Type& value);
		Type read(int fileid);
		void reset(int fileid);
		void close(int fileid);
		bool alive();
};

template <typename Type>
filemanager<Type>::filemanager()
{
	count = 0;
}

template <typename Type>
int filemanager<Type>::open_next()
{
	std::stringstream makename;
	makename << "tempfile" << count << "";
	std::fstream* newstream = new std::fstream(makename.str().c_str(), std::fstream::out);
	newstream->close();
	newstream->open(makename.str().c_str(), std::fstream::out | std::fstream::in);
	//std::cerr << "Opened new file: " << makename.str() << std::endl;
	files.push_back(newstream);
	return count++;
}

template <typename Type>
void filemanager<Type>::write(int fileid, Type& value)
{
	*files[fileid] << value << " ";//Primitive types only ;(
}

template <typename Type>
Type filemanager<Type>::read(int fileid)
{
	Type ret;//If no default constructor?
	*files[fileid] >> ret;
	read_success = !files[fileid]->fail();
	return ret;
}

template <typename Type>
void filemanager<Type>::reset(int fileid)
{
	files[fileid]->seekg(std::fstream::beg);
	//read_success = true;
}

template <typename Type>
void filemanager<Type>::close(int fileid)
{
	//std::cerr << "Closing file, id " << fileid << std::endl;
	files[fileid]->close();
}

template <typename Type>
bool filemanager<Type>::alive()
{
	for(int i = 0; i < count; i++)
	{
		if(!files[i] || !files[i]->is_open() || files[i]->eof())
			return false;
	}
	return true;
}
