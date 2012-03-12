#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

struct string
{
	size_t size, asize;
	char *a;
};

#define string struct string
//#define pstring string*

string new_string ()
{
	string s;
	s.size = 0, s.asize = 1;
	s.a = (char*)malloc(s.asize);
	s.a[0] = '\0';
	return s;
}

string new_sized_string (size_t size)
{
	string s;
	s.size = 0, s.asize = size + 1;
	s.a = (char*)malloc(s.asize);
	s.a[0] = '\0';
	return s;
}

void delete_string (string s)
{
	if (s.a != NULL) free(s.a);
}

string copy_string (string s)
{
	string r = new_sized_string(s.size);
	strcpy(r.a, s.a);
	return r;
}

void string_resize (string *s, size_t size, char c)
{
	if (size < s->size)
	{
		s->size = size;
		s->a[size] = '\0';
	}
	else
	{
		s->asize = size + 1;
		s->a = realloc(s->a, s->asize);
		
		size_t i;
		for (i = s->size; i < s->asize; ++i)
			s->a[i] = '\0';
	}
}

string string_conc (string s1, string s2)
{
	string r = new_sized_string(s1.size + s2.size);
	
	strcpy(r.a, s1.a);
	strcpy(r.a + s1.size, s2.a);
	
	return r;
}

char* string_cstr (string s)
{
	return s.a;
}

void string_pop_back (string *s)
{
	if (s->size != 0)
	{
		s->size--;
		s->a[s->size] = '\0';
	}
}

void string_push_back (string *s, char c)
{
	if (s->size + 1 == s->asize)
	{
		s->asize *= 2;
		s->a = realloc(s->a, s->asize);
		s->a[s->size] = '\0';
	}
	s->size++;
	
	s->a[s->size - 1] = c;
	s->a[s->size] = '\0';
}
