//
// Created by star on 2024/3/15.
//


#ifdef _MSC_VER
#define _SCL_SECURE_NO_WARNINGS
#define ELFIO_NO_INTTYPES
#endif

#include <iostream>
#include <elfio/elfio_dump.hpp>

using namespace ELFIO;

int main( int argc, char** argv )
{
//    if ( argc != 2 ) {
//        printf( "Usage: elfdump <file_name>\n" );
//        return 1;
//    }

    elfio reader;
    char * file="C:\\Users\\star\\Desktop\\symbol_exec\\symex-x86-stack";
    if ( !reader.load( file) ) {
        printf( "File %s is not found or it is not an ELF file\n", argv[1] );
        return 1;
    }

    dump::header( std::cout, reader );
    dump::section_headers( std::cout, reader );
    dump::segment_headers( std::cout, reader );
    dump::symbol_tables( std::cout, reader );
    dump::notes( std::cout, reader );
    dump::modinfo( std::cout, reader );
    dump::dynamic_tags( std::cout, reader );
    dump::section_datas( std::cout, reader );
    dump::segment_datas( std::cout, reader );

    return 0;
}
