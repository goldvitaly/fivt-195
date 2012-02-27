#ifndef MACRO_HPP
#define MACRO_HPP

#define m_assert(value, comment) {if(!(value)) {std::cerr << #comment << " at line " << __LINE__ << " at file " << __FILE__ << std::endl; exit(1);}}

#endif
