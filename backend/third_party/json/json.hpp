// Copyright (c) 2015-2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_HPP
#define TAO_JSON_HPP

// Value Class
#include "json/value.hpp"

// Value Reading
#include "json/from_file.hpp"
#include "json/from_input.hpp"
#include "json/from_stream.hpp"
#include "json/from_string.hpp"
#include "json/parts_parser.hpp"

// Value Writing
#include "json/stream.hpp"  // operator<<
#include "json/to_stream.hpp"
#include "json/to_string.hpp"

// Value Support
#include "json/operators.hpp"
#include "json/self_contained.hpp"

// Custom Types
#include "json/binding.hpp"
#include "json/consume.hpp"
#include "json/consume_file.hpp"
#include "json/consume_string.hpp"
#include "json/produce.hpp"

// Binary Literals
#include "json/binary.hpp"

// Other Formats
#include "json/cbor.hpp"
#include "json/jaxn.hpp"
#include "json/msgpack.hpp"
#include "json/ubjson.hpp"

// Events Implementations
#include "json/events.hpp"

#endif

/*
namespace tao::json {

template< typename T, typename V = void >
struct my_traits
   : public traits< T, V >
{
};

template<typename T>
struct my_traits< std::vector< T > >
{
   template< template< typename... > class Traits >
   static void assign( basic_value< Traits >& v, const std::vector< T >& w )
   {
      v.prepare_array();
      for( const auto i : w ) {
         v.unsafe_emplace_back( i );
      }
   }
};

template<typename T>
auto from_vector(const std::vector<T> values) {
	return basic_value< my_traits >(values);
}

}
*/
