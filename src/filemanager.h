#include <fstream>
#include <vector>
#include <sstream>

template <typename Type>
class filemanager
{
	private:
		std::vector<std::fstream*> files;
		int count;
	public:
		filemanager();
		int open_next();
		void write(int fileid, Type& value);
		Type read(int fileid);
		void reset(int fileid);
		void close(int fileid);
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
	makename << ".tempfile" << count;
	std::fstream* newstream = new std::fstream(makename.str().c_str());
	files.push_back(newstream);
	return count++;
}

template <typename Type>
void filemanager<Type>::write(int fileid, Type& value)
{
	*files[fileid] << value;//Primitive types only ;(
}

template <typename Type>
Type filemanager<Type>::read(int fileid)
{
	Type ret;//If no default constructor?
	*files[fileid] >> ret;
	return ret;
}

template <typename Type>
void filemanager<Type>::reset(int fileid)
{
	files[fileid]->seekg(std::fstream::beg);
}

template <typename Type>
void filemanager<Type>::close(int fileid)
{
	files[fileid]->close();
}


